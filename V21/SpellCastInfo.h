#pragma once

#include "Offsets.h"
#include "SpellData.h"
#include "Vector3.h"

namespace HACKUZAN
{

	class SpellCastInfo
	{
	public:
		union
		{
			DEFINE_MEMBER_0(HACKUZAN::SpellData* BasicAttackSpellData)
			DEFINE_MEMBER_N(HACKUZAN::SpellData* SpellData, Offsets::SpellCastInfo::SpellInfo)
			DEFINE_MEMBER_N(unsigned int Level, Offsets::SpellCastInfo::Level)
			DEFINE_MEMBER_N(Vector3 StartPosition, Offsets::SpellCastInfo::StartPosition)
			DEFINE_MEMBER_N(Vector3 EndPosition, Offsets::SpellCastInfo::EndPosition)
			DEFINE_MEMBER_N(bool HasTarget, Offsets::SpellCastInfo::HasTarget)
			DEFINE_MEMBER_N(unsigned short SourceId, Offsets::SpellCastInfo::SourceId)
			DEFINE_MEMBER_N(unsigned short TargetId, Offsets::SpellCastInfo::TargetId)
			DEFINE_MEMBER_N(float CastDelay, Offsets::SpellCastInfo::CastDelay)
			DEFINE_MEMBER_N(float Delay, Offsets::SpellCastInfo::Delay)
			DEFINE_MEMBER_N(bool IsWindingUp, Offsets::SpellCastInfo::IsWindingUp)
			DEFINE_MEMBER_N(bool IsBasicAttack, Offsets::SpellCastInfo::IsBasicAttack)
			DEFINE_MEMBER_N(bool IsSpecialAttack, Offsets::SpellCastInfo::IsSpecialAttack)
			//DEFINE_MEMBER_N(kSpellSlot Slot, Offsets::SpellCastInfo::Slot)
			DEFINE_MEMBER_N(float StartTime, Offsets::SpellCastInfo::StartTime)
			DEFINE_MEMBER_N(float CastEndTime, Offsets::SpellCastInfo::CastEndTime)
			DEFINE_MEMBER_N(float EndTime, Offsets::SpellCastInfo::EndTime)
			DEFINE_MEMBER_N(bool SpellWasCast, Offsets::SpellCastInfo::SpellWasCast)
			DEFINE_MEMBER_N(bool IsStopped, Offsets::SpellCastInfo::IsStopped)
			DEFINE_MEMBER_N(bool IsInstantCast, Offsets::SpellCastInfo::IsInstantCast)
		};

		bool IsAutoAttack();
		bool IsChanneling();

		Vector3 GetSpellEndPos();
		Vector3 GetSpellStartPos();


		SpellDataResource* GetSpellData() {
			auto aux = *(DWORD*)((DWORD)this + 0x0);
			return *(SpellDataResource**)(aux + 0x44);
		}
	};
}