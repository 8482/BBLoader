#if !defined(AFX_PAGE4_H__A6F0ABD2_718F_459D_A547_9B6E6AD8E81C__INCLUDED_)
#define AFX_PAGE4_H__A6F0ABD2_718F_459D_A547_9B6E6AD8E81C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage4 dialog

class CPage4 : public CDialog
{
// Construction
public:
	CPage4(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPage4)
	enum { IDD = IDD_PAGE4 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage4)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage4)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE4_H__A6F0ABD2_718F_459D_A547_9B6E6AD8E81C__INCLUDED_)
