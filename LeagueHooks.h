struct HookStruct
{
	DWORD og_fun;
	DWORD hk_fun;
	DWORD oldProtection;
};

struct HookStructHWBP
{
	DWORD og_fun;
	DWORD hk_fun;
	uint8_t RegIndex;
};

class LeagueHooks
{
public:
	static bool IsDoneInit;
	static PVOID VEH_Handle;
	static std::vector<HookStruct> hookListPG;
	static std::vector<HookStructHWBP> hookListHWBP;
	static bool AreInSamePage(const DWORD* Addr1, const DWORD* Addr2);
	static LONG WINAPI LeoHandler(EXCEPTION_POINTERS* pExceptionInfo);
	static DWORD init();
	static bool deinit();
};

class LeagueHooksVEH : LeagueHooks
{
private:
	static bool Hook(DWORD original_fun, DWORD hooked_fun);
public:
	static bool addHook(DWORD address, DWORD hkAddress);
	static bool removeHook(DWORD address);
	static bool UnHook(DWORD original_fun);
};

class LeagueHooksHWBP : LeagueHooks
{
private:
	static bool Hook(DWORD original_fun, DWORD hooked_fun, uint8_t RegIndex);
public:
	static bool addHook(DWORD address, DWORD hkAddress, uint8_t RegIndex);
	static bool removeHook(uint8_t RegIndex);
	static bool UnHook(uint8_t RegIndex);
};

struct LeagueDecryptData
{
	int totalSuccessDecrypted;
	int totalSuccess_PAGE_NOACCESS;
	int totalSuccess_EXCEPTION_CONTINUE_EXECUTION;
	int totalFailedDecrypted;
};

class LeagueDecrypt
{
public:
	static int IsMemoryDecrypted(PVOID Address, std::vector<PVECTORED_EXCEPTION_HANDLER> handlers, size_t& i);
	static LeagueDecryptData decrypt(const wchar_t* szModule, std::vector<PVECTORED_EXCEPTION_HANDLER> handlers);
};

struct VECTORED_HANDLER_ENTRY
{
	VECTORED_HANDLER_ENTRY* next;
	VECTORED_HANDLER_ENTRY* previous;
	void* refs;
	void* unk;
	PVECTORED_EXCEPTION_HANDLER handler;
};

struct _VECTORED_HANDLER_LIST
{
	void* mutex_exception;
	VECTORED_HANDLER_ENTRY* first_exception_handler;
	VECTORED_HANDLER_ENTRY* last_exception_handler;
	void* mutex_continue;
	VECTORED_HANDLER_ENTRY* first_continue_handler;
	VECTORED_HANDLER_ENTRY* last_continue_handler;
};

class Process
{
public:
	static DWORD process_cookie_;
	static const DWORD GetProcessCookie();
	static DWORD DecodePointerHandle(DWORD pointer);
	static _VECTORED_HANDLER_LIST* GetVECTORED_HANDLER_LIST(DWORD& veh_addr);
	static DWORD GetVEHOffset();
	static std::vector<PVECTORED_EXCEPTION_HANDLER> GetAllHandlers();
	static void RemoveAllHandlers();
	static void ReAddAllHandlers(std::vector<PVECTORED_EXCEPTION_HANDLER> _PVECTORED_EXCEPTION_HANDLER_list);
	static void ReAddAllContinueHandlers(std::vector<PVECTORED_EXCEPTION_HANDLER> _PVECTORED_EXCEPTION_HANDLER_list);
};
