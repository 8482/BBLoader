// Dlg.h : header file
//

#if !defined(AFX_DLG_H__A77F7B35_C789_4387_B989_ADBA7507F224__INCLUDED_)
#define AFX_DLG_H__A77F7B35_C789_4387_B989_ADBA7507F224__INCLUDED_


#include "TabSheet.h"	// Added by ClassView
#include "Page1.h"	// Added by ClassView
#include "Page2.h"	// Added by ClassView
#include "Page3.h"	// Added by ClassView
#include "Page4.h"	// Added by ClassView
#include "Page5.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MYWM_NOTIFYICON WM_USER+1
/////////////////////////////////////////////////////////////////////////////
// CDlg dialog

class CDlg : public CDialog
{
// Construction
public:
	NOTIFYICONDATA m_tnid;
	CPage1 m_page1;
	CPage2 m_page2;
	CPage3 m_page3;
	CPage4 m_page4;
	CPage5 m_page5;

	CDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg)
	enum { IDD = IDD_BBLOADER_DIALOG };
	CTabSheet m_sheet;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	BOOL m_bWndShow;
	// Generated message map functions
	//{{AFX_MSG(CDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT UpdateIDList(WPARAM wParam, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTopwin();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuBbs();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam); //手工加
	afx_msg LRESULT OnMyIconNotify(WPARAM wParam, LPARAM lParam); //手工加
	afx_msg void OnMenuQuit();
	afx_msg LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuMulti();
	afx_msg void OnMenuAttach();
	afx_msg void OnMenuExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__A77F7B35_C789_4387_B989_ADBA7507F224__INCLUDED_)
