#pragma once

#include "Offsets.h"
#include "BuffScript.h"
#include "BuffScriptInstance.h"

namespace HACKUZAN
{
	enum class BuffType
	{
		Internal,
		Aura,
		CombatEnchancer,
		CombatDehancer,
		SpellShield,
		Stun,
		Invisibility,
		Silence,
		Taunt,
		Polymorph,
		Slow,
		Snare,
		Damage,
		Heal,
		Haste,
		SpellImmunity,
		PhysicalImmunity,
		Invulnerability,
		AttackSpeedSlow,
		NearSight,
		Currency,
		Fear,
		Charm,
		Poison,
		Suppression,
		Blind,
		Counter,
		Shred,
		Flee,
		Knockup,
		Knockback,
		Disarm,
		Grounded,
		Drowsy,
		Asleep,
	};

	class BuffInstance
	{
	public:
		union
		{
			DEFINE_MEMBER_N(BuffType Type, Offsets::BuffInstance::Type)
			DEFINE_MEMBER_N(BuffScript* Script, Offsets::BuffInstance::Script)
			DEFINE_MEMBER_N(float StartTime, Offsets::BuffInstance::StartTime)
			DEFINE_MEMBER_N(float ExpireTime, Offsets::BuffInstance::ExpireTime)
			DEFINE_MEMBER_N(std::vector<DWORD64> ScriptInfo, Offsets::BuffInstance::ScriptInfo)
			DEFINE_MEMBER_N(unsigned int Counter, Offsets::BuffInstance::Counter)
			DEFINE_MEMBER_N(bool IsPermanent, Offsets::BuffInstance::IsPermanent)
		};

		bool IsActive();
		unsigned int GetCount();
		BuffScriptInstance* GetScriptInstance();
	};
}
