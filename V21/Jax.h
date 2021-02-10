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

namespace HACKUZAN {
	namespace Plugins {

		namespace Jax { //change champ name

			namespace Logics 
			{
				void executeSpell(kSpellSlot slot, Vector3 pos = Vector3(-1, -1, -1), GameObject* target = nullptr);
				GameObject* GetTarget(float range);

				namespace Combo
				{
					void ExecuteQ();
					void ExecuteW();
					void ExecuteE();
				}
			}

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
		}
	}
}