#pragma once

#include "Offsets.h"
#include "ItemData.h"

namespace HACKUZAN
{
	class ItemInfo
	{
	public:
		union
		{
			DEFINE_MEMBER_N(ItemData* ItemData, Offsets::ItemInfo::ItemData)
			DEFINE_MEMBER_N(unsigned char Ammo, Offsets::ItemInfo::Ammo)
		};
	};
}