#pragma once

#include "Offsets.h"

namespace V21
{
	class Experience
	{
	public:
		union
		{
			DEFINE_MEMBER_N(unsigned int Level, Offsets::Experience::Level)
		};
	};
}