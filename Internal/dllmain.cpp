#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		return TRUE;
	}

	else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		return TRUE;
	}

	return FALSE;
}