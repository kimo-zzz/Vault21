#include "stdafx.h"
#include "PluginTemplate.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"
#include "Jax.h"

namespace V21
{
	namespace Plugins 
	{
		using namespace V21::SDK;
		using namespace V21::SDK::Orbwalker;

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
			bool ExecutedFirstCounterStrike = false;

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
				auto hero_list = V21::GameObject::GetHeroes();
				for (size_t i = 0; i < hero_list->size; i++)
				{
					auto hero = hero_list->entities[i];

					if (hero != nullptr && hero->IsEnemy() && hero->IsValidTarget(radius)) {
						heroes.push_back(hero);
					}
				}
				return TargetSelector::GetTarget(heroes, DamageType_Physical);
			}

			GameObject* GetMinionTarget(float radius)
			{
				std::vector<GameObject*> minions;
				auto minion_list = V21::GameObject::GetMinions();
				for (size_t i = 0; i < minion_list->size; i++)
				{
					auto hero = minion_list->entities[i];

					if (hero != nullptr && hero->IsEnemy() && hero->IsValidTarget(radius)) {
						minions.push_back(hero);
					}
				}
				return TargetSelector::GetTarget(minions, DamageType_Physical);
			}

			namespace Farming
			{
				void ExecuteQ()
				{
					if (!Config::Farming::UseQ->Value || ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_Q) != kSpellState::SpellState_Ready)
						return;
					float range = ObjectManager::Player->Spellbook.GetSpell(SpellSlot_Q)->SpellData->Resource->Range;
					auto target = GetMinionTarget(range);
					if (target && Distance(target, ObjectManager::Player) <= range)
						executeSpell(SpellSlot_Q, Vector3(0, 0, 0), target);
				}

				void ExecuteW()
				{
					if (!Config::Combo::UseW->Value || ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_W) != kSpellState::SpellState_Ready)
						return;
					float range = ObjectManager::Player->Spellbook.GetSpell(SpellSlot_W)->SpellData->Resource->Range;
					auto target = GetMinionTarget(range);
					if (target && Distance(target, ObjectManager::Player) <= range)
						executeSpell(SpellSlot_W);
				}

				void ExecuteE() 
				{

				}
			}

			namespace Combo
			{
				void ExecuteQ()
				{
					if (!Config::Combo::UseQ->Value || ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_Q) != kSpellState::SpellState_Ready)
						return;
					float range = ObjectManager::Player->Spellbook.GetSpell(SpellSlot_Q)->SpellData->Resource->Range;
					auto target = GetTarget(range);
					if (target && Distance(target, ObjectManager::Player) <= range)
						executeSpell(SpellSlot_Q, Vector3(0, 0, 0), target);
				}

				void ExecuteW()
				{
					if (!Config::Combo::UseW->Value || ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_W) != kSpellState::SpellState_Ready)
						return;
					float range = ObjectManager::Player->Spellbook.GetSpell(SpellSlot_W)->SpellData->Resource->Range;
					auto target = GetTarget(range);
					if (target && Distance(target, ObjectManager::Player) <= range)
						executeSpell(SpellSlot_W);
				}

				void ExecuteE()
				{
					BYTE n = 0;

					if (!Config::Combo::UseW->Value || ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_E) != kSpellState::SpellState_Ready)
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
			Config::Misc::WardJump = misc->AddKeyBind("Jax Ward Jump Key", "Ward Jump", 'N', false, false);


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
		}

		void Jax::OnFinishCast(SpellCastInfo* castInfo, GameObject* object)
		{
		}

		void Jax::OnStopCast(GameObject* caster, StopCast args)
		{
		}

		void Jax::OnNewPath(NewPath args)
		{

		}

		void Jax::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3* position, GameObject* target)
		{
			return;
		}

	}
}

