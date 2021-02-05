#pragma once

#include "Offsets.h"
#include "InventorySlot.h"

namespace HACKUZAN
{
	class HeroInventoryCommon
	{
	public:
		union
		{
			DEFINE_MEMBER_N(InventorySlot* Slots[39], Offsets::HeroInventoryCommon::Slots)
			DEFINE_MEMBER_N(InventorySlot* Slots6, Offsets::HeroInventoryCommon::Slots6)
			DEFINE_MEMBER_N(InventorySlot* Slots7, Offsets::HeroInventoryCommon::Slots7)
			DEFINE_MEMBER_N(InventorySlot* Slots8, Offsets::HeroInventoryCommon::Slots8)
			DEFINE_MEMBER_N(InventorySlot* Slots9, Offsets::HeroInventoryCommon::Slots9)
			DEFINE_MEMBER_N(InventorySlot* Slots10, Offsets::HeroInventoryCommon::Slots10)
			DEFINE_MEMBER_N(InventorySlot* Slots11, Offsets::HeroInventoryCommon::Slots11)
			DEFINE_MEMBER_N(InventorySlot* Slots12, Offsets::HeroInventoryCommon::Slots12)
		};

		InventorySlot* GetItemSpell(kSpellSlot slot)
		{
			return *(InventorySlot**)((DWORD)this + (DWORD)Offsets::HeroInventoryCommon::Slots + (0x4 * slot));
		}
	};
}