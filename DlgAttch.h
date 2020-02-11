#if !defined(AFX_DLGATTCH_H__6961E30C_432F_4BF9_8728_5A51A5DE45F1__INCLUDED_)
#define AFX_DLGATTCH_H__6961E30C_432F_4BF9_8728_5A51A5DE45F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAttch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAttch dialog

class CDlgAttch : public CDialog
{
// Construction
public:
	DWORD m_dwPid;
	CDlgAttch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAttch)
	enum { IDD = IDD_DIALOG_ATTACH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAttch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAttch)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGATTCH_H__6961E30C_432F_4BF9_8728_5A51A5DE45F1__INCLUDED_)
