#include "SpellInfo.h"
class SpellSlot {
public:
	int GetRemainingCharge() {
		return *(int*)((DWORD)this + oSpellSlotRemainingCharge);
	}

	int GetLevel() {
		return *(int*)((DWORD)this + oSpellSlotLevel);
	}

	float GetRemainingRechargeCD(float gameTime) {
		return *(float*)((DWORD)this + oSpellSlotRemainingRechargeCD) - gameTime;
	}

	int GetAbsoluteCoolDown(float gameTime) {
		return GetRemainingCharge() == 0 ? GetRemainingRechargeCD(gameTime) : GetRemainingCD(gameTime);
	
	}

	bool IsDoneAbsoluteCD(float gameTime) {
		return GetAbsoluteCoolDown(gameTime) <= 0;
	}

	float GetTime() {
		return *(float*)((DWORD)this + oSpellSlotTime);
	}

	float GetCD() {
		return *(float*)((DWORD)this + oSpellSlotCD);
	}

	bool IsReady() {
		if (GetSpellInfo()->GetSpellData()->GetCooldownTime() == 0) return true;
		return false;
	}

	float GetDamage() {
		return *(float*)((DWORD)this + oSpellSlotDamage);
	}

	SpellInfo* GetSpellInfo() {
		return *(SpellInfo**)((DWORD)this + oSpellSlotSpellInfo);
	}

	float GetRemainingCD(float gameTime) {
		return *(float*)((DWORD)this + oSpellSlotRemainingCD) - gameTime;
	}

	bool IsDoneCD(float gameTime) {
		return GetRemainingCD(gameTime) <= 0 && GetLevel() >= 1;
	}

};