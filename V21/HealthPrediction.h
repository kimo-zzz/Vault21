#pragma once

#include "GameObject.h"
#include "ManagerTemplate.h"

namespace V21 {
	namespace SDK {
		class IncomingAttack {
		public:
			GameObject* Source;
			GameObject* Target;
			Vector3 SourcePosition;
			float Delay;
			float AnimationTime;
			float MissileSpeed;
			float StartTime;
			float Damage;
			bool Processed;
			GameObjectCombatType CombatType;

			IncomingAttack(GameObject* source, GameObject* target, Vector3 sourcePosition, float Delay, float AnimationTime, float missileSpeed, float startTime, float damage, GameObjectCombatType type);
		};

		namespace HealthPrediction {
			extern std::vector<IncomingAttack> IncomingAttacks;

			void Initialize();
			void OnGameUpdate();
			void OnDeleteMissile(GameObject* unit);
			void OnDoCast(SpellInfo* castInfo, SpellDataResource* spellData);
			void OnProcessSpell(SpellInfo* unit, SpellDataResource* castInfo);
			void OnStopCast(GameObject* unit, StopCast* args);

			float GetHealthPrediction(GameObject* unit, int time, int delay);
			float LaneClearHealthPrediction(GameObject* unit, int time, int delay);
		}
	}
}