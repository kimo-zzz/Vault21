#include "stdafx.h"
#include "Brand.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"
#include "BasicPrediction.h"
#include "Prediction.h"

namespace V21 {
	namespace Plugins {

		using namespace V21::SDK;
		using namespace V21::SDK::Orbwalker;

		namespace BrandConfig {

			namespace BrandCombo {
				CheckBox* UseQ;
				CheckBox* UseQ2;
				CheckBox* UseW;
				CheckBox* UseE;
				CheckBox* UseR;
				Slider* RCount;
			}

			namespace BrandHarass {
				CheckBox* UseQ;
				CheckBox* UseW;
				CheckBox* UseE;
				Slider* HMana;
			}

			namespace BrandFarm {
				CheckBox* UseE;
				CheckBox* UseW;
				Slider* FMana;
			}

			namespace BrandDraw {
				CheckBox* UseQ;
				CheckBox* UseW;
				CheckBox* UseE;
				CheckBox* UseR;
			}
		}

		void Brand::Initialize()
		{
			auto menu = Menu::CreateMenu("Brand", "Brand");

			auto combo = menu->AddMenu("combo", "Combo Settings");
			BrandConfig::BrandCombo::UseQ = combo->AddCheckBox("Use Q", "[Q]", true);
			BrandConfig::BrandCombo::UseQ2 = combo->AddCheckBox("Use Q2", "[Q] only if target has pasive", true);
			BrandConfig::BrandCombo::UseW = combo->AddCheckBox("Use W", "[W]", true);
			BrandConfig::BrandCombo::UseE = combo->AddCheckBox("Use E", "[E]", true);
			BrandConfig::BrandCombo::UseR = combo->AddCheckBox("Use R", "[R]", true);
			BrandConfig::BrandCombo::RCount = combo->AddSlider("RCount", "[R] Min Enemies", 2, 1, 5, 1);

			auto harass = menu->AddMenu("harass", "Harass Settings");
			BrandConfig::BrandHarass::UseQ = harass->AddCheckBox("Use Q", "[Q]", true);
			BrandConfig::BrandHarass::UseW = harass->AddCheckBox("Use W", "[W]", true);
			BrandConfig::BrandHarass::UseE = harass->AddCheckBox("Use E", "[E]", true);
			BrandConfig::BrandHarass::HMana = harass->AddSlider("HMana", "Min mana for Poke", 40, 0, 100, 5);

			auto farm = menu->AddMenu("farm", "Farm Settings");
			BrandConfig::BrandFarm::UseE = farm->AddCheckBox("Use E", "[E]", true);
			BrandConfig::BrandFarm::UseW = farm->AddCheckBox("Use W", "[W]", true);
			BrandConfig::BrandFarm::FMana = farm->AddSlider("FMana", "Min mana for Farm", 40, 0, 100, 5);

			auto draw = menu->AddMenu("draw", "Drawing Settings");
			BrandConfig::BrandDraw::UseQ = draw->AddCheckBox("Use Q", "[Q] range", false);
			BrandConfig::BrandDraw::UseW = draw->AddCheckBox("Use W", "[W] range", false);
			BrandConfig::BrandDraw::UseE = draw->AddCheckBox("Use E", "[E] range", false);
			BrandConfig::BrandDraw::UseR = draw->AddCheckBox("Use R", "[R] range", false);

			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);

			GameClient::PrintChat("Brand Loaded", IM_COL32(255, 69, 255, 255));
		}

		void Brand::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		void Brand::OnDraw()
		{
			if (BrandConfig::BrandDraw::UseQ->Value)
			{
				Renderer::AddCircle((ObjectManager::Player->Position), 1100, 2.0f, IM_COL32(255, 225, 255, 10));
			}

			if (BrandConfig::BrandDraw::UseW->Value)
			{
				Renderer::AddCircle((ObjectManager::Player->Position), 900, 2.0f, IM_COL32(225, 225, 125, 10));
			}

			if (BrandConfig::BrandDraw::UseE->Value)
			{
				Renderer::AddCircle((ObjectManager::Player->Position), 675, 2.0f, IM_COL32(225, 225, 50, 10));
			}

			if (BrandConfig::BrandDraw::UseR->Value)
			{
				Renderer::AddCircle((ObjectManager::Player->Position), 750, 2.0f, IM_COL32(225, 225, 255, 10));
			}
		}

		void Brand::OnGameUpdate()
		{
			if (Orbwalker::OrbwalkerEvading || !ObjectManager::Player->Alive()) // IsDead is ok ???
				return;


			if (ActiveMode & OrbwalkerMode_Combo)
			{
				BrandCombo();
			}
			else if (ActiveMode & OrbwalkerMode_Harass)   // OrbwalkerMode_Harass is ok ???
			{
			//	BrandHarass();
			}
			else if (ActiveMode != OrbwalkerMode_None)  // Mode Farm??????????? Lane and Jungle minions???? --- What is _None ???
			{
				//BrandLaneClear();
			}
		}


		void Brand::BrandCombo()
		{
			auto Wtarget = GetTarget(900.f);
			auto Etarget = GetTarget(1100.f);

			if (Orbwalker::CanCastAfterAttack())
			{


				if (BrandConfig::BrandCombo::UseW->Value)
				{
					//	auto predict = Prediction::BasicPrediction(Wtarget, 260.f, 900.f, 999999999.f, 0.25f);
						//ObjectManager::Player->CastSpellPos(kSpellSlot::SpellSlot_W, (DWORD)ObjectManager::Player, predict);


					if (Etarget && ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_Q) == SpellState_Ready)
					{
						//	auto prediction = Prediction::BasicPrediction(Wtarget, 260.f, 900.f, 0, 0.625f);
						CastPrediction(SpellSlot_Q, ObjectManager::Player->ServerPosition(), 1600.f, 1100.f, 120.f, 0.25f, Etarget, kCollidesWithNothing, HitChance::High);

					}
					//auto test = Prediction::GetPositionOnPath(input);
					if (Wtarget && ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_W) == SpellState_Ready)
					{
						CastPrediction(SpellSlot_W, ObjectManager::Player->ServerPosition(), FLT_MAX, 900.f, 260.f, 0.627f, Wtarget, kCollidesWithNothing, HitChance::High);

					}

				}

			}
		}

		void Brand::BrandHarass()
		{
			if ((ObjectManager::Player->Resource / ObjectManager::Player->MaxResource) < (BrandConfig::BrandHarass::HMana->Value / 100.0))
				return;

			auto Wtarget = GetTarget(1100.f);
			auto Etarget = GetTarget(625);

			if (Etarget && Orbwalker::CanCastAfterAttack())
			{
				if (BrandConfig::BrandHarass::UseE->Value)
				{
					ObjectManager::Player->CastTargetSpell(kSpellSlot::SpellSlot_E, (DWORD)ObjectManager::Player, (DWORD)Etarget, ObjectManager::Player->Position, Etarget->Position, Etarget->NetworkId);
				}

				if (BrandConfig::BrandHarass::UseQ->Value)
				{
					ObjectManager::Player->CastPredictSpell(kSpellSlot::SpellSlot_Q, ObjectManager::Player->Position, Etarget->Position);
				}

				if (BrandConfig::BrandHarass::UseW->Value && HasBrandPassive(Etarget))
				{
					ObjectManager::Player->CastPredictSpell(kSpellSlot::SpellSlot_W, ObjectManager::Player->Position, Etarget->Position);
				}
			}

			if (Wtarget && Orbwalker::CanCastAfterAttack())
			{
				if (BrandConfig::BrandHarass::UseW->Value)
				{
					//auto prediction = Predict(Wtarget, 1100.f, 1600.f, 120.f, 0.25f);
					//ObjectManager::Player->CastSpellPos(SpellSlot_W, (DWORD)ObjectManager::Player, prediction);

					auto prediction = Prediction::BasicPrediction(Wtarget, 120.f, 900.f, 999999.f, 0.627f);
					ObjectManager::Player->CastSpellPos(SpellSlot_W, (DWORD)ObjectManager::Player, prediction);
				}

				if (BrandConfig::BrandHarass::UseE->Value && Wtarget->Position.Distance(ObjectManager::Player->Position) <= 625)
				{
					ObjectManager::Player->CastTargetSpell(kSpellSlot::SpellSlot_E, (DWORD)ObjectManager::Player, (DWORD)Wtarget, ObjectManager::Player->Position, Wtarget->Position, Wtarget->NetworkId);
				}

				if (BrandConfig::BrandHarass::UseQ->Value)
				{
					ObjectManager::Player->CastPredictSpell(kSpellSlot::SpellSlot_Q, ObjectManager::Player->Position, Wtarget->Position);
				}
			}
		}

		GameObject* Brand::GetTarget(float range)
		{
			std::vector<GameObject*> heroes;
			auto hero_list = V21::GameObject::GetHeroes();
			for (size_t i = 0; i < hero_list->size; i++)
			{
				auto hero = hero_list->entities[i];

				if (hero && hero->IsEnemy() && hero->IsValidTarget(range)) {
					heroes.push_back(hero);
				}
			}
			return TargetSelector::GetTarget(heroes, DamageType_Magical);
		}


		inline bool  Brand::HasBrandPassive(GameObject* unit)
		{
			return  unit->FindBuffName("BrandAblaze");
		}

	}
}
