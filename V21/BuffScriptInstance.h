#pragma once

#include "Offsets.h"

namespace HACKUZAN
{
	class BuffScriptInstance
	{
	public:
		union
		{
			DEFINE_MEMBER_N(unsigned short CasterId, Offsets::BuffScriptInstance::CasterId)
		};
	};
}