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
		namespace Activator {
			void Initialize();
			void Dispose();
			void OnCreateMissile(GameObject* unit);
			void OnDeleteMissile(GameObject* unit);
			void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);
			void OnGameUpdate();
			int CountEnemiesInRange(float range);

		}
	}
}