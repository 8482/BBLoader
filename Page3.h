#if !defined(AFX_PAGE3_H__BFC15617_9719_447F_9D2B_DD6BD2F8C92F__INCLUDED_)
#define AFX_PAGE3_H__BFC15617_9719_447F_9D2B_DD6BD2F8C92F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3 dialog

class CPage3 : public CDialog
{
// Construction
public:
	CPage3(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPage3)
	enum { IDD = IDD_PAGE3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage3)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage3)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnOpen();
	afx_msg void OnAdd();
	afx_msg void OnClickListSend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClear();
	afx_msg LRESULT OnChangeOKBtn(WPARAM wParam, LPARAM lParam);
	afx_msg void OnModify();
	afx_msg void OnDel();
	afx_msg void OnClickListPacket(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE3_H__BFC15617_9719_447F_9D2B_DD6BD2F8C92F__INCLUDED_)
