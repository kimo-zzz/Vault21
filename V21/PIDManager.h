#include "stdafx.h"

namespace V21 {
	class PIDManager
	{
	public:
		//static int GetProcessIdByName(LPCTSTR szProcess);
		//static BOOL EnableDebugPriv();
		//static DWORD_PTR GetModuleBase(DWORD dwPid, LPCTSTR szModName);
		static int GetProcessThreadNumByID(DWORD dwPID);
		static int GetAowProcId();
		//static void killProcessByName(LPCWSTR name);
	};
}