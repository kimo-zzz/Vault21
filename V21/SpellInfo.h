#pragma once

#include "Offsets.h"
#include "SpellData.h"
#include "Vector3.h"

namespace HACKUZAN
{
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
		SpellSlot_BasicAttack1 = 64,
		SpellSlot_BasicAttack2 = 65,
	};

	class SpellInfo
	{
	public:
		union
		{
				DEFINE_MEMBER_0(HACKUZAN::SpellData* BasicAttackSpellData)
				DEFINE_MEMBER_N(unsigned int Level, Offsets::SpellInfo::Level)
				DEFINE_MEMBER_N(Vector3 StartPosition, Offsets::SpellInfo::StartPosition)
				DEFINE_MEMBER_N(Vector3 EndPosition, Offsets::SpellInfo::EndPosition)
				DEFINE_MEMBER_N(bool HasTarget, Offsets::SpellInfo::HasTarget)
				DEFINE_MEMBER_N(unsigned int SoureNetworkId, Offsets::SpellInfo::SoureNetworkId)
				DEFINE_MEMBER_N(unsigned short SourceId, Offsets::SpellInfo::SourceId)
				DEFINE_MEMBER_N(unsigned short TargetId, Offsets::SpellInfo::TargetId)
				DEFINE_MEMBER_N(float CastDelay, Offsets::SpellInfo::CastDelay)
				DEFINE_MEMBER_N(float Delay, Offsets::SpellInfo::Delay)
				DEFINE_MEMBER_N(bool IsWindingUp, Offsets::SpellInfo::IsWindingUp)
				DEFINE_MEMBER_N(DWORD IsBasicAttack, Offsets::SpellInfo::IsBasicAttack)
				DEFINE_MEMBER_N(bool IsSpecialAttack, Offsets::SpellInfo::IsSpecialAttack)
				DEFINE_MEMBER_N(kSpellSlot Slot, Offsets::SpellInfo::Slot)
				DEFINE_MEMBER_N(float StartTime, Offsets::SpellInfo::StartTime)
				DEFINE_MEMBER_N(float CastEndTime, Offsets::SpellInfo::CastEndTime)
				DEFINE_MEMBER_N(float EndTime, Offsets::SpellInfo::EndTime)
				DEFINE_MEMBER_N(bool SpellWasCast, Offsets::SpellInfo::SpellWasCast)
				DEFINE_MEMBER_N(bool IsStopped, Offsets::SpellInfo::IsStopped)
				DEFINE_MEMBER_N(bool IsInstantCast, Offsets::SpellInfo::IsInstantCast)
		};

		bool IsAutoAttack();
		bool IsChanneling();


		SpellDataResource* GetSpellData() {
			auto aux = *(DWORD*)((DWORD)this + 0x0);
			return *(SpellDataResource**)(aux + 0x44);
		}
	};
}