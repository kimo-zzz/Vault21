#include "stdafx.h"
#include "Twitch.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"

namespace HACKUZAN {
	namespace Plugins {



		using namespace HACKUZAN::SDK;
		using namespace HACKUZAN::SDK::Orbwalker;

		float eBaseDmg[] = { 0.f, 20.f,30.f,40.f,50.f,60.f };

		float eStackDmg[] = { 0.f, 15.f, 20.f, 25.f, 30.f, 35.f };

		namespace TwitchConfig {

			namespace TwitchCombo {
				CheckBox* UseQ;
				CheckBox* UseW;
				Slider* QmaNa;
				Slider* WmaNa;
				Slider* EmaNa;
				Slider* RmaNa;
				CheckBox* UseE;
				CheckBox* UseR;
				Slider* enemiesInRange;
			}

			namespace TwitchFarm {
				CheckBox* UseQ;
				CheckBox* UseEWaveclear;
				CheckBox* useELasthit;
				Slider* QmaNa;
				Slider* WmaNa;
				Slider* EmaNa;
			}

			namespace TwitchHarass {
				CheckBox* UseW;
				CheckBox* UseE;
				Slider* EStackAmount;

				Slider* WMana;
				Slider* EMana;
			}

			namespace TwitchMisc {
				CheckBox* AutoE;
				CheckBox* EKillable;
			}
		}

		void HACKUZAN::Plugins::Twitch::Initialize()
		{
			auto menu = Menu::CreateMenu("Twitch", "Twitch");

			auto combo = menu->AddMenu("Combo", "Combo Settings");
			TwitchConfig::TwitchCombo::UseQ = combo->AddCheckBox("Twitch Combo Q", "UseQ", true);
			TwitchConfig::TwitchCombo::UseW = combo->AddCheckBox("Twitch Combo W", "UseW", true);
			TwitchConfig::TwitchCombo::UseE = combo->AddCheckBox("Twitch Combo E", "UseE", true);
			TwitchConfig::TwitchCombo::UseR = combo->AddCheckBox("Twitch Combo R", "UseR", true);
			TwitchConfig::TwitchCombo::enemiesInRange = combo->AddSlider("Twitch R Slider", "Minimum Enemies", 2, 1, 5);

			auto combo_mana = combo->AddMenu("Mana Settings", "Mana Settings");
			TwitchConfig::TwitchCombo::QmaNa = combo_mana->AddSlider("combo_mana QmaNa", "QMana", 50, 1, 100);
			TwitchConfig::TwitchCombo::WmaNa = combo_mana->AddSlider("combo_mana WmaNa", "WMana", 50, 1, 100);
			TwitchConfig::TwitchCombo::EmaNa = combo_mana->AddSlider("combo_mana EmaNa", "EMana", 30, 1, 100);
			TwitchConfig::TwitchCombo::RmaNa = combo_mana->AddSlider("combo_mana RmaNa", "RMana", 20, 1, 100);


			auto farm = menu->AddMenu("farm", "Farm Settings");
			TwitchConfig::TwitchFarm::UseQ = farm->AddCheckBox("Twitch Farm Q", "UseQ", true);
			TwitchConfig::TwitchFarm::UseEWaveclear = farm->AddCheckBox("Twitch Farm EWaveclear", "UseE Waveclear", true);
			TwitchConfig::TwitchFarm::useELasthit = farm->AddCheckBox("Twitch Farm ELasthit", "UseE Lasthit", true);

			auto farm_settings = farm->AddMenu("Farm Mana", "Mana Settings");
			TwitchConfig::TwitchFarm::QmaNa = farm_settings->AddSlider("Twitch Farm Q Mana", "QMana", 50, 1, 100);
			TwitchConfig::TwitchFarm::EmaNa = farm_settings->AddSlider("Twitch Farm E Mana", "EMana", 30, 1, 100);

			auto harass = menu->AddMenu("Harass", "Harass Settings");
			TwitchConfig::TwitchHarass::UseE = harass->AddCheckBox("harass UseE", "UseE", true);
			TwitchConfig::TwitchHarass::UseW = harass->AddCheckBox("harass UseW", "UseW", true);
			TwitchConfig::TwitchHarass::EStackAmount = harass->AddSlider("Twitch Harass E Slider", "UseE at x stacks", 3, 1, 5);

			auto harass_mana = harass->AddMenu("Harass Mana", "Mana Settings");
			TwitchConfig::TwitchHarass::EMana = harass_mana->AddSlider("harass_mana EMana", "EMana", 30, 1, 100);
			TwitchConfig::TwitchHarass::WMana = harass_mana->AddSlider("harass_mana WMana", "WMana", 50, 1, 100);

			auto misc = menu->AddMenu("misc", "Misc");
			TwitchConfig::TwitchMisc::AutoE = misc->AddCheckBox("Twitch Misc OoR E", "E if out of range", true);
			TwitchConfig::TwitchMisc::EKillable = misc->AddCheckBox("Twitch Misc E killable", "E if killable", true);

			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);

			GameClient::PrintChat("Twitch Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void Twitch::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		bool Twitch::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position) {
			if (unit == ObjectManager::Player)
			{
				if (Orbwalker::DisableNextMove && order == GameObjectOrder::MoveTo) {

					return false;
				}
			}
			return  true;
		}

		void Twitch::OnCreateObject(GameObject* unit)
		{

		}

		void Twitch::OnDeleteObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;


		}

		int Twitch::CountEnemiesInRange(float range)
		{
			auto count = 0;
			auto hero_list = HACKUZAN::GameObject::GetHeroes();
			for (size_t i = 0; i < hero_list->size; i++)
			{
				auto hero = hero_list->entities[i];
				if (hero && hero->IsValidTarget() && hero->IsEnemy()) {
					if (hero->Position.Distance(ObjectManager::Player->Position) <= range)
					{
						count++;
					}
				}
			}
			return count;
		}

		void Twitch::OnDraw()
		{

		}

		void Twitch::OnGameUpdate()
		{
			auto target = GetTarget(1200.f);

			if (target)
			{
				//Auto E
				if (Distance(target, ObjectManager::Player->Position) <= 1200.f && Distance(target, ObjectManager::Player->Position) > 1120.f)
				{
					ObjectManager::Player->CastSpell(SpellSlot_E, (DWORD)ObjectManager::Player);
				}
			}


			if (!Orbwalker::OrbwalkerEvading) {

				if (ActiveMode & OrbwalkerMode_Combo) {

					if (target && Orbwalker::CanCastAfterAttack()) {

						//E if killable
						if (TwitchConfig::TwitchMisc::EKillable && IsKillable())
							ObjectManager::Player->CastSpell(SpellSlot_E, (DWORD)ObjectManager::Player);

						if ((TwitchConfig::TwitchCombo::QmaNa->Value / 100) * ObjectManager::Player->MaxResource <= ObjectManager::Player->Resource && TwitchConfig::TwitchCombo::UseQ)
							ObjectManager::Player->CastSpell(SpellSlot_Q, (DWORD)ObjectManager::Player);

						/*
						if ((Distance(target, ObjectManager::Player) <= 450.f && TwitchConfig::TwitchCombo::WmaNa->Value / 100) * ObjectManager::Player->MaxResource <= ObjectManager::Player->Resource && TwitchConfig::TwitchCombo::UseW && !ObjectManager::Player->FindBuffName("TwitchHideInShadows"))
							ObjectManager::Player->CastSpellPos(SpellSlot_W, (DWORD)ObjectManager::Player, target->Position);
							*/
						if ((TwitchConfig::TwitchCombo::RmaNa->Value / 100) * ObjectManager::Player->MaxResource <= ObjectManager::Player->Resource && CountEnemiesInRange(ObjectManager::Player->AttackRange + ObjectManager::Player->GetBoundingRadius()) >= TwitchConfig::TwitchCombo::enemiesInRange->Value)
							ObjectManager::Player->CastSpell(SpellSlot_R, (DWORD)ObjectManager::Player);
					}
				}

				if (ActiveMode & OrbwalkerMode_Harass)
				{
					if ((TwitchConfig::TwitchHarass::WMana->Value / 100) * ObjectManager::Player->MaxResource <= ObjectManager::Player->Resource && TwitchConfig::TwitchHarass::UseW)
						ObjectManager::Player->CastPredictSpell(SpellSlot_W, ObjectManager::Player->Position, target->Position);

					if (TwitchConfig::TwitchHarass::UseE)
					{
						int stackAmount = target->FindBuffName("TwitchDeadlyVenom")->GetCount();

						/*
						if ((TwitchConfig::TwitchHarass::EMana->Value / 100) * ObjectManager::Player->MaxResource <= ObjectManager::Player->Resource && stackAmount >= TwitchConfig::TwitchHarass::EStackAmount->Value)
							ObjectManager::Player->CastSpell(SpellSlot_E, (DWORD)ObjectManager::Player);
							*/
					}
				}

				if (ActiveMode & OrbwalkerMode_LastHit)
				{
					if (!ObjectManager::Player->CanAttack())
					{
						if (GetLasthitTarget(1200.f))
							ObjectManager::Player->CastSpell(SpellSlot_E, (DWORD)ObjectManager::Player);
					}
				}
			}

			if (ActiveMode != OrbwalkerMode_None) {

				if (Orbwalker::CanCastAfterAttack()) {

				}
			}
		}


		GameObject* Twitch::GetTarget(int radius)
		{
			std::vector<GameObject*> heroes;
			auto hero_list = HACKUZAN::GameObject::GetHeroes();
			for (size_t i = 0; i < hero_list->size; i++)
			{
				auto hero = hero_list->entities[i];

				if (hero && hero->IsEnemy() && hero->IsValidTarget(radius)) {
					heroes.push_back(hero);
				}
			}
			return TargetSelector::GetTarget(heroes, DamageType_Physical);
		}

		GameObject* Twitch::GetLasthitTarget(int radius)
		{
			std::vector<GameObject*> minions;
			auto minion_list = HACKUZAN::GameObject::GetMinions();
			for (size_t i = 0; i < minion_list->size; i++)
			{
				auto minion = minion_list->entities[i];

				if (minion && minion->IsEnemy() && minion->IsValidTarget(radius)) {
					if (minion->Health < GetPoisonDmg(minion))
						return minion;
				}
			}
			return nullptr;
		}

		float Twitch::GetPoisonDmg(GameObject* target)
		{
			if (!target->FindBuffName("TwitchDeadlyVenom") || target->IsInvulnerable() || target->FindBuffName("KindredRNoDeathBuff"))
				return 0.f;
			int eLevel = ObjectManager::Player->Spellbook.GetSpell(SpellSlot_E)->Level;
			float baseDmg = eBaseDmg[eLevel];

			float stackDmg = (eStackDmg[eLevel] + ObjectManager::Player->TotalAttackDamage() * 0.35f + ObjectManager::Player->TotalAbilityPower() * 0.333f) * target->FindBuffName("TwitchDeadlyVenom")->GetCount();

			float totalDamage = baseDmg + stackDmg;

			return totalDamage;
			//return Damage::CalculatePhysicalDamage(ObjectManager::Player, target, totalDamage);
		}

		bool Twitch::IsKillable()
		{


			auto hero_list = HACKUZAN::GameObject::GetHeroes();
			for (size_t i = 0; i < hero_list->size; i++)
			{
				auto hero = hero_list->entities[i];

				if (hero && hero->IsEnemy() && hero->IsValidTarget(1200.f)) {
					if (hero->Health < GetPoisonDmg(hero))
						return true;
				}
			}
			return false;
		}
	}
}