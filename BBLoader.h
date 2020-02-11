// BBLoader.h : main header file for the BBLOADER application
//

#if !defined(AFX_BBLOADER_H__C95FCAFE_30B5_482A_9A7C_7E687E9103BC__INCLUDED_)
#define AFX_BBLOADER_H__C95FCAFE_30B5_482A_9A7C_7E687E9103BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <winsock2.h>

#include "dll/BBLoader.h"
#pragma comment(lib,"dll/BBLoader.lib")
/////////////////////////////////////////////////////////////////////////////
// CApp:
// See BBLoader.cpp for the implementation of this class
//

class CApp : public CWinApp
{
public:
	CApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#define WM_UPDATELIST	WM_USER+111
#define WM_CHANGEOKBTN	WM_USER+112

extern void SendPacket(const char *lpPacket, int len, SOCKET s, DWORD VsID = 0, int iPosition = 0);
extern void __ShowMessage(LPCTSTR lpInfo);
extern void __ErrShow();
extern BOOL IsSendWndAlive();
extern SOCKET GetSocket(DWORD id);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BBLOADER_H__C95FCAFE_30B5_482A_9A7C_7E687E9103BC__INCLUDED_)
