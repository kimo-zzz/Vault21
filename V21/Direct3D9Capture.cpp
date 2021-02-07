#include "stdafx.h"
#include "Renderer.h"
#include "RenderLayer.h"
#include "HudManager.h"
#include "EventManager.h"
#include "Menu.h"
#include "MenuSettings.h"
#include "Draw.h"

using namespace HACKUZAN::SDK;

namespace HACKUZAN {
	HWND hWnd;
	const auto MenuKey = VK_INSERT;

	WNDPROC Direct3D9Capture::WndProc;


	void Direct3D9Capture::Initialize() {
		WndProc = (WNDPROC)SetWindowLong(Globals::MainWindow, GWL_WNDPROC, (LONG)HookWndProc);

	}

	void Direct3D9Capture::Dispose() {
		SetWindowLong(Globals::MainWindow, GWL_WNDPROC, (LONG)WndProc);
	}

	LRESULT CALLBACK Direct3D9Capture::HookWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg == WM_MOUSEMOVE) {
			HudManager::CursorPos2D.X = (short)lparam;
			HudManager::CursorPos2D.Y = (short)(lparam >> 16);
		}
		Menu::OnWndProc(msg, wparam);
		EventManager::Trigger(LeagueEvents::OnWndProc, msg, wparam);
		return CallWindowProc(WndProc, hwnd, msg, wparam, lparam);
	}

	Direct3D9Capture::Direct3D9Capture()
	{
	}

	Direct3D9Capture::~Direct3D9Capture()
	{
		FreeCapture();
	}

	HMODULE Direct3D9Capture::GetCaptureModule()
	{
		static const std::string d3d9_dll = Capture::SYSTEM_PATH + "\\d3d9.dll";

		return GetModuleHandleA(d3d9_dll.c_str());
	}

	HRESULT Direct3D9Capture::TryCapture()
	{
		HMODULE module = GetCaptureModule();

		if (module == nullptr)
		{
			return CO_E_DLLNOTFOUND;
		}

		//MessageBoxA(0, "Module Captured. Capturing Device (DX9)", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);

		HRESULT hr;

		ComPtr<IDirect3D9Ex> d3d9ex;
		hr = ModuleCall<HRESULT>(module, "Direct3DCreate9Ex", D3D_SDK_VERSION, &d3d9ex);
		if (FAILED(hr))
			return hr;

		D3DPRESENT_PARAMETERS pp;
		ZeroMemory(&pp, sizeof(pp));
		pp.Windowed = 1;
		pp.SwapEffect = D3DSWAPEFFECT_FLIP;
		pp.BackBufferFormat = D3DFMT_A8R8G8B8;
		pp.BackBufferCount = 1;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		ComPtr<IDirect3DDevice9Ex> deviceEx;
		hr = d3d9ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, NULL, &deviceEx);
		if (FAILED(hr))
			return hr;

		Direct3D9Capture::Initialize();
		uintptr_t* vtbl = *(uintptr_t**)deviceEx.Get();
		endscene_hook = FuncHook(vtbl[42], (uintptr_t)EndScene);
		reset_hook = FuncHook(vtbl[16], (uintptr_t)Reset);
		endscene_hook.hook();
		reset_hook.hook();
		return S_OK;
	}

	HRESULT Direct3D9Capture::FreeCapture()
	{
		endscene_hook.unhook();
		reset_hook.unhook();
		Direct3D9Capture::Dispose();
		return S_OK;
	}

	std::unique_ptr<Direct3D9Capture> Direct3D9Capture::capture;
	Direct3D9Capture* Direct3D9Capture::singleton()
	{
		if (capture == nullptr)
		{
			capture = std::unique_ptr<Direct3D9Capture>(new Direct3D9Capture());
		}
		return capture.get();
	}

	void Direct3D9Capture::destroy_singleton()
	{
		Direct3D9Capture::capture.reset();
	}

	void Direct3D9Capture::DoPresent(IDirect3DDevice9* device)
	{
		D3DDEVICE_CREATION_PARAMETERS parameters;
		ZeroMemory(&parameters, sizeof(parameters));
		device->GetCreationParameters(&parameters);
		hWnd = parameters.hFocusWindow;

		static std::once_flag is_init;

		std::call_once(is_init, [&]() {
			Draw.CaptureDevice(device);
			Renderer::Initialize(device);
			});

		if (!Draw.GetDevice()) {
			Draw.SetDevice(device);
		}
		if (!Draw.Screen.Width) {
			Draw.SetScreenInfo();
		}

		Renderer::NewFrame();
		EventManager::Trigger(LeagueEvents::OnPresent);
		Menu::OnDraw();
		//Renderer::AddText("HACKUZAN GAMING", 16.0f, Rect(0.0f, 0.0f, (float)RenderLayer::Instance->ClientWidth, 0.0f), DT_CENTER, IM_COL32(255, 24, 24, 255));
		Renderer::RenderFrame();
	}

	void Direct3D9Capture::DoReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
	{
		Draw.OnLostDevice();
		Renderer::PreReset();
		Draw.OnResetDevice();
		Renderer::PostReset();
	}

	HRESULT WINAPI Direct3D9Capture::EndScene(IDirect3DDevice9* device)
	{
		Direct3D9Capture* capture = Direct3D9Capture::singleton();

		capture->DoPresent(device);

		return capture->endscene_hook.Call<HRESULT>(device);
	}

	HRESULT WINAPI Direct3D9Capture::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
	{
		Direct3D9Capture* capture = Direct3D9Capture::singleton();

		capture->DoReset(device, params);

		return capture->reset_hook.Call<HRESULT>(device, params);
	}
}