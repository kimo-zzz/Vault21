#include "stdafx.h"
#include "makesyscall.h"
#include <winternl.h>
#define JMP_64_SIZE            14
#define JMP_32_SIZE            5

#define X86_NOP                0x90
#define X86_JMP_NEG_5          0xF9EB

class protect_guard
{
private:
	DWORD protect_val;
	size_t size;
	LPVOID addr;
public:
	inline protect_guard(uintptr_t address, size_t size)
		: addr(LPVOID(address)), size(size)
	{
		/*if (VirtualProtect(addr,
			size,
			PAGE_EXECUTE_READWRITE,
			&protect_val) == FALSE)
		{
			// Throw Error
			Instalog::SystemFacades::Win32Exception::ThrowFromLastError();
		}*/

		if (NT_SUCCESS(makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(GetCurrentProcess(), &addr, &size, PAGE_EXECUTE_READWRITE, &protect_val))) {
		}
		else {
			// Throw Error
			Instalog::SystemFacades::Win32Exception::ThrowFromLastError();
		}

	}
	inline ~protect_guard()
	{
		/*if (VirtualProtect(addr,
			size,
			protect_val,
			&protect_val) == FALSE)
		{
			// Throw Error
			Instalog::SystemFacades::Win32Exception::ThrowFromLastError();
		}*/

		if (NT_SUCCESS(makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(GetCurrentProcess(), &addr, &size, protect_val, &protect_val))) {
		}
		else {
			// Throw Error
			Instalog::SystemFacades::Win32Exception::ThrowFromLastError();
		}
	}
};

void FuncHook::rehook32(intptr_t offset)
{
	register protect_guard guard(func_addr - JMP_32_SIZE, JMP_32_SIZE * 2);

	if (!hooked)
	{
		memcpy(unhook_data, LPVOID(func_addr), JMP_32_SIZE);
	}

	if (!started)
	{
		register uint8_t* p = (uint8_t*)func_addr - JMP_32_SIZE;
		register size_t nop_count = 0;

		for (size_t i = 0; i < JMP_32_SIZE; ++i)
		{
			if (p[i] == X86_NOP)
			{
				nop_count++;
			}
		}

		if (nop_count == JMP_32_SIZE
			&& p[5] == 0x8B && p[6] == 0xFF)
		{
			call_addr = (FARPROC)(func_addr + 2);
			type = HOOKTYPE_REVERSE_CHAIN;
			hooked = true;

			p[0] = 0xE9; // jmp
			*((uint32_t*)&p[1]) = (uint32_t)(hook_addr - func_addr);
			*((uint16_t*)&p[5]) = X86_JMP_NEG_5;
		}
		else if (p[0] == 0xE9
			&& *(uint16_t*)&p[5] == X86_JMP_NEG_5)
		{
			if (type == HOOKTYPE_FORWARD_OVERWRITE)
			{

				call_addr = (FARPROC)(func_addr + *((int32_t*)&p[1]));
				type = HOOKTYPE_REVERSE_CHAIN;
				hooked = true;

				*((uint32_t*)&p[1]) = (uint32_t)(hook_addr - func_addr);
			}
		}
		else if (p[5] == 0xE9)
		{
			int32_t cur_offset = *(int32_t*)&p[6];

			if (type == HOOKTYPE_FORWARD_OVERWRITE)
			{
				call_addr = (FARPROC)(func_addr + JMP_32_SIZE + cur_offset);
				type = HOOKTYPE_FORWARD_CHAIN;
				hooked = true;

				*((int32_t*)&p[6]) = (int32_t)offset;
			}
		}
		else if (type != HOOKTYPE_FORWARD_OVERWRITE)
		{
			type = HOOKTYPE_FORWARD_OVERWRITE;
		}

		started = true;
	}
	if (type == HOOKTYPE_FORWARD_OVERWRITE)
	{
		uint8_t *ptr = (uint8_t*)func_addr;

		call_addr = (FARPROC)func_addr;
		type = HOOKTYPE_FORWARD_OVERWRITE;
		hooked = true;

		*(ptr++) = 0xE9;
		*((int32_t*)ptr) = (int32_t)offset;
	}
}

#ifdef _WIN64

static const uint8_t longjmp64[6] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };

void FuncHook::rehook64(intptr_t offset)
{
	register uint8_t data[JMP_64_SIZE];
	uintptr_t *ptr_loc = (uintptr_t*)((uint8_t*)data + sizeof(longjmp64));
	
	register protect_guard guard(func_addr, JMP_64_SIZE);
	memcpy(data, (void*)func_addr, JMP_64_SIZE);
	memcpy(data, longjmp64, sizeof(longjmp64));
	*ptr_loc = hook_addr;

	call_addr = (FARPROC)func_addr;
	type = HOOKTYPE_FORWARD_OVERWRITE;
	hooked = true;

	memcpy((void*)func_addr, data, JMP_64_SIZE);
}
#endif // _WIN64

void FuncHook::hook()
{
	if (hooked)
		return;

	if (func_addr == NULL)
	{
		throw std::runtime_error("Cannot hook null function!");
	}
	else if (hook_addr == NULL)
	{
		throw std::runtime_error("Cannot hook a function with a null function!");
	}


	if (started)
	{
		uintptr_t addr = func_addr;
		size_t size = is_64bit_jump ? JMP_64_SIZE : JMP_32_SIZE;

		if (type == HOOKTYPE_REVERSE_CHAIN)
		{
			addr -= JMP_32_SIZE;
			size = JMP_32_SIZE;
		}

		register protect_guard guard(addr, size);
		memcpy((void*)addr, rehook_data, size);
		hooked = true;
		return;
	}

	intptr_t offset = hook_addr - func_addr - JMP_32_SIZE;

#ifdef _WIN64
	is_64bit_jump = llabs(offset) > 0x7fffffff;

	if (is_64bit_jump)
	{
		rehook64(offset);
	}
	else
#endif
	{
		rehook32(offset);
	}
}

void FuncHook::unhook()
{
	uintptr_t addr;
	size_t size;

	if (!hooked)
		return;

	if (type == HOOKTYPE_REVERSE_CHAIN) {
		size = JMP_32_SIZE * 2;
		addr = (func_addr - JMP_32_SIZE);
	}
	else {
		size = is_64bit_jump ? JMP_64_SIZE : JMP_32_SIZE;
		addr = func_addr;
	}

	register protect_guard guard(addr, size);

	memcpy(rehook_data, (void*)addr, size);

	if (type == HOOKTYPE_FORWARD_OVERWRITE)
		memcpy((void*)func_addr, unhook_data, size);

	hooked = false;
}