#pragma once
#include <Windows.h>

class PIDManager
{
public:
	PIDManager();
	~PIDManager();
	//static int GetProcessIdByName(LPCTSTR szProcess);
	//static BOOL EnableDebugPriv();
	//static DWORD_PTR GetModuleBase(DWORD dwPid, LPCTSTR szModName);
	static int GetProcessThreadNumByID(DWORD dwPID);
	static int GetAowProcId();
	//static void killProcessByName(LPCWSTR name);
};