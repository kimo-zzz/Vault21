#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

template <typename T = NTSTATUS>
class makesyscall
{
public:
	makesyscall(byte sysindex1, byte sysindex2, byte sysindex3, byte sysindex4, LPCSTR lpFuncName, DWORD offsetToFunc,
	            byte retCode, byte ret1, byte ret2);
	~makesyscall();
public:
	template <typename... Args>
	T operator()(Args ... arg);
private:
	uint32_t GetSyscallIndex(const byte* pFuncAddr);
	void CreateShellSysCall(byte sysindex1, byte sysindex2, byte sysindex3, byte sysindex4, LPCSTR lpFuncName,
	                        DWORD offsetToFunc, byte retCode, byte ret1, byte ret2);
private:
	char* m_pShellCode;
};

template <typename T>
template <typename... Args>
T makesyscall<T>::operator()(Args ... arg)
{
	if (!m_pShellCode)
		return T(0);

	typedef T (__stdcall* mFunc)(Args ...);
	mFunc pFunc = mFunc(m_pShellCode);

	return pFunc(arg...);
}


template <typename T>
makesyscall<T>::makesyscall(byte sysindex1, byte sysindex2, byte sysindex3, byte sysindex4, LPCSTR lpFuncName,
                            DWORD offsetToFunc, byte retCode, byte ret1, byte ret2)
{
	//PBYTE pFuncAddr = (PBYTE)GetProcAddress(GetModuleHandleA("ntdll.dll"), lpFuncName);

	CreateShellSysCall(sysindex1, sysindex2, sysindex3, sysindex4, lpFuncName, offsetToFunc, retCode, ret1, ret2);
}

template <typename T>
makesyscall<T>::~makesyscall()
{
	if (m_pShellCode)
		VirtualFree(m_pShellCode, NULL, MEM_RELEASE);
}

template <typename T>
uint32_t makesyscall<T>::GetSyscallIndex(const byte* pFuncAddr)
{
	if (!pFuncAddr)
		return 0;

	for (int8_t i = 0; i < 0xF; ++i)
	{
		if (*(pFuncAddr + i) == 0xB8)
		{
			return *PUINT(pFuncAddr + i + 1);
		}
	}

	return 0;
}

/*
template<typename T>
void makesyscall<T>::CreateShellSysCall(const uint32_t sysindex)
{
	if (!sysindex)
		return;
	
	#ifdef _WIN64
		byte ShellCode[]
		{
			0x4C, 0x8B, 0xD1,					//mov r10, rcx 
			0xB8, 0x00, 0x00, 0x00, 0x00,		        //mov eax, SysCallIndex
			0x0F, 0x05,					        //syscall
			0xC3								//ret				
		};
	#elif _WIN32
		byte ShellCode[]
		{
			//0x90, 0x90, 0x90,					//nop, nop, nop
			0xB8, 0x50, 0x00, 0x00, 0x00,		        //mov eax, SysCallIndex
			0xBA, 0x00, 0x00, 0x00, 0x00,		        //mov edx, [Wow64Transition]
			0xFF, 0xD2,						//call edx
			0xC2, 0x14, 0x00								//ret
		};
 
		//*PDWORD(ShellCode + 6) = *(PDWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlInterlockedCompareExchange64") + 0x170;
	#endif

	//MessageBoxA(0, ("ntdll: " + hexify<DWORD>((DWORD)GetModuleHandleA("ntdll.dll"))).c_str(), "", 0);
	//MessageBoxA(0, ("Wow64Transition: " + hexify<DWORD>((DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlInterlockedCompareExchange64") + 0x170)).c_str(), "", 0);

	//*PDWORD(ShellCode + 1) = sysindex;
 
	m_pShellCode = (char*)VirtualAlloc(nullptr, sizeof(ShellCode), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!m_pShellCode)
		return;
 
	memcpy(m_pShellCode, ShellCode, sizeof(ShellCode));

	*(uintptr_t*)(m_pShellCode + 6) = ((uintptr_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlInterlockedCompareExchange64")) + 0x170;
}
*/

template <typename T>
void makesyscall<T>::CreateShellSysCall(byte sysindex1, byte sysindex2, byte sysindex3, byte sysindex4,
                                        LPCSTR lpFuncName, DWORD offsetToFunc, byte retCode, byte ret1, byte ret2)
{
	if (!sysindex1 && !sysindex2 && !sysindex3 && !sysindex4)
		return;

#ifdef _WIN64
	byte ShellCode[]
	{
		0x4C, 0x8B, 0xD1,					//mov r10, rcx 
		0xB8, 0x00, 0x00, 0x00, 0x00,		        //mov eax, SysCallIndex
		0x0F, 0x05,					        //syscall
		0xC3								//ret				
	};

	m_pShellCode = (char*)VirtualAlloc(nullptr, sizeof(ShellCode), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!m_pShellCode)
		return;

	memcpy(m_pShellCode, ShellCode, sizeof(ShellCode));

	*(byte*)(m_pShellCode + 4) = sysindex1;
	*(byte*)(m_pShellCode + 5) = sysindex2;
	*(byte*)(m_pShellCode + 6) = sysindex3;
	*(byte*)(m_pShellCode + 7) = sysindex4;

#elif _WIN32
	byte ShellCode[]
	{
		0xB8, 0x00, 0x00, 0x00, 0x00, //mov eax, SysCallIndex
		0xBA, 0x00, 0x00, 0x00, 0x00, //mov edx, [function]
		0xFF, 0xD2, //call edx
		0xC2, 0x00, 0x00 //ret
	};

	m_pShellCode = static_cast<char*>(VirtualAlloc(nullptr, sizeof(ShellCode), MEM_RESERVE | MEM_COMMIT,
	                                               PAGE_EXECUTE_READWRITE));

	if (!m_pShellCode)
		return;

	memcpy(m_pShellCode, ShellCode, sizeof(ShellCode));

	*(uintptr_t*)(m_pShellCode + 6) = static_cast<uintptr_t>((DWORD)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), lpFuncName) + offsetToFunc);

	*(byte*)(m_pShellCode + 1) = sysindex1;
	*(byte*)(m_pShellCode + 2) = sysindex2;
	*(byte*)(m_pShellCode + 3) = sysindex3;
	*(byte*)(m_pShellCode + 4) = sysindex4;

	*(byte*)(m_pShellCode + 12) = retCode;
	*(byte*)(m_pShellCode + 13) = ret1;
	*(byte*)(m_pShellCode + 14) = ret2;

	//MessageBoxA(0, ("ntdll: " + hexify<DWORD>((DWORD)GetModuleHandleA("ntdll.dll"))).c_str(), "", 0);
	//MessageBoxA(0, (std::string(lpFuncName) + " " + hexify<DWORD>((DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), lpFuncName) + offsetToFunc)).c_str(), "", 0);
#endif
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
template<typename T = NTSTATUS>
class callNtQueryInformationProcess
{
public:
	callNtQueryInformationProcess();
	~callNtQueryInformationProcess();
public:
	template<typename... Args>
	T operator()(Args... arg);
private:
	uint32_t	GetSyscallIndex(const byte* pFuncAddr);
	void		CreateShellSysCall();
private:
	char* m_pShellCode;
};

template<typename T> template<typename... Args>
T callNtQueryInformationProcess<T>::operator()(Args... arg)
{
	if (!m_pShellCode)
		return T(0);

	typedef T(__stdcall* mFunc)(Args...);
	mFunc pFunc = mFunc(m_pShellCode);

	return pFunc(arg...);
}


template<typename T>
callNtQueryInformationProcess<T>::callNtQueryInformationProcess()
{
	//PBYTE pFuncAddr = (PBYTE)GetProcAddress(GetModuleHandleA("ntdll.dll"), lpFuncName);

	CreateShellSysCall();
}

template<typename T>
callNtQueryInformationProcess<T>::~callNtQueryInformationProcess()
{
	if (m_pShellCode)
		VirtualFree(m_pShellCode, NULL, MEM_RELEASE);
}

template<typename T>
uint32_t callNtQueryInformationProcess<T>::GetSyscallIndex(const byte* pFuncAddr)
{
	if (!pFuncAddr)
		return 0;

	for (int8_t i = 0; i < 0xF; ++i)
	{
		if (*(pFuncAddr + i) == 0xB8)
		{
			return *PUINT(pFuncAddr + i + 1);
		}
	}

	return 0;
}

template<typename T>
void callNtQueryInformationProcess<T>::CreateShellSysCall()
{
	byte ShellCode[]
	{
		0xB8, 0x16, 0x00, 0x00, 0x00,
		0xBA, 0xD0, 0x60, 0x00, 0x77,
		0xFF, 0xD2,
		0xC2, 0x14, 0x00,
		0x90,

		0xB8, 0x17, 0x00, 0x00, 0x00,
		0xBA, 0xD0, 0x60, 0x00, 0x77,
		0xFF, 0xD2,
		0xC2, 0x18, 0x00,
		0x90,

		0xB8, 0x18, 0x00, 0x00, 0x00,
		0xBA, 0xD0, 0x60, 0x00, 0x77,
		0xFF, 0xD2,
		0xC2, 0x18, 0x00,
		0x90

		0xB8, 0x19, 0x00, 0x00, 0x00,
		0xE8, 0x00, 0x00, 0x00, 0x00,
		0x5A,
		0x80, 0x7A, 0x14, 0x4B,
		0x75, 0x0E,
		0x64, 0xFF, 0x15, 0xC0, 0x00, 0x00, 0x00,
		0xC2, 0x14, 0x00,
		0x00, 0x00,
		0xF8,
		0x76, 0xBA,
		0xD0, 0x60, 0x00,
		0x77, 0xFF,
		0xD2, 0xC2,
		0x14, 0x00,
		0x8D, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00
	};


	m_pShellCode = (char*)VirtualAlloc(nullptr, sizeof(ShellCode), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!m_pShellCode)
		return;

	memcpy(m_pShellCode, ShellCode, sizeof(ShellCode));

	//MessageBoxA(0, ("ntdll: " + hexify<DWORD>((DWORD)GetModuleHandleA("ntdll.dll"))).c_str(), "", 0);
	//MessageBoxA(0, (std::string(lpFuncName) + " " + hexify<DWORD>((DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), lpFuncName) + offsetToFunc)).c_str(), "", 0);

}
*/
