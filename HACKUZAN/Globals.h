#pragma once

#include "stdafx.h"

namespace HACKUZAN {
	namespace Globals {
		extern HWND MainWindow;
		extern HMODULE MainModule;
		extern DWORD HiddenModule;
		extern HMODULE D3D9Module;
		extern bool InGame;

		void Initialize();
	}
}