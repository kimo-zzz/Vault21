#pragma once
#include "stdafx.h"
#include <psapi.h>
#include "makesyscall.h"
#define baseAddr (DWORD)GetModuleHandle(NULL)
#define HiddenModule FindHiddenModule()

#define MAXIMUM_TEXT_SIZE 64U

#define STR_MERGE_IMPL(x, y)				x##y
#define STR_MERGE(x,y)						STR_MERGE_IMPL(x,y)
#define MAKE_PAD(size)						BYTE STR_MERGE(pad_, __COUNTER__) [ size ]

#define PAD(SIZE) uint8_t STR_MERGE(_pad, __COUNTER__)[SIZE];

#define DEFINE_MEMBER_0(x, y)				x
#define DEFINE_MEMBER_N(x,offset)			struct { MAKE_PAD(offset); x; }

#define OBJ_MINION  1
#define OBJ_HERO    2
#define OBJ_MISSILE 3

#define IS_NULL_RETN(OBJECT, ADDR, RETN) if ((DWORD*)(OBJECT + ADDR) == nullptr) { return RETN; }

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

template< typename Function > Function CallVirtual(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];

	return (Function)(dwAddress);
}

inline char* GetStr(DWORD offset) {
	if (*(int*)(offset + 0x10) > 15)
		return (char*)(*(DWORD*)offset);
	else
		return (char*)offset;
}

inline char* GetStr2(DWORD offset) {
	//if (*(int*)(offset + 0x10) > 15)
		return (char*)(*(DWORD*)offset);
	//else
		//return (char*)offset;
}

inline float GetEffectiveHP(float Armor, float HP) {
	return HP * (100.0f + Armor) / 100.0f;
}

inline int createRGB(int r, int g, int b)
{
	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
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

template< typename T >
std::string hexify(T i)
{
	std::stringbuf buf;
	std::ostream os(&buf);


	os << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;

	return buf.str().c_str();
}

inline std::string getHumanSpellByName(std::string name) {
	std::string ret = "";
	if (name.find("boost") != std::string::npos)
		ret = "Cleanse";

	if (name.find("exhaust") != std::string::npos)
		ret = "Exhaust";

	if (name.find("flash") != std::string::npos)
		ret = "Flash";

	if (name.find("haste") != std::string::npos)
		ret = "Ghost";

	if (name.find("heal") != std::string::npos)
		ret = "Heal";

	if (name.find("smite") != std::string::npos)
		ret = "Smite";

	if (name.find("teleport") != std::string::npos)
		ret = "Teleport";

	if (name.find("dot") != std::string::npos)
		ret = "Ignite";

	if (name.find("barrier") != std::string::npos)
		ret = "Barrier";

	return ret;
}

inline std::string getSpellImgByName(std::string name, bool IsDoneCD) {
	std::string ret = "";
	std::string _isCD = ((!IsDoneCD) ? "_cd" : "");
	if (name.find("boost") != std::string::npos)
		ret = "images\\cleanse" + _isCD + ".png";

	if (name.find("exhaust") != std::string::npos)
		ret = "images\\exhaust" + _isCD + ".png";

	if (name.find("flash") != std::string::npos)
		ret = "images\\flash" + _isCD + ".png";

	if (name.find("haste") != std::string::npos)
		ret = "images\\ghost" + _isCD + ".png";

	if (name.find("heal") != std::string::npos)
		ret = "images\\heal" + _isCD + ".png";

	if (name.find("smite") != std::string::npos)
		ret = "images\\smite" + _isCD + ".png";

	if (name.find("teleport") != std::string::npos)
		ret = "images\\teleport" + _isCD + ".png";

	if (name.find("dot") != std::string::npos)
		ret = "images\\ignite" + _isCD + ".png";

	if (name.find("barrier") != std::string::npos)
		ret = "images\\barrier" + _isCD + ".png";

	return ret;
	//return "C:\\Users\\dev\\Downloads\\cute.png";
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

			res = makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(GetCurrentProcess(), &addr, &size, oldProtection &oldProtection);
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