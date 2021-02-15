#include "stdafx.h"
#include "BuffScript.h"

namespace V21
{
	const char* BuffScript::GetDisplayName()
	{
		return reinterpret_cast<const char*(__thiscall*)(BuffScript*)>(this->VTable[static_cast<DWORD>(Offsets::BuffScript::Virtual_GetDisplayName)])(this);
	}
}