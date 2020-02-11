// Page3.cpp : implementation file
//

#include "stdafx.h"
#include "BBLoader.h"
#include "Page3.h"


#include <process.h>
#include <map>
#include <winsock2.h>
#include <vector>
#include "SetDlg.h"
#include "Dlg.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3 dialog


CPage3::CPage3(CWnd* pParent /*=NULL*/)
: CDialog(CPage3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPage3)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//	Create(CPage3::IDD,pParent);
}


void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3, CDialog)
//{{AFX_MSG_MAP(CPage3)
ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SEND, OnClickListSend)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_MESSAGE(WM_CHANGEOKBTN,OnChangeOKBtn)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PACKET, OnClickListPacket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3 message handlers

BOOL CPage3::PreTranslateMessage(MSG* pMsg) 
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

struct t_packet
{
//	int		nIndex;	//ListCtrl������,��ʵ�����±�,��ʡ��
	int		len;	//buf����
	BYTE*	buf;	//
};

class CSend
{
public:
	CSend(){};
	static void __cdecl  ThreadSend(void* pcsend);
	void Start();
	void Stop();
public:
	DWORD	id;
//	SOCKET	s;
	int		nIndex;
	DWORD	nSleep;
	DWORD	nTimes;
	BOOL	bEver;
	BOOL	bStop;

};
void CSend::Start()
{
	bStop = FALSE;
	_beginthread(ThreadSend, 0 , this);
}
void CSend::Stop()
{
	bStop = TRUE;
}

vector<t_packet> g_packet;
vector<CSend*> g_send;	

extern HWND		g_hwndSend;
extern CDlg*	g_MainDlg;
extern DWORD	g_nDelay;
void __cdecl  CSend::ThreadSend(void* pcsend)
{
	CSend* pcs = (CSend*)pcsend;
	int		i = pcs->nIndex;
//	SOCKET	s = GetSocket(pcs->id);			//����֧�ֵ�������

	char	*lpBuf;
	int		len = g_packet[i].len;
	len		+= 8;
	lpBuf	= new char[len];
	len		-= 8;
//	memcpy(lpBuf,	&s,		4);	//SOCKET
	memcpy(lpBuf+4,	&len,	4);	//len
	memcpy(lpBuf+8, g_packet[i].buf, g_packet[i].len);	//buf

	COPYDATASTRUCT cds = {0};
	cds.cbData	= len+8;
	cds.lpData	= lpBuf;
	
	i = 0;
	while((!pcs->bStop) && (pcs->bEver || pcs->nTimes))
	{
		SOCKET s = GetSocket(pcs->id);	//����������������
		memcpy(lpBuf, &s, 4);
		::SendMessage(g_hwndSend,WM_COPYDATA,(WPARAM)g_hwndSend,(LPARAM)&cds);		
		Sleep(pcs->nSleep);
		if(pcs->bEver)
			continue;
		else if(++i >= pcs->nTimes)
			pcs->bStop = TRUE;
			//break;
	}
	delete lpBuf;

	HWND hSendDlg = g_MainDlg->m_sheet.GetPage(2)->m_hWnd;
	SendMessage(hSendDlg, WM_CHANGEOKBTN, pcs->id, pcs->bStop);//�ı䡰��ʼ����ť,WPARAMΪid, LPARAMΪbStop

	_endthread();
}

void CPage3::OnOK() 
{
	
	if(!IsSendWndAlive())
	{
		__ShowMessage("���������Ѿ��˳�������������������򸽼��´򿪵ı���");
		return;
	}

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SEND);
	int i = pListCtrl->GetSelectionMark();
	if(-1 == i)
	{
		__ShowMessage("��ѡ��һ�������˺�!");
		return;
	}
	CSend *pcs = g_send[i];

	if(pcs->bStop)	//��û�з���
	{
		SetDlgItemText(IDOK,"ֹͣ");
		pcs->Start();
	}
	else
	{
		SetDlgItemText(IDOK,"��ʼ");
		pcs->Stop();
	}

}

BOOL CPage3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//��ʼ������б�
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PACKET);
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle()|LVS_EX_CHECKBOXES);	//CHECKBOX��ʽ
	pListCtrl->SetColumnWidth(0,200);
	
	//��ʼ�������б�
	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SEND);
	pListCtrl->InsertColumn(0,"�˺�");
	pListCtrl->InsertColumn(1,"S");
	pListCtrl->InsertColumn(2,"T");
	pListCtrl->InsertColumn(3,"�������");
	pListCtrl->SetColumnWidth(0,50);
	pListCtrl->SetColumnWidth(1,38);
	pListCtrl->SetColumnWidth(2,25);
	pListCtrl->SetColumnWidth(3,75);
	//ѡ��һ��
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	COLORREF cr = 0x00ff0000;
	pListCtrl->SetTextColor(cr);

	return TRUE;
}

void CPage3::OnOpen() 
{
	CFileDialog dlg(TRUE, "ѡ��wpe��spt�ļ�", "*.spt");
	dlg.m_ofn.lpstrFilter = "wpe��spt�ļ�(*.spt)\0*.*\0\0";
	dlg.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	dlg.m_ofn.Flags = OFN_EXPLORER|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
	if(IDCANCEL == dlg.DoModal())
		return;
	CString strPath = dlg.GetPathName();
	HANDLE	hFile	= CreateFile(strPath,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{	
		__ErrShow();
		return;
	}

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PACKET);

	TRY{
		DWORD nNumberOfBytesRead = 0;
		DWORD nTotalSPT = 0;
		ReadFile(hFile, &nTotalSPT, 4, &nNumberOfBytesRead, NULL);	//ReadFile��ȡ���ļ�ָ����ָ����ȡ���ݵ�ĩβ��
		DWORD j = SetFilePointer(hFile, 4, NULL, FILE_BEGIN);
	
		DWORD	nBytesRead = 0;
		char	szName[256] = {0};
		int iItemCount = pListCtrl->GetItemCount();//�������б�ĩβ���
		for( int i=0; i<nTotalSPT; i++)	
		{
			ReadFile(hFile,&nBytesRead, 4, &nNumberOfBytesRead, NULL);			//��ȡnBytesRead
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);
			
			memset(szName,0,sizeof(szName));	//!
			ReadFile(hFile,szName,	nBytesRead, &nNumberOfBytesRead, NULL);		//����nBytesRead��ȡ
			j = SetFilePointer(hFile, j+nBytesRead, NULL, FILE_BEGIN);

			pListCtrl->InsertItem(iItemCount++, szName);

			ReadFile(hFile,&nBytesRead, 4, &nNumberOfBytesRead, NULL);			//��ȡnBytesRead
			j = SetFilePointer(hFile, j+4, NULL, FILE_BEGIN);
	
			BYTE *buf = new BYTE[nBytesRead];
			t_packet tmp;
//			tmp.nIndex	= i;
			tmp.len		= nBytesRead;
			tmp.buf		= buf;
			g_packet.push_back(tmp);

			ReadFile(hFile, buf,	nBytesRead, &nNumberOfBytesRead, NULL);		//����nBytesRead��ȡ
			j = SetFilePointer(hFile, j+nBytesRead, NULL, FILE_BEGIN);
		}
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	END_CATCH

	if(hFile)
		CloseHandle(hFile);
}

void CPage3::OnAdd() 
{
	CWnd* pParent = GetParent()->GetParent();
	DWORD id = pParent->GetDlgItemInt(IDC_IDLIST);
	SOCKET s = GetSocket(id);
	if(s == NULL)
	{
		__ShowMessage("��ǰ�˺���Ч��");
		return;
	}

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PACKET);
	int nIndex = 0; 
	while(nIndex < pListCtrl->GetItemCount())
	{
		if(pListCtrl->GetCheck(nIndex))
			break;
		nIndex++;
	}
	if(nIndex == pListCtrl->GetItemCount())
	{
		__ShowMessage("��û�й�ѡ�����");
		return;
	}

	CSetDlg dlg;
	CString str = pListCtrl->GetItemText(nIndex, 0);
	dlg.m_strPacketName = str;
	dlg.m_dwID			= id;
	dlg.m_nSleep		= 1000;
	dlg.m_nTimes		= 1;
	dlg.m_bEver			= FALSE;	
	if(	IDCANCEL == dlg.DoModal())
		return;

	//��ӵ������б�
	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SEND);
	int i = pListCtrl->GetItemCount();
	pListCtrl->InsertItem(i,"");
	str.Empty();
	str.Format("%d",dlg.m_dwID);
	pListCtrl->SetItemText(i, 0, str);
	str.Empty();
	str.Format("%d",dlg.m_nSleep);
	pListCtrl->SetItemText(i, 1, str);
	str.Empty();
	if(dlg.m_bEver)
		str = "-1";
	else
		str.Format("%d",dlg.m_nTimes);
	pListCtrl->SetItemText(i, 2, str);
	pListCtrl->SetItemText(i, 3, dlg.m_strPacketName);
	pListCtrl->SetSelectionMark(i);

	CSend* pcs	= new CSend;
	pcs->bEver	= dlg.m_bEver;
	pcs->id		= id;
	pcs->nIndex	= nIndex;
	pcs->nSleep	= dlg.m_nSleep;
	pcs->nTimes	= dlg.m_nTimes;
//	pcs->s		= s;
	pcs->bStop	= TRUE;

	g_send.push_back(pcs);
}

void CPage3::OnClickListSend(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)	// pNMListView->iSubItem ��
	{
		CSend* pcs = g_send[pNMListView->iItem];
		if(pcs->bStop)
			SetDlgItemText(IDOK,"��ʼ");
		else
			SetDlgItemText(IDOK,"ֹͣ");	
	}
	*pResult = 0;
}

void CPage3::OnClear() 
{
	if(IDCANCEL == MessageBox("�����б���������ݶ�������գ�ȷ����","����",MB_OKCANCEL|MB_ICONWARNING))
		return;

	//������
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SEND);
	pListCtrl->DeleteAllItems();
	for(int i=0; i<g_send.size(); i++)
	{
		delete g_send[i];
	}
	g_send.clear();

	//����������
	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PACKET);
	pListCtrl->DeleteAllItems();
	for(int i = 0; i<g_packet.size(); i++)
	{
		delete g_packet[i].buf;
	}
	g_packet.clear();
}

LRESULT CPage3::OnChangeOKBtn(WPARAM wParam, LPARAM lParam)
{
	DWORD	id		= (DWORD)wParam;
	BOOL	bStop	= (BOOL)lParam;

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SEND);
	int i = pListCtrl->GetSelectionMark();
	if(-1 == i)
	{
		return 0;
	}
	CString strCurID;
	strCurID = pListCtrl->GetItemText(i,0);
	CString strRecvID;
	strRecvID.Format("%d",id);
	if(strCurID.CompareNoCase(strRecvID))
		return 0;
	if(bStop)
		SetDlgItemText(IDOK,"��ʼ");
	else
		SetDlgItemText(IDOK,"ֹͣ");
	return 0;
}

void CPage3::OnModify() 
{
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SEND);
	int i = pListCtrl->GetSelectionMark();
	if(-1 == i)
	{
		__ShowMessage("��ѡ��һ�������˺�!");
		return;
	}

	CSend *pcs = g_send[i];
	if(!(pcs->bStop))
	{
		__ShowMessage("����ֹͣ�������޸ģ�");
		return;
	}

	CSetDlg dlg;
	CString str = pListCtrl->GetItemText(i, 3);
	dlg.m_strPacketName	= str;
	dlg.m_dwID			= pcs->id;
	dlg.m_nSleep		= pcs->nSleep;
	dlg.m_nTimes		= pcs->nTimes;
	dlg.m_bEver			= pcs->bEver;

	if(	IDCANCEL == dlg.DoModal())
		return;

	//��Ҫ���������
	pcs->nSleep	= dlg.m_nSleep;
	pcs->nTimes	= dlg.m_nTimes;
	pcs->bEver	= dlg.m_bEver;
	
	str.Empty();
	str.Format("%d", pcs->nSleep);
	pListCtrl->SetItemText(i, 1, str);
	str.Empty();
	if(pcs->bEver)
		str = "-1";
	else
		str.Format("%d", pcs->nTimes);
	pListCtrl->SetItemText(i, 2, str);
	pListCtrl->SetSelectionMark(i);
}

void CPage3::OnDel() 
{
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_SEND);
	int i = pListCtrl->GetSelectionMark();
	if(-1 == i)
	{
		__ShowMessage("��ѡ��һ�������˺�!");
		return;
	}
	CSend* pcs = g_send[i];
	if(!(pcs->bStop))
	{
		__ShowMessage("����ֹͣ������ɾ����");
		return;
	}
	
	delete pcs;
	g_send.erase(g_send.begin()+i);
	pListCtrl->DeleteItem(i);
}

void CPage3::OnClickListPacket(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)	// pNMListView->iSubItem ��
	{
		CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PACKET);
		if(pListCtrl->GetCheck(pNMListView->iItem))
			pListCtrl->SetCheck(pNMListView->iItem, FALSE);
		else
			pListCtrl->SetCheck(pNMListView->iItem, TRUE);
	}
	*pResult = 0;

}


