// Page4.cpp : implementation file
//

#include "stdafx.h"
#include "BBLoader.h"
#include "Page4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage4 dialog


CPage4::CPage4(CWnd* pParent /*=NULL*/)
	: CDialog(CPage4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPage4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage4)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage4, CDialog)
	//{{AFX_MSG_MAP(CPage4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage4 message handlers

BOOL CPage4::PreTranslateMessage(MSG* pMsg) 
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
const char lpPacket9[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xE9, 0xBC, 0xAD, 0x65, 0x6C, 0x6A, 0x00, 0x00, 0x00, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E};
const char lpPacket16[]= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xE9, 0xBC, 0xAD, 0x65, 0x6C, 0x6B, 0x00, 0x00, 0x00, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E};
const char lpPacket14[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0xE9, 0xA6, 0xAD, 0x6B, 0x7F, 0x6D, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E};//回游乐场
void CPage4::OnOK() 
{
	if(!IsSendWndAlive())
	{
		__ShowMessage("贝贝可能已经退出，请重新启动本程序或附加新打开的贝贝");
		return;
	}

	CWnd* pParent = GetParent()->GetParent();
	DWORD id	= (DWORD)pParent->GetDlgItemInt(IDC_IDLIST);	//当前ID
	SOCKET s = GetSocket(id);
	if(s == NULL)
	{
		__ShowMessage("当前账号无效！");
		return;
	}

	DWORD VsID	= (DWORD)GetDlgItemInt(IDC_ID);
	//角色2
	if(IsDlgButtonChecked(IDC_P4_ROLE))
	{
		VsID += 50000000;
	}
	VsID ^= 0x6E6E6E6E;

	//参战
	if(IsDlgButtonChecked(IDC_P4_R1))
	{
		SendPacket(lpPacket16, sizeof(lpPacket16), s, VsID, 7);
	}

	//观战
	if(IsDlgButtonChecked(IDC_P4_R2))
	{
		SendPacket(lpPacket9, sizeof(lpPacket9), s, VsID, 7);
	}

	//回游乐场,不要ID
	if(IsDlgButtonChecked(IDC_P4_R3))
	{
		SendPacket(lpPacket14, sizeof(lpPacket14), s);
	}
}
