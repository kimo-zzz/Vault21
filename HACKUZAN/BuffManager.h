#pragma once

#include "BuffInstance.h"

namespace HACKUZAN
{
	class BuffManager
	{
	public:
		union
		{
			DEFINE_MEMBER_0(BuffInstance* Buff)
		};
		BYTE pad[0x4];
	};
}
