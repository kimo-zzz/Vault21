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
				GameObject* GetMinionTarget(float range);

				namespace Farming
				{
					void ExecuteQ();
					void ExecuteW();
					void ExecuteE();
				}

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
			void OnPlayAnimation(GameObject* ptr);
			void OnFinishCast(GameObject* object, SpellCastInfo* castInfo);
			void OnStopCast(GameObject* caster, StopCast* args);
			void OnNewPath(NewPath* path);
			bool OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position);
		}
	}
}