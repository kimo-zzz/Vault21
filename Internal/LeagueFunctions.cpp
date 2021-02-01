#pragma once
#include "stdafx.h"
#include "LeagueFunctions.h"
#include "ExampleAppLog.h"

#include <stdio.h>
#include <inttypes.h>
#include "include/Zydis/Zydis.h"


//extern ExampleAppLog AppLog;

PVOID LeagueFunctions::NewIssueOrder = nullptr;
PVOID LeagueFunctions::NewIssueOrderCheck = nullptr;
PVOID LeagueFunctions::NewCastSpell = nullptr;
DWORD LeagueFunctions::NewIssueOrderCheckAddr = 0;
DWORD LeagueFunctions::NewCastSpellAddr = 0;
DWORD* LeagueFunctions::TrueIssueOrderReturnAddress = (DWORD*)(baseAddr + oIssueOrderTrueReturn);
bool LeagueFunctions::IsDonePatchingIssueOrder = false;
bool LeagueFunctions::IsDonePatchingCastSpell = false;

DWORD LeagueFunctions::IssueOrderStartHookGateway = 0;
DWORD LeagueFunctions::IssueOrderEndHookGateway = 0;

DWORD* LeagueFunctions::TrueCastSpellReturnAddress = (DWORD*)(baseAddr + oCastSpellTrueReturn);
DWORD LeagueFunctions::CastSpellStartHookGateway = 0;
DWORD LeagueFunctions::CastSpellEndHookGateway = 0;

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

void LeagueFunctions::HookStartAndEndFunction(DWORD fnAddress, size_t size, int paramCount, DWORD StartHook, DWORD EndHook, DWORD& StartHookGateway, DWORD& EndHookGateway)
{
	/////////////////////////////////////////////
	// HOOK the Start of function
	/////////////////////////////////////////////
	{
		//AppLog.AddLog("HOOK the Start of function\n");
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

		int borrowedBytes = 0;

		while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(fnAddress + offset), length - offset, &instruction)))
		{
			char buffer[256];
			ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
				runtime_address);
			//AppLog.AddLog(buffer);
			//AppLog.AddLog("\n");

			std::string mnemonic(buffer);
			//AppLog.AddLog((hexify<DWORD>(fnAddress + offset) + ": " + mnemonic + "\n").c_str());

			borrowedBytes += (int)instruction.length;

			if (borrowedBytes >= 5) {
				break;
			}

			offset += instruction.length;
			runtime_address += instruction.length;
		}

		//AppLog.AddLog(("borrowedBytes = " + hexify<int>((int)borrowedBytes) + "\n").c_str());

		// Create the gateway (len + 5 for the overwritten bytes + the jmp)
		DWORD gateway = (DWORD)VirtualAlloc(0, borrowedBytes + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		// Put the bytes that will be overwritten in the gateway
		memcpy((PVOID)gateway, (PVOID)(fnAddress), borrowedBytes);

		// Get the gateway to destination addy
		uintptr_t gateJmpAddy = (uintptr_t)((fnAddress) - gateway - 5);

		// Add the jmp opcode to the end of the gateway
		*(char*)(gateway + borrowedBytes) = (char)0xE9;

		// Add the address to the jmp
		*(uintptr_t*)(gateway + borrowedBytes + 1) = gateJmpAddy;

		//AppLog.AddLog(("gateway = " + hexify<DWORD>((DWORD)gateway) + "\n").c_str());

		if (Hook((char*)fnAddress, (char*)StartHook, borrowedBytes))
		{
			StartHookGateway = (DWORD)gateway;
		}
	}
	/////////////////////////////////////////////
	// End HOOK the Start of function
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// HOOK the End(s) of function
	/////////////////////////////////////////////
	{
		//AppLog.AddLog("HOOK the End(s) of function\n");
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

		DWORD last_runtime_address[4] = {0,0,0,0};

		string mnemonicToFind = "ret";
		if (paramCount > 0) {
			mnemonicToFind = hexify<DWORD>((DWORD)(paramCount*4));
			mnemonicToFind = removeZero(mnemonicToFind, '0');
			mnemonicToFind = removeZero(mnemonicToFind, 'x');
			mnemonicToFind = removeZero(mnemonicToFind, '0');
			mnemonicToFind = "ret 0x" + mnemonicToFind;
		}
		//AppLog.AddLog(("mnemonicToFind: " + mnemonicToFind + "\n").c_str());

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

			//AppLog.AddLog((hexify<DWORD>(fnAddress + offset)+": "+mnemonic + "\n").c_str());
			//////////////////////////////////////////////////////////////////////////////////////////
			// FIND POSSIBLE ADDRESSES BEFORE THE RETURN THAT CAN BE JMP PATCHED
			//////////////////////////////////////////////////////////////////////////////////////////
			
			if (mnemonic.find(mnemonicToFind) != std::string::npos) {
				//AppLog.AddLog("\tfound\n");
				int addressToPlaceJmpIndex = 0;
				for (int i = 3; i >= 0; i--) {
					DWORD sizeFromLastToCurr = (DWORD)(fnAddress + (size_t)offset) - last_runtime_address[i];
					if (sizeFromLastToCurr >= 0x5) {
						addressToPlaceJmpIndex = i;
						break;
					}
				}
				DWORD compatibleAddr = last_runtime_address[addressToPlaceJmpIndex];
				DWORD sizeFromLastToCurr = DWORD((fnAddress + (size_t)offset) - compatibleAddr);
				//AppLog.AddLog(("\tcompatible lastAddress:" + hexify<DWORD>(compatibleAddr) + "\n").c_str());
				//AppLog.AddLog(("\tsize from last to curr:" + hexify<DWORD>(sizeFromLastToCurr) + "\n").c_str());

				
				if (Hook((char*)compatibleAddr, (char*)EndHook, sizeFromLastToCurr))
				{
					//AppLog.AddLog(("\tplaced jmp on:" + hexify<DWORD>(compatibleAddr) + "\n").c_str());
				}

			}
			//////////////////////////////////////////////////////////////////////////////////////////
			// END FIND POSSIBLE ADDRESSES BEFORE THE RETURN THAT CAN BE JMP PATCHED
			//////////////////////////////////////////////////////////////////////////////////////////
			last_runtime_address[0] = last_runtime_address[1];
			last_runtime_address[1] = last_runtime_address[2];
			last_runtime_address[2] = last_runtime_address[3];
			last_runtime_address[3] = (DWORD)runtime_address;
			offset += instruction.length;
			runtime_address += instruction.length;
		}
	}
	/////////////////////////////////////////////
	// End HOOK the End(s) of function
	/////////////////////////////////////////////
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

	DWORD toBePatchStart = Address + 0x43;
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

void testValueIssueOrder(DWORD *val, DWORD *val1) {
	/*AppLog.AddLog(("-----------\n*backup_returnAddrNewIssueOrder=" + hexify<DWORD>((DWORD)*val) + "\n"
		"*backup_TrueIssueOrderReturnAddress=" + hexify<DWORD>((DWORD)*val1) + "\n"
		"backup_returnAddrNewIssueOrder=" + hexify<DWORD>((DWORD)val) + "\n"
		"backup_TrueIssueOrderReturnAddress=" + hexify<DWORD>((DWORD)val1)
		).c_str());*/
}

void testValueIssueOrderParams(DWORD* val1, DWORD val2, DWORD val3, DWORD val4, DWORD val5, DWORD val6, DWORD val7) {
	/*AppLog.AddLog(("-----------\nbackup_returnAddrNewIssueOrder=" + hexify<DWORD>((DWORD)val1) + 
		"\n*backup_returnAddrNewIssueOrder=" + hexify<DWORD>((DWORD)*val1) +
		"\nval2=" + hexify<DWORD>((DWORD)val2)  + 
		"\nval3=" + hexify<DWORD>((DWORD)val3)  +
		"\nval4=" + hexify<DWORD>((DWORD)val4)  +
		"\nval5=" + hexify<DWORD>((DWORD)val5)  +
		"\nval6=" + hexify<DWORD>((DWORD)val6)  +
		"\nval7" + hexify<DWORD>((DWORD)val7) + "\n"
		).c_str());*/
}

std::vector<DWORD*> backup_returnAddrStackNewIssueOrder;
DWORD* backup_returnAddrNewIssueOrder;
DWORD backup_eax_NewIssueOrderStartHook;

DWORD* _ret;
DWORD param2, param3, param4, param5, param6, param7;

void __declspec(naked) LeagueFunctions::NewIssueOrderStartHook()
{
	__asm {
		mov backup_eax_NewIssueOrderStartHook, eax
		mov eax, [esp]
		mov backup_returnAddrNewIssueOrder, eax

		/*
		mov eax, [esp]
		mov _ret, eax
		mov eax, [esp + 0x4]
		mov param2, eax
		mov eax, [esp + 0x8]
		mov param3, eax
		mov eax, [esp + 0xC]
		mov param4, eax
		mov eax, [esp + 0x10]
		mov param5, eax
		mov eax, [esp + 0x14]
		mov param6, eax
		mov eax, [esp + 0x18]
		mov param7, eax
		*/

		mov eax, TrueIssueOrderReturnAddress
		mov [esp], eax
		mov eax, backup_eax_NewIssueOrderStartHook
	}

	/*
	__asm pushad
	testValueIssueOrderParams(_ret, param2, param3, param4, param5, param6, param7);
	__asm popad
	*/

	__asm pushad
	backup_returnAddrStackNewIssueOrder.push_back(backup_returnAddrNewIssueOrder);
	__asm popad

	__asm {
		push IssueOrderStartHookGateway
		retn
	}
}

DWORD* backup_TrueIssueOrderReturnAddress;
DWORD backup_eax_NewIssueOrderEndHook;
DWORD* backup_returnAddrFromStackNewIssueOrder;
void __declspec(naked) LeagueFunctions::NewIssueOrderEndHook()
{
	__asm add esp, 0xD0

	__asm pushad
	backup_returnAddrFromStackNewIssueOrder = backup_returnAddrStackNewIssueOrder[backup_returnAddrStackNewIssueOrder.size()-1];
	__asm popad

	__asm pushad
	backup_returnAddrStackNewIssueOrder.pop_back();
	__asm popad

	__asm {
		mov backup_eax_NewIssueOrderEndHook, eax
		mov eax, [esp]
		mov backup_TrueIssueOrderReturnAddress, eax
		mov eax, backup_returnAddrFromStackNewIssueOrder
		mov [esp], eax
		mov eax, backup_eax_NewIssueOrderEndHook
	}

	__asm pushad
	//testValueIssueOrder(backup_returnAddrFromStackNewIssueOrder, backup_TrueIssueOrderReturnAddress);
	__asm popad

	__asm ret 0x18
}

void testValueCastSpell(DWORD val, DWORD val1) {
	//AppLog.AddLog(("-----------\nbackup_returnAddrNewCastSpell=" + hexify<DWORD>((DWORD)val) + "\nbackup_TrueCastSpellReturnAddress=" + hexify<DWORD>((DWORD)val1) + "\n").c_str());
}

std::vector<DWORD*> backup_returnAddrStackNewCastSpell;
DWORD* backup_returnAddrNewCastSpell;
DWORD backup_eax_NewCastSpellStartHook;
void __declspec(naked) LeagueFunctions::NewCastSpellStartHook()
{
	__asm {
		mov backup_eax_NewCastSpellStartHook, eax
		mov eax, [esp]
		mov backup_returnAddrNewCastSpell, eax
		mov eax, TrueCastSpellReturnAddress
		mov [esp], eax
		mov eax, backup_eax_NewCastSpellStartHook
	}

	__asm pushad
	backup_returnAddrStackNewCastSpell.push_back(backup_returnAddrNewCastSpell);
	__asm popad

	__asm {
		push CastSpellStartHookGateway
		retn
	}
}

DWORD* backup_TrueCastSpellReturnAddress;
DWORD backup_eax_NewCastSpellEndHook;
DWORD* backup_returnAddrFromStackNewCastSpell;
void __declspec(naked) LeagueFunctions::NewCastSpellEndHook()
{
	__asm {
		pop     edi
		pop     esi
		add esp, 0x30
	}

	__asm pushad
	backup_returnAddrFromStackNewCastSpell = backup_returnAddrStackNewCastSpell[backup_returnAddrStackNewCastSpell.size()-1];
	__asm popad

	__asm pushad
	backup_returnAddrStackNewCastSpell.pop_back();
	__asm popad

	__asm {
		mov backup_eax_NewCastSpellEndHook, eax
		mov eax, [esp]
		mov backup_TrueCastSpellReturnAddress, eax
		mov eax, backup_returnAddrFromStackNewCastSpell
		mov [esp], eax
		mov eax, backup_eax_NewCastSpellEndHook
	}

	__asm pushad
	//testValueCastSpell(backup_returnAddrFromStackNewCastSpell, backup_TrueCastSpellReturnAddress);
	__asm popad

	__asm ret 0x14
}

void testValueIssueOrderCheckGateway(DWORD val, DWORD val2) {
	//AppLog.AddLog(("val location=" + hexify<DWORD>((DWORD)testValueIssueOrderCheckGateway) + "\n").c_str());
	//AppLog.AddLog(("val=" + hexify<DWORD>((DWORD)val) + " *val=" + hexify<DWORD>((DWORD)val2) + "\n").c_str());
}

/*void* __fastcall LeagueFunctions::IssueOrderCheckGateway(int a1, int a2, int a3, DWORD* a4, char a5, int a6, int a7, int a8, int a9, DWORD* a10) {
	DWORD oldVal = *a10;
	//testValueIssueOrderCheckGateway((DWORD)a10, *a10);
	*a10 = TrueIssueOrderReturnAddress;
	//testValueIssueOrderCheckGateway((DWORD)a10, *a10);
	void* ret = Functions.IssueOrderCheck(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	*a10 = oldVal;
	//testValueIssueOrderCheckGateway((DWORD)a10, *a10);
	return ret;
}*/

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
		if ((*reinterpret_cast<int*>(((DWORD)_peb) + 0x0A00)) > 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
	return 2;
}