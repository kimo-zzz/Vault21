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

		namespace ChampionName { //change champ name

			void Initialize();
			void Dispose();

			void OnGameUpdate();
			void OnDraw();
			void OnCreateObject(GameObject* unit);
			void OnDeleteObject(GameObject* unit);
			void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);
			void OnPlayAnimation(GameObject* ptr);
			void OnFinishCast(GameObject* object, SpellCastInfo* castInfo);
			void OnStopCast(GameObject* caster, StopCast* args);
			void OnNewPath(NewPath* path);
			bool OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position);
			GameObject* GetTarget(float radius);
		}
	}
}