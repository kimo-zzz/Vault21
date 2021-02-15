#pragma once

#include "Globals.h"
#include "makesyscall.h"
#include <psapi.h>

#define RIOT_BUILD

#if defined(RIOT_BUILD)
#define LOL_VERSION "11.3.357.5376"
#include "OffsetsRiot.h"

#endif

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

#define DEFINE_RVA(address) ((DWORD)Globals::MainModule + (DWORD)address)
#define DEFINE_RVA_STUB(address) ((DWORD)Globals::HiddenModule + (DWORD)address)

#define STR_MERGE_IMPL(x, y)				x##y
#define STR_MERGE(x,y)						STR_MERGE_IMPL(x,y)
#define MAKE_PAD(size)						BYTE STR_MERGE(pad_, __COUNTER__) [ size ]
#define DEFINE_MEMBER_0(x)					x;
#define DEFINE_MEMBER_N(x,offset)			struct { MAKE_PAD((DWORD)offset); x; };
#define DEFINE_CHARACTER_INTERMEDIATE(name) DEFINE_MEMBER_N(float name, (DWORD)Offsets::GameObject::CharacterIntermediate + (DWORD)Offsets::CharacterIntermediate::name)
#define DEFINE_EXPERIENCE(type, name) DEFINE_MEMBER_N(type name, (DWORD)Offsets::GameObject::Experience + (DWORD)Offsets::Experience::name)

#define DEFINE_MEMBER_ENCRYPTED(type, name, offset)											\
type name() {																				\
	auto _this = (DWORD)this;																\
	auto _offset = (DWORD)offset;															\
	type result = *(type*)(_this + 4 * *(BYTE*)(_this + _offset + 0x1) + _offset + 0x4);	\
																							\
	auto large_size = *(BYTE*)(_this + _offset + 0x14);										\
	if (large_size) {																		\
		auto key_it = (DWORD*)(_this + _offset + 0x10);										\
		auto i = 0;																			\
		do 																					\
		{																					\
			auto key_dir = *key_it;															\
			key_it++;																		\
			*((DWORD*)&result + i) ^= ~(key_dir ^ 0x5362ECFB);								\
			i++;																			\
		}																					\
		while (i < large_size);																\
	}																						\
																							\
	auto small_size = *(BYTE*)(_this + _offset + 0x15);										\
	if (small_size) {																		\
		for (BYTE i = 4 - small_size; i < 4; i++) {											\
			*((BYTE*)&result + i) ^= *(BYTE*)(_this + i + _offset + 0x10) ^ 0xFB;			\
		}																					\
	}																						\
																							\
	return result;																			\
}

#define DEFINE_MEMBER_ENCRYPTED_BOOLEAN(name, offset)									\
bool name() {																			\
	auto _this = (DWORD)this;															\
	auto _offset = (DWORD)offset;														\
	BYTE result = *(BYTE*)(_this + *(BYTE*)(_this + _offset + 0x1) + _offset + 0x2);	\
																						\
	auto large_size = *(BYTE*)(_this + _offset + 0x6);									\
	if (large_size) {																	\
		auto key_it = (DWORD*)(_this + _offset + 0x5);									\
		auto i = 0;																		\
		do 																				\
		{																				\
			auto key_dir = *key_it;														\
			key_it++;																	\
			*((DWORD*)&result + i) ^= ~(key_dir ^ 0xB291667B);							\
			i++;																		\
		}																				\
		while (i < large_size);															\
	}																					\
																						\
	auto small_size = *(BYTE*)(_this + _offset + 0x7);									\
	if (!small_size)																	\
		return result;																	\
																						\
	auto i = 1 - small_size;															\
	if (i >= 1)																			\
		return result;																	\
																						\
	do 																					\
	{																					\
		*(&result + i) ^= *(BYTE*)(_this + i + _offset + 0x5) ^ 0x7B;					\
		i++;																			\
	}																					\
	while(i < 1);																		\
																						\
	return result;																		\
}

template< typename T >
std::string hexify(T i)
{
	std::stringbuf buf;
	std::ostream os(&buf);


	os << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;

	return buf.str().c_str();
}

template< typename Function > Function CallVirtual(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];

	return (Function)(dwAddress);
}

inline uint8_t* find_signature(LPCSTR szModule, const char* szSignature) {
	auto module = GetModuleHandleA(szModule);
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t*)module + dosHeader->e_lfanew);
	auto textSection = IMAGE_FIRST_SECTION(ntHeaders);

	auto sizeOfImage = textSection->SizeOfRawData;
	auto patternBytes = pattern_to_byte(szSignature);
	auto scanBytes = reinterpret_cast<uint8_t*>(module) + textSection->VirtualAddress;

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	auto mbi = MEMORY_BASIC_INFORMATION{ 0 };
	uint8_t* next_check_address = 0;

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			auto current_address = scanBytes + i + j;
			if (current_address >= next_check_address) {
				if (!VirtualQuery(reinterpret_cast<void*>(current_address), &mbi, sizeof(mbi)))
					break;

				if (mbi.Protect == PAGE_NOACCESS) {
					i += ((std::uintptr_t(mbi.BaseAddress) + mbi.RegionSize) - (std::uintptr_t(scanBytes) + i));
					i--;
					found = false;
					break;
				}
				else {
					next_check_address = reinterpret_cast<uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
				}
			}

			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scanBytes[i];
		}
	}
	return nullptr;
}

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))
inline DWORD FindPatternV2(std::string moduleName, std::string Mask)
{
	const char* pat = Mask.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;

	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2; //one ?
		}
		else
		{
			pat = Mask.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

inline DWORD FindHiddenModule()
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	DWORD start = 0;
	DWORD result = 0;
	while (VirtualQuery((LPVOID)start, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) != 0)
	{
		if (mbi.Protect & PAGE_READWRITE && mbi.State == 0x1000 && mbi.Type == 0x40000)
		{
			result = (DWORD)mbi.AllocationBase;
			if (*(WORD*)(result) == 0x5A4D)
			{
				auto ntPtr = reinterpret_cast<PIMAGE_NT_HEADERS>(result + reinterpret_cast<PIMAGE_DOS_HEADER>(result)->e_lfanew);
				if (ntPtr->FileHeader.NumberOfSections == 9)
				{
					//ENGINE_MSG("found : %02X - TimeDateStamp : %02X - State %02X Type %02X", mbi.AllocationBase, ntPtr->FileHeader.TimeDateStamp, mbi.State, mbi.Type);
					break;
				}

			}
		}

		start += mbi.RegionSize;
	}
	return result;
}

inline bool checkIfLogExistCreateIfNot() {
	if (FILE* file = fopen("log.txt", "r")) {
		fclose(file);
		return true;
	}
	else {
		ofstream MyFile("log.txt");
		MyFile.close();
	}
	return true;
}

inline void writeDataToFile2(string name) {
	checkIfLogExistCreateIfNot();
	FILE* logfile;
	// _CRT_SECURE_NO_DEPRECATE;
	if ((logfile = fopen("log.txt", "a+")) != NULL)
	{
		fprintf(logfile, (name + "\n").c_str());
		fclose(logfile);
	}
}

inline void writeDataToFile(string name, DWORD address) {
	checkIfLogExistCreateIfNot();
	FILE* logfile;
	// _CRT_SECURE_NO_DEPRECATE;
	if ((logfile = fopen("log.txt", "a+")) != NULL)
	{
		fprintf(logfile, (name + " : " + hexify<DWORD>(address) + "\n").c_str());
		fclose(logfile);
	}
}

inline DWORD getMbiBase(DWORD Address) {
	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));
	VirtualQuery((PVOID)(Address), &mbi, sizeof(mbi));
	return (DWORD)mbi.BaseAddress;
}

inline uint8_t* find_RtlDispatchExceptionAddress()
{
	auto address = find_signature("ntdll.dll", "E8 ? ? ? ? 0A C0");

	if (!address)
	{
		return nullptr;
	}
	address = address + *reinterpret_cast<uint32_t*>(address + 1) + 5;

	return address;
}

inline DWORD RestoreZwQueryInformationProcess() {
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	//AppLog.AddLog(("ntdll:" + hexify<DWORD>(DWORD(ntdll))+ "\n").c_str());

	DWORD ZwQueryInformationProcessAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "ZwQueryInformationProcess"));

	BYTE ZwQIP[] = {
		0xB8, 0x19, 0x00, 0x00, 0x00
	};

	int i = 0;
	for (BYTE _byte : ZwQIP) {
		*(BYTE*)(ZwQueryInformationProcessAddr + i) = _byte;
		i++;
	}

	return ZwQueryInformationProcessAddr;
}

inline DWORD RestoreRtlAddVectoredExceptionHandler() {
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	//AppLog.AddLog(("ntdll:" + hexify<DWORD>(DWORD(ntdll))+ "\n").c_str());

	DWORD RtlAddVectoredExceptionHandlerAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "RtlAddVectoredExceptionHandler"));

	BYTE RtlAVE[] = {
		0x8B, 0xFF, 0x55, 0x8B, 0xEC
	};

	DWORD oldProt;

	auto addr = (PVOID)RtlAddVectoredExceptionHandlerAddr;
	auto size = static_cast<SIZE_T>(5);

	if (NT_SUCCESS(
		makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(
			GetCurrentProcess(), &addr, &size, PAGE_EXECUTE_READWRITE, &oldProt)))
	{
		int i = 0;
		for (BYTE _byte : RtlAVE) {
			*(BYTE*)(RtlAddVectoredExceptionHandlerAddr + i) = _byte;
			i++;
		}

		NT_SUCCESS(
			makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(
				GetCurrentProcess(), &addr, &size, oldProt, &oldProt));
	}

	return RtlAddVectoredExceptionHandlerAddr;
}

inline bool Hook(char* src, char* dst, int len)
{
	if (len < 5) return false;

	memset(src, 0x90, len);

	uintptr_t relativeAddress = (uintptr_t)(dst - src - 5);

	*src = (char)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relativeAddress;

	return true;
}

template <typename T_STR, typename T_CHAR>
T_STR remove_leading(T_STR const& str, T_CHAR c)
{
	auto end = str.end();

	for (auto i = str.begin(); i != end; ++i) {
		if (*i != c) {
			return T_STR(i, end);
		}
	}

	// All characters were leading or the string is empty.
	return T_STR();
}

inline string removeZero(string str, char toRemove)
{
	// Count trailing zeros 
	int i = 0;
	while (str[i] == toRemove)
		i++;

	// The erase function removes i characters 
	// from given index (0 here) 
	str.erase(0, i);

	return str;
}

inline void DataLog(EXCEPTION_POINTERS* pExceptionInfo)
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

	writeDataToFile2("--------------------------------------------------------------------");
	writeDataToFile2("ENTRY#"+to_string(j));
	writeDataToFile2(ExceptionCode);
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