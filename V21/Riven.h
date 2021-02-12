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

namespace HACKUZAN
{
	namespace Plugins
	{
		namespace Riven
		{
			void Initialize();
			void Dispose();
			void OnGameUpdate();
			void OnDraw();
			bool OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position);
			void OnCreateObject(GameObject* unit);
			void OnDeleteObject(GameObject* unit);
			void OnFinishCast(GameObject* object, SpellCastInfo* castInfo);
			void OnStopCast(GameObject* caster, StopCast* args);
			void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);
			GameObject* Target();
		}
	}
}