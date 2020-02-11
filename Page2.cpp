// Page2.cpp : implementation file
//

#include "stdafx.h"
#include "BBLoader.h"
#include "Page2.h"

#include <map>
#include <winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2 dialog


CPage2::CPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CPage2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	Create(CPage2::IDD,pParent);
}


void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2, CDialog)
	//{{AFX_MSG_MAP(CPage2)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILTER, OnClickListFilter)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SELALL, OnButtonSelall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2 message handlers

BOOL CPage2::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)   
	{   
		if((VK_ESCAPE== pMsg->wParam) || (VK_RETURN == pMsg->wParam)) 
		{   
			return   TRUE;   
		}  // ����  Esc�� �س�  
		
	}   	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPage2::OnInitDialog() 
{
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_FILTER);
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle()|LVS_EX_CHECKBOXES);	//CHECKBOX��ʽ
	pListCtrl->SetColumnWidth(0,200);

	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage2::OnClickListFilter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)	// pNMListView->iSubItem ��
	{
		CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_FILTER);
		if(pListCtrl->GetCheck(pNMListView->iItem))
			pListCtrl->SetCheck(pNMListView->iItem, FALSE);
		else
			pListCtrl->SetCheck(pNMListView->iItem, TRUE);
	}
	*pResult = 0;
}
struct t_MemFilter
{
	char	*lpName;
	char	bySearch[500];
	char	byModify[500];
	WORD	wSearchPosition[500];
	WORD	wModifyPosition[500];
	WORD	wTotalSearch;
	WORD	wTotalModify;
	BOOL	bWork;
	DWORD	dwMaxSearch;
	DWORD	dwReserved;
	BOOL	bAdv;
	DWORD	dwFunMask;
	BOOL	bAdvFlags;
	BOOL	bBlock;
	DWORD	dwTimes;
};
t_MemFilter g_filter[40] = {0};

BOOL CPage2::SaveFilter(LPCTSTR lpFileName)
{
	BOOL bRet = TRUE;
	HANDLE hFile = CreateFile(lpFileName,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{	
		__ErrShow();
		return FALSE;
	}
	TRY{
		DWORD	j  = 0;
		DWORD	nNumberOfBytes;
		char	RightPaswd[13] = {0};
		ReadFile(hFile, RightPaswd, 13, &nNumberOfBytes, NULL);
		j = SetFilePointer(hFile, j+13, NULL, FILE_BEGIN);
		if(memcmp(RightPaswd, "RightPassword", 13))
		{
			__ShowMessage("�ݲ�֧�ֺ��������flt�ļ���");
			CloseHandle(hFile);
			return FALSE;
		}
		CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_FILTER);
		for(int i=0; i<40; i++)
		{
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);//������i��ʾ��ţ�dwNo��ֱ��������

			//lpName
			DWORD dwNameLen;
			ReadFile(hFile, &dwNameLen, 4, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);	
			g_filter[i].lpName = new char[dwNameLen+1];
			memset(g_filter[i].lpName, 0, dwNameLen+1);
			ReadFile(hFile, g_filter[i].lpName, dwNameLen, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+dwNameLen, NULL, FILE_BEGIN);
			pListCtrl->InsertItem(i, g_filter[i].lpName);	//��ʾ

			//wTotalSearch,bySearch,wSearchPosition
			ReadFile(hFile, &g_filter[i].wTotalSearch, 2, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+2, NULL, FILE_BEGIN);
			for(WORD is=0; is<g_filter[i].wTotalSearch; is++)
			{
				BYTE bySearch[3] = {0};
				ReadFile(hFile, bySearch, 3, &nNumberOfBytes, NULL);
				j = SetFilePointer(hFile, j+3, NULL, FILE_BEGIN);
				g_filter[i].wSearchPosition[is] = *((WORD*)bySearch);
				g_filter[i].bySearch[*((WORD*)bySearch)] = bySearch[2];//Ҫ��λ��һһ��Ӧ����
			}
			
			//wTotalModify,byModify,wModifyPosition
			ReadFile(hFile, &g_filter[i].wTotalModify, 2, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+2, NULL, FILE_BEGIN);
			for(WORD im=0; im<g_filter[i].wTotalModify; im++)
			{
				BYTE byModify[3] = {0};
				ReadFile(hFile, byModify, 3, &nNumberOfBytes, NULL);
				j = SetFilePointer(hFile, j+3, NULL, FILE_BEGIN);
				g_filter[i].wModifyPosition[im] = *((WORD*)byModify);
				g_filter[i].byModify[*((WORD*)byModify)] = byModify[2];
			}

			//dwMaxSearch
			BOOL bMax;	//û�й�ѡdwMaxSearchѡ��ʱ��Ĭ��Ϊ0xFFFFFFFF�������ļ������ȷʵ0xFA���ʶ������˸����
			ReadFile(hFile, &bMax, 4, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);
			ReadFile(hFile, &g_filter[i].dwMaxSearch, 4, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);
			if(!bMax)
				g_filter[i].dwMaxSearch = 0xFFFFFFFF;

			//dwReserved
			ReadFile(hFile, &g_filter[i].dwReserved, 4, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);

			//bAdv		BYTE
			ReadFile(hFile, &g_filter[i].bAdv, 1, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+1, NULL, FILE_BEGIN);

			//dwFunMask
			ReadFile(hFile, &g_filter[i].dwFunMask, 4, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);

			//bAdvFlags	BYTE
			ReadFile(hFile, &g_filter[i].bAdvFlags, 1, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+1, NULL, FILE_BEGIN);

			//bBlock
			ReadFile(hFile, &g_filter[i].bBlock, 4, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);

			//dwTimes
			ReadFile(hFile, &g_filter[i].dwTimes, 4, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);
		}
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
		bRet = FALSE;
	}
	END_CATCH
	CloseHandle(hFile);	

	return bRet;
}

BOOL CPage2::ExtractFile(LPCTSTR lpFileName)
{
	BOOL bRet = TRUE;
	HANDLE	hFile	= CreateFile(lpFileName,GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{	
		__ErrShow();
		return FALSE;
	}
	TRY{
		BYTE	DL = 0;
		BYTE	BL = 0x1C;
		DWORD	j  = 0;
		DWORD	nNumberOfBytes;
		DWORD	dwFileSize = GetFileSize(hFile, NULL);
		for(int i=0; i<dwFileSize; i++)
		{
			ReadFile(hFile, &DL, 1, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j, NULL, FILE_BEGIN);
			DL ^= BL;
			BL += 1;
			WriteFile(hFile,&DL, 1, &nNumberOfBytes, NULL);
			j = SetFilePointer(hFile, j+1, NULL, FILE_BEGIN);
		}
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
		bRet = FALSE;
	}
	END_CATCH
	CloseHandle(hFile);
	
	return bRet;
}

void CPage2::OnOK() 
{
	if(0 == ((CListCtrl*)GetDlgItem(IDC_LIST_FILTER))->GetItemCount())
	{
		__ShowMessage("û�м����˾���");
		return;
	}
	if(!GetFilterState())
	{
		SetFilterState(TRUE);
		((CListCtrl*)GetDlgItem(IDC_LIST_FILTER))->EnableWindow(FALSE);
		SetDlgItemText(IDOK, "ֹͣ");

		//����bWork
		CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_FILTER);
		for(int i=0; i<40; i++)
		{
			g_filter[i].bWork = pListCtrl->GetCheck(i);
		}

		for(int i=0; i<40; i++)
		{
			//�Է�4���ֽڵĶ�����һ����ʱ����
			DWORD wbMax_wTotalSearch	= g_filter[i].dwMaxSearch;
			DWORD wRese_wTotalModify	= g_filter[i].dwReserved;
			wbMax_wTotalSearch	= g_filter[i].wTotalSearch;
			wRese_wTotalModify	= g_filter[i].wTotalModify;

			SetFilter(i,
					&(g_filter[i].bySearch[0]),
					&(g_filter[i].byModify[0]),
					&(g_filter[i].wSearchPosition[0]),
					&(g_filter[i].wModifyPosition[0]),
					wbMax_wTotalSearch,
					wRese_wTotalModify,
					g_filter[i].bWork,
					g_filter[i].dwMaxSearch,
					g_filter[i].dwReserved,
					g_filter[i].bAdv,
					g_filter[i].dwFunMask,
					g_filter[i].bAdvFlags,
					g_filter[i].bBlock,
					g_filter[i].dwTimes
					);
		}
	}
	else
	{
		SetFilterState(FALSE);
		((CListCtrl*)GetDlgItem(IDC_LIST_FILTER))->EnableWindow(TRUE);
		SetDlgItemText(IDOK, "��ʼ");
	}

}

void CPage2::OnButtonAdd() 
{
	if(GetFilterState())
		return;
	CFileDialog dlg(TRUE,"ѡ��wpe��flt�ļ�","*.flt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"wpe��flt�ļ�(*.flt)\0*.*\0\0");
	dlg.m_ofn.Flags = OFN_EXPLORER|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
	if(IDCANCEL == dlg.DoModal())
		return;

	((CListCtrl*)GetDlgItem(IDC_LIST_FILTER))->DeleteAllItems();

	for(int i=0; i<40; i++)
	{
		if(g_filter[i].lpName)
			delete g_filter[i].lpName;
	}
	memset(g_filter, 0, sizeof(t_MemFilter)*40);

	if(!ExtractFile(dlg.GetPathName()))//����
		return;
	SaveFilter(dlg.GetPathName());
	if(!ExtractFile(dlg.GetPathName()))//��ԭ
		return;
}

void CPage2::OnButtonSelall() 
{
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_FILTER);
	if(GetFilterState())
		return;
	if(0 == pListCtrl->GetItemCount())
		return;
	CString str;
	GetDlgItemText(IDC_BUTTON_SELALL, str);
	if(!str.CompareNoCase("ȫѡ"))
	{
		for(int i=0; i<pListCtrl->GetItemCount(); i++)
		{
			pListCtrl->SetCheck(i, TRUE);
		}
		SetDlgItemText(IDC_BUTTON_SELALL,"ȫ��");
	}
	else
	{
		for(int i=0; i<pListCtrl->GetItemCount(); i++)
		{
			pListCtrl->SetCheck(i, FALSE);
		}	
		SetDlgItemText(IDC_BUTTON_SELALL,"ȫѡ");
	}
	
}

