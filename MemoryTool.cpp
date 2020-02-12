
#include "stdafx.h"
#include "MemoryTool.h"
#include <Tlhelp32.h>

extern DWORD g_dwTargetPid;
//HANDLE hProcess = NULL;
//读内存整数
int ReadInt(int address)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "打开进程失败！", "错误", MB_OK);
		return 0;
	}
	LPVOID result;
	SIZE_T length = 4;//长度
	
	//VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);//提权
	int abb;
	abb = ReadProcessMemory(hProcess, (LPCVOID)address, &result, length, FALSE);//内存读整数
	CloseHandle(hProcess);
	return (int)result;
}

//写内存整数
BOOL WriteInt(int address, int data)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "打开进程失败！", "错误", MB_OK);
		return FALSE;
	}
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);
	BOOL a = WriteProcessMemory(hProcess, (LPVOID)address, (LPCVOID)&data, 4, 0);
	CloseHandle(hProcess);
	return a;
}

//读内存小数
float ReadFloat(int address) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "打开进程失败！", "错误", MB_OK);
		return 0;
	}
	float result;
	SIZE_T length = 4;//长度
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);//提权
	int abb;
	abb = ReadProcessMemory(hProcess, (LPCVOID)address, &result, length, FALSE);//内存读整数
	CloseHandle(hProcess);
	return result;
}

//写内存小数
BOOL WriteFloat(int address, float data) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "打开进程失败！", "错误", MB_OK);
		return FALSE;
	}
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);
	BOOL a = WriteProcessMemory(hProcess, (LPVOID)address, &data, 4, 0);
	CloseHandle(hProcess);
	return a;
}

//读内存数组
byte *ReadByte(int address, int length)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "打开进程失败！", "错误", MB_OK);
		return 0;
	}
	byte *p;
	//fhbyte = {};
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);
	p = new byte[length];//取空白字节集以做存储缓冲区 
	memset(p, 0, length);//清空申请的缓存区
	ReadProcessMemory(hProcess, (LPCVOID)address, p, length, NULL);
	CloseHandle(hProcess);
	return p;//这里返回读取到的字节集的首地址
}

//写内存数组
BOOL WriteByte(int address, byte data[], int length)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "打开进程失败！", "错误", MB_OK);
		return FALSE;
	}
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);
	BOOL a = WriteProcessMemory(hProcess, (LPVOID)address, (LPCVOID)&data, length, 0);
	CloseHandle(hProcess);//关闭对象
	return a;
}

INT32 ReadMemoryInt(std::initializer_list<ULONG32> AddressList)
{
	ULONG32 Address = 0;

	for (auto Offset = AddressList.begin(); Offset < AddressList.end() - 1; Offset++)
	{
		Address = ReadInt(Address + *Offset);
		//Address = *(PULONG32)(Address + *Offset);
		if (Address == 0)
		{
			return 0;
		}
	}

	return ReadInt(Address + *(AddressList.end() - 1));
}

VOID WriteMemoryInt(std::initializer_list<ULONG32> AddressList, INT32 Value)
{
	ULONG32 Address = 0;

	for (auto Offset = AddressList.begin(); Offset < AddressList.end() - 1; Offset++)
	{
		Address = *(PULONG32)(Address + *Offset);
		if (Address == 0)
		{
			return;
		}
	}

	*(PINT32)(Address + *(AddressList.end() - 1)) = Value;
}

//WriteMemoryFloat({ 人物基址, 物品栏, 物品栏偏移, 物品栏3, 0xB50, 0x18, 0x4, 0x4, 0x4 }, 0);
FLOAT ReadMemoryFloat(std::initializer_list<ULONG32> AddressList)
{
	ULONG32 Address = 0;

	for (auto Offset = AddressList.begin(); Offset < AddressList.end() - 1; Offset++)
	{
		Address = *(PULONG32)(Address + *Offset);
		if (Address == 0)
		{
			return 0;
		}
	}

	return *(PFLOAT)(Address + *(AddressList.end() - 1));
}


VOID WriteMemoryFloat(std::initializer_list<ULONG32> AddressList, FLOAT Value)
{
	ULONG32 Address = 0;

	for (auto Offset = AddressList.begin(); Offset < AddressList.end() - 1; Offset++)
	{
		Address = *(PULONG32)(Address + *Offset);
		if (Address == 0)
		{
			return;
		}
	}

	*(PFLOAT)(Address + *(AddressList.end() - 1)) = Value;
}

BOOL EnableDebugPrivilege(BOOL bEnable)
{

	//Enabling the debug privilege allows the application to see 
	//information about service application
	BOOL fOK = FALSE;     //Assume function fails
	HANDLE hToken;
	//Try to open this process's acess token
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		//Attempt to modify the "Debug" privilege
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

		fOK = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOK;
}

/*int 字节搜索(BYTE *字节数组, int 字节个数, int 起始地址, int 结束地址) {

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int currentBaseAddress = 起始地址;
	int stopaddress = 结束地址;
	int  j;
	MEMORY_BASIC_INFORMATION mbInfo = { 0 };
	ZeroMemory(&mbInfo, sizeof(mbInfo));
	SIZE_T BaseInfoLen = sizeof(mbInfo);

	BYTE *lpBuf = new BYTE[1];
	DWORD dwBufSize = 1;
	int ret_p = 0;

	while (VirtualQueryEx(hProcess, (LPVOID)currentBaseAddress, &mbInfo, BaseInfoLen) && (currentBaseAddress<stopaddress) && ((currentBaseAddress + (int)mbInfo.RegionSize)>currentBaseAddress)) {   //last check is done to see if it wasn't a 64-bit overflow.
		if (mbInfo.Type != MEM_MAPPED && mbInfo.Type != MEM_PRIVATE  && mbInfo.Protect != PAGE_NOACCESS && mbInfo.Protect != PAGE_GUARD) {//if (mbInfo.Type != MEM_MAPPED && mbInfo.Protect != 16 && mbInfo.Protect != 1 && mbInfo.Protect != 128) {
			delete[] lpBuf;
			if ((int)mbInfo.BaseAddress < 起始地址) {
				mbInfo.RegionSize = mbInfo.RegionSize - (起始地址 - (int)mbInfo.BaseAddress);
			}
			if (((int)mbInfo.BaseAddress + (int)mbInfo.RegionSize) > 结束地址) {
				mbInfo.RegionSize = 结束地址 - (int)mbInfo.BaseAddress;
			}
			dwBufSize = mbInfo.RegionSize;

			lpBuf = new BYTE[dwBufSize];
			if (ReadProcessMemory(hProcess, (LPCVOID)currentBaseAddress, lpBuf, dwBufSize, 0)) {
				for (j = 0; j <= ((int)mbInfo.RegionSize - 字节个数); j++) {
					for (int k = 0; k < 字节个数; k++) {
						if (lpBuf[j + k] != 字节数组[k]) {
							goto s;
						}
					}
					ret_p = (currentBaseAddress + j);
					goto retn;
				s:;
				}
			}
		}
		currentBaseAddress = (int)mbInfo.BaseAddress + mbInfo.RegionSize;
	}
retn:;
	delete[] lpBuf;
	return ret_p;
}*/

int SearchByteShuZu(byte * bForSearch, int ifLen, byte* bWaitSearch, int iwLen, int iFirstSearchPos)
{//int SearchByteShuZu(byte * bForSearch,int ifLen, byte* bWaitSearch, int iwLen,int iFirstSearchPos);//寻找字节集数组(待寻找的,待寻找的最大长度,要寻找的,要寻找的长度,起始寻找位置)  返回-1 为未找到 位置从0开始
	int iCurrentPos = 0;//待搜索位置
	if (iFirstSearchPos>ifLen)//超过待搜索的长度
	{
		return -1;
	}
	if (iwLen>ifLen)//要搜索的长度比 待搜索的长度大 返回
	{
		return -1;
	}
	if (iwLen == 0 || ifLen == 0)
	{
		return -1;//0 返回
	}
	//    bForSearch+=iFirstSearchPos;//设置起始搜索位置
	iCurrentPos += iFirstSearchPos;//设置起始搜索位置
	bForSearch += iCurrentPos;//设置指针 对应
	while (1)
	{
		while (*bForSearch != *bWaitSearch)//找到第一个相同的
		{
			iCurrentPos += 1;//当前搜索位置前进1
			bForSearch += 1;//当前指针前进1
			if (iCurrentPos + iwLen > ifLen)      //长度超过待搜索的整体长度返回
			{
				return -1;
			}
		}
		for (int i = 0; i<iwLen; ++i)//开始对比
		{
			if (*(bForSearch + i) != *(bWaitSearch + i))
			{
				iCurrentPos += 1;//当前搜索位置前进1
				bForSearch += 1;//当前指针前进1
								//设置下指针前进1
				break;//找到一个不同的 跳出
			}
			if (i + 1 == iwLen)
			{
				return iCurrentPos;//找到列
			}
		}
	}
}




HMODULE fnGetProcessBase(DWORD PID)
{
	//获取进程基址  
	HANDLE hSnapShot;
	//通过CreateToolhelp32Snapshot和线程ID，获取进程快照  
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		//GetLastErrorBox(NULL, "无法创建快照");
		return NULL;
	}
	MODULEENTRY32 ModuleEntry32;
	ModuleEntry32.dwSize = sizeof(ModuleEntry32);
	if (Module32First(hSnapShot, &ModuleEntry32))
	{
		do
		{
			TCHAR szExt[5];
			strcpy(szExt, ModuleEntry32.szExePath + strlen(ModuleEntry32.szExePath) - 4);
			for (int i = 0; i < 4; i++)
			{
				if ((szExt[i] >= 'a') && (szExt[i] <= 'z'))
				{
					szExt[i] = szExt[i] - 0x20;
				}
			}
			if (!strcmp(szExt, ".EXE"))
			{
				CloseHandle(hSnapShot);
				return ModuleEntry32.hModule;
			}
		} while (Module32Next(hSnapShot, &ModuleEntry32));
	}
	CloseHandle(hSnapShot);
	return NULL;

}