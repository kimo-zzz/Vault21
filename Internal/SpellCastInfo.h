#pragma once


class SpellCastInfo
{
public:
	SpellInfo* GetSpellInfo() {
		return (SpellInfo*)((DWORD)this + oActive_Spell_Instance);
	}

	uint32_t GetSpellSlot() {
		return (uint32_t)((DWORD)this + oActive_Spell_SpellSlot);
	}

	int GetMissileIndex() {
		return (int)((DWORD)this + oActive_Spell_MissleIndex);
	}

	char* GetCasterName() {
		return (char*)((DWORD)this + oActive_Spell_CasterName);
	}

	uint16_t GetSenderIndex() {
		return (uint16_t)((DWORD)this + oActive_Spell_SenderIndex);
	}

	Vector GetSpellStartPos()
	{
		return *(Vector*)((DWORD)this + oActive_Spell_SpellStartPosition);
	}

	Vector GetSpellEndPos()
	{
		return *(Vector*)((DWORD)this + oActive_Spell_SpellEndPosition);
	}

	Vector GetSpellCastPos()
	{
		return *(Vector*)((DWORD)this + oActive_Spell_CastPos);
	}

	uint16_t GetTargetIndex() {
		return (uint16_t)((DWORD)this + oActive_Spell_TargetIndex);
	}

	float GetWindupTime() {
		return (float)((DWORD)this + oActive_Spell_WindupTime);
	}

	float GetCooldown() {
		return (float)((DWORD)this + oActive_Spell_Cooldown);
	}

	bool IsSpell() {
		return (bool)((DWORD)this + oActive_Spell_Spell);
	}

	bool IsBasicAttack() {
		return (uint32_t)((DWORD)this + oActive_Spell_IsBasicAttack) != 0;
	}

	bool IsSpecialAttack() {
		return (uint32_t)((DWORD)this + oActive_Spell_IsSpecialAttack) != 0;
	}

	float ManaCost() {
		return (float)((DWORD)this + oActive_Spell_ManaCost);
	}

	float CastStartTime() {
		return (float)((DWORD)this + oActive_Spell_CastStartTime);
	}

	float CastEndTime() {
		return (float)((DWORD)this + oActive_Spell_CastEndTime);
	}


};