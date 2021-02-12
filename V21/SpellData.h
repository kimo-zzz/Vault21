#pragma once
#include "Vector3.h"
#include "Offsets.h"
#include "SpellDataScript.h"
#include "SpellDataResource.h"

namespace V21
{
	class SpellData
	{
	public:

		enum kSpellSlot
		{
			SpellSlot_Unknown = -1,
			SpellSlot_Q,
			SpellSlot_W,
			SpellSlot_E,
			SpellSlot_R,
			SpellSlot_Summoner1,
			SpellSlot_Summoner2,
			SpellSlot_Item1,
			SpellSlot_Item2,
			SpellSlot_Item3,
			SpellSlot_Item4,
			SpellSlot_Item5,
			SpellSlot_Item6,
			SpellSlot_Trinket,
			SpellSlot_Recall,
			SpellSlot_SpecialAttack = 45,
			SpellSlot_BasicAttack = 65,
		};


		union
		{
			DEFINE_MEMBER_N(SpellDataScript* Script, Offsets::SpellData::Script)
			DEFINE_MEMBER_N(std::string Name, Offsets::SpellData::Name)
			DEFINE_MEMBER_N(SpellDataResource* Resource, Offsets::SpellData::Resource)
		};
	};
}