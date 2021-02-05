#pragma once
#include "Orbwalker.h"
#include "HudManager.h"
#include "EventManager.h"
#include "ClockFacade.h"
#include "Renderer.h"
#include "Functions.h"

#define PI 3.14159265f

namespace HACKUZAN {
	namespace Plugins {
		namespace Evade {
			void Initialize();
			void Dispose();
			bool OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position);
			void OnCreateObject(GameObject* unit);
			void OnDeleteObject(GameObject* unit);
			void OnCreateMissile(GameObject* unit);
			void OnDeleteMissile(GameObject* unit);
			void OnProcessSpell(SpellInfo* unit, SpellDataResource* castInfo);
			void OnGameUpdate();
			void OnDraw();

			extern Vector3 RenderPos1;
			extern Vector3 RenderPos2;
			extern int LastEvadeTick;
			extern Vector2 EvadeWalkingPoint;
			extern float LastPolygonTick;
		}
	}
}