#pragma once
#include <Windows.h>
#include <inttypes.h>

static DWORD64 m_pid;
static DWORD64 usermode_pid;
static DWORD64 m_base;
static DWORD64 m_game_manager;
static DWORD64 isin_game;
static DWORD64 m_gdi32base;

#include "DriverController.h"

static DriverController* Controller = nullptr;

static DWORD64 GetBaseAddress()
{
	return Controller->GetProcessBase();
}


template <typename T>
static T read(const uint64_t address)
{
	T buffer{ };
	Controller->ReadProcessMemory(address, &buffer, sizeof(T));

	return buffer;
}

template<typename T>
static bool write(uint64_t address, T buffer)
{
	if (address > 0x7FFFFFFFFFFF || address < 1) return 0;

	return Controller->WriteProcessMemory(address, &buffer, sizeof(T));

	return true;
}

///////////////////////////////////////////
//ROAD TO BSOD. HAHAHA
///////////////////////////////////////////
/*
DWORD64 findpattern(char* Pattern, char* Mask)
{
	return Controller->FindPatternProcessMemory(Pattern, Mask);
}
*/

static bool readwtf(uintptr_t Address, void * Buffer, SIZE_T Size)
{
	if (Address > 0x7FFFFFFFFFFF || Address < 1) return 0;

	Controller->ReadProcessMemory(Address, Buffer, Size);

	return true;
}


#define READ64(addr) read<uint64_t>(addr)