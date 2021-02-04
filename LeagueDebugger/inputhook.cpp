#include "stdafx.h"
#include "menu.h"
#define RET_CHK(x) if (DX11::InputHook::bBlockInput && DX11::Menu::bIsOpen) { return x; }\

namespace DX11
{
	WNDPROC InputHook::oWndProc = 0;
	bool InputHook::bBlockInput = true;
	bool InputHook::bHookLoaded = false;
	bool InputHook::bUnhookWhenHidden = true;

	void InputHook::ValidateHook(HWND hWindow)
	{
		if (Menu::bIsOpen && !bHookLoaded) {
			LoadHook(hWindow);
		}
		else if (!Menu::bIsOpen && bHookLoaded && bUnhookWhenHidden) {
			UnloadHook(hWindow);
		}
	}

	void InputHook::LoadHook(HWND hWindow) {
		if (!bHookLoaded) {
			oWndProc = (WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);
			bHookLoaded = true;
			////MessageBoxA(0, "InputHook Validated", "Success", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
	}

	void InputHook::UnloadHook(HWND hWindow) {
		if (bHookLoaded) {
			SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)oWndProc);
			bHookLoaded = false;
		}
	}

	LRESULT APIENTRY InputHook::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_LBUTTONDOWN) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[0] = true;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_LBUTTONUP) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[0] = false;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_RBUTTONDOWN) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[1] = true;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_RBUTTONUP) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[1] = false;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_MBUTTONDOWN) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[2] = true;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_MBUTTONUP) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[2] = false;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_MOUSEWHEEL) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseWheelH += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_MOUSEMOVE) {
			ImGuiIO& io = ImGui::GetIO();
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN) {
			ImGuiIO& io = ImGui::GetIO();
			if (wParam < 256)
				io.KeysDown[wParam] = 1;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP) {
			ImGuiIO& io = ImGui::GetIO();
			if (wParam < 256)
				io.KeysDown[wParam] = 0;
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}
		else if (uMsg == WM_CHAR) {
			ImGuiIO& io = ImGui::GetIO();
			if (wParam > 0 && wParam < 0x10000)
				io.AddInputCharacter((unsigned short)wParam);
			RET_CHK(DefWindowProc(hwnd, uMsg, wParam, lParam));
		}

		return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
	}
}
