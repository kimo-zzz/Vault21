#pragma once
#include "ObjectManager.h"
#include "TargetSelector.h"
#include "Orbwalker.h"
#include "HudManager.h"
#include "EventManager.h"
#include "ClockFacade.h"
#include "Renderer.h"
#include "Functions.h"
#include "Menu.h"

#define PI 3.14159265f

namespace HACKUZAN {
	namespace Plugins {
		namespace Vayne {
			void Initialize();
			void Dispose();
			void OnGameUpdate();
			void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);
			int CountEnemiesInRange(float range);
			GameObject* GetTarget();
			inline Vector3 PredGetUnitPosition(GameObject* target, float delay);
			bool IsCondemnable(GameObject* target);

			extern bool LastAttackCastSpell;
		}
	}
}