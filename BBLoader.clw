; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "bbloader.h"
LastPage=0

ClassCount=8
Class1=CDlg
Class2=CPage1
Class3=CPage2
Class4=CPage3
Class5=CTabSheet

ResourceCount=9
Resource1=IDD_PAGE1
Resource2=IDR_MENU
Resource3=IDD_PAGE3
Resource4=IDD_PAGE2
Resource5=IDD_SETTING
Class6=CSetDlg
Resource6=IDD_BBLOADER_DIALOG
Resource7=IDD_PAGE4
Class7=CPage4
Resource8=IDD_DIALOG_ATTACH
Class8=CDlgAttch
Resource9=IDR_MENU_QUIT

[CLS:CDlg]
Type=0
BaseClass=CDialog
HeaderFile=Dlg.h
ImplementationFile=Dlg.cpp
LastObject=ID_MENU_MULTI
Filter=D
VirtualFilter=dWC

[CLS:CPage1]
Type=0
BaseClass=CDialog
HeaderFile=Page1.h
ImplementationFile=Page1.cpp
Filter=D
VirtualFilter=dWC
LastObject=CPage1

[CLS:CPage2]
Type=0
BaseClass=CDialog
HeaderFile=Page2.h
ImplementationFile=Page2.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST_FILTER

[CLS:CPage3]
Type=0
BaseClass=CDialog
HeaderFile=Page3.h
ImplementationFile=Page3.cpp
LastObject=CPage3
Filter=D
VirtualFilter=dWC

[CLS:CTabSheet]
Type=0
BaseClass=CTabCtrl
HeaderFile=TabSheet.h
ImplementationFile=TabSheet.cpp

[DLG:IDD_BBLOADER_DIALOG]
Type=1
Class=CDlg
ControlCount=4
Control1=IDC_IDLIST,combobox,1344339971
Control2=IDC_STATIC,button,1342177287
Control3=IDC_TOPWIN,button,1342242819
Control4=IDC_TABSHEET,SysTabControl32,1342177280

[DLG:IDD_PAGE1]
Type=1
Class=CPage1
ControlCount=9
Control1=IDC_P1_R1,button,1342308361
Control2=IDC_P1_R2,button,1342177289
Control3=IDOK,button,1342242817
Control4=IDC_TEAM,combobox,1344339971
Control5=IDC_P1_ROLE,button,1342242819
Control6=IDC_RETEAM,button,1342242819
Control7=IDC_DELAY,edit,1350639745
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_PAGE2]
Type=1
Class=CPage2
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_LIST_FILTER,SysListView32,1350631438
Control3=IDC_BUTTON_ADD,button,1342242816
Control4=IDC_BUTTON_SELALL,button,1342242816

[DLG:IDD_PAGE3]
Type=1
Class=CPage3
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_LIST_PACKET,SysListView32,1350631438
Control3=IDC_OPEN,button,1342242816
Control4=IDC_LIST_SEND,SysListView32,1350631437
Control5=IDC_ADD,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_CLEAR,button,1342242816
Control8=IDC_MODIFY,button,1342242816
Control9=IDC_DEL,button,1342242816

[MNU:IDR_MENU]
Type=1
Class=?
Command1=ID_MENU_MULTI
Command2=ID_MENU_ATTACH
Command3=ID_MENU_EXIT
Command4=ID_MENU_BBS
Command5=ID_MENU_ABOUT
CommandCount=5

[DLG:IDD_SETTING]
Type=1
Class=CSetDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_SLEEP,edit,1350639744
Control7=IDC_STATIC,static,1342308352
Control8=IDC_TIMES,edit,1350639744
Control9=IDC_EVER,button,1342242819
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_PACKET,edit,1350633600
Control13=IDC_ID,edit,1350633600

[CLS:CSetDlg]
Type=0
HeaderFile=SetDlg.h
ImplementationFile=SetDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_MENU_LYNN
VirtualFilter=dWC

[MNU:IDR_MENU_QUIT]
Type=1
Class=?
Command1=ID_MENU_QUIT
CommandCount=1

[DLG:IDD_PAGE4]
Type=1
Class=CPage4
ControlCount=6
Control1=IDC_P4_R1,button,1342308361
Control2=IDC_P4_R2,button,1342177289
Control3=IDC_P4_R3,button,1342177289
Control4=IDOK,button,1342242817
Control5=IDC_ID,edit,1350639744
Control6=IDC_P4_ROLE,button,1342242819

[CLS:CPage4]
Type=0
HeaderFile=Page4.h
ImplementationFile=Page4.cpp
BaseClass=CDialog
Filter=D
LastObject=CPage4
VirtualFilter=dWC

[DLG:IDD_DIALOG_ATTACH]
Type=1
Class=CDlgAttch
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_PROCESS,SysListView32,1350631437

[CLS:CDlgAttch]
Type=0
HeaderFile=DlgAttch.h
ImplementationFile=DlgAttch.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_MENU_EXIT
VirtualFilter=dWC

