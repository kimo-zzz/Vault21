#include "stdafx.h"
#include "Xerath.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"

namespace V21 {
	namespace Plugins {



		using namespace V21::SDK;
		using namespace V21::SDK::Orbwalker;

		float QRange = 0;

		namespace XerathConfig {

			namespace ChampionConfig {
				CheckBox* UseQ;
				CheckBox* UseW;
				CheckBox* UseE;
				CheckBox* UseR;

				Slider* QMana;
				Slider* WMana;
				Slider* EMana;
				Slider* RMana;
			}

			namespace XerathFarm {
				CheckBox* UseQ;
				CheckBox* UseW;
				Slider* QMana;
				Slider* EMana;
			}

			namespace XerathMisc {
				CheckBox* AutoE;
			}
		}

		void V21::Plugins::Xerath::Initialize()
		{
			auto menu = Menu::CreateMenu("Xerath", "Xerath");

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


			GameClient::PrintChat("Xerath Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void Xerath::Dispose()
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

		bool Xerath::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position) {
			if (unit == ObjectManager::Player)
			{
				if (Orbwalker::DisableNextMove && order == GameObjectOrder::MoveTo) {

					return false;
				}
			}
			return  true;
		}

		void Xerath::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			if (!castInfo)
				return;

			if (castInfo->IsAutoAttack() || !castInfo->IsChanneling())
				return;

			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];

			if (!caster->IsHero())
				return;

			if (caster->Id != ObjectManager::Player->Id)
				return;
			float castTime = spellData->CastTime;

			QRange = 750.f + (ClockFacade::GetGameTime() - castTime) * 500;
			if (QRange >= 1400.f)
				QRange = 1400.f;

		}

		void Xerath::OnPlayAnimation(GameObject* ptr, char* name, float animationTime)
		{
			if (ptr == nullptr)
				return;
		}

		void Xerath::OnFinishCast(SpellCastInfo* castInfo, GameObject* object)
		{
			if (castInfo == nullptr || object == nullptr)
				return;
		}

		void Xerath::OnStopCast(SpellCastInfo* spellCaster_Client, bool stopAnimation, bool* executeCastFrame,
			bool forceStop, bool destroyMissile, unsigned missileNetworkID)
		{
			if (spellCaster_Client == nullptr)
				return;
		}

		void Xerath::OnNewPath(GameObject* obj, Vector3* start, Vector3* end, Vector3* tail, float* dashSpeed,
			unsigned dash)
		{
			if (obj == nullptr)
				return;
		}

		void Xerath::OnCreateObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;
		}

		void Xerath::OnDeleteObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;


		}

		void Xerath::OnDraw()
		{

		}

		GameObject* Xerath::GetTarget(float radius)
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

		void Xerath::Logics::ComboLogic()
		{
			auto target = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, QRange, DamageType_Magical);

			if (!target)
				return;


		}

		void Xerath::Logics::FarmLogic()
		{
		}

		void Xerath::Logics::JungleLogic()
		{
		}

		void Xerath::OnGameUpdate()
		{
			if (Orbwalker::OrbwalkerEvading || !Orbwalker::CanCastAfterAttack())
				return;

			auto target = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 1000.0f, kDamageType::DamageType_Physical);

			switch (ActiveMode)
			{
			case OrbwalkerMode_Combo:
				Logics::ComboLogic();
				break;
			case OrbwalkerMode_LaneClear:
				Logics::FarmLogic();
				break;
			case OrbwalkerMode_JungleClear:
				Logics::JungleLogic();
				break;
			}
		}
	}
}