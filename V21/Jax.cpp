#include "stdafx.h"
#include "PluginTemplate.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"
#include "Jax.h"

namespace HACKUZAN
{
	namespace Plugins 
	{
		using namespace HACKUZAN::SDK;
		using namespace HACKUZAN::SDK::Orbwalker;

		namespace Config
		{
			namespace Combo
			{
				CheckBox* UseQ;
				CheckBox* UseW;
				CheckBox* UseE;
				CheckBox* UseR;
			}

			namespace Drawings
			{
				CheckBox* DrawQ;
				CheckBox* DrawE;
				CheckBox* DrawFlash;
				CheckBox* DrawTrinket;
			}

			namespace Farming
			{
				CheckBox* UseQ;
				CheckBox* UseW;
				CheckBox* UseE;
			}

			namespace Misc 
			{
				CheckBox* FleeJump;
				KeyBind* WardJump;
			}
		}

		namespace Logics
		{
			void executeSpell(kSpellSlot slot, Vector3 pos = Vector3(-1, -1, -1), GameObject* target = nullptr)
			{
				if (target)
					ObjectManager::Player->CastTargetSpell(slot, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->Position, target->Position, target->NetworkId);
				else if ((pos.X != -1 && pos.Z != -1 && pos.Y != -1))
					ObjectManager::Player->CastSpellPos(slot, (DWORD)ObjectManager::Player, pos);
				else
					ObjectManager::Player->CastSpell(slot, (DWORD)ObjectManager::Player);
			}

			GameObject* GetTarget(float radius)
			{
				std::vector<GameObject*> heroes;
				auto hero_list = HACKUZAN::GameObject::GetHeroes();
				for (size_t i = 0; i < hero_list->size; i++)
				{
					auto hero = hero_list->entities[i];

					if (hero != nullptr && hero->IsEnemy() && hero->IsValidTarget(radius)) {
						heroes.push_back(hero);
					}
				}
				return TargetSelector::GetTarget(heroes, DamageType_Physical);
			}

			namespace Combo
			{
				bool ExecutedFirstCounterStrike = false;

				void ExecuteQ()
				{
					if (!Config::Combo::UseQ->Value && ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_Q) == kSpellState::SpellState_Ready)
						return;
					float range = ObjectManager::Player->Spellbook.GetSpell(SpellSlot_Q)->SpellData->Resource->Range;
					auto target = GetTarget(range);
					if (target && Distance(target, ObjectManager::Player) <= range)
						executeSpell(SpellSlot_Q, Vector3(0, 0, 0), target);
				}

				void ExecuteW()
				{
					if (!Config::Combo::UseW->Value && ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_W) == kSpellState::SpellState_Ready)
						return;
					float range = ObjectManager::Player->Spellbook.GetSpell(SpellSlot_W)->SpellData->Resource->Range;
					auto target = GetTarget(range);
					if (target && Distance(target, ObjectManager::Player) <= range)
						executeSpell(SpellSlot_W);
				}

				void ExecuteE()
				{
					if (!Config::Combo::UseW->Value && ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_E) == kSpellState::SpellState_Ready)
						return;

					auto target1 = GetTarget(ObjectManager::Player->Spellbook.GetSpell(SpellSlot_Q)->SpellData->Resource->Range);
					auto target2 = GetTarget(ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->Range);
					if ((!ExecutedFirstCounterStrike && ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_Q) == kSpellState::SpellState_Ready && target1) || (ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_Q) != kSpellState::SpellState_Ready && target2))
						executeSpell(SpellSlot_E);
				}
			}
		}

		void Jax::Initialize()
		{
			auto menu = Menu::CreateMenu("jax", "Jax");
			auto combo = menu->AddMenu("combo", "Combo Settings");
			auto farming = menu->AddMenu("farming", "Farming Settings");;
			auto drawing = menu->AddMenu("drawing", "Drawing Settings");;
			auto misc = menu->AddMenu("misc", "Misc Settings");

			Config::Combo::UseQ = combo->AddCheckBox("Jax Use Q Combo", "Use Q", true);
			Config::Combo::UseW = combo->AddCheckBox("Jax Use W Combo", "Use W", true);
			Config::Combo::UseE = combo->AddCheckBox("Jax Use E Combo", "Use E", true);
			Config::Combo::UseR = combo->AddCheckBox("Jax Use R Combo", "Use R", true);

			Config::Farming::UseQ = farming->AddCheckBox("Jax Use Q Farming", "Use Q", true);
			Config::Farming::UseW = farming->AddCheckBox("Jax Use W Farming", "Use W", true);
			Config::Farming::UseE = farming->AddCheckBox("Jax Use E Farming", "Use E", false);

			Config::Drawings::DrawQ = drawing->AddCheckBox("Jax Draw Q", "Draw Q Range", true);
			Config::Drawings::DrawE = drawing->AddCheckBox("Jax Draw E", "Draw E Range", true);
			Config::Drawings::DrawFlash = drawing->AddCheckBox("Jax Draw Flash", "Draw Flash Range", false);
			Config::Drawings::DrawTrinket = drawing->AddCheckBox("Jax Draw Trinket", "Draw Trinket Range", true);

			Config::Misc::FleeJump = misc->AddCheckBox("Jax Flee Jump", "Flee Jump", false);
			Config::Misc::FleeJump->AddTooltip("Allows the usage of Ward Jumps during Orbwalker Flee mode.");
			Config::Misc::WardJump = misc->AddKeyBind("Jax Ward Jump Key", "Ward Jump", 'n', false, false);


			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnPlayAnimation, OnPlayAnimation);
			EventManager::AddEventHandler(LeagueEvents::OnFinishCast, OnFinishCast);
			EventManager::AddEventHandler(LeagueEvents::OnStopCast, OnStopCast);
			EventManager::AddEventHandler(LeagueEvents::OnNewPath, OnNewPath);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		void Jax::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnPlayAnimation, OnPlayAnimation);
			EventManager::RemoveEventHandler(LeagueEvents::OnFinishCast, OnFinishCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnStopCast, OnStopCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnNewPath, OnNewPath);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		void Jax::OnGameUpdate()
		{
		}

		void Jax::OnDraw()
		{
		}

		void Jax::OnCreateObject(GameObject* unit)
		{
		}

		void Jax::OnDeleteObject(GameObject* unit)
		{
		}

		void Jax::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{
		}

		void Jax::OnPlayAnimation(GameObject* ptr, char* name, float animationTime)
		{
			GameClient::PrintChat(name, IM_COL32(255, 255, 255, 255));
		}

		void Jax::OnFinishCast(SpellCastInfo* castInfo, GameObject* object)
		{
		}

		void Jax::OnStopCast(SpellCastInfo* spellCaster_Client, bool stopAnimation, bool* executeCastFrame, bool forceStop, bool destroyMissile, unsigned int missileNetworkID)
		{
		}

		void Jax::OnNewPath(GameObject* obj, Vector3* start, Vector3* end, Vector3* tail, float* dashSpeed, unsigned dash)
		{

		}

		bool Jax::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position)
		{
			return true;
		}

	}
}

