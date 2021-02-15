#pragma once

#include "GameObject.h"

namespace V21
{
	namespace SDK
	{
		enum kDamageType
		{
			DamageType_Physical,
			DamageType_Magical,
			DamageType_True,
			DamageType_Adaptive
		};

		struct DamageModifierResult {
			float Flat = 0.0f;
			float Percent = 0.0f;
		};

		struct DamageOnHitResult {
			float PhysicalDamage = 0.0f;
			float MagicalDamage = 0.0f;
			float TrueDamage = 0.0f;
		};

		namespace Damage
		{


			float CalculateAutoAttackDamage(GameObject* source, GameObject* target);
			float CalculatePhysicalDamage(GameObject* source, GameObject* target, float amount);
			float CalculateMagicalDamage(GameObject* source, GameObject* target, float amount);
			DamageModifierResult ComputeDamageModifier(GameObject* source, GameObject* target, kDamageType damageType);
			DamageOnHitResult ComputeDamageOnHit(GameObject* source, GameObject* target);
		};
	}
}