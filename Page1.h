#if !defined(AFX_PAGE1_H__E1A4507C_6F49_4A0A_A90B_75FD0411A035__INCLUDED_)
#define AFX_PAGE1_H__E1A4507C_6F49_4A0A_A90B_75FD0411A035__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1 dialog

class CPage1 : public CDialog
{
// Construction
public:
	CPage1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPage1)
	enum { IDD = IDD_PAGE1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage1)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage1)
	virtual void OnOK();
	afx_msg void OnReteam();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDelay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE1_H__E1A4507C_6F49_4A0A_A90B_75FD0411A035__INCLUDED_)
