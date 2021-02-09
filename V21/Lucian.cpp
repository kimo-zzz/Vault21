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
				CheckBox* UseW;
				Slider* WmaNa;
				CheckBox* UseE;
				Slider* EmaNa;
				CheckBox* UseR;
				Slider* enemiesInRange;
			}

			namespace LucianFarm {
				CheckBox* UseQ;
				CheckBox* UseE;
				Slider* QmaNa;
				Slider* EmaNa;
			}

			namespace LucianMisc {
				CheckBox* AutoE;
				CheckBox* AutoCatch;
				CheckBox* switchCatch;
				CheckBox* UseQforW;
				CheckBox* CatchUnderTurret;
				CheckBox* DrawAxe;
				Slider* DravenAxePickRange;
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
			LucianConfig::LucianCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
			LucianConfig::LucianCombo::WmaNa = combo->AddSlider("QmaNa", "Minimum Q mana", 30, 0, 100, 5);
			LucianConfig::LucianCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
			LucianConfig::LucianCombo::WmaNa = combo->AddSlider("WmaNa", "Minimum W mana", 50, 0, 100, 5);
			LucianConfig::LucianCombo::UseE = combo->AddCheckBox("Use E", "Use SpellSlot E", true);
			LucianConfig::LucianCombo::EmaNa = combo->AddSlider("EmaNa", "Minimum E mana", 20, 0, 100, 5);
			LucianConfig::LucianCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);

			auto farm = menu->AddMenu("farm", "Farm Settings");
			LucianConfig::LucianFarm::UseQ = farm->AddCheckBox("Use Q", "Use SpellSlot Q", true);
			LucianConfig::LucianFarm::QmaNa = farm->AddSlider("QmaNa", "Minimum Q mana", 50, 0, 100, 5);
			LucianConfig::LucianFarm::UseE = farm->AddCheckBox("Use E", "Use SpellSlot E", true);
			LucianConfig::LucianFarm::EmaNa = farm->AddSlider("EmaNa", "Minimum E mana", 50, 0, 100, 5);

			auto drawings = menu->AddMenu("Drawings", "Drawings");
			LucianConfig::LucianDrawings::DrawQ = drawings->AddCheckBox("Draw Q", "Draw Q", true);
			LucianConfig::LucianDrawings::DrawW = drawings->AddCheckBox("Draw Q", "Draw Q", true);

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
			auto target = GetTarget();

			if (!Orbwalker::OrbwalkerEvading) {

				if (ActiveMode != OrbwalkerMode_None)
				{

				}
			}

			if (ActiveMode & OrbwalkerMode_Combo) {

				if (ObjectManager::Player->FindBuffName("lucianpassiveshot"))
					Orbwalker::ResetAutoAttack();

				if (target && Orbwalker::CanCastAfterAttack()) {
					if (Distance(target, ObjectManager::Player) <= 500)
						if (!ObjectManager::Player->FindBuffName("lucianpassiveshot"))
							ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->Position, target->Position, target->NetworkId);

					if (!ObjectManager::Player->FindBuffName("lucianpassiveshot"))
						ObjectManager::Player->CastPredictSpell(SpellSlot_W, ObjectManager::Player->Position, target->Position);

					auto after = ObjectManager::Player->Position - (ObjectManager::Player->Position - HudManager::Instance->CursorTargetLogic->CursorPosition).Normalized() * 425;

					auto disafter = target->Position.DistanceSquared(after);

					if (!ObjectManager::Player->FindBuffName("lucianpassiveshot"))
						ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, after);

				}
			}

			if (ActiveMode & OrbwalkerMode_LastHit) {
				GameObject* lasthitTarget_ = GetLasthitTarget();
				if (lasthitTarget_)
				{
					if (Orbwalker::CanCastAfterAttack() && !Orbwalker::CanAttack(lasthitTarget_)) {

						ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)lasthitTarget_, ObjectManager::Player->Position, lasthitTarget_->Position, lasthitTarget_->NetworkId);
					}
				}

			}
		}




		GameObject* Lucian::GetTarget()
		{
			std::vector<GameObject*> heroes;
			auto hero_list = HACKUZAN::GameObject::GetHeroes();
			for (size_t i = 0; i < hero_list->size; i++)
			{
				auto hero = hero_list->entities[i];

				if (hero && hero->IsEnemy() && hero->IsValidTarget(900)) {
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