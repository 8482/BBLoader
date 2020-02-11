#if !defined(AFX_PAGE2_H__DD92778A_CA9E_4431_AFF0_CFA938382D19__INCLUDED_)
#define AFX_PAGE2_H__DD92778A_CA9E_4431_AFF0_CFA938382D19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2 dialog

class CPage2 : public CDialog
{
// Construction
public:
	BOOL SaveFilter(LPCTSTR lpFileName);
	BOOL ExtractFile(LPCTSTR lpFileName );
	CPage2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPage2)
	enum { IDD = IDD_PAGE2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage2)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListFilter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonSelall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE2_H__DD92778A_CA9E_4431_AFF0_CFA938382D19__INCLUDED_)
