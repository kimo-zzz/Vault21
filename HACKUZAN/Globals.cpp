#include "stdafx.h"
#include "Globals.h"
#include "Offsets.h"

namespace HACKUZAN {
	HWND Globals::MainWindow;
	HMODULE Globals::MainModule;
	DWORD Globals::HiddenModule;
	HMODULE Globals::D3D9Module;
	bool Globals::InGame;

	DWORD Globals::FindHiddenModule()
	{
		MEMORY_BASIC_INFORMATION mbi = { 0 };
		DWORD start = 0;
		DWORD result = 0;
		while (VirtualQuery((LPVOID)start, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) != 0)
		{
			if (mbi.Protect & PAGE_READWRITE && mbi.State == 0x1000 && mbi.Type == 0x40000)
			{
				result = (DWORD)mbi.AllocationBase;
				if (*(WORD*)(result) == 0x5A4D)
				{
					auto ntPtr = reinterpret_cast<PIMAGE_NT_HEADERS>(result + reinterpret_cast<PIMAGE_DOS_HEADER>(result)->e_lfanew);
					if (ntPtr->FileHeader.NumberOfSections == 9)
					{
						//ENGINE_MSG("found : %02X - TimeDateStamp : %02X - State %02X Type %02X", mbi.AllocationBase, ntPtr->FileHeader.TimeDateStamp, mbi.State, mbi.Type);
						break;
					}

				}
			}

			start += mbi.RegionSize;
		}
		return result;
	}


	void Globals::Initialize() {
		MainModule = GetModuleHandle("League of Legends.exe");
		HiddenModule = FindHiddenModule();
		D3D9Module = GetModuleHandle("d3d9.dll");
		MainWindow = *(HWND*)DEFINE_RVA(Offsets::RiotWindow::Instance);
		//MainWindow = FindWindowA(0, "League of Legends (TM) Client");
	}
}