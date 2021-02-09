#pragma once
#include "Skillshot.h"
#include "SpellDB.h"
#include "ClockFacade.h"
#include "NetClient.h"
#include "NavGrid.h"

namespace HACKUZAN {

	inline Vector3 PredGetUnitPosition(GameObject* target, float delay)
	{
		delay = delay + NetClient::Instance->GetPing() / 1000;
		auto waypoint = target->GetWaypointList();
		if (waypoint.size() == 1)
			return waypoint.front();
		if (target->IsDashing())
		{
			auto data = target->GetPathController()->GetNavigationPath();
			float dashdistance = delay * data->DashSpeed;
			return dashdistance >= Distance(target, data->EndPosition) ? data->EndPosition
				: Extend(target->AIManager()->mCurrPosition, data->EndPosition, dashdistance);
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

	inline Vector2 GetFirstCollision(Vector2 From, Vector2 To, int ColiFlag, float speed, float radius)
	{
		Vector2 CollideVec(0, 0);
		vector<GameObject*> Collisons;
		std::vector<GameObject*> AllUnits;
		if (ColiFlag & kHeroes || ColiFlag & kMinions)
		{
			std::vector<GameObject*> Heroes;
			std::vector<GameObject*> Minions;
			if (ColiFlag & kHeroes)
			{
				auto hero_list = HACKUZAN::GameObject::GetHeroes();
				for (size_t i = 0; i < hero_list->size; i++)
				{
					auto hero = hero_list->entities[i];
					if (hero) {
						if (hero->IsAlly() && hero->Alive() && hero != ObjectManager::Player) {
							AllUnits.push_back(hero);
						}
					}
				}
			}
			if (ColiFlag & kMinions)
			{
				auto minion_list = HACKUZAN::GameObject::GetMinions();
				for (size_t i = 0; i < minion_list->size; i++)
				{
					auto minion = minion_list->entities[i];
					if (minion) {
						if (minion->Alive() && minion->IsAlly() && minion->IsValidTarget(Distance(From, To) + 500, false, ToVec3(From))) {
							AllUnits.push_back(minion);
						}
					}
				}
			}

			for (GameObject* unit : AllUnits)
			{
				float delay = Distance(ToVec3(From), unit->Position) / speed;
				Vector3 pred = PredGetUnitPosition(unit, delay);
				float mindistance = unit->GetBoundingRadius() + radius / 2;
				if (/*Distance(hero->GetPosition(), ToVec3(From), ToVec3(To), true) < mindistance
					||*/ Distance(pred, ToVec3(From), ToVec3(To), true) < mindistance
					/*				|| GetSegmentSegmentIntersections(From, To, hero->GetPosition(), pred)*/)
				{
					Collisons.push_back(unit);
				}
			}
		}
		if (ColiFlag & kYasuoWall)
		{
			for (GameObject* allObj : AllUnits)
			{
				if (allObj != nullptr && allObj->Minion() && allObj->IsAlly() && allObj->IsVisible && Contains(allObj->Name, "w_windwall_enemy_")) {

					float length = 300 /*+ 5 * 5*/;
					Vector3 direction = Pendicular(Normalize((allObj->Position - PredLastYasuoWallCastPos)));
					Vector3 WallStart = ToVec3((ToVec2(allObj->Position) + length * ToVec2(direction) / 2));
					Vector3 WallEnd = ToVec3((ToVec2(allObj->Position) - length * ToVec2(direction) / 2));
					float mindistance = 50 + radius / 2 + 50;
					if (Distance(WallStart, ToVec3(From), ToVec3(To), true) < mindistance
						|| Distance(WallEnd, ToVec3(From), ToVec3(To), true) < mindistance
						|| GetSegmentSegmentIntersections(ToVec3(From), ToVec3(To), WallStart, WallEnd))
					{
						Collisons.push_back(allObj);
					}
				}
			}
		}
		if (Collisons.size() == 0)
			return CollideVec;

		//GameClient::PrintChat("collision detected!~", IM_COL32(255, 69, 0, 255));

		std::sort(Collisons.begin(), Collisons.end(), [&](GameObject* a, GameObject* b) { return Distance(a, ToVec3(From)) < Distance(b, ToVec3(From)); });
		CollideVec = Extend(From, To, Distance(From, Collisons.front()->Position.To2D()));
		return CollideVec;
	}

	inline Geometry::IPolygon GetPolygon(DetectedSKillShot skillshot, bool useBoundingRadius = false, int extraRad = 0)
	{
		if (skillshot.Data->missile_connected_to_unit)
		{
			Vector2 dir = (skillshot.End - skillshot.Start).Normalized();
			if (skillshot.IsProcess && skillshot.Caster != nullptr)
			{
				skillshot.Start = skillshot.Caster->Position.To2D();
				skillshot.End = skillshot.Start + dir * skillshot.Data->raw_range;
			}
			//auto caster = ObjectManager::Instance->ObjectsArray[skillshot.MissileObject->SourceID];
			if (skillshot.IsMissile && skillshot.Caster != nullptr)
			{
				skillshot.Start = skillshot.Caster->Position.To2D();
				skillshot.End = skillshot.Start + dir * skillshot.Data->raw_range;
			}
		}
		//
		if (skillshot.Data->missile_follows_unit)
		{

			if (skillshot.IsProcess && skillshot.Caster != nullptr)
			{
				skillshot.End = skillshot.Caster->Position.To2D();
			}
			//auto caster = ObjectManager::Instance->ObjectsArray[skillshot.MissileObject->SourceID];
			if (skillshot.IsMissile && skillshot.Caster != nullptr)
			{
				skillshot.End = skillshot.Caster->Position.To2D();
			}
		}

		int ExtraRadius = (useBoundingRadius ? (skillshot.Data->dont_add_bounding_radius ? 0 : ObjectManager::Player->GetBoundingRadius()) : 0);
		ExtraRadius = ExtraRadius + extraRad;
		if (!skillshot.IsMissile || skillshot.MissileObject == nullptr)
		{
			Vector2 CollisionPos = GetFirstCollision(skillshot.Start, skillshot.End, skillshot.Data->collision_flags, skillshot.Data->missile_speed, skillshot.Data->raw_radius);

			switch (skillshot.Data->skillshot_type)
			{
			case kRing:
				return Polygons::Ring(skillshot.End, skillshot.Data->radius_ex, skillshot.Data->raw_radius).ToPolygon(ExtraRadius);
			case kArc:
				return Polygons::Arc(skillshot.Start, skillshot.End, skillshot.Data->raw_radius).ToPolygon(ExtraRadius);
			case kLine:
				return CollisionPos == Vector2(0, 0) ?
					Polygons::Line(skillshot.Start, skillshot.End, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius):
					Polygons::Line(skillshot.Start, CollisionPos, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius);
			case kMissileLine:
				return CollisionPos == Vector2(0, 0) ?
					Polygons::Line(skillshot.Start, skillshot.End, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius):
					Polygons::Line(skillshot.Start, CollisionPos, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius);
			case kBox:
			{
				Vector2 dir = (skillshot.Start - skillshot.End).Normalized().Perpendicular();
				Vector2 start = skillshot.End - (dir * skillshot.Data->raw_radius);
				Vector2 end = skillshot.End + (dir * skillshot.Data->raw_radius);
				return Polygons::Line(start, end, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius);
			}
			case kCircle:
				return Polygons::Circle(skillshot.End, skillshot.Data->raw_radius).ToPolygon(ExtraRadius);
			case kCone:
			case kMissileCone:
				return  Polygons::Cone(skillshot.Start, (skillshot.End - skillshot.Start).Normalized(), skillshot.Data->raw_radius, skillshot.Data->raw_range).ToPolygon(ExtraRadius);
			default:
				return Polygons::Cone(skillshot.Start, (skillshot.End - skillshot.Start).Normalized(), skillshot.Data->raw_radius, skillshot.Data->raw_range).ToPolygon(ExtraRadius);
			}
		}
		else
		{
			Vector2 CollisionPos = GetFirstCollision(skillshot.MissileObject->Position.To2D(), skillshot.End, skillshot.Data->collision_flags, skillshot.Data->missile_speed, skillshot.Data->raw_radius);
			switch (skillshot.Data->skillshot_type)
			{
			case kRing:
				return Polygons::Ring(skillshot.End, skillshot.Data->radius_ex, skillshot.Data->raw_radius).ToPolygon(ExtraRadius);
			case kArc:
				return Polygons::Arc(skillshot.Start, skillshot.End, skillshot.Data->raw_radius).ToPolygon(ExtraRadius);
			case kLine:
				return CollisionPos == Vector2(0, 0) ?
					Polygons::Line(skillshot.Start, skillshot.End, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius):
					Polygons::Line(skillshot.Start, CollisionPos, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius);
			case kMissileLine:
			{
				return CollisionPos == Vector2(0, 0) ?
					Polygons::Line(skillshot.MissileObject->Position.To2D(), skillshot.End, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius):
					Polygons::Line(skillshot.MissileObject->Position.To2D(), CollisionPos, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius);
			}
			case kBox:
			{
				Vector2 dir = (skillshot.Start - skillshot.End).Normalized().Perpendicular();
				Vector2 start = skillshot.End - (dir * skillshot.Data->raw_radius);
				Vector2 end = skillshot.End + (dir * skillshot.Data->raw_radius);
				return Polygons::Line(start, end, skillshot.Data->raw_radius / 2).ToPolygon(ExtraRadius);
			}
			case kCircle:
				return Polygons::Circle(skillshot.End, skillshot.Data->raw_radius).ToPolygon(ExtraRadius);
			case kCone:
			case kMissileCone:
				return  Polygons::Cone(skillshot.Start, (skillshot.End - skillshot.Start).Normalized(), skillshot.Data->raw_radius, skillshot.Data->raw_range).ToPolygon(ExtraRadius);
			default:
				return Polygons::Cone(skillshot.Start, (skillshot.End - skillshot.Start).Normalized(), skillshot.Data->raw_radius, skillshot.Data->raw_range).ToPolygon(ExtraRadius);
			}
		}
	}

	inline bool  GetDetectedSSOnCast(SkillshotData* spelldata, SpellInfo* Args, GameObject* caster, int extraRadius)
	{
		Vector2 StartPos = Args->StartPosition.To2D();
		if (spelldata->missile_connected_to_unit)
			StartPos = caster->Position.To2D();
		Vector2 EndPos = Args->EndPosition.To2D();
		if (spelldata->missile_follows_unit)
			EndPos = caster->Position.To2D();
		if (spelldata->is_fixed_range || DistanceSqr(ToVec3(StartPos), Args->EndPosition) > pow(spelldata->raw_range, 2))
			EndPos = Extend(Args->StartPosition, Args->EndPosition, spelldata->raw_range).To2D();
		// fast calculation
		{
			Vector2 NewStart = Extend(StartPos, EndPos, -spelldata->behind_start);
			Vector2 NewEnd = Extend(EndPos, StartPos, spelldata->behind_start);
			StartPos = NewStart;
			EndPos = NewEnd;
			NewStart = Extend(StartPos, EndPos, spelldata->infront_start);
			NewEnd = Extend(EndPos, StartPos, -spelldata->infront_start);
			StartPos = NewStart;
			EndPos = NewEnd;
		}

		if (caster->IsEnemy())
		{
			DetectedSKillShot a = DetectedSKillShot(
				spelldata,
				StartPos,
				EndPos,
				ClockFacade::GameTickCount()
				, true, false, nullptr, 0, caster);
			a.Polygon = GetPolygon(a, a.Data->dont_add_bounding_radius, extraRadius);
			a.DetectionTime = ClockFacade::GameTickCount();
			DetectedSkillShots.Add(a);
		}
		return true;
	}

	inline bool GetDetectedSSOnCreateMissile(SkillshotData* missiledata, GameObject* missile)
	{
		if (missile == nullptr)
			return false;

		Vector2 StartPos = missile->MissileStartPosition.To2D();
		Vector2 EndPos = missile->MissileEndPosition.To2D();
		if (missiledata->is_fixed_range)
			EndPos = Extend(missile->MissileStartPosition, missile->MissileEndPosition, missiledata->raw_range).To2D();
		// fast calculation
		{

		}

		auto caster = ObjectManager::Instance->ObjectsArray[missile->SourceID];
		if (caster->IsEnemy())
		{
			DetectedSKillShot a = DetectedSKillShot(
				missiledata,
				StartPos,
				EndPos
				, ClockFacade::GameTickCount()
				, false, true, missile, missile->Id, caster);
			a.Polygon = GetPolygon(a, a.Data->dont_add_bounding_radius);
			a.DetectionTime = ClockFacade::GameTickCount();
			DetectedSkillShots.Add(a);
		}

		return true;
	}

	inline bool GetDetectedSSOnCreateObject(SkillshotData* missiledata, GameObject* objects)
	{
		if (objects == nullptr)
			return false;

		Vector2 StartPos = objects->Position.To2D();
		Vector2 EndPos = objects->Position.To2D();
		if (missiledata->is_fixed_range)
			EndPos = Extend(objects->Position, objects->Position, missiledata->raw_range).To2D();
		// fast calculation
		{

		}

		if (objects->IsAlly() || objects->IsEnemy()) {
			DetectedSKillShot a = DetectedSKillShot(
				missiledata,
				StartPos,
				EndPos
				, ClockFacade::GameTickCount()
				, false, true, objects, objects->Id);
			a.Polygon = GetPolygon(a, a.Data->dont_add_bounding_radius);
			a.DetectionTime = ClockFacade::GameTickCount();
			DetectedSkillShots.Add(a);
		}

		return true;
	}
}