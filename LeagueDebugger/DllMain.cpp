#include "stdafx.h"


DWORD WINAPI InitThread(LPVOID);
std::atomic_bool running;
uintptr_t initThreadHandle;
HMODULE g_hModule;

void OnExit() noexcept;

extern void SetupGameHooks();
extern void RemoveGameHooks();
extern void MainLoop();

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	if (dwReason == DLL_PROCESS_ATTACH)
	{

		g_hModule = hModule;

		std::atexit(OnExit);
		initThreadHandle = _beginthreadex(nullptr, 0, (_beginthreadex_proc_type)InitThread, hModule, 0, nullptr);

		FreeLibrary(g_hModule);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		OnExit();
	}

	return TRUE;
}

Capture* capture = nullptr;

int AddVectoredExceptionHandlerHkCount = 0;
/*FuncHook AddVectoredExceptionHandlerHook;
PVOID hk_AddVectoredExceptionHandler(ULONG First, PVECTORED_EXCEPTION_HANDLER Handler) {
	AddVectoredExceptionHandlerHkCount++;
	return AddVectoredExceptionHandlerHook.Call<PVOID>(First, Handler);
}*/

extern DWORD WINAPI InitThread(LPVOID module);
__declspec(safebuffers)DWORD WINAPI InitThread(LPVOID module)
{
	RestoreZwQueryInformationProcess();
	RestoreRtlAddVectoredExceptionHandler();

	Sleep(20000);
	int count = 0;
	running = true;
	HMODULE hModule = (HMODULE)module;

	//MessageBoxA(0, "Capturing Module...", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);

	Capture* captures[] = {
		Direct3D9Capture::singleton(),
		DXGICapture::singleton(),
	};

	do
	{
		capture = captures[count++ % _countof(captures)];
	} while (capture->GetCaptureModule() == nullptr && running);

	if (!running)
		return 0;
	HRESULT hr = 0;
	//MessageBoxA(0, "Try Capture...", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	do
	{
		hr = capture->TryCapture();
		if (FAILED(hr)) {
			//MessageBoxA(0, "Cannot Capture", "Fatal Error", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
		else if (SUCCEEDED(hr)) {
			//MessageBoxA(0, "Capture Success", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
	} while (FAILED(hr));
	//if (FAILED(hr))
	//{
	//	Instalog::SystemFacades::ThrowFromHResult(hr);
	//	return 0;
	//}

	//BuildMain();
	//BuildMenu();
	SetupGameHooks();

	while (running) {
		MainLoop();
	}

	RemoveGameHooks();
	capture->FreeCapture();
	//AddVectoredExceptionHandlerHook.unhook();
	return 0;
}

void OnExit() noexcept
{
	if (running)
	{
		running = false;
		WaitForSingleObject((HANDLE)initThreadHandle, INFINITE);
		CloseHandle((HANDLE)initThreadHandle);

		try
		{
			Direct3D9Capture::destroy_singleton();
			DXGICapture::destroy_singleton();
		}
		catch (std::exception&)
		{

		}
	}
}