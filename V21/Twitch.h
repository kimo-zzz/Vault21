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

		namespace Twitch { //change champ name

			void Initialize();
			void Dispose();
			void OnGameUpdate();
			void OnDraw();
			bool OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position);
			void OnCreateObject(GameObject* unit);
			void OnDeleteObject(GameObject* unit);
			//void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);
			int CountEnemiesInRange(float range);
			float GetPoisonDmg(GameObject* target);
			bool IsKillable();
			GameObject* GetTarget(int radius);
			GameObject* GetLasthitTarget(int radius);
		}
	}
}