#include "stdafx.h"
#include "HealthPrediction.h"
#include "EventManager.h"
#include "ClockFacade.h"
#include "ObjectManager.h"
#include "Damage.h"
#include "NetClient.h"
#include "Renderer.h"

namespace HACKUZAN {
	namespace SDK {
		std::vector<IncomingAttack> HealthPrediction::IncomingAttacks;

		IncomingAttack::IncomingAttack(GameObject* source, GameObject* target, Vector3 sourcePosition, float Delay, float AnimationTime, float missileSpeed, float startTime, float damage, GameObjectCombatType type) {
			this->Source = source;
			this->Target = target;
			this->SourcePosition = sourcePosition;
			this->Delay = Delay;
			this->AnimationTime = AnimationTime;
			this->MissileSpeed = missileSpeed;
			this->StartTime = startTime;
			this->Damage = damage;
			this->Processed = false;
			this->CombatType = type;
		}

		void HealthPrediction::Initialize() {
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteMissile, OnDeleteMissile);
			EventManager::AddEventHandler(LeagueEvents::OnDoCast, OnDoCast);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnStopCast, OnStopCast);
		}

		void HealthPrediction::OnGameUpdate() {
			auto time = ClockFacade::GameTickCount();
			for (auto it = IncomingAttacks.begin(); it != IncomingAttacks.end();) {
				if (it->StartTime < ClockFacade::GameTickCount() - 3000) {
					IncomingAttacks.erase(it);
				}
				else
				{
					it++;
				}
			}
		}

		void HealthPrediction::OnDeleteMissile(GameObject* unit) {
			auto caster = ObjectManager::Instance->ObjectsArray[unit->SourceID];
			if (unit != nullptr && caster != nullptr && caster->IsAlly()) {
				for (auto it = IncomingAttacks.begin(); it != IncomingAttacks.end(); it++) {
					if (caster->Id == it->Source->Id) {
						it->Processed = true;
					}
				}
			}
		}

		void HealthPrediction::OnDoCast(SpellInfo* castInfo, SpellDataResource* spellData) {
			if (castInfo == nullptr)
				return;
			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];
			for (auto it = IncomingAttacks.begin(); it != IncomingAttacks.end(); it++) {
				if (caster && caster->Id == it->Source->Id && caster->IsMelee()) {
					it->Processed = true;
				}
			}
		}

		void HealthPrediction::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];
			auto target = ObjectManager::Instance->ObjectsArray[castInfo->TargetId];
			if (caster && caster != ObjectManager::Player && caster->IsAlly() && target && target->Minion() && castInfo->IsAutoAttack() && ObjectManager::Player->Position.IsInRange(caster->Position, 2000.0f)) {
				for (auto it = IncomingAttacks.begin(); it != IncomingAttacks.end();) {
					if (caster->Id == it->Source->Id) {
						IncomingAttacks.erase(it);
						//it->Processed = false;
					}
					else
					{
						it++;
					}
				}
				IncomingAttacks.push_back(IncomingAttack(caster, target, castInfo->StartPosition, caster->GetAttackCastDelay() * 1000, caster->GetAttackDelay() * 1000 - (caster->Turret() ? 70 : 0), caster->IsMelee() ? FLT_MAX : spellData->MissileSpeed, ClockFacade::GameTickCount() - NetClient::Instance->GetPing() / 2, (float)Damage::CalculateAutoAttackDamage(caster, target), caster->CombatType));
			}
		}

		void HealthPrediction::OnStopCast(GameObject* unit, StopCast* args) {
			if (unit != nullptr && args->stopAnimation) {
				for (auto it = IncomingAttacks.begin(); it != IncomingAttacks.end();) {
					if (it->Source->Id == unit->Id) {
						IncomingAttacks.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
		}

		float HealthPrediction::GetHealthPrediction(GameObject* unit, int time, int delay = 70)
		{
			auto predictedDamage = 0.0f;

			for (auto attack : IncomingAttacks)
			{
				auto attackDamage = 0.0f;
				if (!attack.Processed && unit != nullptr && unit->Id == attack.Target->Id) {

					auto landTime = attack.StartTime + attack.Delay
						+ 1000 * std::max(0.f, unit->Position.Distance(attack.Source->Position) - attack.Source->GetBoundingRadius())
						/ attack.MissileSpeed + delay;

					if (landTime < ClockFacade::GameTickCount() + time)
					{
						attackDamage = attack.Damage;

					}
				}

				predictedDamage += attackDamage;
			}

			return unit->Health - predictedDamage;
		}

		float HealthPrediction::LaneClearHealthPrediction(GameObject* unit, int time, int delay = 70)
		{
			auto predictedDamage = 0.0f;

			for (auto attack : IncomingAttacks)
			{
				auto n = 0;
				if (ClockFacade::GameTickCount() - 100 <= attack.StartTime + attack.AnimationTime
					&& unit != nullptr && unit->Id == attack.Target->Id)
				{
					auto fromT = attack.StartTime;
					auto toT = ClockFacade::GameTickCount() + time;

					while (fromT < toT)
					{
						if (fromT >= ClockFacade::GameTickCount()
							&& (fromT + attack.Delay
								+ std::max(0.0f, unit->Position.Distance(attack.Source->Position) - attack.Source->GetBoundingRadius())
								/ attack.MissileSpeed < toT))

						{
							n++;
						}
						fromT += (int)attack.AnimationTime;
					}
				}
				predictedDamage += n * attack.Damage;
			}

			return unit->Health - predictedDamage;
		}
	}
}