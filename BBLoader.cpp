// BBLoader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BBLoader.h"
#include "Dlg.h"

//----------------------------------------
//#include "dll/BBLoader.h"
//#pragma comment(lib,"dll/BBLoader.lib")
#include <map>
#include <winsock2.h>
#include <process.h>
//#pragma warning(disable:4786)		//不起作用
//----------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BEGIN_MESSAGE_MAP(CApp, CWinApp)
END_MESSAGE_MAP()

CApp::CApp()
{

}
CApp theApp;

//--------------------------------------------------------------------------------
using namespace std;
typedef map<DWORD, SOCKET> map_id;
map_id	g_id;
HWND	g_hwndClient	= NULL;
HWND	g_hwndSend		= NULL;
HWND	g_hwndMain		= NULL;		//当有帐号来的时候想Dlg发送消息通知更新列表
DWORD	g_dwTargetPid	= 0;
HANDLE	g_hMutex		= NULL;		//只允许一个实例

void __ErrShow()
{
	DWORD  dwErrCode = GetLastError();
	HLOCAL hLocal    = NULL;
	
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		dwErrCode,
		0,
		(PTSTR)&hLocal,
		0,
		NULL);
	
	MessageBox(NULL, (LPCTSTR)LocalLock(hLocal), _T("错误提示"), MB_OK|MB_ICONINFORMATION);
	LocalFree(hLocal);	
}

void __ShowMessage(LPCTSTR lpInfo)
{
	MessageBox(NULL, lpInfo, _T("信息提示"), MB_OK|MB_ICONINFORMATION);
}

BOOL CALLBACK EnumWindowsProc(		//for GetSendWnd()
  HWND hwnd,
  LPARAM lParam
)
{
	DWORD dwTmp;
	char lpWndName[20] = {0};
	GetWindowThreadProcessId(hwnd, &dwTmp);
	GetClassName(hwnd, lpWndName, 20);

	if((dwTmp == (DWORD)lParam) && (0 == strcmp(lpWndName,"BBLoader.SendClass")))
	{
		g_hwndSend = hwnd;
		return FALSE;	//返回FALSE可以直接停止EnumWindows的枚举，否则EnumWindows枚举完所有Top窗口而止
	}
	else
		return TRUE;	//返回TRUE继续枚举
}
void GetSendWnd()	//不能用原来的GetSendWnd了，根据g_dwTargetPid枚举窗口查找
{
	EnumWindows(EnumWindowsProc, g_dwTargetPid);
}
BOOL IsSendWndAlive()
{
	g_hwndSend = NULL;
	GetSendWnd();	
	if(NULL == g_hwndSend)
	{
		return FALSE;
	}
	return TRUE;
}

void SendPacket(const char *lpPacket, int len, SOCKET s, DWORD VsID, int iPosition)
{
	char  *lpBuf = NULL;
	COPYDATASTRUCT cds = {0};
	lpBuf = new char[len];
	len	  -= 8;
	memcpy(lpBuf, lpPacket, len+8);
	memcpy(lpBuf,	&s,		4);	//SOCKET
	memcpy(lpBuf+4,	&len,	4);	//len
	if(VsID)
		memcpy(lpBuf+8+iPosition, &VsID,4);
	cds.cbData	= len+8;
	cds.lpData	= lpBuf;
	::SendMessage(g_hwndSend,WM_COPYDATA,(WPARAM)g_hwndSend,(LPARAM)&cds);
	delete lpBuf;

}

SOCKET GetSocket(DWORD id)
{
	map<DWORD, SOCKET>::iterator i;
	i = g_id.find(id);
	if(i == g_id.end())
	{
		return NULL;
	}
	else
		return i->second;
}

struct t_IDSOCKET
{
	DWORD	id;
	SOCKET	s;
};

const char lpMember[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xE9, 0xA2, 0xAD, 0x63, 0x66, 0x6F, 0x00, 0x00, 0x00, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E};
const char lpLeader[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xE9, 0xA2, 0xAD, 0x63, 0x6D, 0x6F, 0x00, 0x00, 0x00, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E};
extern multimap<DWORD, DWORD>	g_leader;
extern map<DWORD, DWORD>		g_member;
BOOL	g_bReteam	= TRUE;
DWORD	g_nDelay	= 4;
void __cdecl ReTeam(void* RecID)
{
	DWORD id = (DWORD)RecID;
	SOCKET s = GetSocket(id);
	if(s == NULL)
		return;

	DWORD VsID = 0;
	for(int j=0; j<2; j++)		//按第一次角色1，第二次角色2查找
	{
		id ^= 0x6E6E6E6E;		//记录的ID非原始，都^= 0x6E6E6E6E过的
		
		//在队员中寻找
		map<DWORD, DWORD>::iterator it;
		it = g_member.find(id);
		if(it != g_member.end())
		{
			Sleep(g_nDelay*1000);
			VsID = it->second;
			SendPacket(lpMember, sizeof(lpMember), s, VsID, 7);
			_endthread();
		}
		

		//在队长中寻找
		BOOL bFound = FALSE;
		multimap<DWORD, DWORD>::iterator mit;
		mit = g_leader.find(id);
		while(mit != g_leader.end())
		{
			bFound = TRUE;
			Sleep(g_nDelay*1000);
			if(id == mit->first)
			{	
				VsID	= mit->second;
				SendPacket(lpLeader, sizeof(lpLeader), s, VsID, 7);
				Sleep(1000);
			}
			mit++;
		}
		if(bFound == TRUE)
			_endthread();

		id ^= 0x6E6E6E6E;		//还原，下面取角色2
		id += 50000000;	
	}

	_endthread();
}


LRESULT CALLBACK ClientProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)	//接收ID等消息
{
	map_id::iterator i;
	t_IDSOCKET id = {0};
	BOOL bReconnect = FALSE;
	switch(Msg)
	{ 
	case WM_COPYDATA:			//帐号信息
		id =  *((t_IDSOCKET*) (((COPYDATASTRUCT*)lParam)->lpData) );
		i = g_id.find(id.id);
		if(i != g_id.end())
		{
			g_id.erase(i);		//重新连接的，删除；顺便可以加上掉线重组功能
			bReconnect = TRUE;	//掉线重组标志
		}
		g_id.insert(map_id::value_type(id.id,id.s ) );
		if(bReconnect && g_bReteam)
		{
			_beginthread(ReTeam,0, (void*)id.id);		//不能立即在g_id.erase(i);后面调用，因为新连的SOCKET还没关联进去
			bReconnect = FALSE;
		}
		SendMessage(g_hwndMain, WM_UPDATELIST, 0, 0);	//更新Dlg的ID列表
		return 0;
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
}
DWORD WINAPI ClientThread(LPVOID lpParameter)
{
	BOOL result;
	WNDCLASS WndClass;
	struct tagMSG Msg;
	HINSTANCE hCurProcess = GetModuleHandle(NULL);

	memset(&WndClass, 0, sizeof(WndClass));
	WndClass.lpfnWndProc = (WNDPROC)ClientProc;		//回调函数
	WndClass.lpszClassName = "BBLoader.ClientClass";
	WndClass.hInstance = hCurProcess;

	if ( !RegisterClassA(&WndClass) )
		MessageBoxA(NULL, "注册BBLoader.ClientClass失败！", "Error", MB_OK);
	g_hwndClient = CreateWindowEx(0, "BBLoader.ClientClass", "BBLoader.Client", 0, 0, 0, 0, 0, 0, 0, hCurProcess, 0);
	SetClientHwnd(g_hwndClient);	//放在InitInstance里的时候，由于是双线程，估计主线程执行这句的时候，上面的代码还没执行呢
	if ( GetMessage(&Msg, 0, 0, 0) )
	{
		do
		{
			TranslateMessage(&Msg);
			DispatchMessageA(&Msg);
			result = GetMessageA(&Msg, 0, 0, 0);
		}
		while ( result );
	}
	else
	{
		result = 0;
	}
	return result;
}
void InstallClientWnd()
{
	DWORD ThreadId;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientThread, NULL, 0, &ThreadId);
}

void InstallBBDll(HANDLE hProcess)	//inline函数可被多个地方调用
{
	HANDLE hRemoteThread;
	LPVOID lpszLibRemote;
	//TCHAR *lpszDll = "BBLoader.dll";
	TCHAR *lpszDll = new TCHAR[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, lpszDll);
	lstrcat(lpszDll, TEXT("\\BBLoader.dll"));

	lpszLibRemote = VirtualAllocEx(hProcess,
					   NULL,
					   strlen(lpszDll),
					   MEM_COMMIT,
					   PAGE_READWRITE
					   );

	if(!WriteProcessMemory(hProcess, lpszLibRemote, (PVOID)lpszDll,  strlen(lpszDll), NULL))
	{
		__ErrShow();
		goto over;
	}
	hRemoteThread = CreateRemoteThread(hProcess,
						NULL, 
						0, 
						(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"),
						lpszLibRemote,
						0,
						NULL
						);

	if(NULL == hRemoteThread)
	{
		__ErrShow();
		goto over;
	}
	DWORD aaa= WaitForSingleObject(hRemoteThread, INFINITE);
	CloseHandle(hRemoteThread);
	VirtualFreeEx(hProcess, lpszDll, strlen(lpszDll), MEM_RELEASE);
	delete lpszDll;
over:
	;
}
void BBOpen()				//启动，不带多开功能了，多开留给lpk.dll
{
	TCHAR *lpProcessName = _T("wlvipcn.exe");
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi;

	
	if ( !CreateProcess(lpProcessName,NULL,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS, NULL,NULL, &si, &pi) )
	{
		__ErrShow();
		return ;
	}
	if(WaitForInputIdle(pi.hProcess, INFINITE))	//解压完毕
	{
		__ErrShow();
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return;
	}

	g_dwTargetPid =	pi.dwProcessId;	//用于dll中检验是否是目标程序
	SetTargetPid(g_dwTargetPid);	//必须在laodlibary之前调用！
	InstallBBDll(pi.hProcess);		//安装hook dll

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return;	
}

BOOL CApp::InitInstance()
{	
	CString strMutex  = "{0FB7BA3F-12B8-48d2-950C-6FB143BD24C0}"; //GUID
	g_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, strMutex);
	if(g_hMutex == NULL)
	{
		g_hMutex = ::CreateMutex(NULL,NULL, strMutex);
	}
	else
	{
		__ShowMessage("漂流宝宝只能开一个！");
		return FALSE;
	}

	BBOpen();				//启动BB
	InstallClientWnd();		//安装Client窗口，用于接收目标程序Send窗口发送来的数据
	CDlg dlg;
	m_pMainWnd	= &dlg;
	dlg.DoModal();

	if(g_hMutex != NULL)
		::ReleaseMutex(g_hMutex);

	return FALSE;
}
