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

namespace V21 {
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

			void OnGameUpdate(); //OnPresent
			void OnDraw(); //OnPresent

			void OnCreateMissile(GameObject* unit);

			void OnDeleteMissile(GameObject* unit);

			void OnCreateObject(GameObject* unit);

			void OnDeleteObject(GameObject* unit);

			void OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3* position, GameObject* target);

			void OnCastSpell(SpellbookClient* spellbook, SpellDataInst* pSpellInfo, kSpellSlot slot, Vector3* _end_position, Vector3* _start_position, DWORD netId);

			void OnSpellCast(kSpellSlot slot);

			void OnDoCast(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnDoCastDelayed(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnPlayAnimation(GameObject* ptr, char* name, float animationTime);

			void OnFinishCast(SpellCastInfo* castInfo, GameObject* object);

			void OnStopCast(GameObject* unit, StopCast args);

			void OnGapCloserSpell(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnInterruptibleSpell(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnNewPath(NewPath* args);
		}
	}
}