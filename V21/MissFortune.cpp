#include "stdafx.h"
#include "MissFortune.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"
#include "Prediction.h"

namespace V21 {
	namespace Plugins {

		using namespace V21::SDK;
		using namespace V21::SDK::Orbwalker;

		namespace MissFortuneConfig {

			namespace MissFortuneCombo {
				CheckBox* UseQ;
				CheckBox* UseW;
				Slider* WmaNa;
				CheckBox* UseE;
				Slider* EmaNa;
				CheckBox* UseR;
				Slider* enemiesInRange;
			}

			namespace MissFortuneFarm {
				CheckBox* UseQ;
				CheckBox* UseE;
				Slider* QmaNa;
				Slider* EmaNa;
			}

			namespace MissFortuneMisc {
				
				List* HitChance;
			}
		}

		void V21::Plugins::MissFortune::Initialize()
		{
			auto menu = Menu::CreateMenu("MissFortune", "MissFortune");

			auto combo = menu->AddMenu("Combo", "Combo Settings");
			MissFortuneConfig::MissFortuneCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
			MissFortuneConfig::MissFortuneCombo::UseW = combo->AddCheckBox("Use E", "Use SpellSlot W", true);
			MissFortuneConfig::MissFortuneCombo::UseE = combo->AddCheckBox("Use W", "Use SpellSlot E", true);

			auto farm = menu->AddMenu("farm", "Farm Settings");
			MissFortuneConfig::MissFortuneFarm::UseQ = farm->AddCheckBox("Use Q", "Use SpellSlot Q", true);
			MissFortuneConfig::MissFortuneFarm::QmaNa = farm->AddSlider("QMana", "Minimum Q mana", 30, 0, 100, 5);
			MissFortuneConfig::MissFortuneFarm::UseE = farm->AddCheckBox("Use E", "Use SpellSlot E", true);
			MissFortuneConfig::MissFortuneFarm::EmaNa = farm->AddSlider("EMana", "Minimum E mana", 30, 0, 100, 5);

			auto misc = menu->AddMenu("misc", "Misc");
			MissFortuneConfig::MissFortuneMisc::HitChance = misc->AddList("HitChance", "HitChance ", { "Collision","OutOfRange","Impossible","Low","Medium","High","VeryHigh","Dashing","Immobile" }, 3);

			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);

			GameClient::PrintChat("MissFortune Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void MissFortune::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		bool MissFortune::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position) {
			if (unit == ObjectManager::Player)
			{
				if (Orbwalker::DisableNextMove && order == GameObjectOrder::MoveTo) {

					return false;
				}
			}
			return  true;
		}

		void MissFortune::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];
			if (!castInfo)
				return;

		}

		void MissFortune::OnCreateObject(GameObject* unit)
		{

		}

		void MissFortune::OnDeleteObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;


		}

		void MissFortune::OnDraw()
		{

		}

		void MissFortune::OnGameUpdate()
		{

			if (ActiveMode & OrbwalkerMode_Combo) {
				auto target = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 1100.0f, kDamageType::DamageType_Physical);

				if (target && Orbwalker::CanCastAfterAttack() && MissFortuneConfig::MissFortuneCombo::UseQ->Value) {
					if (Distance(target, ObjectManager::Player) <= ObjectManager::Player->Spellbook.GetSpell(SpellSlot_Q)->SpellData->Resource->Range) {
						ObjectManager::Player->CastTargetSpell(kSpellSlot::SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->Position, target->Position, target->NetworkId);
					}
				}

				if (target && Orbwalker::CanCastAfterAttack() && MissFortuneConfig::MissFortuneCombo::UseW->Value) {
					if (ObjectManager::Player->IsInAutoAttackRange(target)) {
						ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_W, (DWORD)ObjectManager::Player);
					}
				}

				if (target && Orbwalker::CanCastAfterAttack() && MissFortuneConfig::MissFortuneCombo::UseE->Value) {

					if (Distance(target, ObjectManager::Player) <= ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->Range) {

						CastPrediction(SpellSlot_E, ObjectManager::Player->Position, ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->MissileSpeed
							, ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->Range, ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->Radius
							, ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->CastDelay, target, kCollidesWithNothing, (HitChance)MissFortuneConfig::MissFortuneMisc::HitChance->Value);
					}
				}
			}

			if (ActiveMode & OrbwalkerMode_LaneClear || ActiveMode & OrbwalkerMode_JungleClear) {

				auto target = TargetSelector::GetTarget(TargetType::TSTARGET_MINION, 1100.0f, kDamageType::DamageType_Physical);
				if (target && Orbwalker::CanCastAfterAttack() && MissFortuneConfig::MissFortuneFarm::UseQ->Value && ObjectManager::Player->Resource >= (MissFortuneConfig::MissFortuneFarm::QmaNa->Value / 100.0) * ObjectManager::Player->MaxResource) {
					if (Distance(target, ObjectManager::Player) <= ObjectManager::Player->Spellbook.GetSpell(SpellSlot_Q)->SpellData->Resource->Range) {
						ObjectManager::Player->CastTargetSpell(kSpellSlot::SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->Position, target->Position, target->NetworkId);
					}
				}


				if (target && Orbwalker::CanCastAfterAttack() && MissFortuneConfig::MissFortuneFarm::UseE->Value && ObjectManager::Player->Resource >= (MissFortuneConfig::MissFortuneFarm::EmaNa->Value / 100.0) * ObjectManager::Player->MaxResource) {

					if (Distance(target, ObjectManager::Player) <= ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->Range) {

						CastPrediction(SpellSlot_E, ObjectManager::Player->Position, ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->MissileSpeed
							, ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->Range, ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->Radius
							, ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->SpellData->Resource->CastDelay, target, kCollidesWithNothing, (HitChance)MissFortuneConfig::MissFortuneMisc::HitChance->Value);
					}
				}
			}
		}

	}
}