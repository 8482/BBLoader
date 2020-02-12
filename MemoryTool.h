
#include <initializer_list>

#pragma once

int ReadInt(int address);

BOOL WriteInt(int address, int data);

float ReadFloat(int address);

BOOL WriteFloat(int address, float data);

byte *ReadByte(int address, int length);

BOOL WriteByte(int address, byte data[], int length);

INT32 ReadMemoryInt(std::initializer_list<ULONG32> AddressList);
VOID WriteMemoryInt(std::initializer_list<ULONG32> AddressList, INT32 Value);
FLOAT ReadMemoryFloat(std::initializer_list<ULONG32> AddressList);
VOID WriteMemoryFloat(std::initializer_list<ULONG32> AddressList, FLOAT Value);

