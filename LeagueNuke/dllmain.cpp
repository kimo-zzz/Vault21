#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>
#include <string>
#include <process.h>
#include <vector>

uint8_t* _RtlDispatchExceptionAddress;

struct LeagueDecryptData
{
    int totalSuccessDecrypted;
    int totalSuccess_PAGE_NOACCESS;
    int totalSuccess_EXCEPTION_CONTINUE_EXECUTION;
    int totalFailedDecrypted;
};

typedef BOOLEAN(__stdcall* t_RtlDispatchException)(PEXCEPTION_RECORD exceptionRecord, PCONTEXT contextRecord);
t_RtlDispatchException fn_RtlDispatchException;

uint8_t* FindSignature(const LPCSTR szModule, const char* szSignature)
{
    const HMODULE module = GetModuleHandleA(szModule);
    static auto patternToByte = [](const char* pattern)
    {
        auto bytes = std::vector<int>{};
        char* start = const_cast<char*>(pattern);
        char* end = const_cast<char*>(pattern) + strlen(pattern);

        for (char* current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else
                bytes.push_back(strtoul(current, &current, 16));
        }
        return bytes;
    };

    if (module)
    {
        auto* dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
        auto* ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(module) + dosHeader->e_lfanew);
        auto* textSection = IMAGE_FIRST_SECTION(ntHeaders);

        const DWORD sizeOfImage = textSection->SizeOfRawData;
        auto patternBytes = patternToByte(szSignature);
        uint8_t* scanBytes = reinterpret_cast<uint8_t*>(module) + textSection->VirtualAddress;

        const size_t size = patternBytes.size();
        int* data = patternBytes.data();

        auto mbi = MEMORY_BASIC_INFORMATION{ nullptr };
        uint8_t* nextCheckAddress = nullptr;

        for (auto i = 0ul; i < sizeOfImage - size; ++i)
        {
            bool found = true;
            for (auto j = 0ul; j < size; ++j)
            {
                unsigned char* const currentAddress = scanBytes + i + j;
                if (currentAddress >= nextCheckAddress)
                {
                    if (!VirtualQuery(reinterpret_cast<void*>(currentAddress), &mbi, sizeof(mbi)))
                        break;

                    if (mbi.Protect == PAGE_NOACCESS)
                    {
                        i += reinterpret_cast<std::uintptr_t>(mbi.BaseAddress) + mbi.RegionSize - (reinterpret_cast<std::uintptr_t>(scanBytes) + i);
                        i--;
                        found = false;
                        break;
                    }
                    nextCheckAddress = static_cast<uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
                }

                if (scanBytes[i + j] != data[j] && data[j] != -1)
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return &scanBytes[i];
        }
    }
    return nullptr;
}

uint8_t* FindRtlDispatchExceptionAddress()
{
    auto* address = FindSignature("ntdll.dll", "E8 ? ? ? ? 0A C0");
    if (!address)
        return nullptr;

    address = address + *reinterpret_cast<uint32_t*>(address + 1) + 5;
    return address;
}

int IsMemoryDecrypted(PVOID address)
{
    CONTEXT ctx;
    EXCEPTION_RECORD exr;
    MEMORY_BASIC_INFORMATION mbi;
    FLOATING_SAVE_AREA w64;
    memset(&mbi, 0, sizeof(mbi));
    VirtualQuery(address, &mbi, sizeof(mbi));
    if (mbi.Protect != PAGE_NOACCESS)
        return 1;
    RtlCaptureContext(&ctx);
    memset(&exr, 0, sizeof(EXCEPTION_RECORD));
    memset(&w64, 0, sizeof(WOW64_FLOATING_SAVE_AREA));

#ifdef _WIN64
    ctx.Rip = reinterpret_cast<DWORD64>(Address);// (DWORD)FinishThread;
#else
    ctx.Eip = reinterpret_cast<DWORD>(address);// (DWORD)FinishThread;
#endif // 

    ctx.ContextFlags = 0x1007F;
    ctx.SegCs = 0x23;
    ctx.SegDs = 0x2B;
    ctx.SegEs = 0x2B;
    ctx.SegFs = 0x53;
    ctx.SegGs = 0x2B;
    ctx.SegSs = 0x2B;
    exr.ExceptionAddress = address;
    exr.NumberParameters = 2;
    exr.ExceptionCode = EXCEPTION_ACCESS_VIOLATION;
    exr.ExceptionInformation[1] = reinterpret_cast<DWORD>(address);
    _EXCEPTION_POINTERS ei;
    ei.ContextRecord = &ctx;
    ei.ExceptionRecord = &exr;

    if (!_RtlDispatchExceptionAddress)
        return 0;

    const DWORD RtlDispatchExceptionAddr = reinterpret_cast<DWORD>(_RtlDispatchExceptionAddress);
    if (RtlDispatchExceptionAddr)
    {
        fn_RtlDispatchException = reinterpret_cast<t_RtlDispatchException>(RtlDispatchExceptionAddr);
        if (fn_RtlDispatchException(&exr, &ctx))
            return 2;
    }
    return 0;
}

LeagueDecryptData Decrypt(const wchar_t* szModule)
{
    LeagueDecryptData ldd;
    ldd.totalFailedDecrypted = 0;
    ldd.totalSuccessDecrypted = 0;
    ldd.totalSuccess_PAGE_NOACCESS = 0;
    ldd.totalSuccess_EXCEPTION_CONTINUE_EXECUTION = 0;

    auto* const module = GetModuleHandle(szModule);
    if (module)
    {
        auto* dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
        auto* ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(module) + dosHeader->e_lfanew);
        auto* textSection = IMAGE_FIRST_SECTION(ntHeaders);

        const DWORD sizeOfImage = textSection->SizeOfRawData;
        uint8_t* scanBytes = reinterpret_cast<uint8_t*>(module) + textSection->VirtualAddress;

        auto mbi = MEMORY_BASIC_INFORMATION{ nullptr };
        uint8_t* nextCheckAddress = nullptr;

        bool isFirstRegion = true;
        for (unsigned int i = 0; i < sizeOfImage; ++i)
        {
            unsigned char* currentAddress = scanBytes + i;
            if (currentAddress >= nextCheckAddress)
            {
                if (!VirtualQuery(reinterpret_cast<void*>(currentAddress), &mbi, sizeof(mbi)))
                    continue;

                if (mbi.Protect != PAGE_NOACCESS || isFirstRegion)
                {
                    isFirstRegion = false;
                    i += reinterpret_cast<std::uintptr_t>(mbi.BaseAddress) + mbi.RegionSize - (reinterpret_cast<std::uintptr_t>(scanBytes) + i);
                    i--;
                    continue;
                }
                nextCheckAddress = static_cast<uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
            }
            const int ret = IsMemoryDecrypted(reinterpret_cast<PVOID>(reinterpret_cast<DWORD>(currentAddress)));
            if (ret != 0)
            {
                if (ret == 1)
                    ldd.totalSuccess_PAGE_NOACCESS++;

                else if (ret == 2)
                    ldd.totalSuccess_EXCEPTION_CONTINUE_EXECUTION++;

                ldd.totalSuccessDecrypted++;
            }
            else
            {
                ldd.totalFailedDecrypted++;
            }
        }
    }
    return ldd;
}

__declspec(safebuffers)DWORD WINAPI InitThread(LPVOID module)
{
    _RtlDispatchExceptionAddress = FindRtlDispatchExceptionAddress();

    LeagueDecryptData ldd = Decrypt(nullptr);
    MessageBoxA(nullptr, (
        "totalFailedDecrypted: " + std::to_string(ldd.totalFailedDecrypted) + "\n" +
        "totalSuccessDecrypted: " + std::to_string(ldd.totalSuccessDecrypted) + "\n" +
        "totalSuccess_EXCEPTION_CONTINUE_EXECUTION: " + std::to_string(ldd.totalSuccess_EXCEPTION_CONTINUE_EXECUTION) + "\n" +
        "totalSuccess_PAGE_NOACCESS: " + std::to_string(ldd.totalSuccess_PAGE_NOACCESS) + "\n" +
        "USE DUMPER NOW! Do not close this MessageBox!"
        ).c_str(), "LeagueNuke", 0);

    return 0;
}

void OnExit() noexcept;
uintptr_t initThreadHandle;

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD dwReason, LPVOID lpReserved)
{
    DisableThreadLibraryCalls(hModule);
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        std::atexit(OnExit);
        initThreadHandle = _beginthreadex(nullptr, 0, reinterpret_cast<_beginthreadex_proc_type>(InitThread), hModule, 0, nullptr);

        FreeLibrary(hModule);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        OnExit();
    }

    return TRUE;
}

void OnExit() noexcept
{
    WaitForSingleObject(reinterpret_cast<HANDLE>(initThreadHandle), INFINITE);
    CloseHandle(reinterpret_cast<HANDLE>(initThreadHandle));
}