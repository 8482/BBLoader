// DlgAttch.cpp : implementation file
//

#include "stdafx.h"
#include "bbloader.h"
#include "DlgAttch.h"
#include <Tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAttch dialog


CDlgAttch::CDlgAttch(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAttch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAttch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAttch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAttch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAttch, CDialog)
	//{{AFX_MSG_MAP(CDlgAttch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAttch message handlers

BOOL CDlgAttch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PROCESS);	
	pListCtrl->InsertColumn(0,"½ø³ÌÃû");
	pListCtrl->InsertColumn(1,"PID");
	pListCtrl->SetColumnWidth(0,120);
	pListCtrl->SetColumnWidth(1,40);
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	PROCESSENTRY32 pe32 = {0}; 
	pe32.dwSize	= sizeof(PROCESSENTRY32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) 
	{
		__ErrShow();
		return TRUE;
	}
	if(Process32First(hProcessSnap, &pe32))
	{
		CString str;
		int	i = 0;
		do
		{
			pListCtrl->InsertItem(i,"");
			pListCtrl->SetItemText(i, 0, pe32.szExeFile);
			str.Empty();
			str.Format("%d",pe32.th32ProcessID);
			pListCtrl->SetItemText(i, 1, str);
		}while (Process32Next(hProcessSnap, &pe32)); 
	}
	CloseHandle (hProcessSnap); 
	return TRUE; 
}

void CDlgAttch::OnOK() 
{
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PROCESS);
	int i = pListCtrl->GetSelectionMark();
	CString str = pListCtrl->GetItemText(i, 1);
	m_dwPid = atoi(str);

	CDialog::OnOK();
}
