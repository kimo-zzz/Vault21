#pragma once
#include "stdafx.h"
#include "UltimateHooks.h"
#include "LeagueHooks.h"
//#include <iostream> 
//#include <string>
#include "makesyscall.h"
#include <comdef.h>
//#include <winternl.h>
#include <TlHelp32.h>
#include "include/Zydis/Zydis.h"
#ifdef _WIN64
#define XIP Rip
#else
#define XIP Eip
#endif

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING;

typedef UNICODE_STRING* PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES;

typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;

#ifndef InitializeObjectAttributes
#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }
#endif

//extern ExampleAppLog AppLog;

std::vector<HookEntries> UltimateHooks::hookEntries = {};

PVOID UltimateHooks::VEH_Handle = nullptr;
PVOID UltimateHooks::VCH_Handle = nullptr;
bool UltimateHooks::IsDoneInit = false;

DWORD UltimateHooks::DR0 = 0;
DWORD UltimateHooks::DR1 = 0;
DWORD UltimateHooks::DR2 = 0;
DWORD UltimateHooks::DR3 = 0;
DWORD UltimateHooks::DR6 = 0;
DWORD UltimateHooks::DR7 = 0;

DWORD UltimateHooks::init()
{
	//Register the Custom Exception Handler
	if (!IsDoneInit)
	{
		VEH_Handle = AddVectoredExceptionHandler(1, (PTOP_LEVEL_EXCEPTION_FILTER)(LeoHandler));
		//VCH_Handle = AddVectoredContinueHandler(1, (PVECTORED_EXCEPTION_HANDLER)VCHandler);
		IsDoneInit = true;
	}
	//AppLog.AddLog(("LeoHandler: " + hexify<DWORD>((DWORD)LeoHandler) + "\n").c_str());
	//AppLog.AddLog(("VEH_Handle: " + hexify<DWORD>((DWORD)VEH_Handle) + "\n").c_str());

	//Toggle PAGE_GUARD flag on the page
	if (VEH_Handle)
	{
		return (DWORD)LeoHandler;
	} //MessageBoxA(0, "no VEH_Hanlde", "", 0);
	return 0;
}

LONG WINAPI UltimateHooks::VCHandler(EXCEPTION_POINTERS* pExceptionInfo) {
	//AppLog.AddLog("VCH called\n");
	pExceptionInfo->ContextRecord->Dr0 = DR0;
	pExceptionInfo->ContextRecord->Dr1 = DR1;
	pExceptionInfo->ContextRecord->Dr2 = DR2;
	pExceptionInfo->ContextRecord->Dr3 = DR3;
	pExceptionInfo->ContextRecord->Dr7 = DR7;
	return EXCEPTION_CONTINUE_EXECUTION;
}

extern std::vector<watchListEntry> executeWatchlist;
LONG WINAPI UltimateHooks::LeoHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
	/*
	DR0 = pExceptionInfo->ContextRecord->Dr0;
	DR1 = pExceptionInfo->ContextRecord->Dr1;
	DR2 = pExceptionInfo->ContextRecord->Dr2;
	DR3 = pExceptionInfo->ContextRecord->Dr3;
	DR6 = pExceptionInfo->ContextRecord->Dr6;
	DR7 = pExceptionInfo->ContextRecord->Dr7;

	pExceptionInfo->ContextRecord->Dr0 = 0;
	pExceptionInfo->ContextRecord->Dr1 = 0;
	pExceptionInfo->ContextRecord->Dr2 = 0;
	pExceptionInfo->ContextRecord->Dr3 = 0;
	pExceptionInfo->ContextRecord->Dr6 = 0;
	pExceptionInfo->ContextRecord->Dr7 = 0;

	string exceptionCode = "";
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
	{
		exceptionCode = ("EXCEPTION_ACCESS_VIOLATION");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ARRAY_BOUNDS_EXCEEDED)
	{
		exceptionCode = ("EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		exceptionCode = ("EXCEPTION_BREAKPOINT");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_DATATYPE_MISALIGNMENT)
	{
		exceptionCode = ("EXCEPTION_DATATYPE_MISALIGNMENT");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_DENORMAL_OPERAND)
	{
		exceptionCode = ("EXCEPTION_FLT_DENORMAL_OPERAND");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_DIVIDE_BY_ZERO)
	{
		exceptionCode = ("EXCEPTION_FLT_DIVIDE_BY_ZERO");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_INEXACT_RESULT)
	{
		exceptionCode = ("EXCEPTION_FLT_INEXACT_RESULT");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_INVALID_OPERATION)
	{
		exceptionCode = ("EXCEPTION_FLT_INVALID_OPERATION");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_OVERFLOW)
	{
		exceptionCode = ("EXCEPTION_FLT_OVERFLOW");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_STACK_CHECK)
	{
		exceptionCode = ("EXCEPTION_FLT_STACK_CHECK");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_UNDERFLOW)
	{
		exceptionCode = ("EXCEPTION_FLT_UNDERFLOW");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ILLEGAL_INSTRUCTION)
	{
		exceptionCode = ("EXCEPTION_ILLEGAL_INSTRUCTION");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
	{
		exceptionCode = ("EXCEPTION_IN_PAGE_ERROR");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO)
	{
		exceptionCode = ("EXCEPTION_INT_DIVIDE_BY_ZERO");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INT_OVERFLOW)
	{
		exceptionCode = ("EXCEPTION_INT_OVERFLOW");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INVALID_DISPOSITION)
	{
		exceptionCode = ("EXCEPTION_INVALID_DISPOSITION");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_NONCONTINUABLE_EXCEPTION)
	{
		exceptionCode = ("EXCEPTION_NONCONTINUABLE_EXCEPTION");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_PRIV_INSTRUCTION)
	{
		exceptionCode = ("EXCEPTION_PRIV_INSTRUCTION");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		exceptionCode = ("EXCEPTION_SINGLE_STEP");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
	{
		exceptionCode = ("EXCEPTION_STACK_OVERFLOW");
	}
	exceptionCode += ("\nExceptionCode: " + hexify<DWORD>(pExceptionInfo->ExceptionRecord->ExceptionCode) + "\nEIP: " + hexify<DWORD>((DWORD)pExceptionInfo->ContextRecord->XIP) + 
		"\nExceptionAddr: " + hexify<DWORD>((DWORD)pExceptionInfo->ExceptionRecord->ExceptionAddress) + "\nExceptionInfo[0]: " + hexify<DWORD>((DWORD)pExceptionInfo->ExceptionRecord->ExceptionInformation[0])
		+ "\nExceptionInfo[1]: " + hexify<DWORD>((DWORD)pExceptionInfo->ExceptionRecord->ExceptionInformation[1]));
	if ((pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) &&
		(pExceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_BREAKPOINT) &&
		(pExceptionInfo->ExceptionRecord->ExceptionCode != 0xc0000420)
		) {
		//MessageBoxA(0, exceptionCode.c_str(), "exceptionCode", 0);
	}
	//DataLog(pExceptionInfo);
	*/
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
	{
		//if((DWORD)V21::Globals::HiddenModule >= pExceptionInfo->ContextRecord->XIP)
		//	return EXCEPTION_CONTINUE_SEARCH;

		for (HookEntries hs : hookEntries)
		{
			//MessageBoxA(0, "EXCEPTION_ACCESS_VIOLATION", "EXCEPTION_ACCESS_VIOLATION", 0);
			if ((hs.addressToHook == pExceptionInfo->ContextRecord->XIP) &&
				(pExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 8)) {
				//DataLog(pExceptionInfo);
				std::vector<watchListEntry> __executeWatchlist = executeWatchlist;
				std::vector<watchListEntry> _executeWatchlist;
				for (watchListEntry wl : __executeWatchlist) {
					if ((wl.address == hs.addressToHook) && (wl.isStarted)) {

						std::vector<WatchData> _watchData;

						bool isFound;
						//////////////////////////////////////////////////////////////////
						// check if watchData exsists, if yes, then just add hitCount
						//////////////////////////////////////////////////////////////////
						for (WatchData wd : wl.watchData) {

							if (wd.exceptionAddress == (DWORD)pExceptionInfo->ExceptionRecord->ExceptionAddress) {
								wd.hitCount++;
								isFound = true;
							}
							_watchData.push_back(wd);
						}
						//////////////////////////////////////////////////////////////////
						// check if watchData exsists, if no, then add new watchData
						//////////////////////////////////////////////////////////////////
						if (!isFound) {
							WatchData wd;
							wd.exceptionAddress = (DWORD)pExceptionInfo->ExceptionRecord->ExceptionAddress;
							wd.cr.Eax = pExceptionInfo->ContextRecord->Eax;
							wd.cr.Ebp = pExceptionInfo->ContextRecord->Ebp;
							wd.cr.Ebx = pExceptionInfo->ContextRecord->Ebx;
							wd.cr.Ecx = pExceptionInfo->ContextRecord->Ecx;
							wd.cr.Edi = pExceptionInfo->ContextRecord->Edi;
							wd.cr.Edx = pExceptionInfo->ContextRecord->Edx;
							wd.cr.Eip = pExceptionInfo->ContextRecord->Eip;
							wd.cr.Esi = pExceptionInfo->ContextRecord->Esi;
							wd.cr.Esp = pExceptionInfo->ContextRecord->Esp;
							////////////////////////////////////////////////////////////////////////////////////////////
							// disassemble address to get disassemblerInstruction and disassemblerInstructionLenght
							////////////////////////////////////////////////////////////////////////////////////////////

							ZydisDecoder decoder;
							ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

							ZydisFormatter formatter;
							ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

							ZyanUSize offset = 0;
							const ZyanUSize length = 0xFFF;
							ZydisDecodedInstruction instruction;

							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(wd.exceptionAddress), length - offset, &instruction))) {

								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), wd.exceptionAddress);

								std::string mnemonic(buffer);
								wd.disassemblerInstruction = mnemonic;
								wd.disassemblerInstructionLenght = instruction.length;
							}
							wd.hitCount = 1;
							_watchData.push_back(wd);
						}

						wl.watchData = _watchData;
					}
					_executeWatchlist.push_back(wl);
				}
				executeWatchlist = _executeWatchlist;
			}

			if ((hs.addressToHookMbiStart - 0x1000 <= pExceptionInfo->ContextRecord->XIP) &&
				(hs.addressToHookMbiEnd + 0x1000 >= pExceptionInfo->ContextRecord->XIP) &&
				(pExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 8)
				) {
				//MessageBoxA(0, exceptionCode.c_str(), "exceptionCode", 0);
				int offset = pExceptionInfo->ContextRecord->XIP - hs.addressToHookMbiStart;
				pExceptionInfo->ContextRecord->XIP = static_cast<DWORD>(hs.allocatedAddressStart + offset);
				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}
		//MessageBoxA(0, exceptionCode.c_str(), "exceptionCode", 0);
		//MessageBoxA(0, hexify<DWORD>(DWORD(pExceptionInfo->ContextRecord->XIP)).c_str(), "", 0);
		//return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

bool UltimateHooks::deinit()
{
	DWORD old;
	if (VEH_Handle)
	{
		if (RemoveVectoredExceptionHandler(VEH_Handle))
		{

			for (HookEntries hs : hookEntries)
			{

				auto addr = (PVOID)hs.addressToHook;
				auto size = static_cast<SIZE_T>(static_cast<int>(1));

				if (NT_SUCCESS(
					makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14,
						0x00)(GetCurrentProcess(), &addr, &size, hs.addressToHookOldProtect, &old)))
				{
					//MessageBoxA(0, "VirtualProtect success", "", 0);
					//MessageBoxA(0, ("addr:" + hexify<DWORD>((DWORD)addr)).c_str(), "", 0);
					//MessageBoxA(0, ("size:" + hexify<int>((int)size)).c_str(), "", 0);
					//MessageBoxA(0, ("oldProtection:" + hexify<DWORD>((DWORD)old)).c_str(), "", 0);
				}
				else
				{
					//MessageBoxA(0, "VirtualProtect failed", "", 0);
				}
			}
			hookEntries.clear();
			RemoveVectoredContinueHandler(VCH_Handle);
			return true;
		} //MessageBoxA(0, "RemoveVectoredExceptionHandler failed", "", 0);
	}
	else
	{
		//MessageBoxA(0, "no VEH_Hanlde", "", 0);
	}

	return false;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
bool UltimateHooks::Hook(DWORD original_fun, size_t offset)
{
	auto mbi = MEMORY_BASIC_INFORMATION{ 0 };
	if (!VirtualQuery(reinterpret_cast<void*>(original_fun), &mbi, sizeof(mbi))) {
		return false;
	}

	HookEntries hs;
	hs.addressToHook = original_fun;
	hs.addressToHookOldProtect = mbi.Protect;
	hs.addressToHookMbiStart = ((DWORD)mbi.BaseAddress);
	hs.addressToHookMbiEnd = ((DWORD)mbi.BaseAddress) + 0x1000;
	hs.addressToHookMbiSize = 0x1000;
	
	PVOID NewRegionPVOID = nullptr;
	DWORD NewRegion = VirtualAllocateRegion(NewRegionPVOID, ((DWORD)mbi.BaseAddress - 0x1000), 0x3000) + 0x1000;
	CopyRegion((DWORD)NewRegionPVOID, ((DWORD)mbi.BaseAddress - 0x1000), 0x3000);
	FixRellocation(((DWORD)mbi.BaseAddress - 0x1000), ((DWORD)mbi.BaseAddress - 0x1000) + 0x3000, (DWORD)NewRegionPVOID, 0x3000, offset);

	hs.allocatedAddressStart = NewRegion;
	hs.allocatedAddressEnd = NewRegion + 0x1000;
	hs.allocatedAddressSize = 0x1000;
	hs.addressToHookoffsetFromStart = original_fun - ((DWORD)mbi.BaseAddress);

	for (HookEntries he : hookEntries) {
		if ((he.addressToHookMbiStart == hs.addressToHookMbiStart) && 
			(he.addressToHookMbiEnd == hs.addressToHookMbiEnd))
		{
			return true; // region is already in hookEntries and should not be duplicated
		}
	}

	//Register the Custom Exception Handler
	if (!IsDoneInit)
	{
		VEH_Handle = AddVectoredExceptionHandler(true, static_cast<PTOP_LEVEL_EXCEPTION_FILTER>(LeoHandler));
		IsDoneInit = true;
	}

	//Toggle PAGE_GUARD flag on the page
	if (VEH_Handle)
	{
		/*
		oldProtection = Controller->VirtualProtect((DWORD)og_fun, 1, PAGE_EXECUTE_READ | PAGE_GUARD);
		if (oldProtection) {
			//MessageBoxA(0, "VirtualProtect success", "", 0);
			return true;
		}else {
			//MessageBoxA(0, "VirtualProtect failed", "", 0);
		}
		*/

		auto addr = (PVOID)original_fun;
		auto size = static_cast<SIZE_T>(static_cast<int>(1));
		
		if (NT_SUCCESS(
			makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(
				GetCurrentProcess(), &addr, &size, PAGE_READONLY, &hs.addressToHookOldProtect)))
		{
			hookEntries.push_back(hs);
			return true;
		} //MessageBoxA(0, "VirtualProtect failed", "", 0);
	}
	else
	{
		//MessageBoxA(0, "no VEH_Hanlde", "", 0);
	}

	return false;
}

bool UltimateHooks::UnHook(DWORD original_fun)
{
	bool isOrigFun_found = false;
	HookEntries _hs;
	std::vector<HookEntries> hookListCopy = hookEntries;
	for (HookEntries hs : hookListCopy)
	{
		if (hs.addressToHook == original_fun)
		{
			isOrigFun_found = true;
			_hs = hs;
		}
	}
	if (!isOrigFun_found)
		return false;

	if (!IsDoneInit || !VEH_Handle)
	{
		return false;
	}

	//Toggle PAGE_GUARD flag on the page
	if (VEH_Handle)
	{
		/*
		oldProtection = Controller->VirtualProtect((DWORD)og_fun, 1, PAGE_EXECUTE_READ | PAGE_GUARD);
		if (oldProtection) {
			//MessageBoxA(0, "VirtualProtect success", "", 0);
			return true;
		}else {
			//MessageBoxA(0, "VirtualProtect failed", "", 0);
		}
		*/

		auto addr = (PVOID)original_fun;
		auto size = static_cast<SIZE_T>(static_cast<int>(1));

		if (NT_SUCCESS(
			makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(
				GetCurrentProcess(), &addr, &size, _hs.addressToHookOldProtect, &_hs.addressToHookOldProtect)))
		{
			//MessageBoxA(0, ("addr:" + hexify<DWORD>((DWORD)addr)).c_str(), "", 0);
			//MessageBoxA(0, ("size:" + hexify<int>((int)size)).c_str(), "", 0);
			//MessageBoxA(0, ("oldProtection:" + hexify<DWORD>((DWORD)oldProtection)).c_str(), "", 0);
			//MessageBoxA(0, "VirtualProtect success", "", 0);
		}
		else
		{
			//MessageBoxA(0, "VirtualProtect failed", "", 0);
		}
		return true;
	} //MessageBoxA(0, "no VEH_Hanlde", "", 0);

	return false;
}

bool UltimateHooks::addHook(DWORD address, size_t offset)
{
	if (Hook(address, offset))
	{
		return true;
	} //MessageBoxA(0, "Hook Success!!!", "", 0);
	return false;
	//MessageBoxA(0, "Hooked", "", 0);
}

bool UltimateHooks::removeHook(DWORD address)
{
	if (UnHook(address))
	{
		std::vector<HookEntries> hookListCopy = hookEntries;
		std::vector<HookEntries> _hookList = {};
		for (HookEntries hs : hookListCopy)
		{
			if (hs.addressToHook != address)
				_hookList.push_back(hs);
		}
		hookEntries = _hookList;
		return true;
	}
	else
	{
		//MessageBoxA(0, "Hook Success!!!", "", 0);
	}
	return false;
	//MessageBoxA(0, "Hooked", "", 0);
}

DWORD UltimateHooks::VirtualAllocateRegion(PVOID& NewFunction, DWORD OrigAddress, size_t size) {
	NewFunction = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	return (DWORD)NewFunction;
}

void UltimateHooks::CopyRegion(DWORD dest, DWORD source, size_t size) {
	(void)memcpy((void*)dest, (PVOID)source, size);
}

void UltimateHooks::FixRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size, size_t _offset)
{
	// Initialize decoder context
	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

	// Initialize formatter. Only required when you actually plan to do instruction
	// formatting ("disassembling"), like we do here
	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

	// Loop over the instructions in our buffer.
	// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
	// visualize relative addressing
	ZyanU32 runtime_address = NewFnAddress + _offset;
	ZyanUSize offset = _offset;
	const ZyanUSize length = size;
	ZydisDecodedInstruction instruction;
	int fixedAddressesCount = 0;

	while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction)))
	{
		// Print current instruction pointer.
		//AppLog.AddLog("%08" PRIX32 " ", runtime_address);

		// Format & print the binary instruction structure to human readable format
		char buffer[256];
		ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
			runtime_address);
		//AppLog.AddLog(buffer);
		//AppLog.AddLog("\n");

		std::string mnemonic(buffer);
		//V21::GameClient::PrintChat(buffer, IM_COL32(255, 69, 0, 255));
		if (mnemonic.find("call 0x") != std::string::npos) {
			//AppLog.AddLog("%08" PRIX32 " ", (runtime_address));
			//AppLog.AddLog(buffer);
			DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);

			DWORD originalCall = 0;
			/////////////////////////////////////////////
			// GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			{
				// Initialize decoder context
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

				// Initialize formatter. Only required when you actually plan to do instruction
				// formatting ("disassembling"), like we do here
				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);

				// Loop over the instructions in our buffer.
				// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
				// visualize relative addressing
				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);

						if (mnemonic1.find("call 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(5, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			/////////////////////////////////////////////
			// END GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			//AppLog.AddLog(" => ");
			/////////////////////////////////////////////
			// APPLY RELOCATION FIXES
			/////////////////////////////////////////////
			{
				DWORD calc1 = (runtime_address - originalCall + 4);
				DWORD calc = 0xFFFFFFFF - calc1;
				*(DWORD*)(runtime_address + 1) = calc;

				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					//AppLog.AddLog(buffer);
					//////////////////////////////////////////////////////////////////////////////////////////
					// CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
					{
						DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress+0x1000) && (hex <= OldFnAddressEnd-0x1000)) {
							DWORD calc1 = (runtime_address - hex + 4);
							DWORD calc = 0xFFFFFFFF - calc1;
							*(DWORD*)(runtime_address + 1) = calc;

							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

								std::string mnemonic(buffer);
								//AppLog.AddLog(" => ");
								//AppLog.AddLog(buffer);
							}

						}
					}
					//////////////////////////////////////////////////////////////////////////////////////////
					// END CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
				}
			}
			/////////////////////////////////////////////
			// END APPLY RELOCATION FIXES
			/////////////////////////////////////////////

			//AppLog.AddLog("\n");
			fixedAddressesCount++;
		}
		else if (mnemonic.find("int3") != std::string::npos) {
			*(BYTE*)(NewFnAddress + offset) = 0x90;
			fixedAddressesCount++;
		}
		else if ((mnemonic.find("jmp 0x") != std::string::npos) && (*(BYTE*)(runtime_address) == 0xe9)) {

			//AppLog.AddLog("%08" PRIX32 " ", (runtime_address));
			//AppLog.AddLog(buffer);
			DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);

			DWORD originalCall = 0;
			/////////////////////////////////////////////
			// GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			{
				// Initialize decoder context
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

				// Initialize formatter. Only required when you actually plan to do instruction
				// formatting ("disassembling"), like we do here
				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);

				// Loop over the instructions in our buffer.
				// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
				// visualize relative addressing
				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);
						if (mnemonic1.find("jmp 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(4, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			/////////////////////////////////////////////
			// END GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			//AppLog.AddLog(" => ");
			/////////////////////////////////////////////
			// APPLY RELOCATION FIXES
			/////////////////////////////////////////////
			{
				DWORD calcx = originalCall - (OldFnAddress + offset);
				DWORD calcy = calcx + (OldFnAddress + offset);
				DWORD calc = calcy - runtime_address - 0x5;

				*(DWORD*)(runtime_address + 1) = calc;
				
				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					//AppLog.AddLog(buffer);
					//////////////////////////////////////////////////////////////////////////////////////////
					// CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
					{
						DWORD hex = std::strtoul((mnemonic.substr(4, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress + 0x1000) && (hex <= OldFnAddressEnd - 0x1000)) {
							DWORD calc = calcx - 0x5;
							*(DWORD*)(runtime_address + 1) = calc;

							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

								std::string mnemonic(buffer);
								//AppLog.AddLog(" => ");
								//AppLog.AddLog(buffer);
							}

						}
					}
					//////////////////////////////////////////////////////////////////////////////////////////
					// END CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
				}
			}
			/////////////////////////////////////////////
			// END APPLY RELOCATION FIXES
			/////////////////////////////////////////////

			//AppLog.AddLog("\n");
			fixedAddressesCount++;
			
		}

		offset += instruction.length;
		runtime_address += instruction.length;
	}
	//AppLog.AddLog(("Relocated Address Count: " + to_string(fixedAddressesCount) + "\n").c_str());
}

void UltimateHooks::FixFuncRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size)
{
	// Initialize decoder context
	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

	// Initialize formatter. Only required when you actually plan to do instruction
	// formatting ("disassembling"), like we do here
	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

	// Loop over the instructions in our buffer.
	// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
	// visualize relative addressing
	ZyanU32 runtime_address = NewFnAddress;
	ZyanUSize offset = 0;
	const ZyanUSize length = size;
	ZydisDecodedInstruction instruction;
	int fixedAddressesCount = 0;

	while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction)))
	{
		// Print current instruction pointer.
		//AppLog.AddLog("%08" PRIX32 " ", runtime_address);

		// Format & print the binary instruction structure to human readable format
		char buffer[256];
		ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
			runtime_address);
		//AppLog.AddLog(buffer);
		//AppLog.AddLog("\n");

		std::string mnemonic(buffer);

		if (mnemonic.find("call 0x") != std::string::npos) {
			//AppLog.AddLog("%08" PRIX32 " ", (runtime_address));
			//AppLog.AddLog(buffer);
			DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);

			DWORD originalCall = 0;
			/////////////////////////////////////////////
			// GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			{
				// Initialize decoder context
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

				// Initialize formatter. Only required when you actually plan to do instruction
				// formatting ("disassembling"), like we do here
				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);

				// Loop over the instructions in our buffer.
				// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
				// visualize relative addressing
				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);

						if (mnemonic1.find("call 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(5, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			/////////////////////////////////////////////
			// END GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			//AppLog.AddLog(" => ");
			/////////////////////////////////////////////
			// APPLY RELOCATION FIXES
			/////////////////////////////////////////////
			{
				DWORD calc1 = (runtime_address - originalCall + 4);
				DWORD calc = 0xFFFFFFFF - calc1;
				*(DWORD*)(runtime_address + 1) = calc;

				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					//AppLog.AddLog(buffer);
					//////////////////////////////////////////////////////////////////////////////////////////
					// CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
					{
						DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress) && (hex <= OldFnAddressEnd)) {
							DWORD calc1 = (runtime_address - hex + 4);
							DWORD calc = 0xFFFFFFFF - calc1;
							*(DWORD*)(runtime_address + 1) = calc;

							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

								std::string mnemonic(buffer);
								//AppLog.AddLog(" => ");
								//AppLog.AddLog(buffer);
							}

						}
					}
					//////////////////////////////////////////////////////////////////////////////////////////
					// END CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
				}
			}
			/////////////////////////////////////////////
			// END APPLY RELOCATION FIXES
			/////////////////////////////////////////////

			//AppLog.AddLog("\n");
			fixedAddressesCount++;
		}
		else if (mnemonic.find("int3") != std::string::npos) {
			*(BYTE*)(NewFnAddress + offset) = 0x90;
			fixedAddressesCount++;
		}
		else if ((mnemonic.find("jmp 0x") != std::string::npos) && (*(BYTE*)(runtime_address) == 0xe9)) {

			//AppLog.AddLog("%08" PRIX32 " ", (runtime_address));
			//AppLog.AddLog(buffer);
			DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);

			DWORD originalCall = 0;
			/////////////////////////////////////////////
			// GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			{
				// Initialize decoder context
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

				// Initialize formatter. Only required when you actually plan to do instruction
				// formatting ("disassembling"), like we do here
				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);

				// Loop over the instructions in our buffer.
				// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
				// visualize relative addressing
				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);
						if (mnemonic1.find("jmp 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(4, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			/////////////////////////////////////////////
			// END GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			//AppLog.AddLog(" => ");
			/////////////////////////////////////////////
			// APPLY RELOCATION FIXES
			/////////////////////////////////////////////
			{
				DWORD calcx = originalCall - (OldFnAddress + offset);
				DWORD calcy = calcx + (OldFnAddress + offset);
				DWORD calc = calcy - runtime_address - 0x5;

				*(DWORD*)(runtime_address + 1) = calc;

				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					//AppLog.AddLog(buffer);
					//////////////////////////////////////////////////////////////////////////////////////////
					// CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
					{
						DWORD hex = std::strtoul((mnemonic.substr(4, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress) && (hex <= OldFnAddressEnd)) {
							DWORD calc = calcx - 0x5;
							*(DWORD*)(runtime_address + 1) = calc;

							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

								std::string mnemonic(buffer);
								//AppLog.AddLog(" => ");
								//AppLog.AddLog(buffer);
							}

						}
					}
					//////////////////////////////////////////////////////////////////////////////////////////
					// END CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
				}
			}
			/////////////////////////////////////////////
			// END APPLY RELOCATION FIXES
			/////////////////////////////////////////////

			//AppLog.AddLog("\n");
			fixedAddressesCount++;

		}

		offset += instruction.length;
		runtime_address += instruction.length;
	}
	//AppLog.AddLog(("Relocated Address Count: " + to_string(fixedAddressesCount) + "\n").c_str());
}