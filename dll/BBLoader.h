///////////////////////////////////////////
//BBLoader.h
#ifndef BBLOADER_H
#define BBLOADER_H
extern "C"
{
void	WINAPI SetClientHwnd	(HWND	hClientHwnd);
void	WINAPI SetTargetPid		(DWORD	dwPid);
DWORD	WINAPI GetTargetPid		();
void	WINAPI SetFilterState	(BOOL bFilter);
BOOL	WINAPI GetFilterState	();
void	WINAPI SetFilter(int nCurSet,
					  const char *lpSearch, 
					  const char *lpModify, 
					  const WORD *lpSearchPosition, 
					  const WORD *lpModifyPosition,
					  DWORD	wbMax_wTotalSearch,			//还是跟wpe一样算了
					  DWORD	wRese_wTotalModify,
					  BOOL	bWork,
					  DWORD	dwMaxSearch,
					  DWORD	dwReserved,
					  BOOL	bAdv,
					  DWORD	dwFunMask,
					  BOOL	bAdvFlags,
					  BOOL	bBlock,
					  DWORD	dwTimes
					  );
};
#endif // BBLOADER_H