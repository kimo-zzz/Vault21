#include <winternl.h>

struct ReturnSig {
	std::vector<BYTE> returnSig;
	int returnCount;
};

struct AddressesToCopy {
	DWORD address;
	ReturnSig sig;
};

class LeagueFunctions {
public:
	static std::vector<AddressesToCopy> addressToCopyList;
	static PVOID NewIssueOrder;
	static DWORD* TrueIssueOrderReturnAddress;
	static DWORD IssueOrderStartHookGateway;
	static DWORD IssueOrderEndHookGateway;
	static bool IsDonePatchingIssueOrder;

	static PVOID NewCastSpell;
	static DWORD* TrueCastSpellReturnAddress;
	static DWORD CastSpellStartHookGateway;
	static DWORD CastSpellEndHookGateway;
	static bool IsDonePatchingCastSpell;

	static PVOID NewUpdateChargableSpell;
	static DWORD* TrueUpdateChargableSpellReturnAddress;
	static DWORD UpdateChargableSpellStartHookGateway;
	static DWORD UpdateChargableSpellEndHookGateway;
	static bool IsDonePatchingUpdateChargableSpell;

	static DWORD CalcFunctionSize(DWORD OrigAddress, size_t& size, ReturnSig retSig);
	static DWORD VirtualAllocateFunction(PVOID& NewFunction, DWORD OrigAddress, size_t size);
	static void CopyFunction(DWORD dest, DWORD source, size_t size);
	static void FixRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size);
	static void ReplaceCall(DWORD origAddress, DWORD newAddress, DWORD fnAddress, size_t size);
	static void HookStartAndEndFunction(DWORD fnAddress, size_t size, int paramCount, DWORD StartHook, DWORD EndHook, DWORD& StartHookGateway, DWORD& EndHookGateway);
	//IssueOrder
	static void NewIssueOrderStartHook();
	static void NewIssueOrderEndHook();

	//CastSpell
	static void NewCastSpellStartHook();
	static void NewCastSpellEndHook();

	//UpdateChargableSpell
	static void NewUpdateChargableSpellStartHook();
	static void NewUpdateChargableSpellEndHook();

	static PPEB getCurrentProcessEnvironmentBlock();
	static PPEB getProcessEnvironmentBlockAddress(HANDLE processHandle);
	static int IsDetected();
};