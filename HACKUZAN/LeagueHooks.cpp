#pragma once
#include "stdafx.h"
#include "LeagueHooks.h"

#include "PIDManager.h"
//#include <iostream> 
//#include <string>
#include "makesyscall.h"
#include <comdef.h>
//#include <winternl.h>
#include <TlHelp32.h>
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
/*
template< typename T >
std::string hexify(T i)
{
	std::stringbuf buf;
	std::ostream os(&buf);


	os << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;

	return buf.str().c_str();
}
*/

/*
int __fastcall hk_OnProcessSpell(void* spellBook, void* edx, SpellInfo* spellInfo) {
	MessageBoxA(0, "Called hooked OnProcessSpell!", "", 0);
	return oOnProcessSpell_t(spellBook, spellInfo);
}

bool Hook(char* src, char* dst, char* realBase, int len)
{
	if (len < 5) return false;

	DWORD curProtection;

	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(src, 0x90, len);

	uintptr_t relativeAddress = (uintptr_t)(dst - realBase - 5);

	*src = (char)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relativeAddress;

	DWORD temp;
	VirtualProtect(src, len, curProtection, &temp);

	return true;
}

char* TrampHook(char* src, char* dst, char* realBase, unsigned int len)
{
	if (len < 5) return 0;

	// Create the gateway (len + 5 for the overwritten bytes + the jmp)
	char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	// Put the bytes that will be overwritten in the gateway
	memcpy(gateway, realBase, len);

	// Get the gateway to destination addy
	uintptr_t gateJmpAddy = (uintptr_t)(realBase - gateway - 5);

	// Add the jmp opcode to the end of the gateway
	*(gateway + len) = (char)0xE9;

	// Add the address to the jmp
	*(uintptr_t*)(gateway + len + 1) = gateJmpAddy;

	// Place the hook at the destination
	if (Hook(src, dst, realBase, len))
	{
		return gateway;
	}
	else return nullptr;
}

void LeagueHooksVEH::init() {
	DWORD toHookAddress = FindHiddenModule() + oOnprocessSpell;
	DWORD base = (DWORD)GetModuleHandleA(NULL) + oOnprocessSpell;
	oOnProcessSpell_t = (hk_OnProcessSpell_t)TrampHook((char*)toHookAddress, (char*)hk_OnProcessSpell, (char*)base, 6);
	MessageBoxA(0, "Hooked", "", 0);
}

void LeagueHooksVEH::deinit() {
}
*/
////////////////////////////////THIS DOES NOT WORKS/////////////////////////////////////////
/*
int __fastcall hk_OnProcessSpell(void* spellBook, void* edx, SpellInfo* spellInfo) {
	MessageBoxA(0, "Called hooked OnProcessSpell!", "", 0);
	return hook_manager::get()["OnProcessSpellHook"]->call<int>(spellBook, spellInfo);
}

void LeagueHooksVEH::init() {
	DWORD toHookAddress = FindHiddenModule() + oOnprocessSpell;
	DWORD base = (DWORD)GetModuleHandleA(NULL) + oOnprocessSpell;

	auto& mgr = hook_manager::get();

	mgr.init();
	mgr["OnProcessSpellHook"]->hook((void*)toHookAddress, hk_OnProcessSpell);

	MessageBoxA(0, "Hooked", "", 0);
}

void LeagueHooksVEH::deinit() {
}
*/


/*
void print_parameters(PCONTEXT debug_context) {

	MessageBoxA(0,(
		"EAX: "+hexify<DWORD>(debug_context->Eax) + "\n" + 
		"EBX: " + hexify<DWORD>(debug_context->Ebx) + "\n" +
		"EDX: " + hexify<DWORD>(debug_context->Edx) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"ESP: " + hexify<DWORD>(debug_context->Esp) + "\n" +
		"EBP: " + hexify<DWORD>(debug_context->Ebp) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"ESI: " + hexify<DWORD>(debug_context->Esi) + "\n" +
		"EDI: " + hexify<DWORD>(debug_context->Edi) + "\n" +
		"ECX: " + hexify<DWORD>(debug_context->Ecx) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"p0: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esp + 0x0))) + "\n" +
		"p1: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esp + 0x4))) + "\n" +
		"p2: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esp + 0x8))) + "\n" +
		"p3: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esp + 0xC))) + "\n" +
		"p4: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esp + 0x10))) + "\n" +
		"p5: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esp + 0x14))) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"p0: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebp + 0x0))) + "\n" +
		"p1: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebp + 0x4))) + "\n" +
		"p2: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebp + 0x8))) + "\n" +
		"p3: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebp + 0xC))) + "\n" +
		"p4: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebp + 0x10))) + "\n" +
		"p5: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebp + 0x14))) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"p0: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esi + 0x0))) + "\n" +
		"p1: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esi + 0x4))) + "\n" +
		"p2: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esi + 0x8))) + "\n" +
		"p3: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esi + 0xC))) + "\n" +
		"p4: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esi + 0x10))) + "\n" +
		"p5: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Esi + 0x14))) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"p0: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edi + 0x0))) + "\n" +
		"p1: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edi + 0x4))) + "\n" +
		"p2: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edi + 0x8))) + "\n" +
		"p3: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edi + 0xC))) + "\n" +
		"p4: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edi + 0x10))) + "\n" +
		"p5: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edi + 0x14))) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"p0: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Eax + 0x0))) + "\n" +
		"p1: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Eax + 0x4))) + "\n" +
		"p2: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Eax + 0x8))) + "\n" +
		"p3: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Eax + 0xC))) + "\n" +
		"p4: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Eax + 0x10))) + "\n" +
		"p5: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Eax + 0x14))) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"p0: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebx + 0x0))) + "\n" +
		"p1: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebx + 0x4))) + "\n" +
		"p2: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebx + 0x8))) + "\n" +
		"p3: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebx + 0xC))) + "\n" +
		"p4: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebx + 0x10))) + "\n" +
		"p5: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ebx + 0x14))) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"p0: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edx + 0x0))) + "\n" +
		"p1: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edx + 0x4))) + "\n" +
		"p2: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edx + 0x8))) + "\n" +
		"p3: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edx + 0xC))) + "\n" +
		"p4: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edx + 0x10))) + "\n" +
		"p5: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Edx + 0x14))) + "\n"
		).c_str(), "", 0);

	MessageBoxA(0, (
		"p0: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ecx + 0x0))) + "\n" +
		"p1: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ecx + 0x4))) + "\n" +
		"p2: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ecx + 0x8))) + "\n" +
		"p3: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ecx + 0xC))) + "\n" +
		"p4: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ecx + 0x10))) + "\n" +
		"p5: " + hexify<DWORD>((DWORD)(*(DWORD*)(debug_context->Ecx + 0x14))) + "\n"
		).c_str(), "", 0);
}*/


std::vector<HookStruct> LeagueHooks::hookListPG = {};
std::vector<HookStructHWBP> LeagueHooks::hookListHWBP = {};
PVOID LeagueHooks::VEH_Handle = nullptr;
PVOID LeagueHooks::VCH_Handle = nullptr;
bool LeagueHooks::IsDoneInit = false;
//uint32_t LeagueHooksVEH::oldProtection = 0;

DWORD LeagueHooks::DR0 = 0;
DWORD LeagueHooks::DR1 = 0;
DWORD LeagueHooks::DR2 = 0;
DWORD LeagueHooks::DR3 = 0;
DWORD LeagueHooks::DR6 = 0;
DWORD LeagueHooks::DR7 = 0;
DWORD LeagueHooks::init()
{
	//Register the Custom Exception Handler
	if (!IsDoneInit)
	{
		VEH_Handle = AddVectoredExceptionHandler(1, (PTOP_LEVEL_EXCEPTION_FILTER)(LeoHandler));
		VCH_Handle = AddVectoredContinueHandler(1, (PVECTORED_EXCEPTION_HANDLER)VCHandler);
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

bool LeagueHooks::AreInSamePage(const DWORD* Addr1, const DWORD* Addr2)
{
	MEMORY_BASIC_INFORMATION mbi1;
	if (!VirtualQuery(Addr1, &mbi1, sizeof(mbi1))) //Get Page information for Addr1
		return true;

	MEMORY_BASIC_INFORMATION mbi2;
	if (!VirtualQuery(Addr2, &mbi2, sizeof(mbi2))) //Get Page information for Addr1
		return true;

	if (mbi1.BaseAddress == mbi2.BaseAddress) //See if the two pages start at the same Base Address
		return true; //Both addresses are in the same page, abort hooking!

	return false;
}


void DataLog(EXCEPTION_POINTERS* pExceptionInfo)
{
	static int j = 0;
	//AppLog.AddLog(("Entry: " + to_string(j) + "\n").c_str());
	string ExceptionCode = "";
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
	{
		ExceptionCode = "EXCEPTION_ACCESS_VIOLATION";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ARRAY_BOUNDS_EXCEEDED)
	{
		ExceptionCode = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		ExceptionCode = "EXCEPTION_BREAKPOINT";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_DATATYPE_MISALIGNMENT)
	{
		ExceptionCode = "EXCEPTION_DATATYPE_MISALIGNMENT";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_DENORMAL_OPERAND)
	{
		ExceptionCode = "EXCEPTION_FLT_DENORMAL_OPERAND";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_DIVIDE_BY_ZERO)
	{
		ExceptionCode = "EXCEPTION_FLT_DIVIDE_BY_ZERO";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_INEXACT_RESULT)
	{
		ExceptionCode = "EXCEPTION_FLT_INEXACT_RESULT";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_INVALID_OPERATION)
	{
		ExceptionCode = "EXCEPTION_FLT_INVALID_OPERATION";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_OVERFLOW)
	{
		ExceptionCode = "EXCEPTION_FLT_OVERFLOW";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_STACK_CHECK)
	{
		ExceptionCode = "EXCEPTION_FLT_STACK_CHECK";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_UNDERFLOW)
	{
		ExceptionCode = "EXCEPTION_FLT_UNDERFLOW";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ILLEGAL_INSTRUCTION)
	{
		ExceptionCode = "EXCEPTION_ILLEGAL_INSTRUCTION";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
	{
		ExceptionCode = "EXCEPTION_IN_PAGE_ERROR";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO)
	{
		ExceptionCode = "EXCEPTION_INT_DIVIDE_BY_ZERO";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INT_OVERFLOW)
	{
		ExceptionCode = "EXCEPTION_INT_OVERFLOW";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INVALID_DISPOSITION)
	{
		ExceptionCode = "EXCEPTION_INVALID_DISPOSITION";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_NONCONTINUABLE_EXCEPTION)
	{
		ExceptionCode = "EXCEPTION_NONCONTINUABLE_EXCEPTION";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_PRIV_INSTRUCTION)
	{
		ExceptionCode = "EXCEPTION_PRIV_INSTRUCTION";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		ExceptionCode = "EXCEPTION_SINGLE_STEP";
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
	{
		ExceptionCode = "EXCEPTION_STACK_OVERFLOW";
	}

	writeDataToFile("--------------------------------------------------------------------", 0x0);
	writeDataToFile("ENTRY", static_cast<DWORD>(j));
	writeDataToFile(ExceptionCode, 0x0);
	writeDataToFile("ContextRecord->ContextFlags", pExceptionInfo->ContextRecord->ContextFlags);
	writeDataToFile("ContextRecord->EFlags", pExceptionInfo->ContextRecord->EFlags);
	writeDataToFile("ContextRecord->Dr0", pExceptionInfo->ContextRecord->Dr0);
	writeDataToFile("ContextRecord->Dr1", pExceptionInfo->ContextRecord->Dr1);
	writeDataToFile("ContextRecord->Dr2", pExceptionInfo->ContextRecord->Dr2);
	writeDataToFile("ContextRecord->Dr3", pExceptionInfo->ContextRecord->Dr3);
	writeDataToFile("ContextRecord->Dr6", pExceptionInfo->ContextRecord->Dr6);
	writeDataToFile("ContextRecord->Dr7", pExceptionInfo->ContextRecord->Dr7);
	writeDataToFile("ContextRecord->Eax", pExceptionInfo->ContextRecord->Eax);
	writeDataToFile("ContextRecord->Ebp", pExceptionInfo->ContextRecord->Ebp);
	writeDataToFile("ContextRecord->Ebx", pExceptionInfo->ContextRecord->Ebx);
	writeDataToFile("ContextRecord->Ecx", pExceptionInfo->ContextRecord->Ecx);
	writeDataToFile("ContextRecord->Edi", pExceptionInfo->ContextRecord->Edi);
	writeDataToFile("ContextRecord->Edx", pExceptionInfo->ContextRecord->Edx);
	writeDataToFile("ContextRecord->Eip", pExceptionInfo->ContextRecord->Eip);
	writeDataToFile("ContextRecord->Esi", pExceptionInfo->ContextRecord->Esi);
	writeDataToFile("ContextRecord->Esp", pExceptionInfo->ContextRecord->Esp);
	writeDataToFile("ContextRecord->SegCs", pExceptionInfo->ContextRecord->SegCs);
	writeDataToFile("ContextRecord->SegDs", pExceptionInfo->ContextRecord->SegDs);
	writeDataToFile("ContextRecord->SegEs", pExceptionInfo->ContextRecord->SegEs);
	writeDataToFile("ContextRecord->SegFs", pExceptionInfo->ContextRecord->SegFs);
	writeDataToFile("ContextRecord->SegGs", pExceptionInfo->ContextRecord->SegGs);
	writeDataToFile("ContextRecord->SegSs", pExceptionInfo->ContextRecord->SegSs);
	int i = 0;
	for (BYTE b : pExceptionInfo->ContextRecord->ExtendedRegisters)
	{
		writeDataToFile("ContextRecord->ExtendedRegisters[" + to_string(i) + "]", static_cast<DWORD>(b));
		i++;
	}
	writeDataToFile("ContextRecord->FloatSave.ControlWord", pExceptionInfo->ContextRecord->FloatSave.ControlWord);
	writeDataToFile("ContextRecord->FloatSave.DataOffset", pExceptionInfo->ContextRecord->FloatSave.DataOffset);
	writeDataToFile("ContextRecord->FloatSave.DataSelector", pExceptionInfo->ContextRecord->FloatSave.DataSelector);
	writeDataToFile("ContextRecord->FloatSave.ErrorOffset", pExceptionInfo->ContextRecord->FloatSave.ErrorOffset);
	writeDataToFile("ContextRecord->FloatSave.ErrorSelector", pExceptionInfo->ContextRecord->FloatSave.ErrorSelector);
	writeDataToFile("ContextRecord->FloatSave.Spare0", pExceptionInfo->ContextRecord->FloatSave.Spare0);
	writeDataToFile("ContextRecord->FloatSave.StatusWord", pExceptionInfo->ContextRecord->FloatSave.StatusWord);
	writeDataToFile("ContextRecord->FloatSave.TagWord", pExceptionInfo->ContextRecord->FloatSave.TagWord);
	i = 0;
	for (BYTE b : pExceptionInfo->ContextRecord->FloatSave.RegisterArea)
	{
		writeDataToFile("ContextRecord->FloatSave.RegisterArea[" + to_string(i) + "]", static_cast<DWORD>(b));
		i++;
	}
	writeDataToFile("ExceptionRecord->ExceptionAddress", (DWORD)pExceptionInfo->ExceptionRecord->ExceptionAddress);
	writeDataToFile("ExceptionRecord->ExceptionCode", pExceptionInfo->ExceptionRecord->ExceptionCode);
	writeDataToFile("ExceptionRecord->ExceptionFlags", pExceptionInfo->ExceptionRecord->ExceptionFlags);
	writeDataToFile("ExceptionRecord->NumberParameters", pExceptionInfo->ExceptionRecord->NumberParameters);
	i = 0;
	for (ULONG_PTR u : pExceptionInfo->ExceptionRecord->ExceptionInformation)
	{
		writeDataToFile("ExceptionRecord->ExceptionInformation[" + to_string(i) + "]", static_cast<DWORD>(u));
		i++;
	}
	j++;
}

LONG WINAPI LeagueHooks::VCHandler(EXCEPTION_POINTERS* pExceptionInfo) {
	//AppLog.AddLog("VCH called\n");
	pExceptionInfo->ContextRecord->Dr0 = DR0;
	pExceptionInfo->ContextRecord->Dr1 = DR1;
	pExceptionInfo->ContextRecord->Dr2 = DR2;
	pExceptionInfo->ContextRecord->Dr3 = DR3;
	pExceptionInfo->ContextRecord->Dr7 = DR7;
	return EXCEPTION_CONTINUE_EXECUTION;
}

LONG WINAPI LeagueHooks::LeoHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
	//we'll clear this shit so that other handlers thinks pExceptionInfo is clean
	pExceptionInfo->ContextRecord->Dr0 = 0;
	pExceptionInfo->ContextRecord->Dr1 = 0;
	pExceptionInfo->ContextRecord->Dr2 = 0;
	pExceptionInfo->ContextRecord->Dr3 = 0;
	pExceptionInfo->ContextRecord->Dr6 = 0;
	pExceptionInfo->ContextRecord->Dr7 = 0;

	/*if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
	{
		AppLog.AddLog("EXCEPTION_ACCESS_VIOLATION\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ARRAY_BOUNDS_EXCEEDED)
	{
		AppLog.AddLog("EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		AppLog.AddLog("EXCEPTION_BREAKPOINT\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_DATATYPE_MISALIGNMENT)
	{
		AppLog.AddLog("EXCEPTION_DATATYPE_MISALIGNMENT\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_DENORMAL_OPERAND)
	{
		AppLog.AddLog("EXCEPTION_FLT_DENORMAL_OPERAND\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_DIVIDE_BY_ZERO)
	{
		AppLog.AddLog("EXCEPTION_FLT_DIVIDE_BY_ZERO\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_INEXACT_RESULT)
	{
		AppLog.AddLog("EXCEPTION_FLT_INEXACT_RESULT\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_INVALID_OPERATION)
	{
		AppLog.AddLog("EXCEPTION_FLT_INVALID_OPERATION\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_OVERFLOW)
	{
		AppLog.AddLog("EXCEPTION_FLT_OVERFLOW\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_STACK_CHECK)
	{
		AppLog.AddLog("EXCEPTION_FLT_STACK_CHECK\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_UNDERFLOW)
	{
		AppLog.AddLog("EXCEPTION_FLT_UNDERFLOW\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ILLEGAL_INSTRUCTION)
	{
		AppLog.AddLog("EXCEPTION_ILLEGAL_INSTRUCTION\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
	{
		AppLog.AddLog("EXCEPTION_IN_PAGE_ERROR\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO)
	{
		AppLog.AddLog("EXCEPTION_INT_DIVIDE_BY_ZERO\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INT_OVERFLOW)
	{
		AppLog.AddLog("EXCEPTION_INT_OVERFLOW\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INVALID_DISPOSITION)
	{
		AppLog.AddLog("EXCEPTION_INVALID_DISPOSITION\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_NONCONTINUABLE_EXCEPTION)
	{
		AppLog.AddLog("EXCEPTION_NONCONTINUABLE_EXCEPTION\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_PRIV_INSTRUCTION)
	{
		AppLog.AddLog("EXCEPTION_PRIV_INSTRUCTION\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		AppLog.AddLog("EXCEPTION_SINGLE_STEP\n");
	}
	else if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
	{
		AppLog.AddLog("EXCEPTION_STACK_OVERFLOW\n");
	}*/

	//DataLog(pExceptionInfo);

	//PAGE_GUARD
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION)
		//We will catch PAGE_GUARD Violation
	{
		for (HookStruct hs : hookListPG)
		{
			if (pExceptionInfo->ContextRecord->XIP == static_cast<DWORD>(hs.og_fun))
				//Make sure we are at the address we want within the page
			{
				//print_parameters(pExceptionInfo->ContextRecord);
				//hk_OnProcessSpell2(pExceptionInfo->ContextRecord);
				pExceptionInfo->ContextRecord->XIP = static_cast<DWORD>(hs.hk_fun);
				//Modify EIP/RIP to where we want to jump to instead of the original function
			}
		}

		pExceptionInfo->ContextRecord->EFlags |= 0x100;
		//Will trigger an STATUS_SINGLE_STEP exception right after the next instruction get executed. In short, we come right back into this exception handler 1 instruction later
		return EXCEPTION_CONTINUE_EXECUTION; //Continue to next instruction
	}

	if (pExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP)
		//We will also catch STATUS_SINGLE_STEP, meaning we just had a PAGE_GUARD violation
	{
		DWORD dwOld;
		for (HookStruct hs : hookListPG)
		{
			auto addr = (PVOID)hs.og_fun;
			auto size = static_cast<SIZE_T>(static_cast<int>(1));
			NTSTATUS res = makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2,
			                                     0x14, 0x00)(GetCurrentProcess(), &addr, &size,
			                                                 PAGE_EXECUTE_READ | PAGE_GUARD, &dwOld);
		}

		for (HookStructHWBP hs : hookListHWBP)
		{
			if (pExceptionInfo->ContextRecord->XIP == static_cast<DWORD>(hs.og_fun))
				//Make sure we are at the address we want within the page
			{
				pExceptionInfo->ContextRecord->Dr6 = 0;
				pExceptionInfo->ContextRecord->XIP = static_cast<DWORD>(hs.hk_fun);
				//Modify EIP/RIP to where we want to jump to instead of the original function
			}
		}
		return EXCEPTION_CONTINUE_EXECUTION; //Continue the next instruction
	}

	return EXCEPTION_CONTINUE_SEARCH;
	//Keep going down the exception handling list to find the right handler IF it is not PAGE_GUARD nor SINGLE_STEP
}

bool LeagueHooks::deinit()
{
	DWORD old;
	if (VEH_Handle)
	{
		std::vector<HookStructHWBP> _hookList = hookListHWBP;
		for (HookStructHWBP hs : _hookList)
		{
			LeagueHooksHWBP::UnHook(hs.RegIndex);
		}

		if (RemoveVectoredExceptionHandler(VEH_Handle))
		{
			//MessageBoxA(0, "Hook Remove success", "", 0);
			hookListHWBP.clear();

			for (HookStruct hs : hookListPG)
			{
				/*
				old = Controller->VirtualProtect((DWORD)og_fun, 1, oldProtection);
				if (old) {
					//MessageBoxA(0, "VirtualProtect success", "", 0);
					if (RemoveVectoredExceptionHandler(VEH_Handle)) {
						//MessageBoxA(0, "Hook Remove success", "", 0);
						return true;
					}else {
						//MessageBoxA(0, "RemoveVectoredExceptionHandler failed", "", 0);
					}
				}else {
					//MessageBoxA(0, "VirtualProtect failed", "", 0);
				}
				*/

				auto addr = (PVOID)hs.og_fun;
				auto size = static_cast<SIZE_T>(static_cast<int>(1));

				if (NT_SUCCESS(
					makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14,
						0x00)(GetCurrentProcess(), &addr, &size, hs.oldProtection, &old)))
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
			hookListPG.clear();
			//MessageBoxA(0, "Hook Remove success", "", 0);
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
bool LeagueHooksVEH::Hook(DWORD original_fun, DWORD hooked_fun)
{
	HookStruct hs;
	hs.og_fun = original_fun;
	hs.hk_fun = hooked_fun;

	//We cannot hook two functions in the same page, because we will cause an infinite callback
	if (AreInSamePage((const DWORD*)original_fun, (const DWORD*)hooked_fun))
		return false;

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
		hookListPG.push_back(hs);
		if (NT_SUCCESS(
			makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(
				GetCurrentProcess(), &addr, &size, PAGE_EXECUTE_READ | PAGE_GUARD, &hs.oldProtection)))
		{
			//MessageBoxA(0, ("addr:" + hexify<DWORD>((DWORD)addr)).c_str(), "", 0);
			//MessageBoxA(0, ("size:" + hexify<int>((int)size)).c_str(), "", 0);
			//MessageBoxA(0, ("oldProtection:" + hexify<DWORD>((DWORD)oldProtection)).c_str(), "", 0);
			//MessageBoxA(0, "VirtualProtect success", "", 0);
			std::vector<HookStruct> hookListCopy = hookListPG;
			for (HookStruct _hs : hookListCopy)
			{
				if (_hs.og_fun == original_fun)
				{
					_hs.oldProtection = hs.oldProtection;
				}
			}
			hookListPG = hookListCopy;
			return true;
		} //MessageBoxA(0, "VirtualProtect failed", "", 0);
	}
	else
	{
		//MessageBoxA(0, "no VEH_Hanlde", "", 0);
	}

	return false;
}

bool LeagueHooksVEH::UnHook(DWORD original_fun)
{
	bool isOrigFun_found = false;
	HookStruct _hs;
	std::vector<HookStruct> hookListCopy = hookListPG;
	for (HookStruct hs : hookListCopy)
	{
		if (hs.og_fun == original_fun)
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
				GetCurrentProcess(), &addr, &size, _hs.oldProtection, &_hs.oldProtection)))
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

bool LeagueHooksVEH::addHook(DWORD address, DWORD hkAddress)
{
	/////////////////////////////////////
	////      DRIVER USE ONLY
	/////////////////////////////////////
	/*
	{
		if (Controller) {
			//MessageBoxA(0, "Controller is Initialized", "", 0);
		}
		else {
			PIDManager _PIDManager;
			m_pid = _PIDManager.GetAowProcId();

			if (m_pid) {
				Controller = new DriverController(m_pid);

				usermode_pid = GetCurrentProcessId();

				if (usermode_pid) {
					//printf(("> usermode_pid: %d\n"), usermode_pid);
					//MessageBoxA(0, ("usermode_pid: " + hexify<DWORD64>(usermode_pid)).c_str(), "", 0);

					m_base = GetBaseAddress();

					if (m_base) {
						//printf(("> m_pid: %d base: 0x%llx\n"), m_pid, m_base);
						//MessageBoxA(0, ("m_base: " + hexify<DWORD64>(m_base)).c_str(), "", 0);
					}
				}
			}
		}
	}
	*/

	if (Hook(address, hkAddress))
	{
		return true;
	} //MessageBoxA(0, "Hook Success!!!", "", 0);
	return false;
	//MessageBoxA(0, "Hooked", "", 0);
}

bool LeagueHooksVEH::removeHook(DWORD address)
{
	if (UnHook(address))
	{
		std::vector<HookStruct> hookListCopy = hookListPG;
		std::vector<HookStruct> _hookList = {};
		for (HookStruct hs : hookListCopy)
		{
			if (hs.og_fun != address)
				_hookList.push_back(hs);
		}
		hookListPG = _hookList;
		return true;
	}
	else
	{
		//MessageBoxA(0, "Hook Success!!!", "", 0);
	}
	return false;
	//MessageBoxA(0, "Hooked", "", 0);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////HWBP////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool LeagueHooksHWBP::UnHook(uint8_t RegIndex)
{
	//We cannot hook two functions in the same page, because we will cause an infinite callback
	//if (AreInSamePage((const DWORD*)original_fun, (const DWORD*)hooked_fun))
	//	return false;

	//Register the Custom Exception Handler
	if (!IsDoneInit || !VEH_Handle)
	{
		return false;
	}

	//Toggle PAGE_GUARD flag on the page
	if (VEH_Handle)
	{
		HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
		THREADENTRY32 te32;
		hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

		if (hThreadSnap)
		{
			te32.dwSize = sizeof(THREADENTRY32);

			if (!Thread32First(hThreadSnap, &te32))
			{
				CloseHandle(hThreadSnap);
			}

			do
			{
				if (te32.th32OwnerProcessID == GetCurrentProcessId() && te32.th32ThreadID != GetCurrentThreadId())
					//Ignore threads from other processes AND the own thread of course
				{
					std::string threadId = hexify<DWORD>(static_cast<DWORD>(te32.th32ThreadID));

					//bool isDone = false;
					//while (!isDone) {
					//MessageBoxA(0, (threadId + " Opening thread").c_str(), "", 0);

					HANDLE ThreadHandle = nullptr;
					OBJECT_ATTRIBUTES ObjectAttributes;
					CLIENT_ID ClientId;
					InitializeObjectAttributes(&ObjectAttributes, NULL, NULL, NULL, NULL);

					ClientId.UniqueProcess = (PVOID)te32.th32OwnerProcessID;
					ClientId.UniqueThread = (PVOID)te32.th32ThreadID;

					NTSTATUS res = makesyscall<NTSTATUS>(0x2E, 0x01, 0x00, 0x00, "RtlInterlockedCompareExchange64",
					                                     0x170, 0xC2, 0x10, 0x00)(
						&ThreadHandle, THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME,
						&ObjectAttributes, &ClientId);
					if (NT_SUCCESS(res))
					{
						if (ThreadHandle)
						{
							//AppLog.AddLog((threadId + ": OpenThread Success\n").c_str());
							CONTEXT Ctx;
							Ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

							//MessageBoxA(0, (threadId + " GetThreadContext").c_str(), "", 0);
							//if (GetThreadContext(hThread, &Ctx))
							if (NT_SUCCESS(
								makesyscall<NTSTATUS>(0xF2, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170,
									0xC2, 0x08, 0x00)(ThreadHandle, &Ctx)))
							{
								//AppLog.AddLog((threadId + ": GetThreadContext Success\n").c_str());

								if (te32.th32ThreadID != GetCurrentThreadId())
								{
									//SuspendThread(hThread);
									NTSTATUS res = makesyscall<NTSTATUS>(
										0xBC, 0x01, 0x07, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x08,
										0x00)(ThreadHandle, NULL);
								}
								else
								{
									//AppLog.AddLog((threadId + ": CurrentThread detected. Maybe logs cannot be printed.\n").c_str());
								}

								//MessageBoxA(0, (threadId + " SuspendThread").c_str(), "", 0);
								/*uint8_t RegIndex = 0;
								bool FoundReg = false;
								for (; RegIndex < 4; RegIndex++)
								{
									if ((Ctx.Dr7 & (1 << (RegIndex * 2))) == 0)
									{
										FoundReg = true;
										break;
									}
								}*/
								bool FoundReg = true;
								if (FoundReg)
								{
									//AppLog.AddLog((threadId + ": Found free Reg\n").c_str());

									bool isInvalidReg = false;

									switch (RegIndex)
									{
									case 0:
										Ctx.Dr0 = static_cast<DWORD_PTR>(0);
										DR0 = 0;
										break;
									case 1:
										Ctx.Dr1 = static_cast<DWORD_PTR>(0);
										DR1 = 0;
										break;
									case 2:
										Ctx.Dr2 = static_cast<DWORD_PTR>(0);
										DR2 = 0;
										break;
									case 3:
										Ctx.Dr3 = static_cast<DWORD_PTR>(0);
										DR3 = 0;
										break;
									default:
										isInvalidReg = true;
										break;
									}

									if (!isInvalidReg)
									{
										//AppLog.AddLog((threadId + ": Valid Reg\n").c_str());

										//Turn a local register on
										Ctx.Dr7 &= ~(3ULL << (16 + 4 * RegIndex));
										//00b at 16-17, 20-21, 24-25, 28-29 is execute bp
										Ctx.Dr7 &= ~(3ULL << (18 + 4 * RegIndex));
										// size of 1 (val 0), at 18-19, 22-23, 26-27, 30-31
										Ctx.Dr7 |= 1ULL << (2 * RegIndex);
										DWORD bitsToRemove = ((1 << (2 * RegIndex)) | (3 << (((2 * (RegIndex)) * 2) + 16
										)) | (3 << (((2 * (RegIndex)) * 2) + 18)));
										Ctx.Dr7 -= bitsToRemove;
										DR7 = Ctx.Dr7;

										//Still need to call suspend thread *TODO*
										//MessageBoxA(0, (threadId + " SetThreadContext").c_str(), "", 0);
										//if (SetThreadContext(hThread, &Ctx))
										if (NT_SUCCESS(
											makesyscall<NTSTATUS>(0x8B, 0x01, 0x00, 0x00,
												"RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x08, 0x00)(ThreadHandle
												, &Ctx)))
										{
											//AppLog.AddLog((threadId + ": SetThreadContext success\n").c_str());
											//isDone = true;
										}
										else
										{
											//AppLog.AddLog((threadId + ": Failed to set thread context\n").c_str());
										}
									}
									else
									{
										//AppLog.AddLog((threadId + ": Invalid Reg\n").c_str());
									}
								}
								else
								{
									//AppLog.AddLog((threadId + ": Failed to find free Reg\n").c_str());
								}
								//MessageBoxA(0, (threadId + " ResumeThread").c_str() , "", 0);
								if (te32.th32ThreadID != GetCurrentThreadId())
								{
									//ResumeThread(hThread);
									NTSTATUS res = makesyscall<NTSTATUS>(
										0x6E, 0x00, 0x07, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x08,
										0x00)(ThreadHandle, NULL);
								}
								else
								{
									//AppLog.AddLog((threadId + ": Done manipulating current thread.\n").c_str());
								}
							}
							else
							{
								//AppLog.AddLog((threadId + ": Failed to get context\n").c_str());
							}
							//MessageBoxA(0, (threadId + " CloseHandle").c_str(), "", 0);
							CloseHandle(ThreadHandle);
						}
						else
						{
							//AppLog.AddLog((threadId + ": hThread failed\n").c_str());
						}
					}
					else
					{
						//AppLog.AddLog((threadId + ": Failed to Open Thread\n").c_str());
					}
					//}
				}
			}
			while (Thread32Next(hThreadSnap, &te32));
			CloseHandle(hThreadSnap);
			return true;
		}
	}
	else
	{
		//MessageBoxA(0, "no VEH_Hanlde", "", 0);
	}

	return false;
}

bool LeagueHooksHWBP::Hook(DWORD original_fun, DWORD hooked_fun, uint8_t RegIndex)
{
	std::vector<HookStructHWBP> hookListCopy = hookListHWBP;
	for (HookStructHWBP hs : hookListCopy)
	{
		if (hs.RegIndex == RegIndex)
		{
			return false;
		}
	}
	HookStructHWBP hs;
	hs.og_fun = original_fun;
	hs.hk_fun = hooked_fun;
	hs.RegIndex = RegIndex;
	//We cannot hook two functions in the same page, because we will cause an infinite callback
	//if (AreInSamePage((const DWORD*)original_fun, (const DWORD*)hooked_fun))
	//	return false;

	//Register the Custom Exception Handler
	if (!IsDoneInit)
	{
		VEH_Handle = AddVectoredExceptionHandler(true, static_cast<PTOP_LEVEL_EXCEPTION_FILTER>(LeoHandler));
		IsDoneInit = true;
	}

	//Toggle PAGE_GUARD flag on the page
	if (VEH_Handle)
	{
		hookListHWBP.push_back(hs);
		HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
		THREADENTRY32 te32;
		hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

		if (hThreadSnap)
		{
			te32.dwSize = sizeof(THREADENTRY32);

			if (!Thread32First(hThreadSnap, &te32))
			{
				CloseHandle(hThreadSnap);
			}

			do
			{
				if (te32.th32OwnerProcessID == GetCurrentProcessId() && te32.th32ThreadID != GetCurrentThreadId())
					//Ignore threads from other processes AND the own thread of course
				{
					std::string threadId = hexify<DWORD>(static_cast<DWORD>(te32.th32ThreadID));

					//bool isDone = false;
					//while (!isDone) {
					//MessageBoxA(0, (threadId + " Opening thread").c_str(), "", 0);

					HANDLE ThreadHandle = nullptr;
					OBJECT_ATTRIBUTES ObjectAttributes;
					CLIENT_ID ClientId;
					InitializeObjectAttributes(&ObjectAttributes, NULL, NULL, NULL, NULL);

					ClientId.UniqueProcess = (PVOID)te32.th32OwnerProcessID;
					ClientId.UniqueThread = (PVOID)te32.th32ThreadID;

					NTSTATUS res = makesyscall<NTSTATUS>(0x2E, 0x01, 0x00, 0x00, "RtlInterlockedCompareExchange64",
					                                     0x170, 0xC2, 0x10, 0x00)(
						&ThreadHandle, THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME,
						&ObjectAttributes, &ClientId);
					if (NT_SUCCESS(res))
					{
						if (ThreadHandle)
						{
							//AppLog.AddLog((threadId + ": OpenThread Success\n").c_str());
							CONTEXT Ctx;
							Ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

							//MessageBoxA(0, (threadId + " GetThreadContext").c_str(), "", 0);
							//if (GetThreadContext(hThread, &Ctx))
							if (NT_SUCCESS(
								makesyscall<NTSTATUS>(0xF2, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170,
									0xC2, 0x08, 0x00)(ThreadHandle, &Ctx)))
							{
								//AppLog.AddLog((threadId + ": GetThreadContext Success\n").c_str());

								if (te32.th32ThreadID != GetCurrentThreadId())
								{
									//SuspendThread(hThread);
									NTSTATUS res = makesyscall<NTSTATUS>(
										0xBC, 0x01, 0x07, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x08,
										0x00)(ThreadHandle, NULL);
								}
								else
								{
									//AppLog.AddLog((threadId + ": CurrentThread detected. Maybe logs cannot be printed.\n").c_str());
								}

								//MessageBoxA(0, (threadId + " SuspendThread").c_str(), "", 0);
								/*uint8_t RegIndex = 0;
								bool FoundReg = false;
								for (; RegIndex < 4; RegIndex++)
								{
									if ((Ctx.Dr7 & (1 << (RegIndex * 2))) == 0)
									{
										FoundReg = true;
										break;
									}
								}*/
								bool FoundReg = true;
								if (FoundReg)
								{
									//AppLog.AddLog((threadId + ": Found free Reg\n").c_str());

									bool isInvalidReg = false;

									switch (RegIndex)
									{
									case 0:
										Ctx.Dr0 = static_cast<DWORD_PTR>(original_fun);
										DR0 = (DWORD_PTR)original_fun;
										break;
									case 1:
										Ctx.Dr1 = static_cast<DWORD_PTR>(original_fun);
										DR1 = (DWORD_PTR)original_fun;
										break;
									case 2:
										Ctx.Dr2 = static_cast<DWORD_PTR>(original_fun);
										DR2 = (DWORD_PTR)original_fun;
										break;
									case 3:
										Ctx.Dr3 = static_cast<DWORD_PTR>(original_fun);
										DR3 = (DWORD_PTR)original_fun;
										break;
									default:
										isInvalidReg = true;
										break;
									}

									if (!isInvalidReg)
									{
										//AppLog.AddLog((threadId + ": Valid Reg\n").c_str());

										//Turn a local register on
										//Ctx.Dr7 |= ((1 << (2 * RegIndex)) | (3 << (((2 * (RegIndex)) * 2) + 16)) | (3 << (((2 * (RegIndex)) * 2) + 18)));
										Ctx.Dr7 &= ~(3ULL << (16 + 4 * RegIndex));
										//00b at 16-17, 20-21, 24-25, 28-29 is execute bp
										Ctx.Dr7 &= ~(3ULL << (18 + 4 * RegIndex));
										// size of 1 (val 0), at 18-19, 22-23, 26-27, 30-31
										Ctx.Dr7 |= 1ULL << (2 * RegIndex);
										DR7 = Ctx.Dr7;

										//Still need to call suspend thread *TODO*
										//MessageBoxA(0, (threadId + " SetThreadContext").c_str(), "", 0);
										//if (SetThreadContext(hThread, &Ctx))
										if (NT_SUCCESS(
											makesyscall<NTSTATUS>(0x8B, 0x01, 0x00, 0x00,
												"RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x08, 0x00)(ThreadHandle
												, &Ctx)))
										{
											//AppLog.AddLog((threadId + ": SetThreadContext success\n").c_str());
											//isDone = true;
										}
										else
										{
											//AppLog.AddLog((threadId + ": Failed to set thread context\n").c_str());
										}
									}
									else
									{
										//AppLog.AddLog((threadId + ": Invalid Reg\n").c_str());
									}
								}
								else
								{
									//AppLog.AddLog((threadId + ": Failed to find free Reg\n").c_str());
								}
								//MessageBoxA(0, (threadId + " ResumeThread").c_str() , "", 0);
								if (te32.th32ThreadID != GetCurrentThreadId())
								{
									//ResumeThread(hThread);
									NTSTATUS res = makesyscall<NTSTATUS>(
										0x6E, 0x00, 0x07, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x08,
										0x00)(ThreadHandle, NULL);
								}
								else
								{
									//AppLog.AddLog((threadId + ": Done manipulating current thread.\n").c_str());
								}
							}
							else
							{
								//AppLog.AddLog((threadId + ": Failed to get context\n").c_str());
							}
							//MessageBoxA(0, (threadId + " CloseHandle").c_str(), "", 0);
							CloseHandle(ThreadHandle);
						}
						else
						{
							//AppLog.AddLog((threadId + ": hThread failed\n").c_str());
						}
					}
					else
					{
						//AppLog.AddLog((threadId + ": Failed to Open Thread\n").c_str());
					}
					//}
				}
			}
			while (Thread32Next(hThreadSnap, &te32));
			CloseHandle(hThreadSnap);

			return true;
		}
	}
	else
	{
		//MessageBoxA(0, "no VEH_Hanlde", "", 0);
	}

	return false;
}

bool LeagueHooksHWBP::addHook(DWORD address, DWORD hkAddress, uint8_t RegIndex)
{
	//waitForMap(address);
	if (Hook(address, hkAddress, RegIndex))
	{
		return true;
	}
	else
	{
		//MessageBoxA(0, "Hook Success!!!", "", 0);
	}
	return false;
	//MessageBoxA(0, "Hooked", "", 0);
}

bool LeagueHooksHWBP::removeHook(uint8_t RegIndex)
{
	//waitForMap(address);
	if (UnHook(RegIndex))
	{
		std::vector<HookStructHWBP> hookListCopy = hookListHWBP;
		std::vector<HookStructHWBP> _hookList = {};
		for (HookStructHWBP hs : hookListCopy)
		{
			if (hs.RegIndex != RegIndex)
				_hookList.push_back(hs);
		}
		hookListHWBP = _hookList;
		return true;
	}
	else
	{
		//MessageBoxA(0, "Hook Success!!!", "", 0);
	}
	return false;
	//MessageBoxA(0, "Hooked", "", 0);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
LeagueDecryptData LeagueDecrypt::decrypt(char* szModule) {
	LeagueDecryptData ldd;
	ldd.totalFailedDecrypted = 0;
	ldd.totalSuccessDecrypted = 0;
	ldd.totalSuccess_PAGE_NOACCESS = 0;
	ldd.totalSuccess_EXCEPTION_CONTINUE_EXECUTION = 0;

	auto module = GetModuleHandleA(szModule);


	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t*)module + dosHeader->e_lfanew);
	auto textSection = IMAGE_FIRST_SECTION(ntHeaders);

	auto sizeOfImage = textSection->SizeOfRawData;
	auto scanBytes = reinterpret_cast<uint8_t*>(module) + textSection->VirtualAddress;


	auto mbi = MEMORY_BASIC_INFORMATION{ 0 };
	uint8_t* next_check_address = 0;

	bool isFirstRegion = true;
	for (auto i = 0; i < sizeOfImage; ++i) {

		auto current_address = scanBytes + i;
		if (current_address >= next_check_address) {
			if (!VirtualQuery(reinterpret_cast<void*>(current_address), &mbi, sizeof(mbi)))
				continue;

			if (mbi.Protect != PAGE_NOACCESS || isFirstRegion) {
				isFirstRegion = false;
				i += ((std::uintptr_t(mbi.BaseAddress) + mbi.RegionSize) - (std::uintptr_t(scanBytes) + i));
				i--;
				continue;
			}
			else {
				next_check_address = reinterpret_cast<uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
			}
		}
		int ret = IsMemoryDecrypted((PVOID)((DWORD)current_address));
		if (ret != 0) {
			if (ret == 1) {
				ldd.totalSuccess_PAGE_NOACCESS++;
			}
			else if (ret == 2) {
				ldd.totalSuccess_EXCEPTION_CONTINUE_EXECUTION++;
			}
			//AppLog.AddLog(("Decryption Success for address " + hexify<uint8_t*>(current_address)).c_str());
			ldd.totalSuccessDecrypted++;
		}
		else {
			//AppLog.AddLog(("Decryption Failed for address " + hexify<uint8_t*>(current_address)).c_str());
			ldd.totalFailedDecrypted++;
		}
	}
	return ldd;
}

typedef BOOLEAN(__stdcall* t_RtlDispatchException)(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT ContextRecord);
t_RtlDispatchException fn_RtlDispatchException;

uint8_t* LeagueDecrypt::_RtlDispatchExceptionAddress = nullptr;

int LeagueDecrypt::IsMemoryDecrypted(PVOID Address)
{
	CONTEXT ctx;
	EXCEPTION_RECORD exr;
	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));
	VirtualQuery(Address, &mbi, sizeof(mbi));
	if (mbi.Protect != PAGE_NOACCESS)
	{
		return 1;
	}
	RtlCaptureContext(&ctx);
	memset(&exr, 0, sizeof(EXCEPTION_RECORD));

#ifdef _WIN64
	ctx.Rip = reinterpret_cast<DWORD64>(Address);// (DWORD)FinishThread;
#else
	ctx.Eip = reinterpret_cast<DWORD>(Address);// (DWORD)FinishThread;
#endif // 

	ctx.ContextFlags = 0x1007F;
	ctx.SegCs = 0x23;
	ctx.SegDs = 0x2b;
	ctx.SegEs = 0x2b;
	ctx.SegFs = 0x53;
	ctx.SegGs = 0x2b;
	ctx.SegSs = 0x2b;
	exr.ExceptionAddress = Address;
	exr.NumberParameters = 2;
	exr.ExceptionCode = EXCEPTION_ACCESS_VIOLATION;
	exr.ExceptionInformation[1] = reinterpret_cast<DWORD>(Address);
	_EXCEPTION_POINTERS ei;
	ei.ContextRecord = &ctx;
	ei.ExceptionRecord = &exr;

	if (!_RtlDispatchExceptionAddress)
		return 0;

	DWORD RtlDispatchExceptionAddr = (DWORD)(_RtlDispatchExceptionAddress);

	if (RtlDispatchExceptionAddr) {
		fn_RtlDispatchException = (t_RtlDispatchException)(RtlDispatchExceptionAddr);
		if (fn_RtlDispatchException(&exr, &ctx)) {
			return 2;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
#define ROR(x, y) ((unsigned)(x) >> (y) | (unsigned)(x) << 32 - (y))

typedef
enum _PROCESSINFOCLASS
{
	ProcessBasicInformation,
	ProcessCookie = 36
} PROCESSINFOCLASS;

#pragma comment(lib, "ntdll.lib")
extern "C" NTSYSCALLAPI NTSTATUS NTAPI NtQueryInformationProcess(IN HANDLE ProcessHandle,
                                                                 IN PROCESSINFOCLASS ProcessInformationClass,
                                                                 OUT PVOID ProcessInformation,
                                                                 IN ULONG ProcessInformationLength,
                                                                 OUT PULONG ReturnLength OPTIONAL);

DWORD Process::process_cookie_ = 0;

_VECTORED_HANDLER_LIST* Process::GetVECTORED_HANDLER_LIST(DWORD& veh_addr)
{
	DWORD ntdll = (DWORD)GetModuleHandleA("ntdll.dll");
	_VECTORED_HANDLER_LIST* handler_list;
	DWORD VEHOffset = GetVEHOffset();
	//AppLog.AddLog(("VEHOffset: " + hexify<DWORD>(VEHOffset) + "\n").c_str());
	veh_addr = ntdll + VEHOffset;
	//AppLog.AddLog(("veh_addr: " + hexify<DWORD>(veh_addr) + "\n").c_str());
	handler_list = &*(_VECTORED_HANDLER_LIST*)static_cast<DWORD>(veh_addr);
	//printf("First entry: 0x%p\n", handler_list.first_exception_handler);
	//printf("Last entry: 0x%p\n", handler_list.last_exception_handler);
	if (reinterpret_cast<DWORD>(handler_list->first_exception_handler) ==
		veh_addr + sizeof(DWORD))
	{
		//AppLog.AddLog("VEH list is empty\n");
	}
	return handler_list;
}

const DWORD Process::GetProcessCookie()
{
	DWORD dwProcessCookie = NULL;
	NTSTATUS NtStatus = NtQueryInformationProcess(GetCurrentProcess(), ProcessCookie, &dwProcessCookie, 4, nullptr);

	if (NT_SUCCESS(NtStatus))
	{
		//AppLog.AddLog("NtQueryInformationProcess success\n");
	}
	else
	{
		//AppLog.AddLog("NtQueryInformationProcess failed\n");
		return NULL;
	}

	return dwProcessCookie;
}

DWORD Process::DecodePointerHandle(DWORD pointer)
{
	if (!process_cookie_)
	{
		process_cookie_ = GetProcessCookie();
		if (!process_cookie_)
		{
			return 0;
		}
	}

	unsigned char shift_size = 0x20 - (process_cookie_ & 0x1f);
	return ROR(pointer, shift_size) ^ process_cookie_;
}

DWORD Process::GetVEHOffset()
{
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	//AppLog.AddLog(("ntdll:" + hexify<DWORD>(DWORD(ntdll))+ "\n").c_str());

	BYTE* add_exception_handler = reinterpret_cast<BYTE*>(
		GetProcAddress(ntdll, "RtlAddVectoredExceptionHandler"));
	//AppLog.AddLog(("RtlAddVectoredExceptionHandler:" + hexify<DWORD>(DWORD(add_exception_handler))+"\n").c_str());

	const BYTE* add_exception_handler_sub = add_exception_handler + 0x16;
	//AppLog.AddLog(("RtlpAddVectoredExceptionHandler:" + hexify<DWORD>(DWORD(add_exception_handler_sub))+"\n").c_str());

	std::string sig_str = "89 46 10 81 C3 ?? ?? ?? ??";

	uint8_t* match_list = (uint8_t*)FindPatternV2("ntdll.dll", sig_str);
	//AppLog.AddLog(("match_list:" + hexify<DWORD>(DWORD(match_list))+"\n").c_str());
	BYTE* veh_list = *reinterpret_cast<BYTE**>(match_list + 5);
	size_t veh_list_offset = veh_list - reinterpret_cast<BYTE*>(ntdll);
	//AppLog.AddLog(("LdrpVectorHandlerList:" + hexify<DWORD>(DWORD(veh_list)) + " (ntdll+" + hexify<size_t>(veh_list_offset)+")"+"\n").c_str());

	return veh_list_offset;
}

std::vector<PVECTORED_EXCEPTION_HANDLER> Process::GetAllHandlers()
{
	std::vector<PVECTORED_EXCEPTION_HANDLER> _PVECTORED_EXCEPTION_HANDLER_list;

	DWORD veh_addr;
	//AppLog.AddLog(("Process cookie: " + hexify<DWORD>(Process::GetProcessCookie()) + "\n").c_str());
	//AppLog.AddLog(("GetVEHOffset: " + hexify<DWORD>(Process::GetVEHOffset()) + "\n").c_str());
	_VECTORED_HANDLER_LIST* handler_list = GetVECTORED_HANDLER_LIST(veh_addr);

	//AppLog.AddLog(("First entry: " + hexify<DWORD>((DWORD)handler_list->first_exception_handler) + "\n").c_str());
	//AppLog.AddLog(("Last entry: " + hexify<DWORD>((DWORD)handler_list->last_exception_handler) + "\n").c_str());

	//AppLog.AddLog("Dumping the entries:\n");
	VECTORED_HANDLER_ENTRY* entry;
	entry = &*(VECTORED_HANDLER_ENTRY*)((DWORD)handler_list->first_exception_handler);

	while (true)
	{
		DWORD handler = reinterpret_cast<DWORD>(entry->handler);
		//AppLog.AddLog(("  handler = " + hexify<DWORD>((DWORD)handler) + " => " + hexify<DWORD>(Process::DecodePointerHandle(handler)) + "\n").c_str());
		_PVECTORED_EXCEPTION_HANDLER_list.push_back((PVECTORED_EXCEPTION_HANDLER)(DecodePointerHandle(handler)));

		if (reinterpret_cast<DWORD>(entry->next) == veh_addr + sizeof(DWORD))
		{
			break;
		}
		entry = &*(VECTORED_HANDLER_ENTRY*)((DWORD)entry->next);
	}
	//AppLog.AddLog(("Size of PVECTORED_EXCEPTION_HANDLER_list: " + hexify<DWORD>((DWORD)PVECTORED_EXCEPTION_HANDLER_list.size()) + "\n").c_str());
	return _PVECTORED_EXCEPTION_HANDLER_list;
}

typedef PVOID (__stdcall* t_AddVectoredExceptionHandler)(ULONG FirstHandler,
                                                         PVECTORED_EXCEPTION_HANDLER VectorHandler/*, ULONG Type*/);
t_AddVectoredExceptionHandler fn_AddVectoredExceptionHandler;

typedef BOOL (__stdcall* t_RemoveVectoredExceptionHandler)(PVOID Handle/*, ULONG Type*/);
t_RemoveVectoredExceptionHandler fn_RemoveVectoredExceptionHandler;

void Process::RemoveAllHandlers()
{
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");

	DWORD remove_exception_handler = reinterpret_cast<DWORD>(GetProcAddress(ntdll, "RtlRemoveVectoredExceptionHandler")
	);
	fn_RemoveVectoredExceptionHandler = (t_RemoveVectoredExceptionHandler)remove_exception_handler;
	//AppLog.AddLog(("t_RemoveVectoredExceptionHandler: " + hexify<DWORD>((DWORD)remove_exception_handler) + "\n").c_str());

	for (int i = 0; i < GetAllHandlers().size(); i++)
	{
		DWORD veh_addr;
		_VECTORED_HANDLER_LIST* handler_list = GetVECTORED_HANDLER_LIST(veh_addr);
		//AppLog.AddLog(("first_exception_handler: " + hexify<DWORD>((DWORD)handler_list->first_exception_handler) + "\n").c_str());
		if (fn_RemoveVectoredExceptionHandler(static_cast<PVOID>(handler_list->first_exception_handler)))
		{
			//AppLog.AddLog("\tfn_RemoveVectoredExceptionHandler: Success\n");
		}
		else
		{
			//AppLog.AddLog("\tfn_RemoveVectoredExceptionHandler: Failed\n");
		}
	}
}

void Process::ReAddAllHandlers(std::vector<PVECTORED_EXCEPTION_HANDLER> _PVECTORED_EXCEPTION_HANDLER_list)
{
	for (PVECTORED_EXCEPTION_HANDLER pvh : _PVECTORED_EXCEPTION_HANDLER_list)
	{
		//AppLog.AddLog(("pvh: " + hexify<DWORD>((DWORD)pvh) + "\n").c_str());
		if (AddVectoredExceptionHandler(0, pvh))
		{
			//AppLog.AddLog("\tAddVectoredExceptionHandler: Success\n");
		}
		else
		{
			//AppLog.AddLog("\tAddVectoredExceptionHandler: Failed\n");
		}
	}
}

void Process::ReAddAllContinueHandlers(std::vector<PVECTORED_EXCEPTION_HANDLER> _PVECTORED_EXCEPTION_HANDLER_list)
{
	for (PVECTORED_EXCEPTION_HANDLER pvh : _PVECTORED_EXCEPTION_HANDLER_list)
	{
		//AppLog.AddLog(("AddVectoredExceptionHandler: " + hexify<DWORD>((DWORD)pvh) + "\n").c_str());
		if (AddVectoredContinueHandler(0, pvh))
		{
			//AppLog.AddLog("\tAddVectoredExceptionHandler: Success\n");
		}
		else
		{
			//AppLog.AddLog("\tAddVectoredExceptionHandler: Failed\n");
		}
	}
}

void Process::GetAllModules(DWORD procId) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				//convert from wide char to narrow char array
				char ch[256];
				char DefChar = ' ';

				uintptr_t modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
				_bstr_t c(modEntry.szModule);
				const WCHAR* wc = c;
				WideCharToMultiByte(CP_ACP, 0, (LPCWCH)wc, -1, ch, 256, &DefChar, NULL);

				//AppLog.AddLog((string(ch) + ": " + hexify<uintptr_t>(modBaseAddr) + "\n").c_str());
				//writeDataToFile(ch, (DWORD)modBaseAddr);

			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
}