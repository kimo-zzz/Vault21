#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include "DirectX9.h"
#include "Kiero.h"
#include "Utils.h"

namespace Globals {
	const char* Window_Name = "League of Legends (TM) Client";
	WNDPROC Original_WndProc = nullptr;
	HWND hWnd = nullptr;
	HMODULE hModule;

	bool Unload;

	bool ShowMenu = true;
}

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HRESULT WINAPI Hooked_Present(LPDIRECT3DDEVICE9 Device, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion) {
	DirectX9::Offsets::pDevice = Device;
	static bool ImGuiInit = false;
	if (!ImGuiInit) {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(Globals::hWnd);
		ImGui_ImplDX9_Init(Device);
	}
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Text("Test");
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	return DirectX9::Functions::Original_Present(Device, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

HRESULT WINAPI Hooked_Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT result = DirectX9::Functions::Original_Reset(pDevice, pPresentationParameters);

	if (result >= 0)
		ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}

LRESULT WINAPI WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
	switch (u_msg)
	{
	case WM_KEYDOWN:
		if (w_param == VK_END)
			Globals::Unload = !Globals::Unload;
		break;
	default:
		break;
	}

	if (Globals::ShowMenu && ImGui_ImplWin32_WndProcHandler(hwnd, u_msg, w_param, l_param))
		return true;

	return CallWindowProcA(Globals::Original_WndProc, hwnd, u_msg, w_param, l_param);
}

void __stdcall Start() {

	Globals::hWnd = FindWindowA(NULL, Globals::Window_Name);
	Globals::Original_WndProc = WNDPROC(SetWindowLongA(Globals::hWnd, GWL_WNDPROC, LONG_PTR(WndProc)));

	if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success) {
		DirectX9::Functions::Original_Reset = (DirectX9::TypeDefs::Prototype_Reset)Utils::GetDeviceAddress(16);
		DirectX9::Functions::Original_Present = (DirectX9::TypeDefs::Prototype_Present)Utils::GetDeviceAddress(17);

		kiero::bind(16, (void**)&DirectX9::Functions::Original_Reset, Hooked_Reset);
		kiero::bind(17, (void**)&DirectX9::Functions::Original_Present, Hooked_Present);
	}

	while (!Globals::Unload)
		Sleep(1);

	Globals::Original_WndProc = WNDPROC(SetWindowLongA(Globals::hWnd, GWL_WNDPROC, LONG_PTR(Globals::Original_WndProc)));

	if (kiero::getRenderType() == kiero::RenderType::D3D9) {
		kiero::unbind(16);
		kiero::unbind(17);
	}
	
	Sleep(3);

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX9_Shutdown();
	ImGui::DestroyContext(ImGui::GetCurrentContext());

	FreeLibraryAndExitThread(Globals::hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		Globals::hModule = hModule;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Start, 0, 0, 0);
		return TRUE;
	}

	else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		return TRUE;
	}

	return FALSE;
}