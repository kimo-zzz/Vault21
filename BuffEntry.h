#pragma once
#include "stdafx.h"
#include "Offsets.h"

enum BuffType{
	Internal = 0,
	Aura = 1,
	CombatEnchancer = 2,
	CombatDehancer = 3,
	SpellShield = 4,
	Stun = 5,
	Invisibility = 6,
	Silence = 7,
	Taunt = 8,
	Polymorph = 9,
	Slow = 10,
	Snare = 11,
	Damage = 12,
	Heal = 13,
	Haste = 14,
	SpellImmunity = 15,
	PhysicalImmunity = 16,
	Invulnerability = 17,
	Sleepp = 18,
	NearSight = 19,
	Frenzy = 20,
	Fear = 21,
	Charm = 22,
	Poison = 23,
	Suppression = 24,
	Blind = 25,
	Counter = 26,
	Shred = 27,
	Flee = 28,
	Knockup = 29,
	Knockback = 30,
	Disarm = 31
};
class BuffEntry
{
public:
	DWORD strptr;

	BYTE getBuffType() {
		return *(BYTE*)((DWORD)this + oBuffType);
	}
	bool isBuffEmpty() {
		return strptr == 0;
	}
	bool IsAlive();

	bool IsValid() {
		if (this == NULL || (DWORD)this <= 0x1000)
			return false;

		return strcmp(GetBuffName(), "NULL") && GetBuffCountAlt() > 0;
	}

	float GetBuffStartTime() {
		return *(float*)((DWORD)this + O_BUFFMGR_STARTTIME);
	}

	float GetBuffEndTime() {
		return *(float*)((DWORD)this + O_BUFFMGR_ENDTIME);
	}

	int GetBuffCountAlt() {
		return (*(int*)((DWORD)this + oBuffCountAlt) - *(int*)((DWORD)this + oBuffCountAlt2)) >> 3;
	}

	float GetBuffCountFloat() {
		return *(float*)((DWORD)this + O_BUFFMGR_flBUFFCOUNT);
	}

	int GetBuffCountInt() {
		return *(int*)((DWORD)this + O_BUFFMGR_iBUFFCOUNT);
	}

	char* GetBuffName() {
		DWORD aux = *(DWORD*)((DWORD)this + O_BUFFMGR_BUFFNAME);
		if (aux == NULL)
			return "NULL";

		if (*(DWORD*)(aux + O_BUFFMGR_BUFFNAME) == NULL)
			return "NULL";

		return (char*)(aux + O_BUFFMGR_BUFFNAME);
	}
	
};

