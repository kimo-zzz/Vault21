#include "stdafx.h"
#include "Globals.h"
#include "Offsets.h"

namespace HACKUZAN {
	HWND Globals::MainWindow;
	HMODULE Globals::MainModule;
	DWORD Globals::HiddenModule;
	HMODULE Globals::D3D9Module;
	bool Globals::InGame;


	void Globals::Initialize() {
		MainModule = GetModuleHandle("League of Legends.exe");
		HiddenModule = FindHiddenModule();
		D3D9Module = GetModuleHandle("d3d9.dll");
		MainWindow = *(HWND*)DEFINE_RVA(Offsets::RiotWindow::Instance);
		//MainWindow = FindWindowA(0, "League of Legends (TM) Client");
	}
}