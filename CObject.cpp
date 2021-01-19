#include "stdafx.h"
#include "Hooks.h"

bool CObject::IsAlive() {
	return Functions.IsAlive(this) && this->GetHealth() > 0.0f;
}

bool CObject::IsMinion() {
	return Functions.IsMinion(this);
}

bool CObject::IsDragon() {
	return Functions.IsDragon(this);
}

bool CObject::IsTurret() {
	return Functions.IsTurret(this);
}

bool CObject::IsHero() {
	return CompareObjectTypeFlags((int)ObjectTypeFlags::Hero);
}

bool CObject::IsMissile() {
	return Functions.IsMissile(this);
}

bool CObject::IsTargetable() {
	return Functions.IsTargetable(this);
}

bool CObject::IsDeletedObject()
{
	return CompareObjectTypeFlags((int)ObjectTypeFlags::DeadObject);
}

bool CObject::IsInvalidObject()
{
	return CompareObjectTypeFlags((int)ObjectTypeFlags::InvalidObject);
}



bool CObject::IsNexus() {
	return Functions.IsNexus(this);
}

bool CObject::IsInhibitor() {
	return Functions.IsInhibitor(this);
}

bool CObject::IsTroyEnt() {
	return Functions.IsTroyEnt(this);
}

float CObject::GetAttackDelay()
{
	return Functions.GetAttackDelay(this);
}

float CObject::GetAttackCastDelay()
{
	return Functions.GetAttackCastDelay(this);
}


Vector CObject::GetHpBarPosition()
{
	Vector baseDrawPosition;
	Vector screen;
	Vector ret;

	Functions.GetBaseDrawPosition(this, &baseDrawPosition);
	Functions.WorldToScreen(&baseDrawPosition, &screen);
	float delta = abs(baseDrawPosition.Y - this->GetPos().Y);
	delta *= 5 / 6.0f;

	Functions.WorldToScreen(&baseDrawPosition, &ret);

	ret.X -= 60;
	ret.Y -= delta;
	ret.Z = 0;
	return ret;
}

CObject* CObject::GetOwner() {
	return Functions.GetObjectOwner(this);
}

bool CObject::is_lane_minion() {
	return reinterpret_cast<xor_value<bool>*>(std::uintptr_t(this) + IsLaneMinion)->decrypt();
}

character_data_stack* CObject::get_character_data_stack() {
	return reinterpret_cast<character_data_stack*>(std::uintptr_t(this) + CharacterDataStack);
}

