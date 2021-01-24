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
	static PVOID NewIssueOrderCheck;
	static DWORD NewIssueOrderCheckAddr;
	static DWORD TrueIssueOrderReturnAddress;
	static PVOID NewCastSpell;
	static DWORD NewCastSpellAddr;

	static bool IsDonePatchingIssueOrder;
	static bool IsDonePatchingCastSpell;

	static DWORD CalcFunctionSize(DWORD OrigAddress, size_t& size, ReturnSig retSig);
	static DWORD VirtualAllocateFunction(PVOID& NewFunction, DWORD OrigAddress, size_t size);
	static void CopyFunction(DWORD dest, DWORD source, size_t size);
	static void FixRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size);
	static void ReplaceCall(DWORD origAddress, DWORD newAddress, DWORD fnAddress, size_t size);

	//IssueOrder
	static void ApplyIssueOrderPatches(DWORD NewIssueOrder, size_t size);
	static void ApplyIssueOrderCheckPatches(DWORD NewIssueOrder, size_t size);
	static void* __fastcall IssueOrderCheckGateway(int a1, int a2, int a3, DWORD* a4, char a5, int a6, int a7, int a8, int a9, DWORD* a10);

	//CastSpell
	static void ApplyCastSpellPatches(DWORD Address, size_t size);

	static PPEB getCurrentProcessEnvironmentBlock();
	static PPEB getProcessEnvironmentBlockAddress(HANDLE processHandle);
	static int IsDetected();
};