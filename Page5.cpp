#include "StdAfx.h"
#include "BBLoader.h"
#include "Page5.h"

#include <map>
#include <winsock2.h>


using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPage5::CPage5(CWnd* pParent /*=NULL*/)
	: CDialog(CPage5::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPage1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	Create(CPage1::IDD,pParent);
}


void CPage5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage5, CDialog)
	//{{AFX_MSG_MAP(CPage1)
	//ON_BN_CLICKED(IDC_RETEAM, OnReteam)
	//ON_EN_CHANGE(IDC_DELAY, OnChangeDelay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1 message handlers

BOOL CPage5::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if ((VK_ESCAPE == pMsg->wParam) || (VK_RETURN == pMsg->wParam))
		{
			return   TRUE;
		}  // ÆÁ±Î  EscºÍ »Ø³µ  

	}
	return CDialog::PreTranslateMessage(pMsg);
}