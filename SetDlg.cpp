// SetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bbloader.h"
#include "SetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog


CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDlg)
	m_bEver = FALSE;
	m_strPacketName = _T("");
	m_nSleep = 0;
	m_nTimes = 0;
	m_dwID = 0;
	//}}AFX_DATA_INIT
}


void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDlg)
	DDX_Check(pDX, IDC_EVER, m_bEver);
	DDX_Text(pDX, IDC_PACKET, m_strPacketName);
	DDX_Text(pDX, IDC_SLEEP, m_nSleep);
	DDX_Text(pDX, IDC_TIMES, m_nTimes);
	DDX_Text(pDX, IDC_ID, m_dwID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDlg message handlers

void CSetDlg::OnOK() 
{
	UpdateData(1);
	CDialog::OnOK();
}

BOOL CSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
