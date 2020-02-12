
#include "stdafx.h"
#include "MemoryTool.h"
#include <Tlhelp32.h>

extern DWORD g_dwTargetPid;
//HANDLE hProcess = NULL;
//���ڴ�����
int ReadInt(int address)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "�򿪽���ʧ�ܣ�", "����", MB_OK);
		return 0;
	}
	LPVOID result;
	SIZE_T length = 4;//����
	
	//VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);//��Ȩ
	int abb;
	abb = ReadProcessMemory(hProcess, (LPCVOID)address, &result, length, FALSE);//�ڴ������
	CloseHandle(hProcess);
	return (int)result;
}

//д�ڴ�����
BOOL WriteInt(int address, int data)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "�򿪽���ʧ�ܣ�", "����", MB_OK);
		return FALSE;
	}
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);
	BOOL a = WriteProcessMemory(hProcess, (LPVOID)address, (LPCVOID)&data, 4, 0);
	CloseHandle(hProcess);
	return a;
}

//���ڴ�С��
float ReadFloat(int address) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "�򿪽���ʧ�ܣ�", "����", MB_OK);
		return 0;
	}
	float result;
	SIZE_T length = 4;//����
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);//��Ȩ
	int abb;
	abb = ReadProcessMemory(hProcess, (LPCVOID)address, &result, length, FALSE);//�ڴ������
	CloseHandle(hProcess);
	return result;
}

//д�ڴ�С��
BOOL WriteFloat(int address, float data) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "�򿪽���ʧ�ܣ�", "����", MB_OK);
		return FALSE;
	}
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);
	BOOL a = WriteProcessMemory(hProcess, (LPVOID)address, &data, 4, 0);
	CloseHandle(hProcess);
	return a;
}

//���ڴ�����
byte *ReadByte(int address, int length)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "�򿪽���ʧ�ܣ�", "����", MB_OK);
		return 0;
	}
	byte *p;
	//fhbyte = {};
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);
	p = new byte[length];//ȡ�հ��ֽڼ������洢������ 
	memset(p, 0, length);//�������Ļ�����
	ReadProcessMemory(hProcess, (LPCVOID)address, p, length, NULL);
	CloseHandle(hProcess);
	return p;//���ﷵ�ض�ȡ�����ֽڼ����׵�ַ
}

//д�ڴ�����
BOOL WriteByte(int address, byte data[], int length)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, g_dwTargetPid);
	if (hProcess == NULL) {
		::MessageBoxA(NULL, "�򿪽���ʧ�ܣ�", "����", MB_OK);
		return FALSE;
	}
	VirtualProtectEx(hProcess, (LPVOID)address, 10, PAGE_EXECUTE_READWRITE, NULL);
	BOOL a = WriteProcessMemory(hProcess, (LPVOID)address, (LPCVOID)&data, length, 0);
	CloseHandle(hProcess);//�رն���
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

//WriteMemoryFloat({ �����ַ, ��Ʒ��, ��Ʒ��ƫ��, ��Ʒ��3, 0xB50, 0x18, 0x4, 0x4, 0x4 }, 0);
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

/*int �ֽ�����(BYTE *�ֽ�����, int �ֽڸ���, int ��ʼ��ַ, int ������ַ) {

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int currentBaseAddress = ��ʼ��ַ;
	int stopaddress = ������ַ;
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
			if ((int)mbInfo.BaseAddress < ��ʼ��ַ) {
				mbInfo.RegionSize = mbInfo.RegionSize - (��ʼ��ַ - (int)mbInfo.BaseAddress);
			}
			if (((int)mbInfo.BaseAddress + (int)mbInfo.RegionSize) > ������ַ) {
				mbInfo.RegionSize = ������ַ - (int)mbInfo.BaseAddress;
			}
			dwBufSize = mbInfo.RegionSize;

			lpBuf = new BYTE[dwBufSize];
			if (ReadProcessMemory(hProcess, (LPCVOID)currentBaseAddress, lpBuf, dwBufSize, 0)) {
				for (j = 0; j <= ((int)mbInfo.RegionSize - �ֽڸ���); j++) {
					for (int k = 0; k < �ֽڸ���; k++) {
						if (lpBuf[j + k] != �ֽ�����[k]) {
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
{//int SearchByteShuZu(byte * bForSearch,int ifLen, byte* bWaitSearch, int iwLen,int iFirstSearchPos);//Ѱ���ֽڼ�����(��Ѱ�ҵ�,��Ѱ�ҵ���󳤶�,ҪѰ�ҵ�,ҪѰ�ҵĳ���,��ʼѰ��λ��)  ����-1 Ϊδ�ҵ� λ�ô�0��ʼ
	int iCurrentPos = 0;//������λ��
	if (iFirstSearchPos>ifLen)//�����������ĳ���
	{
		return -1;
	}
	if (iwLen>ifLen)//Ҫ�����ĳ��ȱ� �������ĳ��ȴ� ����
	{
		return -1;
	}
	if (iwLen == 0 || ifLen == 0)
	{
		return -1;//0 ����
	}
	//    bForSearch+=iFirstSearchPos;//������ʼ����λ��
	iCurrentPos += iFirstSearchPos;//������ʼ����λ��
	bForSearch += iCurrentPos;//����ָ�� ��Ӧ
	while (1)
	{
		while (*bForSearch != *bWaitSearch)//�ҵ���һ����ͬ��
		{
			iCurrentPos += 1;//��ǰ����λ��ǰ��1
			bForSearch += 1;//��ǰָ��ǰ��1
			if (iCurrentPos + iwLen > ifLen)      //���ȳ��������������峤�ȷ���
			{
				return -1;
			}
		}
		for (int i = 0; i<iwLen; ++i)//��ʼ�Ա�
		{
			if (*(bForSearch + i) != *(bWaitSearch + i))
			{
				iCurrentPos += 1;//��ǰ����λ��ǰ��1
				bForSearch += 1;//��ǰָ��ǰ��1
								//������ָ��ǰ��1
				break;//�ҵ�һ����ͬ�� ����
			}
			if (i + 1 == iwLen)
			{
				return iCurrentPos;//�ҵ���
			}
		}
	}
}




HMODULE fnGetProcessBase(DWORD PID)
{
	//��ȡ���̻�ַ  
	HANDLE hSnapShot;
	//ͨ��CreateToolhelp32Snapshot���߳�ID����ȡ���̿���  
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		//GetLastErrorBox(NULL, "�޷���������");
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