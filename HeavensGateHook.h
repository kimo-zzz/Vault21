#include "makesyscall.h"
#include "syscalls.hpp"

const char* call = nullptr;
DWORD syscall;

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
LPVOID HGateCopy = nullptr;

DWORD Backup_Eax;

extern ExampleAppLog AppLog;

const DWORD_PTR __declspec(naked) GetGateAddress()
{
	__asm
		{
		mov eax, dword ptr fs : [0xC0]
		ret
		}
}

void saveToLog(std::string msg)
{
	FILE* logfile;
	if ((logfile = fopen("log.txt", "a+")) != nullptr)
	{
		fprintf(logfile, (msg).c_str());
		fclose(logfile);
	}
}

void LogNtProtectVirtualMemory(DWORD Handle, DWORD* Address_1, DWORD* DwSizee, DWORD New, DWORD _ret1, DWORD _ret2)
{
	std::string elem;
	elem = "NtProtectVirtualMemory RETURN : [" + hexify<DWORD>(static_cast<DWORD>(_ret2)) +
		"]\n\t\tAddress : [" + hexify<DWORD>(static_cast<DWORD>(*Address_1)) +
		//create a filter on desired module range to avoid crash
		"]\tSize : [" + hexify<DWORD>(static_cast<DWORD>(*DwSizee)) +
		//create a filter on desired module range to avoid crash
		"]\tNewProtect : [" + hexify<DWORD>(static_cast<DWORD>(New)) + "]\n";
	AppLog.AddLog(elem.c_str());
}

DWORD Handle, *Address_1, New, Old, *DwSizee;
DWORD _ret1, _ret2;

void __declspec(naked) hk_NtProtectVirtualMemory()
{
	__asm {
		mov Backup_Eax, eax
		mov eax, [esp] // - 4 bytes ret1
		mov _ret1, eax
		mov eax, [esp + 0x4] // - 4 bytes ret2
		mov _ret2, eax
		mov eax, [esp + 0x8] //HANDLE - 4 bytes ProcessHandle
		mov Handle, eax
		mov eax, [esp + 0xC] //PVOID - 4 bytes *BaseAddress
		mov Address_1, eax
		mov eax, [esp + 0x10] //PULONG - 4 bytes NumberOfBytesToProtect
		mov DwSizee, eax
		mov eax, [esp + 0x14] //ULONG - 4 bytes NewAccessProtection
		mov New, eax
		mov eax, [esp + 0x18] //ULONG OldAccessProtection 
		mov Old, eax
		mov eax, Backup_Eax
		pushad
		}

	LogNtProtectVirtualMemory(Handle, Address_1, DwSizee, New, _ret1, _ret2);

	__asm popad
	__asm jmp HGateCopy
}

void LogNtQueryVirtualMemory(DWORD ProcessHandle, DWORD BaseAddress, DWORD _ret1, DWORD _ret2)
{
	std::string elem;
	elem = "NtQueryVirtualMemory RETURN : [" + hexify<DWORD>(static_cast<DWORD>(_ret2)) + "]\n\t\tAddress : [" + hexify<
		DWORD>(static_cast<DWORD>(BaseAddress)) + "]\n";
	AppLog.AddLog(elem.c_str());
}

DWORD ProcessHandle, BaseAddress, BaseAddress2, AllocationBase, AllocationProtect, PartitionId,
      RegionSize, State, Protect, typex, MemoryInformation, MemoryInformationLength, ReturnLength;

DWORD ret1, ret2;

void __declspec(naked) hk_NtQueryVirtualMemory()
{
	__asm {
		mov Backup_Eax, eax
		mov eax, [esp] // - 4 bytes ret1
		mov ret1, eax
		mov eax, [esp + 0x4] // - 4 bytes ret2
		mov ret2, eax
		mov eax, [esp + 0x8] //HANDLE - 4 bytes ProcessHandle
		mov ProcessHandle, eax
		mov eax, [esp + 0xC] //PVOID - 4 bytes BaseAddress
		mov BaseAddress, eax
		/*
		mov eax, [esp + 0x10] //PVOID - 4 bytes MBI - BaseAddress
		mov BaseAddress2, eax
		mov eax, [esp + 0x14] //PVOID - 4 bytes MBI - AllocationBase
		mov AllocationBase, eax
		mov eax, [esp + 0x18] //DWORD - 4 bytes MBI - AllocationProtect
		mov AllocationProtect, eax
		mov eax, [esp + 0x1C] //WORD - 2 bytes MBI - PartitionId
		mov PartitionId, eax
		mov eax, [esp + 0x20] //SIZE_T - 4 bytes MBI - RegionSize
		mov RegionSize, eax
		mov eax, [esp + 0x24] //DWORD - 4 bytes MBI - State
		mov State, eax
		mov eax, [esp + 0x28] //DWORD - 4 bytes MBI - Protect
		mov Protect, eax
		mov eax, [esp + 0x2C] //DWORD - 4 bytes MBI - Type
		mov typex, eax
		mov eax, [esp + 0x30] //PVOID - 4 bytes - MemoryInformation
		mov MemoryInformation, eax
		mov eax, [esp + 0x34] //SIZE_T - 4 bytes - MemoryInformationLength
		mov MemoryInformationLength, eax
		mov eax, [esp + 0x38] //PSIZE_T - 4 bytes - ReturnLength
		mov ReturnLength, eax
		*/
		mov eax, Backup_Eax
		pushad
		}

	LogNtQueryVirtualMemory(ProcessHandle, BaseAddress, ret1, ret2);

	__asm popad
	__asm jmp HGateCopy
}

void __declspec(naked) hk_NtReadVirtualMemory()
{
	__asm pushad
	__asm popad
	__asm jmp HGateCopy
}

void LogUniversal(const char* pCall)
{
	std::string elem;
	elem = std::string(pCall) + "\n";
	AppLog.AddLog(elem.c_str());
}

void _LogUniversal(const char* pCall, DWORD addr1, DWORD addr2)
{
	std::string elem;
	elem = std::string(pCall) + " : RETURN [" + hexify<DWORD>(addr2) + "]" + "\n";
	AppLog.AddLog(elem.c_str());
}

DWORD RETURN1, RETURN2;
_CRT_STDIO_INLINE void __CRTDECL LogGateData()
{
	call = findName(syscall);

	if (call == nullptr)
	{
	}
	else
	{
		//LogUniversal(call);

		//create a filter on desired module range to avoid crash
		_LogUniversal(call, RETURN1, RETURN2);
	}
}

NTSTATUS hk_NtGetContextThread(HANDLE ThreadHandle, PCONTEXT pContext)
{
	AppLog.AddLog("NtGetContextThread : Hooked! Doing nothing! XD\n");
	return static_cast<NTSTATUS>(0);
}

NTSTATUS hk_NtSetContextThread(HANDLE ThreadHandle, PCONTEXT pContext)
{
	AppLog.AddLog("NtSetContextThread : Hooked! Doing nothing! XD\n");
	return static_cast<NTSTATUS>(0);
}

NTSTATUS hk_NtSuspendThread(HANDLE ThreadHandle, PULONG PreviousSuspendCount)
{
	AppLog.AddLog("NtSuspendThread : Plz dont suspend threads :(\n");
	return static_cast<NTSTATUS>(0);
}

NTSTATUS hk_NtTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus)
{
	AppLog.AddLog("NtTerminateProcess : Plz dont terminate process :(\n");
	return static_cast<NTSTATUS>(0);
}

void __declspec(naked) hk_NtRaiseException()
{
	__asm {
		pushad
		}
	AppLog.AddLog("NtRaiseException : Called.\n");
	__asm popad
	__asm jmp HGateCopy
}

void __declspec(naked) hk_Wow64Trampoline()
{
	//////////////////////////
	//  create a filter on desired module range to avoid crash
	//////////////////////////

	__asm {
		//cmp eax, 0x23 //64bit Syscall id of NtQVM
		//je hk_NtQueryVirtualMemory

		//cmp eax, 0x50 //64bit Syscall id of NtPVM
		//je hk_NtProtectVirtualMemory

		//cmp eax, 0xF2 //64bit Syscall id of NtGCT
		//je hk_NtGetContextThread

		//cmp eax, 0x18B //64bit Syscall id of NtSCT
		//je hk_NtSetContextThread

		cmp eax, 0x1BC //64bit Syscall id of NtSuspendThread
		je hk_NtSuspendThread

		cmp eax, 0x2C //64bit Syscall id of NtTerminateProcess
		je hk_NtTerminateProcess

		cmp eax, 0x166 //64bit Syscall id of NtRaiseException
		je hk_NtRaiseException
		}
	/*
	__asm pushad
	__asm mov syscall, eax
	__asm {
		mov Backup_Eax, eax
		mov eax, [esp] // - 4 bytes ret1
		mov RETURN1, eax
		mov eax, [esp + 0x4] // - 4 bytes ret2
		mov RETURN2, eax
		mov eax, Backup_Eax
	}

	__asm call LogGateData;
	__asm popad
	*/

	__asm jmp HGateCopy
}

const LPVOID CreateNewJump()
{
	DWORD_PTR Gate = GetGateAddress();
	HGateCopy = VirtualAlloc(nullptr, 0x1000, MEM_RESERVE | MEM_COMMIT,
	                         PAGE_EXECUTE_READWRITE);
	memcpy(HGateCopy, (void*)Gate, 9);

	return HGateCopy;
}

const void WriteJump(const DWORD_PTR dwWow64Address, const void* pBuffer, size_t ulSize)
{
	DWORD dwOldProtect = 0;

	auto addr = (PVOID)dwWow64Address;
	auto size = static_cast<SIZE_T>(0x1000);

	if (NT_SUCCESS(
		makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(
			GetCurrentProcess(), &addr, &size, PAGE_EXECUTE_READWRITE, &dwOldProtect)))
	{
	}
	else
	{
	}

	(void)memcpy((void*)dwWow64Address, pBuffer, ulSize);

	if (NT_SUCCESS(
		makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(
			GetCurrentProcess(), &addr, &size, dwOldProtect, &dwOldProtect)))
	{
	}
	else
	{
	}
}

const void EnableHeavensGateHook()
{
	AppLog.AddLog(("Gate: " + hexify<DWORD>(static_cast<DWORD>(GetGateAddress())) + "\n").c_str());
	AppLog.AddLog(("Trampoline Gate: " + hexify<DWORD>(DWORD(CreateNewJump())) + "\n").c_str());
	AppLog.AddLog(("Hook Gate: " + hexify<DWORD>(DWORD(hk_Wow64Trampoline)) + "\n").c_str());

	LPVOID Hook_Gate = &hk_Wow64Trampoline;

	char trampolineBytes[] =
	{
		0x68, 0xDD, 0xCC, 0xBB, 0xAA, /*push 0xAABBCCDD*/
		0xC3, /*ret*/
		0xCC, 0xCC, 0xCC /*padding*/
	};
	memcpy(&trampolineBytes[1], &Hook_Gate, 4);
	WriteJump(GetGateAddress(), trampolineBytes, sizeof(trampolineBytes));
}
