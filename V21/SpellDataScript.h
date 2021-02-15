#pragma once

#include "Offsets.h"

namespace V21
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