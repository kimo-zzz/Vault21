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

		namespace Xerath { //change champ name

			void Initialize();
			void Dispose();

#pragma region callbacks
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

			GameObject* GetTarget(float radius);

			bool IsEnemyInQRange();
#pragma endregion callbacks

#pragma region Logics
			namespace Logics
			{
				void ComboLogic();
				void FarmLogic();
				void JungleLogic();
			}

#pragma endregion Logic
		}
	}
}