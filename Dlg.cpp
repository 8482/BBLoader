// Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "BBLoader.h"
#include "Dlg.h"
#include "DlgAttch.h"
//------------------------------------------
#include <map>
#include <winsock2.h>
UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));;
//------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg dialog

CDlg::CDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg)
		DDX_Control(pDX, IDC_TABSHEET, m_sheet);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CDlg, CDialog)
	//{{AFX_MSG_MAP(CDlg)
	ON_WM_PAINT()
	ON_MESSAGE(WM_UPDATELIST, UpdateIDList)
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TOPWIN, OnTopwin)
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	ON_COMMAND(ID_MENU_BBS, OnMenuBbs)
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(MYWM_NOTIFYICON, OnMyIconNotify)
	ON_COMMAND(ID_MENU_QUIT, OnMenuQuit)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreated)
	ON_COMMAND(ID_MENU_MULTI, OnMenuMulti)
	ON_COMMAND(ID_MENU_ATTACH, OnMenuAttach)
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg message handlers

extern HWND g_hwndMain;
CDlg* g_MainDlg;
BOOL CDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
//-------------------------------------------窗口移到右上角
	g_MainDlg = this;
	g_hwndMain = m_hWnd;
	CRect cr;

	this->GetWindowRect(&cr);
	MoveWindow( GetSystemMetrics(SM_CXSCREEN)	- cr.Width(),
				0,//GetSystemMetrics(SM_CYSCREEN)	- cr.Height(),
				cr.Width(),
				cr.Height()
				);

	m_sheet.AddPage("组队", &m_page1, IDD_PAGE1);
	m_sheet.AddPage("战斗", &m_page4, IDD_PAGE4);
	m_sheet.AddPage("封包", &m_page3, IDD_PAGE3);
	m_sheet.AddPage("滤镜", &m_page2, IDD_PAGE2);
	m_sheet.AddPage("补品", &m_page5, IDD_PAGE5);
	m_sheet.Show();

	CheckDlgButton(IDC_TOPWIN, BST_CHECKED);
	SetWindowPos(&this->wndTopMost,		0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	//托盘
	m_tnid.cbSize = sizeof(NOTIFYICONDATA);
	m_tnid.hWnd = g_hwndMain;
	m_tnid.uID = IDR_MAINFRAME;
	m_tnid.uFlags = NIF_ICON | NIF_MESSAGE;
	m_tnid.uCallbackMessage = MYWM_NOTIFYICON;
	m_tnid.hIcon = m_hIcon;
	Shell_NotifyIcon(NIM_ADD, &m_tnid);

	m_bWndShow = TRUE;
//----------------------------------------
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//----------------------------------------------------------------------------------
#include <shellapi.h>
using namespace std;
extern map<DWORD, SOCKET> g_id;
extern map<DWORD, DWORD> g_inxd;;
LRESULT CDlg::UpdateIDList(WPARAM wParam, LPARAM lParam)	//现在只是看有没有了，没有就加上，而不是清空再添加了
{
	map<DWORD, SOCKET>::iterator i = g_id.begin();
	CComboBox* pCB		=  (CComboBox*)GetDlgItem(IDC_IDLIST);
	CComboBox* pcbTeam	=  (CComboBox*)m_sheet.GetPage(0)->GetDlgItem(IDC_TEAM);

	for(i; i!=g_id.end(); i++)
	{
		char text[10] = {0} ;
		sprintf(text, "%d", (*i).first);
		if(CB_ERR == pCB->FindString(-1, text))
			pCB->AddString(text);
		if(CB_ERR == pcbTeam->FindString(-1, text))
			pcbTeam->AddString(text);
		map<DWORD, DWORD>::iterator Li;
		Li = g_inxd.find((*i).first);
		if (Li == g_inxd.end())
		{
			g_inxd.insert(map<DWORD, DWORD>::value_type((*i).first,getrole((*i).first)));
		}
	}

	if(0 == GetDlgItemInt(IDC_IDLIST))	//选择下一个
		pCB->SetCurSel(pCB->GetCurSel()+1);
	if(0 == m_sheet.GetPage(0)->GetDlgItemInt(IDC_TEAM))	//选择下一个
		pcbTeam->SetCurSel(pcbTeam->GetCurSel()+1);
	return 0;
}

void CDlg::OnTopwin() 
{
	if(IsDlgButtonChecked(IDC_TOPWIN))
		SetWindowPos(&this->wndTopMost,		0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	else
		SetWindowPos(&this->wndNoTopMost,	0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
}

void CDlg::OnMenuAbout() 
{
	CString strAbout = CString((LPCSTR)IDS_STRING1);
	MessageBox(strAbout,"关于",MB_OK|MB_ICONINFORMATION);
}

BOOL CDlg::PreTranslateMessage(MSG* pMsg)// 屏蔽Esc和回车
{
	if(WM_KEYDOWN == pMsg->message)   
	{   
		if((VK_ESCAPE== pMsg->wParam) || (VK_RETURN == pMsg->wParam)) 
		{   
			return   TRUE;   
		}  	
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlg::OnMenuBbs() 
{
	ShellExecute(NULL,"open","http://lynn.5d6d.com",NULL,NULL,SW_SHOWNORMAL );
}


//图标回调消息MYWM_NOTIFYICON
LRESULT CDlg::OnMyIconNotify( WPARAM wParam, LPARAM lParam )
{
	UINT uMouseMsg = (UINT) lParam;

	if(uMouseMsg == WM_LBUTTONDOWN)
	{
		if( m_bWndShow ) 
		{
			ShowWindow(SW_HIDE );
			m_bWndShow = FALSE;
		}
		else
		{
			ShowWindow(SW_SHOW);
			m_bWndShow = TRUE;
		}
	}
	if(uMouseMsg == WM_RBUTTONDOWN)
	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU_QUIT);
		CMenu* pPop = menu.GetSubMenu(0);
		POINT p;
		GetCursorPos(&p);
		SetForegroundWindow();
		pPop->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
		menu.DestroyMenu();
	}
	return 0;
}


void CDlg::OnSysCommand(UINT nID, LPARAM lParam )
{
	if (nID == SC_CLOSE)
	{
		ShowWindow(SW_HIDE );
		m_bWndShow = FALSE;
	}
	else
		CWnd::OnSysCommand(nID, lParam );
}

void CDlg::OnMenuQuit() 
{
	if(IDOK ==  MessageBox("你确定要退出宝宝？","请确认",MB_OKCANCEL|MB_ICONQUESTION))
		OnCancel();
}

BOOL CDlg::DestroyWindow() 
{
	m_tnid.cbSize = sizeof(NOTIFYICONDATA);
	m_tnid.hWnd = g_hwndMain;
	m_tnid.uID = IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_DELETE, &m_tnid);
	
	return CDialog::DestroyWindow();
}
LRESULT CDlg::OnTaskBarCreated(WPARAM wParam, LPARAM lParam)//要按标准消息处理函数写，没有WPARAM wParam, LPARAM lParam会出错！
{														//因为ON_REGISTERED_MESSAGE这个宏的使用而导致了错误
	//若界面隐藏，则重新显示
	m_tnid.cbSize = sizeof(NOTIFYICONDATA);
	m_tnid.hWnd = g_hwndMain;
	m_tnid.uID = IDR_MAINFRAME;
	m_tnid.uFlags = NIF_ICON | NIF_MESSAGE;
	m_tnid.uCallbackMessage = MYWM_NOTIFYICON;
	m_tnid.hIcon = m_hIcon;
	Shell_NotifyIcon(NIM_ADD, &m_tnid);
	return 0;
}


void CDlg::OnMenuMulti() 
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	if(!CreateProcess("BBMulti.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		__ErrShow();
		return;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
extern  void InstallBBDll(HANDLE);
extern	DWORD g_dwTargetPid;
void CDlg::OnMenuAttach() 
{
	if(IsSendWndAlive())
	{
		if(IDCANCEL == MessageBox("当前有贝贝正在运行，如果附加到另一个贝贝，当前贝贝会脱离宝宝的支持，你确定要这样做吗？","警告",MB_OKCANCEL|MB_ICONWARNING))
			return;
	}

	CDlgAttch dlg;
	if(	IDCANCEL == dlg.DoModal())
		return;
	
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dlg.m_dwPid);
	if(NULL == hProcess)
	{
		__ErrShow;
		return;
	}
	g_dwTargetPid =	dlg.m_dwPid;
	SetTargetPid(g_dwTargetPid);
	InstallBBDll(hProcess);
	CloseHandle(hProcess);
	
}

void CDlg::OnMenuExit() 
{
	if(IDOK ==  MessageBox("你确定要退出宝宝？","请确认",MB_OKCANCEL|MB_ICONQUESTION))
		OnCancel();
}
