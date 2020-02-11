// Page1.cpp : implementation file
//

#include "stdafx.h"
#include "BBLoader.h"
#include "Page1.h"

#include <map>
#include <winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1 dialog


CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CPage1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPage1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	Create(CPage1::IDD,pParent);
}


void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
	//{{AFX_MSG_MAP(CPage1)
	ON_BN_CLICKED(IDC_RETEAM, OnReteam)
	ON_EN_CHANGE(IDC_DELAY, OnChangeDelay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1 message handlers

BOOL CPage1::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)   
	{   
		if((VK_ESCAPE== pMsg->wParam) || (VK_RETURN == pMsg->wParam)) 
		{   
			return   TRUE;   
		}  // 屏蔽  Esc和 回车  
		
	}   	
	return CDialog::PreTranslateMessage(pMsg);
}


using namespace std;
extern map<DWORD, SOCKET,DWORD> g_id;
extern void GetSendWnd();
extern HWND	g_hwndSend;

/*封包结构：
struct {
	SOCKET	s;
	int		len;
	char	buf[...];
}
*/
//组队封包,0x00 0x00 0x00是帐号，只三个字节
const char lpPacket2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xE9, 0xA2, 0xAD, 0x63, 0x6D, 0x6F, 0x00, 0x00, 0x00, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E};
const char lpPacket4[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xE9, 0xA2, 0xAD, 0x63, 0x66, 0x6F, 0x00, 0x00, 0x00, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E};

multimap<DWORD, DWORD>	g_leader;
map<DWORD, DWORD>		g_member;

void CPage1::OnOK() 
{
	if(!IsSendWndAlive())
	{
		__ShowMessage("贝贝可能已经退出，请重新启动本程序或附加新打开的贝贝");
		return;
	}

	CWnd* pParent = GetParent()->GetParent();
	DWORD id	= (DWORD)pParent->GetDlgItemInt(IDC_IDLIST);	//当前ID
	DWORD VsID	= (DWORD)GetDlgItemInt(IDC_TEAM);

	SOCKET s = GetSocket(id);
	if(s == NULL)
	{
		__ShowMessage("当前账号无效！");
		return;
	}

	//角色2
	if(IsDlgButtonChecked(IDC_P1_ROLE))
	{
		VsID += 50000000;
	}
	VsID	^= 0x6E6E6E6E;
	id		^= 0x6E6E6E6E;

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	map<DWORD,DWORD>::iterator it;
	multimap<DWORD, DWORD>::iterator mit;

	//加入对方队伍
	if(IsDlgButtonChecked(IDC_P1_R1))
	{
		//当前帐号为队员
		it = g_member.find(id);
		if(it != g_member.end())
		{
			g_member.erase(it);
		}
		g_member.insert(make_pair(id, VsID));

		//顺便把对方添加到队长的记录里
		mit = g_leader.find(VsID);
		while(mit != g_leader.end())
		{
			if(mit->first == VsID)
			{
				if(mit->second	== id)
					g_leader.erase(mit);
			}
			mit++;
		}
		g_leader.insert(make_pair(VsID, id));

		SendPacket(lpPacket4, sizeof(lpPacket4), s, VsID, 7);
	}

	//拉入我的队伍
	if(IsDlgButtonChecked(IDC_P1_R2))
	{
		//当前帐号为队长		
		mit = g_leader.find(id);
		while(mit != g_leader.end())
		{
			if(mit->first == id)
			{
				if(mit->second	== VsID)
					g_leader.erase(mit);
			}
			mit++;
		}
		g_leader.insert(make_pair(id, VsID));

		//顺便把对方添加到队员的记录里
		it = g_member.find(VsID);
		if(it != g_member.end())
		{
			g_member.erase(it);
		}
		g_member.insert(make_pair(VsID, id));

		SendPacket(lpPacket2, sizeof(lpPacket2), s, VsID, 7);
	}

	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

extern BOOL g_bReteam;
extern DWORD g_nDelay;
void CPage1::OnReteam() 
{	
	if(IsDlgButtonChecked(IDC_RETEAM))
		g_bReteam = TRUE;
	else
	{
		g_leader.clear();
		g_member.clear();
		g_bReteam = FALSE;
	}
}

BOOL CPage1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CheckDlgButton(IDC_RETEAM, BST_CHECKED);
	g_bReteam = TRUE;
	SetDlgItemInt(IDC_DELAY, g_nDelay);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage1::OnChangeDelay() 
{
	g_nDelay = GetDlgItemInt(IDC_DELAY);
}
