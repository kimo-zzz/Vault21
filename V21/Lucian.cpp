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

		void Lucian::OnDraw()
		{
			if (LucianConfig::LucianDrawings::DrawQ)
				Renderer::AddCircle(ObjectManager::Player->Position, 500.f, 1, IM_COL32(255, 0, 255, 155));

			if (LucianConfig::LucianDrawings::DrawW)
				Renderer::AddCircle(ObjectManager::Player->Position, 900.f, 1, IM_COL32(255, 255, 0, 155));
		}

		void Lucian::OnGameUpdate()
		{
			auto Qtarget = GetTarget(500.f);
			auto Wtarget = GetTarget(900.f);

			if (!Orbwalker::OrbwalkerEvading) {

				if (ActiveMode & OrbwalkerMode_Combo || ActiveMode & OrbwalkerMode_LaneClear || ActiveMode & OrbwalkerMode_JungleClear) {

					float minimum_q_mana_ = LucianConfig::LucianCombo::QmaNa->Value / 100 * ObjectManager::Player->Resource;
					float minimum_w_mana_ = LucianConfig::LucianCombo::WmaNa->Value / 100 * ObjectManager::Player->Resource;
					float minimum_e_mana_ = LucianConfig::LucianCombo::EmaNa->Value / 100 * ObjectManager::Player->Resource;

					if (ActiveMode != OrbwalkerMode_Combo)
					{
						minimum_q_mana_ = LucianConfig::LucianFarm::QmaNa->Value / 100 * ObjectManager::Player->Resource;
						minimum_w_mana_ = LucianConfig::LucianFarm::WMana->Value / 100 * ObjectManager::Player->Resource;
						minimum_e_mana_ = LucianConfig::LucianFarm::EmaNa->Value / 100 * ObjectManager::Player->Resource;
						Qtarget = TargetSelector::GetTarget(TargetType::TSTARGET_MINION, 500.f, DamageType_Physical);
					}


					if (Qtarget != nullptr && Orbwalker::CanCastAfterAttack()) {
						if (minimum_q_mana_ <= ObjectManager::Player->MaxResource && !ObjectManager::Player->FindBuffName("LucianPassiveBuff"))
							ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)Qtarget, ObjectManager::Player->Position, Qtarget->Position, Qtarget->NetworkId);

						if (minimum_w_mana_ <= ObjectManager::Player->MaxResource && !ObjectManager::Player->FindBuffName("LucianPassiveBuff"))
							ObjectManager::Player->CastPredictSpell(SpellSlot_W, ObjectManager::Player->Position, Wtarget->Position);

						auto after = ObjectManager::Player->Position - (ObjectManager::Player->Position - HudManager::Instance->CursorTargetLogic->CursorPosition).Normalized() * 425;

						//auto disafter = target->Position.DistanceSquared(after);
						if (Distance(after, Qtarget->Position) <= ObjectManager::Player->AttackRange + ObjectManager::Player->GetBoundingRadius())
						{
							if (minimum_e_mana_ <= ObjectManager::Player->MaxResource && !ObjectManager::Player->FindBuffName("LucianPassiveBuff"))
								ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, after);
						}

					}
				}

				if (ActiveMode & OrbwalkerMode_LastHit) {
					GameObject* lasthitTarget_ = TargetSelector::GetTarget(TargetType::TSTARGET_MINION, 500.f, DamageType_Physical);
					if (lasthitTarget_ && LucianConfig::LucianFarm::QmaNa->Value / 100 * ObjectManager::Player->Resource <= ObjectManager::Player->MaxResource)
					{
						if (Orbwalker::CanCastAfterAttack() && !Orbwalker::CanAttack(lasthitTarget_)) {

							ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)lasthitTarget_, ObjectManager::Player->Position, lasthitTarget_->Position, lasthitTarget_->NetworkId);
						}
					}
				}

				if (ActiveMode != OrbwalkerMode_None)
				{

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