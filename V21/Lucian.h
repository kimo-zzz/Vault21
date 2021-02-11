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

		namespace Lucian { //change champ name

			void Initialize();
			void Dispose();

			void OnGameUpdate();
			void OnDraw();
			void OnCreateObject(GameObject* unit);
			void OnDeleteObject(GameObject* unit);
			void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);
			void OnPlayAnimation(GameObject* ptr, char* name, float animationTime);
			void OnFinishCast(SpellCastInfo* castInfo, GameObject* object);
			void OnStopCast(SpellCastInfo* spellCaster_Client, bool stopAnimation, bool* executeCastFrame,
				bool forceStop, bool destroyMissile, unsigned int missileNetworkID);
			void OnNewPath(GameObject* obj, Vector3* start, Vector3* end, Vector3* tail, float* dashSpeed, unsigned dash);
			bool OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position);
			bool HasDoubleshotPassive();
			GameObject* GetTarget(float radius);

			void Combo();
			void Farm();
			void Lasthit();
			void Jungle();
		}
	}
}