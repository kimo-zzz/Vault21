#pragma once
#include "stdafx.h"
#include "LeagueFunctions.h"
#include "ExampleAppLog.h"

#include <stdio.h>
#include <inttypes.h>
#include <Zydis/Zydis.h>


extern ExampleAppLog AppLog;

PVOID LeagueFunctions::NewIssueOrder = nullptr;
PVOID LeagueFunctions::NewIssueOrderCheck = nullptr;
PVOID LeagueFunctions::NewCastSpell = nullptr;

std::vector<AddressesToCopy> LeagueFunctions::addressToCopyList = {};

DWORD LeagueFunctions::CalcFunctionSize(DWORD OrigAddress, size_t& size, ReturnSig retSig){
	size = 0;
	int maxLoop = 0xFFFF;
	DWORD OrigFnEndAddr = 0;
	int foundRet = 0;

	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));
	if (!VirtualQuery((PVOID)OrigAddress, &mbi, sizeof(mbi))) {
		//AppLog.AddLog("VirtualQuery failed\n");
		return 0;
	}

	if (!(mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS))
	{
		//AppLog.AddLog("Address is not Readable\n");
		return 0;
	}

	while (true) {

		int matchBytes = 0;

		for (int i = 0; i < retSig.returnSig.size(); i++) {
			if (!VirtualQuery((PVOID)(OrigAddress + size + i), &mbi, sizeof(mbi))) {
				//AppLog.AddLog("VirtualQuery failed\n");
				return 0;
			}

			if (!(mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS))
			{
				//AppLog.AddLog("Address is not Readable\n");
				return 0;
			}

			if (((*(DWORD*)(OrigAddress + size + i)) & 0xFF) == retSig.returnSig[i]) {
				matchBytes++;
			}

			if (i == retSig.returnSig.size() - 1) {
				if(matchBytes==i)
					foundRet++;
			}
		}

		if (foundRet == retSig.returnCount) {
			size += (retSig.returnSig.size() + 1);
			OrigFnEndAddr = OrigAddress + size;

			break;
		}

		if (maxLoop == size) {
			break;
		}

		size++;
	}

	return OrigFnEndAddr;
}

DWORD LeagueFunctions::CalcVirtualFunctionSize(DWORD Address, size_t& size, ReturnSig retSig) {
	size = 0;
	int maxLoop = 0xFFFF;
	DWORD OrigFnEndAddr = 0;
	int foundRet = 0;

	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));
	if (!VirtualQuery((PVOID)Address, &mbi, sizeof(mbi))) {
		//AppLog.AddLog("VirtualQuery failed\n");
		return 0;
	}

	//AppLog.AddLog(("State: " + hexify<DWORD>(mbi.State) + "\n").c_str());
	//AppLog.AddLog(("Protect: " + hexify<DWORD>(mbi.Protect) + "\n").c_str());

	if ((mbi.State == (MEM_COMMIT)) && (mbi.Protect == PAGE_EXECUTE_READWRITE)) {
		while (true) {

			int matchBytes = 0;

			for (int i = 0; i < retSig.returnSig.size(); i++) {
				if (!VirtualQuery((PVOID)(Address + size + i), &mbi, sizeof(mbi))) {
					//AppLog.AddLog("VirtualQuery failed\n");
					return 0;
				}

				//AppLog.AddLog(("State: " + hexify<DWORD>(mbi.State) + "\n").c_str());
				//AppLog.AddLog(("Protect: " + hexify<DWORD>(mbi.Protect) + "\n").c_str());

				if ((mbi.State == (MEM_COMMIT)) && (mbi.Protect == PAGE_EXECUTE_READWRITE)) {
					if (((*(DWORD*)(Address + size + i)) & 0xFF) == retSig.returnSig[i]) {
						matchBytes++;
					}

					if (i == retSig.returnSig.size() - 1) {
						if (matchBytes == i)
							foundRet++;
					}
				}else {
					//AppLog.AddLog("Address Not Readable\n");
				}

			}

			if (foundRet == retSig.returnCount) {
				size += (retSig.returnSig.size() + 1);
				OrigFnEndAddr = Address + size;

				break;
			}

			if (maxLoop == size) {
				break;
			}

			size++;
		}

		return OrigFnEndAddr;
	}

	//AppLog.AddLog("Address Not Readable\n");
	return 0;
}

DWORD LeagueFunctions::VirtualAllocateFunction(PVOID& NewFunction, DWORD OrigAddress, size_t size) {
	NewFunction = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	return (DWORD)NewFunction;
}

void LeagueFunctions::CopyFunction(DWORD dest, DWORD source, size_t size) {
	(void)memcpy((void*)dest, (PVOID)source, size);
}

void LeagueFunctions::FixRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size)
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
				*(DWORD*)(NewFnAddress + offset + 1) = calc;

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
						if (hex >= OldFnAddress && hex <= OldFnAddressEnd) {
							DWORD calc1 = (runtime_address - NewFnAddress + 4);
							DWORD calc = 0xFFFFFFFF - calc1;
							*(DWORD*)(NewFnAddress + offset + 1) = calc;


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

void LeagueFunctions::ApplyIssueOrderPatches(DWORD Address, size_t size) {
	///////////////////////////////////////////////////////////////////////////////
	// PATCH RET ADDR BITSHIFTING (CAN BE LEFT UNDONE BUT JUST TO BE SAFE)
	///////////////////////////////////////////////////////////////////////////////

	DWORD toBePatchStart = Address + 0x4E0;
	DWORD toBePatchEnd = Address + 0x4F6 + 1;

	//AppLog.AddLog(("toBePatchStart: " + hexify<DWORD>(toBePatchStart)+"\n").c_str());
	//AppLog.AddLog(("toBePatchEnd: " + hexify<DWORD>(toBePatchEnd) + "\n").c_str());

	size_t currSize = 0;
	size_t targetSize = toBePatchEnd - toBePatchStart;
	while (currSize != targetSize) {

		*(BYTE*)(toBePatchStart + currSize) = 0x90;
		//AppLog.AddLog(("Patching: " + hexify<DWORD>(toBePatchStart + currSize) + "\n").c_str());
		currSize++;
	}
	///////////////////////////////////////////////////////////////////////////////
	// END PATCH RET ADDR BITSHIFTING (CAN BE LEFT UNDONE BUT JUST TO BE SAFE)
	///////////////////////////////////////////////////////////////////////////////

	//AppLog.AddLog("Done Patching\n");
}

void LeagueFunctions::ApplyIssueOrderCheckPatches(DWORD Address, size_t size) {
	///////////////////////////////////////////////////////////////////////////////
	// DO NOT SET PEB FLAGS!
	///////////////////////////////////////////////////////////////////////////////

	DWORD toBePatchStart = Address + 0x37;
	DWORD toBePatchEnd = Address + 0x7C + 1;

	//AppLog.AddLog(("toBePatchStart: " + hexify<DWORD>(toBePatchStart) + "\n").c_str());
	//AppLog.AddLog(("toBePatchEnd: " + hexify<DWORD>(toBePatchEnd) + "\n").c_str());

	size_t currSize = 0;
	size_t targetSize = toBePatchEnd - toBePatchStart;
	while (currSize != targetSize) {

		*(BYTE*)(toBePatchStart + currSize) = 0x90;
		//AppLog.AddLog(("Patching: " + hexify<DWORD>(toBePatchStart + currSize) + "\n").c_str());
		currSize++;
	}
	///////////////////////////////////////////////////////////////////////////////
	// END DO NOT SET PEB FLAGS!
	///////////////////////////////////////////////////////////////////////////////

	//AppLog.AddLog("Done Patching\n");
}
/*
typedef int(__fastcall* tIssueOrderCheck)(int a1, int a2, int a3, DWORD* a4, char a5, int a6, int a7, int a8, int a9, DWORD* a10);
tIssueOrderCheck IssueOrderCheck;

int LeagueFunctions::NewIssueOrderCheckGateway(int a1, int a2, int a3, DWORD* a4, char a5, int a6, int a7, int a8, int a9, DWORD* a10) {
	AppLog.AddLog("NewIssueOrderCheckGateway: Called\n");
	//MessageBoxA(0, "NewIssueOrderCheckGateway: Called","", 0);
	//MessageBoxA(0, ("a10 = "+hexify<DWORD>(DWORD(a10))).c_str(), "", 0);
	//return IssueOrderCheck(a1, a2, a3, a4, a5, a6, a7, a8, a9, (DWORD*)(baseAddr + 0x17681E));
	IssueOrderCheck = (tIssueOrderCheck)((DWORD)LeagueFunctions::NewIssueOrderCheck);
	return IssueOrderCheck(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	//return LeagueFunctions::NewIssueOrderCheckHook.Call<int>(a1, a2, a3, a4, a5, a6, a7, a8, a9, (DWORD*)(baseAddr + 0x17681E));
}*/

void Nah(DWORD ret1, DWORD ret2, DWORD a1, DWORD a2, DWORD a3, DWORD* a4, DWORD a5, DWORD a6, DWORD a7, DWORD a8, DWORD a9, DWORD* a10, DWORD a11, DWORD a12, DWORD a13, DWORD a14, DWORD a15, DWORD a16, DWORD a17, DWORD a18, DWORD a19, DWORD a20) {
	/*AppLog.AddLog(("ret1 = " + hexify<DWORD>(DWORD(ret1)) + "\n").c_str());
	AppLog.AddLog(("ret2 = " + hexify<DWORD>(DWORD(ret2)) + "\n").c_str());
	AppLog.AddLog(("a1 = " + hexify<DWORD>(DWORD(a1)) + "\n").c_str());
	AppLog.AddLog(("a2 = " + hexify<DWORD>(DWORD(a2)) + "\n").c_str());
	AppLog.AddLog(("a3 = " + hexify<DWORD>(DWORD(a3)) + "\n").c_str());
	AppLog.AddLog(("a4 = " + hexify<DWORD>(DWORD(a4)) + "\n").c_str());
	AppLog.AddLog(("a5 = " + hexify<DWORD>(DWORD(a5)) + "\n").c_str());
	AppLog.AddLog(("a6 = " + hexify<DWORD>(DWORD(a6)) + "\n").c_str());
	AppLog.AddLog(("a7 = " + hexify<DWORD>(DWORD(a7)) + "\n").c_str());
	AppLog.AddLog(("a8 = " + hexify<DWORD>(DWORD(a8)) + "\n").c_str());
	AppLog.AddLog(("a9 = " + hexify<DWORD>(DWORD(a9)) + "\n").c_str());
	AppLog.AddLog(("a10 = " + hexify<DWORD>(DWORD(a10)) + "\n").c_str());
	AppLog.AddLog(("a11 = " + hexify<DWORD>(DWORD(a11)) + "\n").c_str());
	AppLog.AddLog(("a12 = " + hexify<DWORD>(DWORD(a12)) + "\n").c_str());
	AppLog.AddLog(("a13 = " + hexify<DWORD>(DWORD(a13)) + "\n").c_str());
	AppLog.AddLog(("a14 = " + hexify<DWORD>(DWORD(a14)) + "\n").c_str());
	AppLog.AddLog(("a15 = " + hexify<DWORD>(DWORD(a15)) + "\n").c_str());
	AppLog.AddLog(("a16 = " + hexify<DWORD>(DWORD(a16)) + "\n").c_str());
	AppLog.AddLog(("a17 = " + hexify<DWORD>(DWORD(a17)) + "\n").c_str());
	AppLog.AddLog(("a18 = " + hexify<DWORD>(DWORD(a18)) + "\n").c_str());
	AppLog.AddLog(("a19 = " + hexify<DWORD>(DWORD(a19)) + "\n").c_str());
	AppLog.AddLog(("a20 = " + hexify<DWORD>(DWORD(a20)) + "\n").c_str());*/
}

DWORD __Backup_Eax;
DWORD __ret1;
DWORD __ret2;
DWORD a1, a2, a3, *a4, a5, a6, a7, a8, a9, *a10, a11, a12, a13, a14, a15, a16, a17, a18, a19,  a20;
DWORD __NewIssueOrderCheck;

void __declspec(naked) LeagueFunctions::NewIssueOrderCheckGateway()
{
	//AppLog.AddLog("NewIssueOrderCheckGateway: Called\n");
	
	__asm {
		/*mov __Backup_Eax, eax
		mov eax, [esp]
		mov __ret1, eax
		mov eax, [esp+0x4]
		mov __ret2, eax
		mov eax, [esp + 0x8]
		mov a1, eax
		mov eax, [esp + 0xC]
		mov a2, eax
		mov eax, [esp + 0x10]
		mov a3, eax
		mov eax, [esp + 0x14]
		mov a4, eax
		mov eax, [esp + 0x18]
		mov a5, eax
		mov eax, [esp + 0x1C]
		mov a6, eax
		mov eax, [esp + 0x20]
		mov a7, eax
		mov eax, [esp + 0x24]
		mov a8, eax
		mov eax, [esp + 0x28]
		mov a9, eax
		mov eax, [esp + 0x2C]
		mov a10, eax
		mov eax, [esp + 0x30]
		mov a11, eax
		mov eax, [esp + 0x34]
		mov a12, eax
		mov eax, [esp + 0x38]
		mov a13, eax
		mov eax, [esp + 0x3C]
		mov a14, eax
		mov eax, [esp + 0x40] //max
		mov a15, eax
		mov eax, [esp + 0x44]
		mov a16, eax
		mov eax, [esp + 0x48]
		mov a17, eax
		mov eax, [esp + 0x4C]
		mov a18, eax
		mov eax, [esp + 0x50]
		mov a19, eax
		mov eax, [esp + 0x54]
		mov a20, eax*/
		mov eax, __Backup_Eax
	}
	//__asm pushad
	//Nah(__ret1, __ret2, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
	//__asm popad
	
	__asm jmp __NewIssueOrderCheck
}

void LeagueFunctions::ReplaceCall(DWORD origAddress, DWORD newAddress, DWORD fnAddress, size_t size) {
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
	ZyanU32 runtime_address = fnAddress;
	ZyanUSize offset = 0;
	const ZyanUSize length = size;
	ZydisDecodedInstruction instruction;
	int fixedAddressesCount = 0;

	while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(fnAddress + offset), length - offset, &instruction)))
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

		
		//////////////////////////////////////////////////////////////////////////////////////////
		// CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
		//////////////////////////////////////////////////////////////////////////////////////////
		if (mnemonic.find("call 0x") != std::string::npos) {

			//AppLog.AddLog("%08" PRIX32 " ", (runtime_address));
			//AppLog.AddLog(buffer);

			DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);
			if (hex == origAddress) {
				DWORD calc1 = (runtime_address - newAddress + 4);
				DWORD calc = 0xFFFFFFFF - calc1;
				*(DWORD*)(fnAddress + offset + 1) = calc;


				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(fnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					//AppLog.AddLog(" => ");
					//AppLog.AddLog(buffer);
				}
				fixedAddressesCount++;
			}
			//AppLog.AddLog("\n");
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		// END CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
		//////////////////////////////////////////////////////////////////////////////////////////

		offset += instruction.length;
		runtime_address += instruction.length;
	}
	//AppLog.AddLog(("Redirected Call Count: " + to_string(fixedAddressesCount) + "\n").c_str());
}

PPEB LeagueFunctions::getCurrentProcessEnvironmentBlock()
{
	return getProcessEnvironmentBlockAddress(GetCurrentProcess());
}

typedef NTSTATUS (NTAPI * t_NTAPINtQueryInformationProcess)(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass,PVOID ProcessInformation,ULONG ProcessInformationLength,PULONG ReturnLength);
t_NTAPINtQueryInformationProcess fn_NTAPINtQueryInformationProcess;

PPEB LeagueFunctions::getProcessEnvironmentBlockAddress(HANDLE processHandle)
{
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	//AppLog.AddLog(("ntdll:" + hexify<DWORD>(DWORD(ntdll))+ "\n").c_str());

	DWORD ZwQueryInformationProcessAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "ZwQueryInformationProcess"));

	ULONG lReturnLength = 0;
	PROCESS_BASIC_INFORMATION processBasicInformation;

	fn_NTAPINtQueryInformationProcess = (t_NTAPINtQueryInformationProcess)(ZwQueryInformationProcessAddr);
	if ((fn_NTAPINtQueryInformationProcess(processHandle, ProcessBasicInformation, &processBasicInformation, sizeof(PROCESS_BASIC_INFORMATION), &lReturnLength) >= 0) && (lReturnLength == sizeof(PROCESS_BASIC_INFORMATION)))
	{
		//AppLog.AddLog("NtQueryInformationProcess success %d\n",sizeof(PROCESS_BASIC_INFORMATION));

		return processBasicInformation.PebBaseAddress;
	}
	else
	{
		//AppLog.AddLog("NtQueryInformationProcess failed %d vs %d\n",lReturnLength,sizeof(PROCESS_BASIC_INFORMATION));
		return 0;
	}
}

int LeagueFunctions::IsDetected() {
	auto _peb = getCurrentProcessEnvironmentBlock();
	if (_peb) {
		if (*reinterpret_cast<bool*>(((DWORD)_peb) + 0x0A00)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	return 2;
}