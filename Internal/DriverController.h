#pragma once

struct CUSTOM_IOCTL_CALL
{
	ULONG64 Filter;
	ULONG ControlCode;
};

struct READ_PROCESS_MEMORY : CUSTOM_IOCTL_CALL
{
	unsigned __int64 ProcessId;
	unsigned __int64 ProcessAddress;
	unsigned __int64 Length;
	unsigned __int64 OutBuffer;
};

///////////////////////////////////////////
//ROAD TO BSOD. HAHAHA
///////////////////////////////////////////
/*
struct FIND_PATTERN_PROCESS_MEMORY : CUSTOM_IOCTL_CALL
{
	unsigned __int64 ProcessId;
	unsigned __int64 StartAddress;
	unsigned __int64 EndAddress;
	char* Pattern;
	char* Mask;
	unsigned __int64 OutBuffer;
};
*/

struct VIRTUAL_QUERY_EX : CUSTOM_IOCTL_CALL
{
	unsigned __int64 ProcessId;
	PVOID BaseAddress;
	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T size;
};

struct VIRTUAL_PROTECT : CUSTOM_IOCTL_CALL
{
	unsigned __int64 ProcessId;
	ULONG protect;
	DWORD addr;
	int size;
};


struct WRITE_PROCESS_MEMORY : CUSTOM_IOCTL_CALL
{
	unsigned __int64 ProcessId;
	unsigned __int64 ProcessAddress;
	unsigned __int64 Length;
	unsigned __int64 InBuffer;
};

struct WRITE_PROTECTED_VIRTUAL_MEMORY : CUSTOM_IOCTL_CALL
{
	unsigned __int64 ProcessId;
	UINT64 Address; // Target
	PVOID Value; // Source
	SIZE_T Size;
};

struct GET_PROCESS_BASE : CUSTOM_IOCTL_CALL
{
	unsigned __int64 ProcessId;
	unsigned __int64 ProcessBaseAddres;
};

class DriverController
{
public:
	DriverController(const wchar_t* TargetProcessName);
	DriverController(unsigned int TargetProcessPid);
	~DriverController();
	bool WriteProcessMemory(unsigned __int64 Address, void* Buffer, unsigned __int32 Length);
	bool ReadProcessMemory(unsigned __int64 Address, void* Buffer, unsigned __int32 Length);
	bool WriteProtectedProcessMemory(UINT64 Address, PVOID Value, SIZE_T Size);

	SIZE_T VirtualQueryEx(PVOID BaseAddress, MEMORY_BASIC_INFORMATION& mbi);
	uint32_t VirtualProtect(DWORD address, int size, uint32_t protect);
	///////////////////////////////////////////
	//ROAD TO BSOD. HAHAHA
	///////////////////////////////////////////
	//bool FindPatternProcessMemory(char* Pattern, char* Mask);

	bool ResetProcessId(const wchar_t* TargetProcessName);
	unsigned __int64 GetProcessBase();
	unsigned long long ReadUInt64(unsigned long long Address);
	unsigned short ReadUInt16(unsigned long long Address);
	unsigned int  ReadUInt32(unsigned long long Address);
	float ReadFloat(unsigned long long Address);
	unsigned char ReadByte(unsigned long long Address);
	void WriteByte(unsigned long long Address, unsigned char data);
	void WriteUInt32(unsigned long long Address, unsigned int data);
	void WriteUInt16(unsigned long long Address, unsigned short data);
	void WriteUInt64(unsigned long long Address, unsigned long long data);

	unsigned __int32 TargetProcessPid;
private:
	HANDLE DriverHandle;
	unsigned __int32 GetProcessPidByName(const wchar_t* ProcessName);
};

#define READ_PROCESS_MEMORY_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2331, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define WRITE_PROCESS_MEMORY_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2332, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define GET_PROCESS_BASE_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2333, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define VIRTUAL_QUERY_EX_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2336, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define WRITE_PROTECTED_VIRTUAL_MEMORY_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2337, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define VIRTUAL_PROTECT_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2338, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
///////////////////////////////////////////
//ROAD TO BSOD. HAHAHA
///////////////////////////////////////////
//#define FIND_PATTERN_PROCESS_MEMORY_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2335, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)