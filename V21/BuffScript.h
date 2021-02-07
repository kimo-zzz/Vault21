#pragma once

#include "Offsets.h"

namespace HACKUZAN
{

	class BuffScript
	{
	public:
		union
		{
			DEFINE_MEMBER_0(DWORD* VTable)
			DEFINE_MEMBER_N(char Name[128], Offsets::BuffScript::Name)
			DEFINE_MEMBER_N(unsigned int Hash, Offsets::BuffScript::Hash)
		};

		const char* GetDisplayName();
	};
}