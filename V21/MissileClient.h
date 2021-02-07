#pragma once

#include "Offsets.h"

#include "Vector3.h"

namespace HACKUZAN
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