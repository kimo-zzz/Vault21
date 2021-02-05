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

inline bool UnHookNTDLL() {
	HANDLE process = GetCurrentProcess();
	MODULEINFO mi = {};
	HMODULE ntdllModule = GetModuleHandleA("ntdll.dll");

	GetModuleInformation(process, ntdllModule, &mi, sizeof(mi));
	LPVOID ntdllBase = (LPVOID)mi.lpBaseOfDll;
	HANDLE ntdllFile = CreateFileA("c:\\windows\\syswow64\\ntdll.dll", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	HANDLE ntdllMapping = CreateFileMapping(ntdllFile, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, NULL);
	LPVOID ntdllMappingAddress = MapViewOfFile(ntdllMapping, FILE_MAP_READ, 0, 0, 0);

	PIMAGE_DOS_HEADER hookedDosHeader = (PIMAGE_DOS_HEADER)ntdllBase;
	PIMAGE_NT_HEADERS hookedNtHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)ntdllBase + hookedDosHeader->e_lfanew);

	for (WORD i = 0; i < hookedNtHeader->FileHeader.NumberOfSections; i++) {
		PIMAGE_SECTION_HEADER hookedSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)IMAGE_FIRST_SECTION(hookedNtHeader) + ((DWORD_PTR)IMAGE_SIZEOF_SECTION_HEADER * i));

		if (!strcmp((char*)hookedSectionHeader->Name, (char*)".text")) {
			DWORD oldProtection = 0;
			//bool isProtected = VirtualProtect((LPVOID)((DWORD_PTR)ntdllBase + (DWORD_PTR)hookedSectionHeader->VirtualAddress), hookedSectionHeader->Misc.VirtualSize, PAGE_EXECUTE_READWRITE, &oldProtection);

			auto addr = (PVOID)((DWORD_PTR)ntdllBase + (DWORD_PTR)hookedSectionHeader->VirtualAddress);
			auto size = (SIZE_T)(hookedSectionHeader->Misc.VirtualSize);
			NTSTATUS res = makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(GetCurrentProcess(), &addr, &size, PAGE_EXECUTE_READWRITE, &oldProtection);
			if (!NT_SUCCESS(res)) {
				CloseHandle(process);
				CloseHandle(ntdllFile);
				CloseHandle(ntdllMapping);
				return false;
			}
			memcpy((LPVOID)((DWORD_PTR)ntdllBase + (DWORD_PTR)hookedSectionHeader->VirtualAddress), (LPVOID)((DWORD_PTR)ntdllMappingAddress + (DWORD_PTR)hookedSectionHeader->VirtualAddress), hookedSectionHeader->Misc.VirtualSize);
			//isProtected = VirtualProtect((LPVOID)((DWORD_PTR)ntdllBase + (DWORD_PTR)hookedSectionHeader->VirtualAddress), hookedSectionHeader->Misc.VirtualSize, oldProtection, &oldProtection);

			res = makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(GetCurrentProcess(), &addr, &size, oldProtection & oldProtection);
			if (!NT_SUCCESS(res)) {
				CloseHandle(process);
				CloseHandle(ntdllFile);
				CloseHandle(ntdllMapping);
				return false;
			}
		}
	}

	CloseHandle(process);
	CloseHandle(ntdllFile);
	CloseHandle(ntdllMapping);
	//FreeLibrary(ntdllModule);
	return true;
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

inline void writeDataToFile(string name, DWORD address) {
	FILE* logfile;
	// _CRT_SECURE_NO_DEPRECATE;
	if ((logfile = fopen("log.txt", "a+")) != NULL)
	{
		//fprintf(logfile, (name + " : " + hexify<DWORD>(address) + "\n").c_str());
		fclose(logfile);
	}
}

inline DWORD getMbiBase(DWORD Address) {
	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));
	VirtualQuery((PVOID)(Address), &mbi, sizeof(mbi));
	return (DWORD)mbi.BaseAddress;
}

#define pakeke80
inline DWORD RestoreZwQueryInformationProcess() {
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	//AppLog.AddLog(("ntdll:" + hexify<DWORD>(DWORD(ntdll))+ "\n").c_str());

	DWORD ZwQueryInformationProcessAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "ZwQueryInformationProcess"));

#if defined(pakeke80)
	BYTE ZwQIP[] = {
		0xB8, 0x19, 0x00, 0x00, 0x00,
		0xE8, 0x00, 0x00, 0x00, 0x00,
		0x5A,	
		0x80, 0x7A, 0x14, 0x4B,
		0x75, 0x0E,
		0x64, 0xFF, 0x15, 0xC0, 0x00, 0x00, 0x00,
		0xC2, 0x14, 0x00,
		0x00, 0x00,
		0x24, 0x77,
		0xBA, 0xD0, 0x60, 0x2C, 0x77,
		0xFF, 0xD2,
		0xC2, 0x14, 0x00,
		0x8D, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00
	};
#endif
#if defined(dencelle)
	BYTE ZwQIP[] = {
		0xB8, 0x19, 0x00, 0x00, 0x00,
		0xE8, 0x00, 0x00, 0x00, 0x00,
		0x5A,
		0x80, 0x7A, 0x14, 0x4B,
		0x75, 0x0E,
		0x64, 0xFF, 0x15, 0xC0, 0x00, 0x00, 0x00,
		0xC2, 0x14, 0x00,
		0x00, 0x00,
		0xE8, 0x76, 0xBA, 0x70, 0x71,
		0xF0, 0x76, 0xFF,
		0xD2, 0xC2,
		0x14, 0x00,
		0x8D, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00
	};
#endif
#if defined(jiingz)
	BYTE ZwQIP[] = {
		0xB8, 0x19, 0x00, 0x00, 0x00,
		0xE8, 0x00, 0x00, 0x00, 0x00,
		0x5A,
		0x80, 0x7A, 0x14, 0x4B,
		0x75, 0x0E,
		0x64, 0xFF, 0x15, 0xC0, 0x00, 0x00, 0x00,
		0xC2, 0x14, 0x00,
		0x00, 0x00,
		0xE8, 0x76, 0xBA, 0x70, 0x71,
		0xF0, 0x76, 0xFF,
		0xD2, 0xC2,
		0x14, 0x00,
		0x8D, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00
	};
#endif
#if defined(earl)
	BYTE ZwQIP[] = {
		0xB8, 0x19, 0x00, 0x00, 0x00,
		0xE8, 0x00, 0x00, 0x00, 0x00,
		0x5A,
		0x80, 0x7A, 0x14, 0x4B,
		0x75, 0x0E,
		0x64, 0xFF, 0x15, 0xC0, 0x00, 0x00, 0x00,
		0xC2, 0x14, 0x00,
		0x00, 0x00,
		0xE8, 0x76, 0xBA, 0x70, 0x71,
		0xF0, 0x76, 0xFF,
		0xD2, 0xC2,
		0x14, 0x00,
		0x8D, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00
	};
#endif

	int i = 0;
	for (BYTE _byte : ZwQIP) {
		*(BYTE*)(ZwQueryInformationProcessAddr + i) = _byte;
		i++;
	}

	return ZwQueryInformationProcessAddr;
}