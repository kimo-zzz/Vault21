#pragma once

#include "Offsets.h"

namespace HACKUZAN
{
	class SpellDataScript
	{
	public:
		union
		{
			DEFINE_MEMBER_N(unsigned int Hash, Offsets::SpellDataScript::Hash)
		};
	};
}