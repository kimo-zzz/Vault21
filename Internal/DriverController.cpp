#pragma once
#include <Windows.h>
#include <iostream>
#include <intrin.h>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <string>
#include <filesystem>

#include <Dwmapi.h> 
#include <TlHelp32.h>
#include <Psapi.h>

#include "DriverController.h"
#include "stdafx.h"

unsigned __int32 DriverController::GetProcessPidByName(const wchar_t* ProcessName)
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &pt)) {
		do {
			if (!lstrcmpi(pt.szExeFile, ProcessName)) {
				CloseHandle(hsnap);
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap);
	return 0;
}

bool DriverController::ResetProcessId(const wchar_t* TargetProcessName)
{
	TargetProcessPid = GetProcessPidByName(TargetProcessName);

	if (!TargetProcessPid)
		return false;

	return true;
}

DriverController::DriverController(unsigned int TargetProcessPid)
{
	DriverHandle = INVALID_HANDLE_VALUE;

	this->TargetProcessPid = TargetProcessPid;

	if (!this->TargetProcessPid) {

		printf("Target Process Does not Exist... did you mean to do this?\n");
	}

	DriverHandle = CreateFile(TEXT("\\\\.\\\ACPI_ROOT_OBJECT"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
}

DriverController::DriverController(const wchar_t* TargetProcessName)
{
	DriverHandle = INVALID_HANDLE_VALUE;

	TargetProcessPid = GetProcessPidByName(TargetProcessName);

	if (!TargetProcessPid) {

		printf("Target Process Does not Exist... did you mean to do this?\n");
	}

	DriverHandle = CreateFile(TEXT("\\\\.\\\ACPI_ROOT_OBJECT"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
}

DriverController::~DriverController()
{
	if (DriverHandle != INVALID_HANDLE_VALUE)
		CloseHandle(DriverHandle);
}

bool DriverController::WriteProcessMemory(unsigned __int64 Address, void* Buffer, unsigned __int32 Length)
{
	if (!Address || !Buffer || !Length)
		return false;

	unsigned char* TempAllocationBuffer = (unsigned char*)malloc(Length);

	WRITE_PROCESS_MEMORY DriverCall;

	DriverCall.Filter = 0xDEADBEEFCAFEBEEF;
	DriverCall.ControlCode = WRITE_PROCESS_MEMORY_IOCTL;

	DriverCall.ProcessAddress = Address;
	DriverCall.InBuffer = (unsigned __int64)TempAllocationBuffer;
	DriverCall.Length = Length;
	DriverCall.ProcessId = TargetProcessPid;

	memcpy(TempAllocationBuffer, Buffer, Length);

	DWORD BytesOut = 0;

	if (DeviceIoControl(DriverHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, &DriverCall, sizeof(DriverCall), &DriverCall, sizeof(DriverCall), &BytesOut, 0)) {

		free(TempAllocationBuffer);
		return true;
	}

	free(TempAllocationBuffer);
	return false;
}

bool DriverController::WriteProtectedProcessMemory(UINT64 Address, PVOID Value, SIZE_T Size)
{
	if (!Address || !Value || !Size)
		return false;

	WRITE_PROTECTED_VIRTUAL_MEMORY DriverCall;

	DriverCall.Filter = 0xDEADBEEFCAFEBEEF;
	DriverCall.ControlCode = WRITE_PROTECTED_VIRTUAL_MEMORY_IOCTL;

	DriverCall.Address = Address;
	DriverCall.Value = Value;
	DriverCall.Size = Size;
	DriverCall.ProcessId = TargetProcessPid;

	MessageBoxA(0, std::to_string(TargetProcessPid).c_str(), "", 0);

	DWORD BytesOut = 0;

	if (DeviceIoControl(DriverHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, &DriverCall, sizeof(DriverCall), &DriverCall, sizeof(DriverCall), &BytesOut, 0)) {
	}
	return true;
}

bool DriverController::ReadProcessMemory(unsigned __int64 Address, void* Buffer, unsigned __int32 Length)
{
	if (!Address || !Buffer || !Length)
		return false;

	unsigned char* TempAllocationBuffer = (unsigned char*)malloc(Length);

	READ_PROCESS_MEMORY DriverCall;
	DriverCall.Filter = 0xDEADBEEFCAFEBEEF;
	DriverCall.ControlCode = READ_PROCESS_MEMORY_IOCTL;

	DriverCall.ProcessAddress = Address;
	DriverCall.OutBuffer = (unsigned __int64)TempAllocationBuffer;
	DriverCall.Length = Length;
	DriverCall.ProcessId = TargetProcessPid;

	DWORD BytesOut = 0;

	if (DeviceIoControl(DriverHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, &DriverCall, sizeof(DriverCall), &DriverCall, sizeof(DriverCall), &BytesOut, 0)) {

		memcpy(Buffer, TempAllocationBuffer, Length);

		free(TempAllocationBuffer);
		return true;
	}

	free(TempAllocationBuffer);
	return false;
}



SIZE_T DriverController::VirtualQueryEx(PVOID BaseAddress, MEMORY_BASIC_INFORMATION& mbi)
{
	if (!BaseAddress)
		return 0;

	VIRTUAL_QUERY_EX DriverCall;
	DriverCall.Filter = 0xDEADBEEFCAFEBEEF;
	DriverCall.ControlCode = VIRTUAL_QUERY_EX_IOCTL;

	DriverCall.ProcessId = TargetProcessPid;
	DriverCall.BaseAddress = BaseAddress;
	DriverCall.mbi = mbi;
	DriverCall.size = 0;

	DWORD BytesOut = 0;

	if (DeviceIoControl(DriverHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, &DriverCall, sizeof(DriverCall), &DriverCall, sizeof(DriverCall), &BytesOut, 0)) {
	}

	mbi = DriverCall.mbi;
	return DriverCall.size;
}

uint32_t DriverController::VirtualProtect(DWORD address, int size, uint32_t protect)
{
	if (!address || !size || !protect)
		return 0;

	//MessageBoxA(0, ("pSize: " + hexify<int>(size)).c_str(), "", 0);

	VIRTUAL_PROTECT DriverCall;
	DriverCall.Filter = 0xDEADBEEFCAFEBEEF;
	DriverCall.ControlCode = VIRTUAL_PROTECT_IOCTL;

	DriverCall.ProcessId = TargetProcessPid;
	DriverCall.addr = address;
	DriverCall.protect = protect;
	DriverCall.size = size;

	DWORD BytesOut = 0;

	if (DeviceIoControl(DriverHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, &DriverCall, sizeof(DriverCall), &DriverCall, sizeof(DriverCall), &BytesOut, 0)) {
		//MessageBoxA(0, ("post oldProtection: " + hexify<ULONG>(DriverCall.protect)).c_str(), "", 0);
		return protect;
	}

	return 0;
}

///////////////////////////////////////////
//ROAD TO BSOD. HAHAHA
///////////////////////////////////////////
/*
bool DriverController::FindPatternProcessMemory(char* Pattern, char* Mask)
{
	if (!Pattern || !Mask)
		return false;


	FIND_PATTERN_PROCESS_MEMORY DriverCall;
	DriverCall.Filter = 0xDEADBEEFCAFEBEEF;
	DriverCall.ControlCode = FIND_PATTERN_PROCESS_MEMORY_IOCTL;

	DriverCall.ProcessId = TargetProcessPid;

	SYSTEM_INFO sysInfo = { 0 };
	GetSystemInfo(&sysInfo);

	uint64_t mem_region_start = (uint64_t)sysInfo.lpMinimumApplicationAddress;
	uint64_t mem_region_end = (uint64_t)sysInfo.lpMaximumApplicationAddress;

	DriverCall.StartAddress = mem_region_start;
	DriverCall.EndAddress = mem_region_end;

	DriverCall.Pattern = Pattern;
	DriverCall.Mask = Mask;

	DWORD BytesOut = 0;

	if (DeviceIoControl(DriverHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, &DriverCall, sizeof(DriverCall), &DriverCall, sizeof(DriverCall), &BytesOut, 0)) {
	}

	return DriverCall.OutBuffer;
}
*/

unsigned __int64 DriverController::GetProcessBase()
{
	GET_PROCESS_BASE DriverCall;

	DriverCall.Filter = 0xDEADBEEFCAFEBEEF;
	DriverCall.ControlCode = GET_PROCESS_BASE_IOCTL;

	DriverCall.ProcessId = TargetProcessPid;
	DriverCall.ProcessBaseAddres = -1;

	DWORD BytesOut = 0;

	if (DeviceIoControl(DriverHandle, IOCTL_DISK_GET_DRIVE_GEOMETRY, &DriverCall, sizeof(DriverCall), &DriverCall, sizeof(DriverCall), &BytesOut, 0)) {
	}

	return DriverCall.ProcessBaseAddres;
}


unsigned long long DriverController::ReadUInt64(unsigned long long Address)
{
	unsigned long long buffer = 0;

	this->ReadProcessMemory((unsigned long long)Address, (void*)&buffer, 8);

	return (buffer);
}

unsigned short DriverController::ReadUInt16(unsigned long long Address)
{
	unsigned short buffer = 0;

	this->ReadProcessMemory((unsigned long long)Address, (void*)&buffer, 2);

	return (buffer);
}

unsigned int  DriverController::ReadUInt32(unsigned long long Address)
{
	unsigned int  buffer = 0;

	this->ReadProcessMemory((unsigned long long)Address, (void*)&buffer, 4);

	return (buffer);
}

float DriverController::ReadFloat(unsigned long long Address)
{
	float buffer = 0;

	this->ReadProcessMemory((unsigned long long)Address, (void*)&buffer, 4);

	return (buffer);
}

unsigned char DriverController::ReadByte(unsigned long long Address)
{
	unsigned char buffer = 0;

	this->ReadProcessMemory((unsigned long long)Address, (void*)&buffer, 1);

	return (buffer);
}

void DriverController::WriteByte(unsigned long long Address, unsigned char data)
{
	this->WriteProcessMemory((unsigned long long)Address, (void*)&data, 1);
}

void DriverController::WriteUInt32(unsigned long long Address, unsigned int data)
{
	this->WriteProcessMemory((unsigned long long)Address, (void*)&data, 4);
}

void DriverController::WriteUInt16(unsigned long long Address, unsigned short data)
{
	this->WriteProcessMemory((unsigned long long)Address, (void*)&data, 2);
}

void DriverController::WriteUInt64(unsigned long long Address, unsigned long long data)
{
	this->WriteProcessMemory((unsigned long long)Address, (void*)&data, 8);
}

