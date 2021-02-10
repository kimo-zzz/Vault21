#include "stdafx.h"
#include "Lucian.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"
#include "HealthPrediction.h"
namespace HACKUZAN {
	namespace Plugins {

		using namespace HACKUZAN::SDK;
		using namespace HACKUZAN::SDK::Orbwalker;

		namespace LucianConfig {

			namespace LucianCombo {
				CheckBox* UseQ;
				Slider* QmaNa;
				CheckBox* UseW;
				Slider* WmaNa;
				CheckBox* UseE;
				Slider* EmaNa;
				CheckBox* UseR;
				Slider* enemiesInRange;
			}

			namespace LucianFarm {
				CheckBox* UseQLasthit;

				Slider* QmaNa;
				Slider* EmaNa;
				Slider* WMana;
			}

			namespace LucianMisc {
				CheckBox* AutoE;
				CheckBox* KillSteal;
			}

			namespace LucianDrawings {
				CheckBox* DrawQ;
				CheckBox* DrawW;
			}


		}

		void HACKUZAN::Plugins::Lucian::Initialize()
		{
			auto menu = Menu::CreateMenu("Lucian", "Lucian");

			auto combo = menu->AddMenu("Combo", "Combo Settings");
			LucianConfig::LucianCombo::UseQ = combo->AddCheckBox("Use Q", "UseQ", true);
			LucianConfig::LucianCombo::UseW = combo->AddCheckBox("Use W", "UseW", true);
			LucianConfig::LucianCombo::UseE = combo->AddCheckBox("Use E", "UseE", true);
			//LucianConfig::LucianCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);

			auto combo_mana = combo->AddMenu("Mana Settings Combo", "Mana Settings");
			LucianConfig::LucianCombo::QmaNa = combo_mana->AddSlider("QMana", "Minimum Q mana", 30, 0, 100, 5);
			LucianConfig::LucianCombo::WmaNa = combo_mana->AddSlider("Wmana", "Minimum W mana", 50, 0, 100, 5);
			LucianConfig::LucianCombo::EmaNa = combo_mana->AddSlider("EMana", "Minimum E mana", 20, 0, 100, 5);

			auto farm = menu->AddMenu("farm", "Farm Settings");
			farm->AddCheckBox("Farm Use Spells", "Use Spells", true);

			auto farm_mana = farm->AddMenu("Farm Mana", "Mana Settings");
			LucianConfig::LucianFarm::QmaNa = farm_mana->AddSlider("QMana", "Minimum Q mana", 30, 0, 100, 5);
			LucianConfig::LucianFarm::WMana = farm_mana->AddSlider("Wmana", "Minimum W mana", 50, 0, 100, 5);
			LucianConfig::LucianFarm::EmaNa = farm_mana->AddSlider("EMana", "Minimum E mana", 20, 0, 100, 5);

			auto misc = menu->AddMenu("Misc", "Misc");
			misc->AddCheckBox("Killsteal", "Killsteal", true);

			auto drawings = menu->AddMenu("Drawings", "Drawings");
			LucianConfig::LucianDrawings::DrawQ = drawings->AddCheckBox("Draw Q", "Draw Q", true);
			LucianConfig::LucianDrawings::DrawW = drawings->AddCheckBox("Draw W", "Draw W", true);

			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);
			GameClient::PrintChat("Lucian Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void Lucian::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		bool Lucian::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position) {
			if (unit == ObjectManager::Player)
			{
				if (Orbwalker::DisableNextMove && order == GameObjectOrder::MoveTo) {

					return false;
				}
			}
			return  true;
		}

		void Lucian::OnCreateObject(GameObject* unit)
		{

		}

		void Lucian::OnDeleteObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;


		}

		void Lucian::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			if (castInfo == nullptr || spellData == nullptr)
				return;

			if (castInfo->IsBasicAttack)
				return;
			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];
			if (caster == nullptr || !caster->IsHero() || !caster->IsEnemy())
				return;

			/*
			auto hero = caster;
			if (hero != nullptr && hero->IsEnemy()) {
				for (GapCloser* gapcloser : GapClosersDB->GapCloserSpells) {
					if (gapcloser->ChampionName.c_str() == hero->BaseCharacterData->SkinName) {
						auto endpos = gapcloser->Type == GapCloserType::targeted ? castInfo->StartPosition : castInfo->EndPosition;
						auto targetPos = ObjectManager::Player->Position.Extended(endpos, 425.f);

						//add check if targetPos would be under enemy turret

						switch (gapcloser->Type)
						{
						case GapCloserType::targeted:
							if (castInfo->TargetId == ObjectManager::Player->Id)
								ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, targetPos);
							break;

						case GapCloserType::skillshot:
							if (Distance(castInfo->EndPosition, ObjectManager::Player->Position) <= ObjectManager::Player->GetAutoAttackRange())
								ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, targetPos);
							break;
						}
					}
				}
			}
			*/

		}

		void Lucian::OnGapCloserSpell(SpellInfo* castInfo)
		{
			
		}

		void Lucian::OnDraw()
		{
			if (LucianConfig::LucianDrawings::DrawQ->Value)
				Renderer::AddCircle(ObjectManager::Player->Position, 500.f, 1, IM_COL32(255, 0, 255, 155));

			if (LucianConfig::LucianDrawings::DrawW->Value)
				Renderer::AddCircle(ObjectManager::Player->Position, 900.f, 1, IM_COL32(255, 255, 0, 155));
		}

		void Lucian::OnGameUpdate()
		{

			if (!Orbwalker::OrbwalkerEvading) {

				auto Qtarget = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 500.f, DamageType_Physical);
				auto Wtarget = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 900.f, DamageType_Physical);
				auto Etarget = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 425.f + ObjectManager::Player->GetAutoAttackRange(), DamageType_Physical);

				if (LucianConfig::LucianMisc::KillSteal)
				{
					if (Qtarget != nullptr && Qtarget->Health <= Damage::CalculatePhysicalDamage(ObjectManager::Player, Qtarget, 95 + ((60 * ObjectManager::Player->Spellbook.GetSpell(SpellSlot_Q)->Level) / ObjectManager::Player->TotalBonusAttackDamage())))
						ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)Qtarget, ObjectManager::Player->Position, Qtarget->Position, Qtarget->NetworkId);
				}

				if (ActiveMode & OrbwalkerMode_Combo || ActiveMode & OrbwalkerMode_LaneClear || ActiveMode & OrbwalkerMode_JungleClear) {


					float minimum_q_mana_ = LucianConfig::LucianCombo::QmaNa->Value / 100 * ObjectManager::Player->Resource;
					float minimum_w_mana_ = LucianConfig::LucianCombo::WmaNa->Value / 100 * ObjectManager::Player->Resource;
					float minimum_e_mana_ = LucianConfig::LucianCombo::EmaNa->Value / 100 * ObjectManager::Player->Resource;

					if (ActiveMode & OrbwalkerMode_LaneClear || ActiveMode & OrbwalkerMode_JungleClear)
					{
						minimum_q_mana_ = LucianConfig::LucianFarm::QmaNa->Value / 100 * ObjectManager::Player->Resource;
						minimum_w_mana_ = LucianConfig::LucianFarm::WMana->Value / 100 * ObjectManager::Player->Resource;
						minimum_e_mana_ = LucianConfig::LucianFarm::EmaNa->Value / 100 * ObjectManager::Player->Resource;
						Qtarget = TargetSelector::GetTarget(TargetType::TSTARGET_MINION, 500.f, DamageType_Physical);
						Wtarget = TargetSelector::GetTarget(TargetType::TSTARGET_MINION, 900.f, DamageType_Physical);
					}


					if (Orbwalker::CanCastAfterAttack()) {
						if (Qtarget != nullptr && minimum_q_mana_ <= ObjectManager::Player->MaxResource && !ObjectManager::Player->FindBuffName("LucianPassiveBuff"))
							ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)Qtarget, ObjectManager::Player->Position, Qtarget->Position, Qtarget->NetworkId);

						if (Etarget != nullptr && minimum_w_mana_ <= ObjectManager::Player->MaxResource && !ObjectManager::Player->FindBuffName("LucianPassiveBuff"))
							ObjectManager::Player->CastPredictSpell(SpellSlot_W, ObjectManager::Player->Position, Etarget->Position);

						if (Etarget != nullptr && Etarget->IsValidTarget())
						{
							auto posAfterE = ObjectManager::Player->Position.Extended(HudManager::Instance->CursorTargetLogic->CursorPosition, 425.f);

							if (ObjectManager::Player->CountEnemiesInRange(1000.f) > 3 || Distance(Etarget, posAfterE) >= ObjectManager::Player->GetAutoAttackRange())
								return;
							if (minimum_e_mana_ <= ObjectManager::Player->MaxResource && !ObjectManager::Player->FindBuffName("LucianPassiveBuff"))
								ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, posAfterE);
						}


						if (ActiveMode & OrbwalkerMode_LastHit) {
							GameObject* lasthitTarget_ = TargetSelector::GetTarget(TargetType::TSTARGET_MINION, 500.f, DamageType_Physical, ObjectManager::Player->Position, true);
							if (lasthitTarget_ != nullptr && LucianConfig::LucianFarm::QmaNa->Value / 100 * ObjectManager::Player->Resource <= ObjectManager::Player->MaxResource)
							{
								if (Orbwalker::CanCastAfterAttack() && !ObjectManager::Player->CanAttack()) {

									ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)lasthitTarget_, ObjectManager::Player->Position, lasthitTarget_->Position, lasthitTarget_->NetworkId);
								}
							}
						}


					}
				}

			}

		}




		GameObject* Lucian::GetTarget(float radius)
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

		GameObject* Lucian::GetLasthitTarget()
		{
			std::vector<GameObject*> minions;
			auto minion_list = HACKUZAN::GameObject::GetMinions();
			for (size_t i = 0; i < minion_list->size; i++)
			{
				auto minion = minion_list->entities[i];

				if (minion && minion->IsMinion() && minion->IsValidTarget(500)) {
					auto dmg = Damage::CalculatePhysicalDamage(ObjectManager::Player, minion, 95 + ((60 * ObjectManager::Player->Spellbook.GetSpell(SpellSlot_Q)->Level) / ObjectManager::Player->TotalBonusAttackDamage()));
					if (dmg >= minion->TotalHealth())
						return minion;
				}
			}
			return nullptr;
		}

		GameObject* Lucian::GetWaveclerTarget()
		{
			return nullptr;
		}
	}
}