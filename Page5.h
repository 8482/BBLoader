#if !defined(AFX_PAGE5_H__624DE302_00ED_4CE3_A0BC_A336B1E5D384)
#define AFX_PAGE5_H__624DE302_00ED_4CE3_A0BC_A336B1E5D384

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPage5 : public CDialog
{
public:
	CPage5(CWnd* pParent = NULL);
	
	//~CPage5();
	enum { IDD = IDD_PAGE5 };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

};

#endif