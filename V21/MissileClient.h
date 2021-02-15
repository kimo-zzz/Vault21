#pragma once

#include "Offsets.h"

#include "Vector3.h"

namespace V21
{

	class MissileClient
	{
	public:
		union
		{
			DEFINE_MEMBER_N(SpellData* MissileSpellCastInfo, Offsets::SpellData::Resource)
		};
	};
}