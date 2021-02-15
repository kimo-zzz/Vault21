#include "stdafx.h"
#include "Xerath.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"
#include "Prediction.h"
namespace HACKUZAN {
	namespace Plugins {



		using namespace HACKUZAN::SDK;
		using namespace HACKUZAN::SDK::Orbwalker;

		float CastTime = 0.0f;

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

		void HACKUZAN::Plugins::Xerath::Initialize()
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
			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];

			if (!caster->IsHero())
				return;

			if (caster->Id != ObjectManager::Player->Id)
				return;

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

		bool Xerath::IsEnemyInQRange()
		{
			auto heroList_ = GameObject::GetHeroes();
			for (size_t i = 0; i < heroList_->size; i++)
			{
				auto hero = heroList_->entities[i];

				if (hero && hero->IsValidTarget(1400.f, true, ObjectManager::Player->ServerPosition()));
				return true;
			}
			return false;
		}

		void Xerath::Logics::ComboLogic()
		{


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


			if (ObjectManager::Player->FindBuffName("XerathArcanopulseChargeUp"))
			{
				CastTime = ObjectManager::Player->FindBuffName("XerathArcanopulseChargeUp")->StartTime;
			}
			else
				CastTime = 0.0f;

			float qRange_ = 0.0f;

			if (CastTime > 0.0f)
			{
				qRange_ = 750.f + (ClockFacade::GetGameTime() - CastTime) * 500;
				if (qRange_ >= 1400.f)
					qRange_ = 1400.f;


				if (!IsEnemyInQRange())
					return;

				ObjectManager::Player->CastSpell(SpellSlot_Q, (DWORD)ObjectManager::Player);

				auto chargingTarget = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, qRange_, kDamageType::DamageType_Physical);

				if (chargingTarget != nullptr && Distance(chargingTarget, ObjectManager::Player) <= qRange_)
				{
					GameClient::PrintChat("IN RANGE", 255);
					auto prediction = PredGetPrediction(ObjectManager::Player->ServerPosition(), FLT_MAX, qRange_, 0.5f, chargingTarget, SpellCollisionFlags::kYasuoWall, 290.f);

					ObjectManager::Player->UpdateChargeableSpell(SpellSlot_Q, &prediction.CastPosition, true);
				}
				else if (chargingTarget == nullptr && qRange_ == 1400.f)
					ObjectManager::Player->UpdateChargeableSpell(SpellSlot_Q, &HudManager::Instance->CursorTargetLogic->CursorPosition, true);

			}

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