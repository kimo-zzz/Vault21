#pragma once
#include "stdafx.h"
#include "ObjectManager.h"
#include "NetClient.h"
#include "Geometry.h"
#include "ManagerTemplate.h"


namespace V21 {

	class PredOutPut
	{
	public:
		Vector3 CastPosition;
		Vector3 UnitPosition;
		std::vector<GameObject*> CollisionObjects;
		HitChance HitsChance = HitChance::Immobile;
		PredOutPut(Vector3 castposition, Vector3 unitposition, std::vector<GameObject*> collisionobjects, HitChance hitchance)
		{
			CastPosition = castposition;
			UnitPosition = unitposition;
			CollisionObjects = collisionobjects;
			HitsChance = hitchance;
		}
		PredOutPut() {};
	};

	inline Vector3 PredGetUnitPosition(GameObject* target, float delay)
	{
		delay = delay + NetClient::Instance->GetPing() / 1000;
		std::vector<Vector3> waypoint = target->GetWaypointList();
		if (waypoint.size() == 1)
			return waypoint.front();
		if (target->IsDashing() && PredAllDashData[target->NetworkId].sender != nullptr)
		{
			auto data = PredAllDashData[target->NetworkId];
			float dashdistance = delay * data.dashSpeed;
			return dashdistance >= Distance(target, data.end) ? data.end
				: Extend(target->Position, data.end, dashdistance);
		}
		float distance = target->MoveSpeed * delay;
		for (int i = 1; i < waypoint.size(); i = i + 1)
		{
			float waydistance = Distance(waypoint[i - 1], waypoint[i]);
			if (waydistance >= distance)
			{
				return  Extend(waypoint[i - 1], waypoint[i], distance);
			}
			if (i = waypoint.size() - 1)
				return waypoint[i];
			distance = distance - waydistance;
		}
		return target->Position;
	}

	inline vector<GameObject*> PredGetCollisions(Vector3 From, Vector3 To, int ColiFlag, GameObject* target, float spelldelay, float spellspeed, float spellradius)
	{
		vector<GameObject*> Collisons;
		if (ColiFlag & kCollidesWithHeroes || ColiFlag & kCollidesWithMinions)
		{
			//GGame->PrintChat("a");
			SArray<GameObject*> Heroes;
			SArray<GameObject*> Minions;
			SArray<GameObject*> AllUnits;

			auto aibase_list = V21::GameObject::GetAIBases();
			for (size_t i = 0; i < aibase_list->size; i++)
			{
				auto unit = aibase_list->entities[i];
				if (unit && unit->Hero() && unit->IsAlly() && unit->Alive() && unit->NetworkId != target->NetworkId) {

					if (ColiFlag & kCollidesWithHeroes)
					{
						Heroes.Add(unit);
					}
				}

				if (unit && unit->Minion() && unit->IsAlly() && unit->Alive() && unit->NetworkId == target->NetworkId) {

					if (ColiFlag & kCollidesWithMinions)
					{
						if (Distance(From, unit->Position) + 500 <= 600) {
							Minions.Add(unit);
						}
					}
				}
			}

			AllUnits.AddRange(Heroes);
			AllUnits.AddRange(Minions);
			for (GameObject* hero : AllUnits.elems)
			{
				float delay = spelldelay + Distance(From, hero->Position) / spellspeed;
				Vector3 pred = PredGetUnitPosition(hero, delay);
				float mindistance = hero->GetBoundingRadius() + spellradius / 2 + 25;
				if (Distance(hero->Position, From, To, true) < mindistance
					|| Distance(pred, From, To, true) < mindistance
					|| GetSegmentSegmentIntersections(From, To, hero->Position, pred))
				{
					Collisons.push_back(hero);
				}
			}
		}
		if (ColiFlag & kCollidesWithYasuoWall)
		{
			GameObject* Wall;
			SArray<GameObject*> AllUnits;
			auto aibase_list = V21::GameObject::GetAIBases();
			for (size_t i = 0; i < aibase_list->size; i++)
			{
				auto unit = aibase_list->entities[i];
				if (unit) {
					AllUnits.Add(unit);
				}
			}
			AllUnits = AllUnits.Where([&](GameObject* i) {return i != nullptr && Contains(i->Name, "w_windwall_enemy_"); });
			if (AllUnits.Any()) {
				Wall = AllUnits.FirstOrDefault();
				float length = 300 + 5 * 5;
				Vector3 direction = Pendicular(Normalize((Wall->Position - PredLastYasuoWallCastPos)));
				Vector3 WallStart = ToVec3((ToVec2(Wall->Position) + length * ToVec2(direction) / 2));
				Vector3 WallEnd = ToVec3((ToVec2(Wall->Position) - length * ToVec2(direction) / 2));
				float mindistance = 50 + spellradius / 2 + 50;
				if (Distance(WallStart, From, To, true) < mindistance
					|| Distance(WallEnd, From, To, true) < mindistance
					|| GetSegmentSegmentIntersections(From, To, WallStart, WallEnd))
				{
					Collisons.push_back(Wall);
				}
			}
		}
		return Collisons;
	}

	inline PredOutPut PredGetPrediction(Vector3 startpos, float spellspeed, float spellrange, float spelldelay, GameObject* target, int collisionFlags, float spellradius)
	{
		if (target != nullptr) {
			PredOutPut output;
			output.HitsChance = HitChance::Impossible;
			std::vector<Vector3> waypoint = target->GetWaypointList();
			Vector3 RangeCheckFrom = startpos;
			if (waypoint.size() == 1)
			{
				output.CastPosition = waypoint[0];
				output.UnitPosition = waypoint[0];
				output.HitsChance = HitChance::High;
			}

			float speed = target->IsDashing() && PredAllDashData[target->NetworkId].sender != nullptr ? PredAllDashData[target->NetworkId].dashSpeed : target->MoveSpeed;
			float realspelldelay = spelldelay; /*> spell->Radius() / 2.f / speed ? spell->GetDelay() - spell->Radius() / 2.f / speed : 0.f;*/
			float time = 0.f;
			for (int i = 1; i < waypoint.size(); i = i + 1)
			{
				float distance = Distance(waypoint[i - 1], waypoint[i]);
				for (float j = 0; j <= distance; j = j + 5)
				{
					Vector3 Position = Extend(waypoint[i - 1], waypoint[i], j);
					//time = Vector3(Position - RangeCheckFrom).Length() / spellspeed;
					//time += realspelldelay;
					float spelldistance = Distance(RangeCheckFrom, Position);
					float targettime = time + j / speed;
					float spelltime = realspelldelay + spelldistance / spellspeed;
					if (abs(targettime - spelltime) < 10 / target->MoveSpeed)
					{
						output.CastPosition = Position;
						output.UnitPosition = Position;
						output.HitsChance = HitChance::High;
						goto ABC;
					}
				}
				time = time + distance / target->MoveSpeed;
			}
		ABC:
			if (output.HitsChance > HitChance::Impossible)
			{
				if (PredAllNewPathTicks[target->NetworkId] != 0 && ClockFacade::GameTickCount() - PredAllNewPathTicks[target->NetworkId] < 100)
					output.HitsChance = HitChance::VeryHigh;

				if (Distance(target, RangeCheckFrom) <= 300)
					output.HitsChance = HitChance::VeryHigh;

				if (target->IsDashing())
					output.HitsChance = HitChance::Dashing;

				else if (waypoint.size() == 1 && (target->FindBuffType(BuffType::Stun) || target->FindBuffType(BuffType::Knockup) || target->FindBuffType(BuffType::Knockback) || target->FindBuffType(BuffType::Charm) || target->FindBuffType(BuffType::Flee) ||
					target->FindBuffType(BuffType::Snare) || target->FindBuffType(BuffType::Fear) || target->FindBuffType(BuffType::Taunt) || target->FindBuffType(BuffType::Polymorph)))
					output.HitsChance = HitChance::Immobile;

				if (Distance(output.CastPosition, RangeCheckFrom) > spellrange && spellrange != 0)
				{
					output.HitsChance = HitChance::OutOfRange;
				}
				auto collisionobjects = PredGetCollisions(RangeCheckFrom, output.CastPosition, collisionFlags, target, spelldelay, spellspeed, spellradius);
				if (collisionobjects.size() != 0)
				{
					output.HitsChance = HitChance::Collision;
				}
				output.CollisionObjects = collisionobjects;
				return output;
			}
			output.CastPosition = waypoint.back();
			output.UnitPosition = waypoint.back();
			output.HitsChance = HitChance::Impossible;
			output.CollisionObjects = PredGetCollisions(RangeCheckFrom, output.CastPosition, collisionFlags, target, spelldelay, spellspeed, spellradius);
			return output;
		}
	}
	//cast
	inline bool CastPrediction(kSpellSlot slot, Vector3 startpos, float spellspeed, float spellrange, float spellradius, float spelldelay, GameObject* target, int collFlags, HitChance MinHitChance = HitChance::Medium)
	{
		PredOutPut pred = PredGetPrediction(startpos, spellspeed, spellrange, spelldelay, target, collFlags, spellradius);
		if (pred.HitsChance >= MinHitChance)
		{
			ObjectManager::Player->CastSpellPos(slot, (DWORD)ObjectManager::Player, pred.CastPosition);
			Draw.Line(startpos, pred.CastPosition, 2, IM_COL32(255, 255, 69, 255));
			Draw.DrawCircle3D(pred.CastPosition, 30, spellradius, IM_COL32(255, 255, 69, 255));
			return true;
		}
		return false;
	}
}