#pragma once

#include "Globals.h"
#include "makesyscall.h"
#include <psapi.h>

#define RIOT_BUILD

#if defined(CHINA_BUILD)
#define LOL_VERSION "9.8.270.9450"
#include "OffsetsChina.h"

#elif defined(GARENA_BUILD)
#define LOL_VERSION "9.8.270.9450"
#include "OffsetsGarena.h"

#elif defined(RIOT_BUILD)
#define LOL_VERSION "9.8.270.9450"
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

inline void writeDataToFile(string name, DWORD address) {
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