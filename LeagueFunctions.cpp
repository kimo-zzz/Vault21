#pragma once
#include "stdafx.h"
#include "LeagueFunctions.h"
#include "ExampleAppLog.h"

#include <stdio.h>
#include <inttypes.h>
#include <Zydis/Zydis.h>


//extern ExampleAppLog AppLog;

PVOID LeagueFunctions::NewIssueOrder = nullptr;
PVOID LeagueFunctions::NewIssueOrderCheck = nullptr;
PVOID LeagueFunctions::NewCastSpell = nullptr;
DWORD LeagueFunctions::NewIssueOrderCheckAddr = 0;
DWORD LeagueFunctions::NewCastSpellAddr = 0;
DWORD LeagueFunctions::TrueIssueOrderReturnAddress = (DWORD)(baseAddr + oIssueOrderTrueReturn);
bool LeagueFunctions::IsDonePatchingIssueOrder = false;
bool LeagueFunctions::IsDonePatchingCastSpell = false;

std::vector<AddressesToCopy> LeagueFunctions::addressToCopyList = {};

DWORD LeagueFunctions::CalcFunctionSize(DWORD OrigAddress, size_t& size, ReturnSig retSig) {
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
				if (matchBytes == i)
					foundRet++;
			}
		}

		if (foundRet == retSig.returnCount) {
			size += (retSig.returnSig.size() + 1);
			return OrigFnEndAddr = OrigAddress + size;
		}

		if (maxLoop == size) {
			break;
		}

		size++;
	}

	size = 0;
	return OrigFnEndAddr;
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
	/*
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
	*/
	///////////////////////////////////////////////////////////////////////////////
	// END PATCH RET ADDR BITSHIFTING (CAN BE LEFT UNDONE BUT JUST TO BE SAFE)
	///////////////////////////////////////////////////////////////////////////////

	//AppLog.AddLog("Done Patching\n");
}

void LeagueFunctions::ApplyCastSpellPatches(DWORD Address, size_t size) {
	{
		///////////////////////////////////////////////////////////////////////////////
		// DO NOT SET BAN FLAGS!
		///////////////////////////////////////////////////////////////////////////////

		DWORD toBePatchStart = Address + 0x1C2;
		DWORD toBePatchEnd = Address + 0x1F1 + 1;

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
		// END DO NOT SET BAN FLAGS!
		///////////////////////////////////////////////////////////////////////////////
	}

	{
		///////////////////////////////////////////////////////////////////////////////
		// DO NOT SET AL (unknown effect but just check in IDA XD)
		///////////////////////////////////////////////////////////////////////////////

		DWORD toBePatchStart = Address + 0x1F6;
		DWORD toBePatchEnd = Address + 0x1F9 + 1;

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
		// END DO NOT SET AL (unknown effect but just check in IDA XD)
		///////////////////////////////////////////////////////////////////////////////
	}

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

void testValueIssueOrderCheckGateway(DWORD val, DWORD val2) {
	//AppLog.AddLog(("val location=" + hexify<DWORD>((DWORD)testValueIssueOrderCheckGateway) + "\n").c_str());
	//AppLog.AddLog(("val=" + hexify<DWORD>((DWORD)val) + " *val=" + hexify<DWORD>((DWORD)val2) + "\n").c_str());
}

void* __fastcall LeagueFunctions::IssueOrderCheckGateway(int a1, int a2, int a3, DWORD* a4, char a5, int a6, int a7, int a8, int a9, DWORD* a10) {
	DWORD oldVal = *a10;
	//testValueIssueOrderCheckGateway((DWORD)a10, *a10);
	*a10 = TrueIssueOrderReturnAddress;
	//testValueIssueOrderCheckGateway((DWORD)a10, *a10);
	void* ret = Functions.IssueOrderCheck(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	*a10 = oldVal;
	//testValueIssueOrderCheckGateway((DWORD)a10, *a10);
	return ret;
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