#include "stdafx.h"
#include "ImRenderer.h"

HWND hWnd;


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

	uintptr_t* vtbl = *(uintptr_t**)deviceEx.Get();
	endscene_hook = FuncHook(vtbl[42], (uintptr_t)EndScene);
	reset_hook = FuncHook(vtbl[16], (uintptr_t)Reset);
	endscene_hook.hook();
	reset_hook.hook();
	return S_OK;
}

HRESULT Direct3D9Capture::FreeCapture()
{
	DX11::InputHook::UnloadHook(hWnd);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	endscene_hook.unhook();
	reset_hook.unhook();

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

		render.init9(device);

		ImGui::GetIO().ImeWindowHandle = hWnd;
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		DX11::InputHook::LoadHook(hWnd);

		ImGui_ImplDX9_Init(device);
		ImGui_ImplWin32_Init(hWnd);
		ImGui::CreateContext();
		});
	

	render.begin_draw9();//begin for draw rende.drawline.... and etc

	if (GetAsyncKeyState(MenuKey) & 0x1) {
		DX11::Menu::bIsOpen = !DX11::Menu::bIsOpen;
		////MessageBoxA(0, "DoPresent. Opening Menu.", "Success", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	}

	DX11::InputHook::ValidateHook(hWnd);

	DX11::Menu::Render9();

	render.end_draw9();//end for draw render.drawline.... and etc
}

void Direct3D9Capture::DoReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	ImGui_ImplDX9_CreateDeviceObjects();
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