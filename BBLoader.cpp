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
//#pragma warning(disable:4786)		//��������
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
HWND	g_hwndMain		= NULL;		//�����ʺ�����ʱ����Dlg������Ϣ֪ͨ�����б�
DWORD	g_dwTargetPid	= 0;
HANDLE	g_hMutex		= NULL;		//ֻ����һ��ʵ��

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
	
	MessageBox(NULL, (LPCTSTR)LocalLock(hLocal), _T("������ʾ"), MB_OK|MB_ICONINFORMATION);
	LocalFree(hLocal);	
}

void __ShowMessage(LPCTSTR lpInfo)
{
	MessageBox(NULL, lpInfo, _T("��Ϣ��ʾ"), MB_OK|MB_ICONINFORMATION);
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
		return FALSE;	//����FALSE����ֱ��ֹͣEnumWindows��ö�٣�����EnumWindowsö��������Top���ڶ�ֹ
	}
	else
		return TRUE;	//����TRUE����ö��
}
void GetSendWnd()	//������ԭ����GetSendWnd�ˣ�����g_dwTargetPidö�ٴ��ڲ���
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
	for(int j=0; j<2; j++)		//����һ�ν�ɫ1���ڶ��ν�ɫ2����
	{
		id ^= 0x6E6E6E6E;		//��¼��ID��ԭʼ����^= 0x6E6E6E6E����
		
		//�ڶ�Ա��Ѱ��
		map<DWORD, DWORD>::iterator it;
		it = g_member.find(id);
		if(it != g_member.end())
		{
			Sleep(g_nDelay*1000);
			VsID = it->second;
			SendPacket(lpMember, sizeof(lpMember), s, VsID, 7);
			_endthread();
		}
		

		//�ڶӳ���Ѱ��
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

		id ^= 0x6E6E6E6E;		//��ԭ������ȡ��ɫ2
		id += 50000000;	
	}

	_endthread();
}


LRESULT CALLBACK ClientProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)	//����ID����Ϣ
{
	map_id::iterator i;
	t_IDSOCKET id = {0};
	BOOL bReconnect = FALSE;
	switch(Msg)
	{ 
	case WM_COPYDATA:			//�ʺ���Ϣ
		id =  *((t_IDSOCKET*) (((COPYDATASTRUCT*)lParam)->lpData) );
		i = g_id.find(id.id);
		if(i != g_id.end())
		{
			g_id.erase(i);		//�������ӵģ�ɾ����˳����Լ��ϵ������鹦��
			bReconnect = TRUE;	//���������־
		}
		g_id.insert(map_id::value_type(id.id,id.s ) );
		if(bReconnect && g_bReteam)
		{
			_beginthread(ReTeam,0, (void*)id.id);		//����������g_id.erase(i);������ã���Ϊ������SOCKET��û������ȥ
			bReconnect = FALSE;
		}
		SendMessage(g_hwndMain, WM_UPDATELIST, 0, 0);	//����Dlg��ID�б�
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
	WndClass.lpfnWndProc = (WNDPROC)ClientProc;		//�ص�����
	WndClass.lpszClassName = "BBLoader.ClientClass";
	WndClass.hInstance = hCurProcess;

	if ( !RegisterClassA(&WndClass) )
		MessageBoxA(NULL, "ע��BBLoader.ClientClassʧ�ܣ�", "Error", MB_OK);
	g_hwndClient = CreateWindowEx(0, "BBLoader.ClientClass", "BBLoader.Client", 0, 0, 0, 0, 0, 0, 0, hCurProcess, 0);
	SetClientHwnd(g_hwndClient);	//����InitInstance���ʱ��������˫�̣߳��������߳�ִ������ʱ������Ĵ��뻹ûִ����
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

void InstallBBDll(HANDLE hProcess)	//inline�����ɱ�����ط�����
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
void BBOpen()				//�����������࿪�����ˣ��࿪����lpk.dll
{
	TCHAR *lpProcessName = _T("wlvipcn.exe");
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi;

	
	if ( !CreateProcess(lpProcessName,NULL,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS, NULL,NULL, &si, &pi) )
	{
		__ErrShow();
		return ;
	}
	if(WaitForInputIdle(pi.hProcess, INFINITE))	//��ѹ���
	{
		__ErrShow();
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return;
	}

	g_dwTargetPid =	pi.dwProcessId;	//����dll�м����Ƿ���Ŀ�����
	SetTargetPid(g_dwTargetPid);	//������laodlibary֮ǰ���ã�
	InstallBBDll(pi.hProcess);		//��װhook dll

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
		__ShowMessage("Ư������ֻ�ܿ�һ����");
		return FALSE;
	}

	BBOpen();				//����BB
	InstallClientWnd();		//��װClient���ڣ����ڽ���Ŀ�����Send���ڷ�����������
	CDlg dlg;
	m_pMainWnd	= &dlg;
	dlg.DoModal();

	if(g_hMutex != NULL)
		::ReleaseMutex(g_hMutex);

	return FALSE;
}
