#pragma once

#include "Offsets.h"
#include "ItemInfo.h"

namespace HACKUZAN
{
	class InventorySlot
	{
	public:
		union
		{
			DEFINE_MEMBER_0(unsigned char Stacks)
			DEFINE_MEMBER_N(ItemInfo* ItemInfo, Offsets::InventorySlot::ItemInfo)
		};
	};
}