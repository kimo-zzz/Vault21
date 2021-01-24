#pragma once
#include "stdafx.h"

class SpellData
{
public:
	char* GetMissileName()
	{
		return GetStr((DWORD)this + oSpellDataMissileName);
	}

	char* GetSpellName()
	{
		return GetStr((DWORD)this + oSpellDataSpellName);
	}

	char* GetSpellName2()
	{
		return GetStr2((DWORD)this + oSpellDataSpellName);
	}

	char* GetDescription()
	{
		return GetStr((DWORD)this + oSpellDataDescription);
	}

	float GetEffectAmount()
	{
		return *(float*)((DWORD)this + oSpellDataEffectAmount);
	}

	float GetIncreaseDamage()
	{
		return *(float*)((DWORD)this + oSpellDataIncreaseDamage);
	}

	float GetSpellDuration()
	{
		return *(float*)((DWORD)this + oSpellDataSpellDuration);
	}

	float GetRootDuration()
	{
		return *(float*)((DWORD)this + oSpellDataRootDuration);
	}

	float GetIncreaseDamageBonus()
	{
		return *(float*)((DWORD)this + oSpellDataIncreaseDamageBonus);
	}

	float GetCoefficient()
	{
		return *(float*)((DWORD)this + oSpellDataCoefficient);
	}

	float GetCoefficient2()
	{
		return *(float*)((DWORD)this + oSpellDataCoefficient2);
	}

	int GetMaxHighlightTargets()
	{
		return *(int*)((DWORD)this + oSpellDataMaxHighlightTargets);
	}

	float GetCooldownTime()
	{
		return *(float*)((DWORD)this + oSpellDataCooldownTime);
	}

	float GetDelayCastOffsetPercent()
	{
		return *(float*)((DWORD)this + oSpellDataDelayCastOffsetPercent);
	}

	float GetDelayTotalTimePercent()
	{
		return *(float*)((DWORD)this + oSpellDataDelayTotalTimePercent);
	}

	int GetMaxAmmo()
	{
		return *(int*)((DWORD)this + oSpellDataMaxAmmo);
	}

	int GetAmmoUsed()
	{
		return *(int*)((DWORD)this + oSpellDataAmmoUsed);
	}

	float GetAmmoRechargeTime()
	{
		return *(float*)((DWORD)this + oSpellDataAmmoRechargetime);
	}

	float GetMissileSpeed()
	{
		return *(float*)((DWORD)this + oSpellDataMissileSpeed);
	}

	float GetSpellWidth()
	{
		return *(float*)(DWORD)(this + oSpellDataSpellWidth);
	}

	float GetSpellRange(DWORD lvl)
	{
		return *(float*)(DWORD)(this + (oSpellDataSpellRange + (0x4 * lvl)));
	}

	float GetSpellEffectRange()
	{
		return *(float*)(DWORD)(this + oSpellDataEffectSpellRange);
	}

	float GetSpellEffectRangeAfterEffect()
	{
		return *(float*)(DWORD)(this + oSpellDataEffectSpellRangeAfterEffect);
	}

	float GetSpellEffectRangeAbs()
	{
		if (GetSpellEffectRange() > GetSpellEffectRangeAfterEffect())
			return GetSpellEffectRange();

		return GetSpellEffectRangeAfterEffect();
	}
};
