struct HookEntries
{
	DWORD addressToHook;
	DWORD addressToHookOldProtect;
	DWORD addressToHookMbiStart;
	DWORD addressToHookMbiEnd;
	DWORD addressToHookMbiSize;
	DWORD allocatedAddressStart;
	DWORD allocatedAddressEnd;
	DWORD allocatedAddressSize;
	DWORD addressToHookoffsetFromStart;
};

class UltimateHooks
{
private:
	static bool Hook(DWORD original_fun, size_t offset);
public:
	static DWORD DR0;
	static DWORD DR1;
	static DWORD DR2;
	static DWORD DR3;
	static DWORD DR6;
	static DWORD DR7;
	static bool IsDoneInit;
	static PVOID VEH_Handle;
	static PVOID VCH_Handle;
	static std::vector<HookEntries> hookEntries;
	static bool AreInSamePage(const DWORD* Addr1, const DWORD* Addr2);
	static LONG WINAPI LeoHandler(EXCEPTION_POINTERS* pExceptionInfo);
	static LONG WINAPI VCHandler(EXCEPTION_POINTERS* pExceptionInfo);
	static DWORD init();
	static bool deinit();
	static bool addHook(DWORD address, size_t offset);
	static bool removeHook(DWORD address);
	static bool UnHook(DWORD original_fun);

	static DWORD VirtualAllocateRegion(PVOID& NewFunction, DWORD OrigAddress, size_t size);
	static void CopyRegion(DWORD dest, DWORD source, size_t size);
	static void FixRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size, size_t _offset);
	static void FixFuncRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size);
};