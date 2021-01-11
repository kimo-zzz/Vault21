#include "main.h"

//#pragma alloc_text(INIT, DriverEntry)

//This is a .asm file
/* 
.code
DispatchHook proc
	add rsp, 8h
	mov rax, 0DEADBEEFCAFEBEEFh
	jmp rax
DispatchHook endp

end
*/
//#include "winbase.h"

extern "C" void DispatchHook();

PDRIVER_DISPATCH ACPIOriginalDispatch = 0;

typedef enum _MEMORY_INFORMATION_CLASS_EX {
	MemoryBasicInformationEx = 0,
} MEMORY_INFORMATION_CLASS_EX;

///////////////////////////////////////////
//ROAD TO BSOD. HAHAHA
///////////////////////////////////////////
/*
uintptr_t scanPattern(char* base, const size_t size, char* pattern, char* mask) {
	const auto patternSize = strlen(mask);

	for (size_t i = {}; i < size - patternSize; i++)
	{
		for (size_t j = {}; j < patternSize; j++)
		{
			if (mask[j] != '?' && *(base + i + j) != static_cast<char>(pattern[j]))
				break;

			if (j == patternSize - 1)
				return reinterpret_cast<uintptr_t>(base) + i;
		}
	}

	return {};
}
*/

NTSTATUS ProcessReadWriteMemory(PEPROCESS SourceProcess, PVOID SourceAddress, PEPROCESS TargetProcess, PVOID TargetAddress, SIZE_T Size)
{
	SIZE_T Bytes = 0;

	if (NT_SUCCESS(MmCopyVirtualMemory(SourceProcess, SourceAddress, TargetProcess, TargetAddress, Size, UserMode, &Bytes)))
		return STATUS_SUCCESS;
	else
		return STATUS_ACCESS_DENIED;
}


NTSTATUS CustomDispatch(PDEVICE_OBJECT device, PIRP irp)
{
	PIO_STACK_LOCATION ioc = IoGetCurrentIrpStackLocation(irp);
	NTSTATUS Status;
	ULONG BytesIO = 0;

	//Here you can do your custom calls

	if (ioc->Parameters.DeviceIoControl.IoControlCode == IOCTL_DISK_GET_DRIVE_GEOMETRY)
	{
		CUSTOM_IOCTL_CALL* Buffer = (CUSTOM_IOCTL_CALL*)irp->AssociatedIrp.SystemBuffer;

		if (Buffer->Filter == 0xDEADBEEFCAFEBEEF)
		{
			if (Buffer->ControlCode == READ_PROCESS_MEMORY_IOCTL)
			{
				READ_PROCESS_MEMORY* UserlandBuffer = (READ_PROCESS_MEMORY*)irp->AssociatedIrp.SystemBuffer;

				PEPROCESS TargetProcess = 0;

				if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)UserlandBuffer->ProcessId, &TargetProcess))) {

					Status = ProcessReadWriteMemory(TargetProcess, (PVOID)UserlandBuffer->ProcessAddress, IoGetCurrentProcess(), (PVOID)UserlandBuffer->OutBuffer, UserlandBuffer->Length);
					ObfDereferenceObject(TargetProcess);
				}
				Status = STATUS_SUCCESS;
				BytesIO = sizeof(READ_PROCESS_MEMORY);
				Printf("READ_PROCESS_MEMORY_IOCTL");
				Printf("ProcessId %d", UserlandBuffer->ProcessId);
				Printf("ProcessAddress %llu", UserlandBuffer->ProcessAddress);
				Printf("Length %d", UserlandBuffer->Length);
			}
			else if (Buffer->ControlCode == WRITE_PROCESS_MEMORY_IOCTL)
			{
				WRITE_PROCESS_MEMORY* UserlandBuffer = (WRITE_PROCESS_MEMORY*)irp->AssociatedIrp.SystemBuffer;

				PEPROCESS TargetProcess = 0;

				if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)UserlandBuffer->ProcessId, &TargetProcess))) {
					Status = ProcessReadWriteMemory(IoGetCurrentProcess(), (PVOID)UserlandBuffer->InBuffer, TargetProcess, (PVOID)UserlandBuffer->ProcessAddress, UserlandBuffer->Length);

					ObfDereferenceObject(TargetProcess);
				}
				Status = STATUS_SUCCESS;
				BytesIO = sizeof(WRITE_PROCESS_MEMORY);
				Printf("WRITE_PROCESS_MEMORY_IOCTL");
				Printf("ProcessId %d", UserlandBuffer->ProcessId);
				Printf("ProcessAddress %llu", UserlandBuffer->ProcessAddress);
				Printf("Length %d", UserlandBuffer->Length);
			}
			else if (Buffer->ControlCode == GET_PROCESS_BASE_IOCTL)
			{
				GET_PROCESS_BASE* UserlandBuffer = (GET_PROCESS_BASE*)irp->AssociatedIrp.SystemBuffer;

				PEPROCESS TargetProcess = 0;

				UserlandBuffer->ProcessBaseAddres = -1;

				if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)UserlandBuffer->ProcessId, &TargetProcess))) {
					UserlandBuffer->ProcessBaseAddres = (unsigned __int64)PsGetProcessSectionBaseAddress(TargetProcess);

					ObfDereferenceObject(TargetProcess);
				}
				Status = STATUS_SUCCESS;
				BytesIO = sizeof(GET_PROCESS_BASE);
				Printf("GET_PROCESS_BASE_IOCTL");
				Printf("ProcessId %d", UserlandBuffer->ProcessId);
				Printf("ProcessBaseAddres %llu", UserlandBuffer->ProcessBaseAddres);
			}
			else if (Buffer->ControlCode == GET_PROCESS_PEB_IOCTL)
			{
				GET_PROCESS_PEB* UserlandBuffer = (GET_PROCESS_PEB*)irp->AssociatedIrp.SystemBuffer;

				PEPROCESS TargetProcess = 0;

				UserlandBuffer->ProcessBaseAddres = -1;

				if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)UserlandBuffer->ProcessId, &TargetProcess))) {
					UserlandBuffer->ProcessBaseAddres = (unsigned __int64)PsGetProcessPeb(TargetProcess);

					ObfDereferenceObject(TargetProcess);
				}
				Status = STATUS_SUCCESS;
				BytesIO = sizeof(GET_PROCESS_PEB);
				Printf("GET_PROCESS_PEB_IOCTL");
				Printf("ProcessId %d", UserlandBuffer->ProcessId);
				Printf("ProcessBaseAddres %llu", UserlandBuffer->ProcessBaseAddres);
			}
			else if (Buffer->ControlCode == VIRTUAL_QUERY_EX_IOCTL)
			{
				VIRTUAL_QUERY_EX* UserlandBuffer = (VIRTUAL_QUERY_EX*)irp->AssociatedIrp.SystemBuffer;

				// Acquire target process handle.
				PEPROCESS target = nullptr;
				const auto status = PsLookupProcessByProcessId((HANDLE)UserlandBuffer->ProcessId, &target); 

				if(NT_SUCCESS(status)) {
					// Attach to process.
					KAPC_STATE state = {};
					KeStackAttachProcess(target, &state);

					SIZE_T size = 0;
					ZwQueryVirtualMemory(ZwCurrentProcess(), UserlandBuffer->BaseAddress, static_cast<MEMORY_INFORMATION_CLASS>(MemoryBasicInformationEx), &UserlandBuffer->mbi, sizeof(UserlandBuffer->mbi), &size);
					UserlandBuffer->size = size;

					// Detach from process.
					KeUnstackDetachProcess(&state);

					// Release process handle.
					ObfDereferenceObject(target);
				}

				Status = STATUS_SUCCESS;
				BytesIO = sizeof(VIRTUAL_QUERY_EX);
				Printf("VIRTUAL_QUERY_EX_IOCTL");
				Printf("ProcessId %d", UserlandBuffer->ProcessId);
				Printf("BaseAddress %llu", UserlandBuffer->BaseAddress);
			}
			//////////////////////////////////////////
			//ROAD TO BSOD. HAHAHA
			///////////////////////////////////////////
			/*
			else if (Buffer->ControlCode == WRITE_PROTECTED_VIRTUAL_MEMORY_IOCTL)
			{
				WRITE_PROTECTED_VIRTUAL_MEMORY* UserlandBuffer = (WRITE_PROTECTED_VIRTUAL_MEMORY*)irp->AssociatedIrp.SystemBuffer;

				PEPROCESS target = nullptr;
				KAPC_STATE state;
				NTSTATUS Status;

				if (!NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)UserlandBuffer->ProcessId, &target))) {
					//return STATUS_INVALID_PARAMETER_1;
				}else{
					PVOID Address = (PVOID)UserlandBuffer->Address;
					PVOID ProtectedAddress = (PVOID)UserlandBuffer->Address;
					SIZE_T Size = UserlandBuffer->Size;
					SIZE_T ProtectedSize = UserlandBuffer->Size;

					PVOID* Buffer = (PVOID*)ExAllocatePool(NonPagedPool, Size); // Pointer to Allocated Memory
					if (Buffer == NULL) {
						ObfDereferenceObject(target);

						//return STATUS_MEMORY_NOT_ALLOCATED;
					}
					else {
						RtlSecureZeroMemory(Buffer, Size);

						__try {
							memcpy(Buffer, UserlandBuffer->Value, Size);

							KeStackAttachProcess(target, &state);

							ULONG OldProtection;
							Status = ZwProtectVirtualMemory(ZwCurrentProcess(), &ProtectedAddress, &ProtectedSize, PAGE_EXECUTE_READWRITE, &OldProtection);
							if (!NT_SUCCESS(Status)) {
								KeUnstackDetachProcess(&state);

								ExFreePool(Buffer);
								ObfDereferenceObject(target);

								//return Status;
							}
							else {
								ProtectedAddress = Address;
								ProtectedSize = Size;

								MEMORY_BASIC_INFORMATION info;
								Status = ZwQueryVirtualMemory(ZwCurrentProcess(), Address, MemoryBasicInformation, &info, sizeof(MEMORY_BASIC_INFORMATION), NULL);
								if (!NT_SUCCESS(Status)) {
									KeUnstackDetachProcess(&state);

									ExFreePool(Buffer);
									ObfDereferenceObject(target);

									//return Status;
								}
								else {
									if (!(info.State & MEM_COMMIT)) {
										ZwProtectVirtualMemory(ZwCurrentProcess(), &ProtectedAddress, &ProtectedSize, OldProtection, &OldProtection);
										KeUnstackDetachProcess(&state);

										ExFreePool(Buffer);
										ObfDereferenceObject(target);

										//Status = STATUS_ACCESS_DENIED;

										//return Status;
									}
									else {
										memcpy(Address, Buffer, Size);

										ZwProtectVirtualMemory(ZwCurrentProcess(), &ProtectedAddress, &ProtectedSize, OldProtection, &OldProtection);
										KeUnstackDetachProcess(&state);

										//Status = STATUS_SUCCESS;
									}
								}
							}
						}
						__except (EXCEPTION_EXECUTE_HANDLER) {
							KeUnstackDetachProcess(&state);
						}

						ExFreePool(Buffer);
						ObfDereferenceObject(target);
					}
				}
				Status = STATUS_SUCCESS;
				BytesIO = sizeof(WRITE_PROTECTED_VIRTUAL_MEMORY);
				Printf("WRITE_PROTECTED_VIRTUAL_MEMORY_IOCTL");
				Printf("ProcessId %d", UserlandBuffer->ProcessId);
				Printf("Address %llu", UserlandBuffer->Address); 
				Printf("Size %d", UserlandBuffer->Size);
			}
			*/
			else if (Buffer->ControlCode == VIRTUAL_PROTECT_IOCTL)
			{
			VIRTUAL_PROTECT* UserlandBuffer = (VIRTUAL_PROTECT*)irp->AssociatedIrp.SystemBuffer;

			// Acquire target process handle.
			PEPROCESS target = nullptr;
			auto status = PsLookupProcessByProcessId((HANDLE)UserlandBuffer->ProcessId, &target);

			if (NT_SUCCESS(status)) {
				KAPC_STATE apc;
				ULONG old_protection;
				KeStackAttachProcess(target, &apc);
				auto addr = (PVOID)UserlandBuffer->addr;
				auto size = (size_t)UserlandBuffer->size;
				status = ZwProtectVirtualMemory(ZwCurrentProcess(), &addr, &size, UserlandBuffer->protect, &old_protection);
				if (NT_SUCCESS(status)) {
					Printf("ZwProtectVirtualMemory success");
				}
				else {
					Printf("ZwProtectVirtualMemory failed");
				}
				UserlandBuffer->size = (int)size;
				UserlandBuffer->addr = (DWORD)addr;
				KeUnstackDetachProcess(&apc);
				UserlandBuffer->protect = old_protection;
				ObfDereferenceObject(target);
			}
			else {
				Printf("PsLookupProcessByProcessId failed");
			}

			//Status = STATUS_SUCCESS;
			BytesIO = sizeof(VIRTUAL_PROTECT);
			Printf("VIRTUAL_PROTECT_IOCTL");
			Printf("ProcessId %d", UserlandBuffer->ProcessId);
			Printf("Address %ld", UserlandBuffer->addr);
			Printf("size %d", UserlandBuffer->size);
			Printf("old_protection %ld", UserlandBuffer->protect);
			}

			///////////////////////////////////////////
			//ROAD TO BSOD. HAHAHA
			///////////////////////////////////////////
			/*
			else if (Buffer->ControlCode == FIND_PATTERN_PROCESS_MEMORY_IOCTL) {

				FIND_PATTERN_PROCESS_MEMORY* UserlandBuffer = (FIND_PATTERN_PROCESS_MEMORY*)irp->AssociatedIrp.SystemBuffer;

				MEMORY_BASIC_INFORMATION mbi = { 0 };

				unsigned __int64 mem_region_start = 0;
				unsigned __int64 mem_region_end = 0;

				mem_region_start = UserlandBuffer->StartAddress;
				mem_region_end = UserlandBuffer->EndAddress;

				Printf("[+] Scanning process from: 0x%016llx - 0x%016llx", mem_region_start, mem_region_end);

				unsigned __int64 potential_ptr = 0;

				do {
					if (NT_SUCCESS(ZwQueryVirtualMemory((HANDLE)UserlandBuffer->ProcessId, (PVOID)mem_region_start, MemoryBasicInformation , &mbi, sizeof(MEMORY_BASIC_INFORMATION), NULL))) {
						Printf("[+] Scanning page: 0x%016llx", mem_region_start);
						if (mbi.State == MEM_COMMIT || mbi.State == MEM_MAPPED && ((mbi.Protect & PAGE_GUARD) == 0) && ((mbi.Protect & PAGE_NOACCESS) == 0)) {
							bool is_writeable = (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_WRITECOPY);
							if (is_writeable) {
								//setup dump page
								unsigned char* dump = new unsigned char[mbi.RegionSize + 1];
								memset(dump, 0x00, mbi.RegionSize + 1);

								//dump memory
								PEPROCESS Process;
								if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)UserlandBuffer->ProcessId, &Process))) {
									if (!NT_SUCCESS(MmCopyVirtualMemory(Process, mbi.BaseAddress, PsGetCurrentProcess(), dump, mbi.RegionSize, KernelMode, NULL))) {
										Printf("  [-] Dumping page failed!");
										mem_region_start += mbi.RegionSize;
										delete[] dump;
										continue;
									}

									auto potentialptr = scanPattern((char*)dump, mbi.RegionSize, UserlandBuffer->Pattern, UserlandBuffer->Mask);

									if (potentialptr) {

										Printf("   [+] potential Offset found at dump_offset:: 0x%016llx", (potentialptr - (uintptr_t)dump));
										potential_ptr = (unsigned __int64)((uintptr_t)mbi.BaseAddress + (potentialptr - (uintptr_t)dump));
										delete[] dump;
										break;
									}
								}

								//cuz we are clean boyz
								delete[] dump;
							}
						}
						mem_region_start += mbi.RegionSize;
					}
					else {
						Printf("[-] VirtualQueryEx failed!");
						break;
					}
				} while (mem_region_start <= mem_region_end);

				UserlandBuffer->OutBuffer = potential_ptr;

				Status = STATUS_SUCCESS;
				BytesIO = sizeof(FIND_PATTERN_PROCESS_MEMORY);

			}
			*/
			irp->IoStatus.Status = Status;
			irp->IoStatus.Information = BytesIO;

			IofCompleteRequest(irp, IO_NO_INCREMENT);
			return Status;
		}
	}

	return ACPIOriginalDispatch(device, irp);
}

NTSTATUS DriverEntry(PVOID lpBaseAddress, DWORD32 dwSize)
{
	RetrieveMmUnloadedDriversData();
	ClearPiDDBCacheTable();

	UNICODE_STRING iqvw64e = RTL_CONSTANT_STRING(L"iqvw64e.sys");
	ClearMmUnloadedDrivers(&iqvw64e, true);

	PDRIVER_OBJECT ACPIDriverObject = nullptr;

	UNICODE_STRING DriverObjectName = RTL_CONSTANT_STRING(L"\\Driver\\ACPI");
	ObReferenceObjectByName(&DriverObjectName, OBJ_CASE_INSENSITIVE, 0, 0, *IoDriverObjectType, KernelMode, 0, (PVOID*)&ACPIDriverObject);

	if (ACPIDriverObject)
	{
		ACPIOriginalDispatch = ACPIDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL];

		ULONG64 DispatchHookAddr = (ULONG64)DispatchHook;

		*(ULONG64*)(DispatchHookAddr + 0x6) = (ULONG64)CustomDispatch;

		ULONG64 TraceMessageHookInst = FindPattern((UINT64)ACPIDriverObject->DriverStart, ACPIDriverObject->DriverSize, (BYTE*)"\xB8\x0C\x00\x00\x00\x44\x0F\xB7\xC8\x8D\x50\x00", "xxxxxxxxxxx?");

		if (TraceMessageHookInst)
		{
			TraceMessageHookInst += 0xC;

			ULONG64 pfnWppTraceMessagePtr = (ULONG64)ResolveRelativeAddress((PVOID)TraceMessageHookInst, 3, 7);

			if (pfnWppTraceMessagePtr)
			{
				*(ULONG64*)(pfnWppTraceMessagePtr) = DispatchHookAddr;

				ACPIDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)TraceMessageHookInst;

				Printf("ACAPI IRP_MJ_DEVICE_CONTROL Hooked!\n");
			}
		}
	}
	return STATUS_SUCCESS;
}
