#include "stdafx.h"
#include "PluginTemplate.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"

namespace HACKUZAN {
	namespace Plugins {



		using namespace HACKUZAN::SDK;
		using namespace HACKUZAN::SDK::Orbwalker;

		namespace ChampionConfig {

			namespace ChampionConfig {
				CheckBox* UseQ;
				CheckBox* UseW;
				Slider* Wmana;
				CheckBox* UseE;
				Slider* EMana;
				CheckBox* UseR;
				Slider* enemiesInRange;
			}

			namespace ChampionFarm {
				CheckBox* UseQ;
				CheckBox* UseE;
				Slider* QMana;
				Slider* EMana;
			}

			namespace ChampionMisc {
				CheckBox* AutoE;
			}
		}

		void HACKUZAN::Plugins::ChampionName::Initialize()
		{
			auto menu = Menu::CreateMenu("Template", "Template");

			auto combo = menu->AddMenu("Combo", "Combo Settings");

			auto farm = menu->AddMenu("farm", "Farm Settings");


			auto misc = menu->AddMenu("misc", "Misc");


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


			GameClient::PrintChat("Template Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void ChampionName::Dispose()
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

		bool ChampionName::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position) {
			if (unit == ObjectManager::Player)
			{
				if (Orbwalker::DisableNextMove && order == GameObjectOrder::MoveTo) {

					return false;
				}
			}
			return  true;
		}

		void ChampionName::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{

			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];
		}

		void ChampionName::OnPlayAnimation(GameObject* ptr)
		{

		}

		void ChampionName::OnFinishCast(SpellCastInfo* castInfo, GameObject* object)
		{

		}

		void ChampionName::OnStopCast(GameObject* unit, StopCast* args)
		{

		}

		void ChampionName::OnNewPath(NewPath* args)
		{

		}

		void ChampionName::OnCreateObject(GameObject* unit)
		{

		}

		void ChampionName::OnDeleteObject(GameObject* unit)
		{

		}

		void ChampionName::OnDraw()
		{

		}

		GameObject* ChampionName::GetTarget(float radius)
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

		void ChampionName::OnGameUpdate()
		{
			if (Orbwalker::OrbwalkerEvading || !Orbwalker::CanCastAfterAttack())
				return;

			auto target = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 1000.0f, kDamageType::DamageType_Physical);

			switch (ActiveMode)
			{
			case OrbwalkerMode_Combo:

				break;
			}
		}

	}
}