#pragma once

enum HookType {
	HOOKTYPE_FORWARD_OVERWRITE,
	HOOKTYPE_FORWARD_CHAIN,
	HOOKTYPE_REVERSE_CHAIN
};

class FuncHook
{
private:
	FARPROC call_addr;
	uintptr_t func_addr;
	uintptr_t hook_addr;
	std::atomic_bool hooked, started;
	bool is_64bit_jump;
	HookType type;
	uint8_t unhook_data[14];
	uint8_t rehook_data[14];

	void rehook32(intptr_t offset);
#ifdef _WIN64
	void rehook64(intptr_t offset);
#endif // _WIN64
public:
	inline FuncHook(HookType type = HOOKTYPE_FORWARD_OVERWRITE)
		: hooked(false), started(false), func_addr(NULL), hook_addr(NULL), is_64bit_jump(false), type(type)
	{
	}

	inline FuncHook(uintptr_t func_address, uintptr_t hook_address, HookType type = HOOKTYPE_FORWARD_OVERWRITE)
		: hooked(false), started(false), func_addr(func_address), hook_addr(hook_address), is_64bit_jump(false), type(type)
	{
	}

	inline FuncHook(void* func_address, void* hook_address, HookType type = HOOKTYPE_FORWARD_OVERWRITE)
		: FuncHook((uintptr_t)func_address, (uintptr_t)hook_address, type)
	{
	}

	FuncHook& operator=(const FuncHook& lhs)
	{
		this->call_addr = lhs.call_addr;
		this->func_addr = lhs.func_addr;
		this->hook_addr = lhs.hook_addr;
		this->hooked = lhs.hooked.load();
		this->started = lhs.started.load();
		this->is_64bit_jump = lhs.is_64bit_jump;
		this->type = lhs.type;
		memcpy(unhook_data, lhs.unhook_data, sizeof(unhook_data));
		memcpy(rehook_data, lhs.rehook_data, sizeof(rehook_data));
		return *this;
	}

	inline ~FuncHook()
	{
		if (hooked)
			unhook();
	}

	void hook();
	void unhook();

	template<typename Result, typename... Args>
	inline Result WINAPI Call(Args... args);
};

class hook_guard
{
private:
	FuncHook& hook;
public:
	inline hook_guard(FuncHook& func_hook)
		: hook(func_hook)
	{
		hook.unhook();
	}
	inline ~hook_guard()
	{
		hook.hook();
	}
};

template<typename Result, typename... Args>
inline Result WINAPI FuncHook::Call(Args... args)
{
	hook_guard hg(*this);

	typedef Result (WINAPI *FunctionType)(Args... args);

	return (Result) ((FunctionType)call_addr)(args...);
}


template<typename Result, typename... Args>
inline Result WINAPI ModuleCall(HMODULE module, LPCSTR procName, Args... args)
{
	typedef Result(WINAPI *FunctionType)(Args... args);

	FARPROC proc = GetProcAddress(module, procName);
	if (proc == nullptr)
	{
		throw Instalog::SystemFacades::Win32Exception::FromLastError();
	}

	return (Result)((FunctionType)proc)(args...);
}