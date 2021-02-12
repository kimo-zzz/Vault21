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

namespace V21 {
	namespace Plugins {

		namespace ChampionName { //change champ name

			void Initialize();
			void Dispose();

			void OnGameUpdate();
			void OnDraw();
			void OnCreateObject(GameObject* unit);
			void OnDeleteObject(GameObject* unit);
			void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);
			void OnPlayAnimation(GameObject* ptr);
			void OnFinishCast(SpellCastInfo* castInfo, GameObject* object);
			void OnStopCast(GameObject* unit, StopCast* args);
			void OnNewPath(NewPath* args);
			bool OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position);
			GameObject* GetTarget(float radius);
		}
	}
}