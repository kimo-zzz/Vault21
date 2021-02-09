#pragma once

#include "GameObject.h"
#include "Damage.h"

namespace HACKUZAN {
	namespace SDK {
		enum class TargetSelectorMode {
			Auto,
			LeastHealth,
			MostAttackDamage,
			MostAbilityPower,
			Closest,
			HighestPriority,
			LessAttack,
			LessCast,
			NearMouse
		};

		enum class TargetType {
			TSTARGET_HEROES,
			TSTARGET_MINION
		};

		namespace TargetSelector {
			extern std::vector<GameObject*> Enemies;
			extern std::unordered_map<Character, unsigned int> Priorities;
			extern TargetSelectorMode ActiveMode;

			extern GameObject* SelectedTarget;

			void Initialize();
			void Dispose();

			void OnDraw();
			void OnWndProc(UINT msg, WPARAM wparam);

			float GetReducedPriority(GameObject* hero);
			int GetPriority(GameObject* hero);

			GameObject* GetTarget(TargetType tsType, float range, kDamageType damageType, Vector3 source = Vector3(0, 0, 0), bool addBoundingRadius = false);
			GameObject* GetTarget(std::vector<GameObject*> targets, kDamageType damageType);
		};
	}
}