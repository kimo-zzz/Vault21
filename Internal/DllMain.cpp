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
	/*HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	DWORD NtQueryInformationProcessAddr = (DWORD)GetProcAddress(ntdll, "NtQueryInformationProcess");

	AddVectoredExceptionHandlerHook = FuncHook(AddVectoredExceptionHandler, hk_AddVectoredExceptionHandler);
	AddVectoredExceptionHandlerHook.hook();*/

	if (isMainThreadAllow) {
		while (Engine::GetGameTime() < 1.0f || !me) {
			/*
			if (!me) {
				MessageBoxA(0, "false", "!ME", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			}
			if (Engine::GetGameTime() < 1.0f) {
				MessageBoxA(0, "less than 1.0f", "GetGameTime", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			}
			MessageBoxA(0, "Waiting...", "ERROR", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			*/
			Sleep(1);
		}
		//MessageBoxA(0, "Initiallized.", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);

	}
	Sleep(5000);

	ObjManager = (CObjectManager*)(*(DWORD*)(baseAddr + oObjManager)); 

	Functions.PrintChat = (Typedefs::fnPrintChat)(baseAddr + oPrintChat);
	Functions.SendChat = (Typedefs::fnSendChat)(baseAddr + oSendChat); 
	//Functions.IsTargetable = (Typedefs::fnIsTargetable)(baseAddr + oIsTargetable);
	//Functions.IsAlive = (Typedefs::fnIsAlive)(baseAddr + oIsAlive); 

	//Functions.IsMinion = (Typedefs::fnIsMinion)(baseAddr + oIsMinion); 
	Functions.IsDragon = (Typedefs::fnIsDragon)(baseAddr + oIsDragon); 
	//Functions.IsTurret = (Typedefs::fnIsTurret)(baseAddr + oIsTurret); 
	//Functions.IsHero = (Typedefs::fnIsHero)(baseAddr + oIsHero); 
	//Functions.IsMissile = (Typedefs::fnIsMissile)(baseAddr + oIsMissile); 
	Functions.IsNexus = (Typedefs::fnIsNexus)(baseAddr + oIsNexus); 
	Functions.IsInhibitor = (Typedefs::fnIsInhibitor)(baseAddr + oIsInhib); 
	Functions.IsTroyEnt = (Typedefs::fnIsTroyEnt)(baseAddr + oIsTroy);

	//Functions.CastSpell = (Typedefs::fnCastSpell)(baseAddr + oCastSpell); //retaddr
	//Functions.IssueOrder = (Typedefs::fnIssueOrder)(baseAddr + oIssueOrder); //retaddr
	//Functions.DrawCircle = (Typedefs::fnDrawCircle)(baseAddr + oDrawCircle); //retaddr
	Functions.WorldToScreen = (Typedefs::WorldToScreen)(baseAddr + oWorldToScreen);
	Functions.WorldToScreenL = (Typedefs::WorldToScreenL)(baseAddr + oWorldToScreen);

	Functions.GetAttackCastDelay = (Typedefs::fnGetAttackCastDelay)(baseAddr + oGetAttackCastDelay);
	Functions.GetAttackDelay = (Typedefs::fnGetAttackDelay)(baseAddr + oGetAttackDelay);
	Functions.GetTimerExpiry = (Typedefs::fnGetTimerExpiry)(baseAddr + oGetTimerExpiry);

	Functions.GetGameVer = (Typedefs::fnGetGameVer)(baseAddr + oGameVersion);

	Functions.GetBaseDrawPosition = (Typedefs::fnBaseDrawPosition)(baseAddr + oBaseDrawPosition);
	Functions.GetObjectOwner = (Typedefs::fnGetObjectOwner)(baseAddr + GetOwnerObject);

	Functions.OnProcessSpell_h = (Typedefs::event_OnProcessSpell_h)(HiddenModule + oOnprocessSpell);
	Functions.OnCreateObject_h = (Typedefs::event_OnCreateObject_h)(HiddenModule + OnCreateObject);
	Functions.OnDeleteObject_h = (Typedefs::event_OnDeleteObject_h)(HiddenModule + oOnDeleteObject);
	Functions.OnNewPath_h = (Typedefs::event_OnNewPath_h)(HiddenModule + oOnNewPath);
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
	config::save();
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