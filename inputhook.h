#pragma once

#include "stdafx.h"

namespace DX11
{
	class InputHook
	{
	public:
		static WNDPROC oWndProc;
		static bool bBlockInput;
		static bool bHookLoaded;
		static bool bUnhookWhenHidden;

		static void ValidateHook(HWND);
		static void LoadHook(HWND);
		static void UnloadHook(HWND);
		static LRESULT APIENTRY WndProc(HWND, UINT, WPARAM, LPARAM);
	};
}
