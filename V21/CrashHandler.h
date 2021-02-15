class CrashHandler
{
public:
	static DWORD init();
	static bool deinit();
	static bool IsDoneInit;
	static PVOID VEH_Handle;
	static LONG WINAPI LeoHandler(EXCEPTION_POINTERS* pExceptionInfo);
};