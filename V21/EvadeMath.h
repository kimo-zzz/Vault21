#pragma once
#include "FastMath.h"
#include "Config.h"
#include "HudManager.h"

namespace HACKUZAN {

	inline int GetDangerousLevel(Vector2 Position, SArray<DetectedSKillShot> Detected, DetectedSKillShot& SkillShot)
	{
		int danger = 0;
		for (DetectedSKillShot skillshot : Detected.elems)
		{
			if (skillshot.Polygon.IsInside(Position) && EvadeSkillShotOptions[skillshot.Data->MenuName()].DangerLevel->Value > danger)
			{
				danger = EvadeSkillShotOptions[skillshot.Data->MenuName()].DangerLevel->Value;
				SkillShot = skillshot;
			}
		}
		return danger;
	}
	inline bool IsAbleWalkEvade(Vector2 Point, DetectedSKillShot skillshot, GameObject* target)
	{
		if (skillshot.IsProcess)
		{
			float distance = Distance(target->Position, ToVec3(skillshot.Start));
			int time = skillshot.Data->delay + distance * 1000 / skillshot.Data->missile_speed;
			int time2 = Distance(target, ToVec3(Point)) * 1000 / target->MoveSpeed;
			return time2 + NetClient::Instance->GetPing() + ClockFacade::GameTickCount() < time + skillshot.DetectionTime;
		}
		if (skillshot.IsMissile && skillshot.MissileObject != nullptr)
		{
			float distance = Distance(skillshot.MissileObject, target);
			int time = distance * 100 / skillshot.Data->missile_speed;
			int time2 = Distance(target, ToVec3(Point)) * 1000 / target->MoveSpeed;
			return time2 + NetClient::Instance->GetPing() < time;
		}
		return false;
	}


	inline Vector2 GetEvadePosition(SArray<DetectedSKillShot> Detected, GameObject* target, int Dangerouslevel)
	{
		/*SArray<DetectedSKillShot> detected = Detected.Where([&](DetectedSKillShot i) {return GetDodgeStage(i, Dangerouslevel); });
		DetectedSKillShot skillshot;
		int dangerouslevel = GetDangerousLevel(target->Position.To2D(), detected, skillshot);
		// an toan ne
		if (dangerouslevel == 0)
			return Vector2(0, 0);
		// nguy hiem ne
		//float distance = 10000000000000000;
		Vector2 EvadePoint = Vector2(0, 0);
		Geometry::IPolygon Polygon = skillshot.Polygon;

		// tim tat ca cac diem quen thuoc
		SArray<Vector2> Points;
		if (skillshot.Data->skillshot_type == kLine || skillshot.Data->skillshot_type == kMissileLine || skillshot.Data->skillshot_type == kMissileCone)
		{
			for (int i = 1; i < Polygon.Points.size(); i = i + 1)
			{
				Points.Add(Polygon.Points[i]);
				float dists = Distance(target->Position, ToVec3(Polygon.Points[i - 1]), ToVec3(Polygon.Points[i]));
				Vector2 middle = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular(ToVec3((Polygon.Points[i - 1] - Polygon.Points[i]))).To2D(),
					Polygon.Points[i - 1], Polygon.Points[i]), dists + 50);
				Vector2 middle2 = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular2(ToVec3((Polygon.Points[i - 1] - Polygon.Points[i]))).To2D(),
					Polygon.Points[i - 1], Polygon.Points[i]), dists + 50);
				Points.Add(middle);
				Points.Add(middle2);
				float dist = Distance(ToVec3(Polygon.Points[i - 1]), ToVec3(Polygon.Points[i]));
				// more maybe no need ?
				if (Polygon.Points.size() < 6)
				{
					for (int j = -4; j < 5; j = j + 1)
					{
						Points.Add(Extend(Polygon.Points[i - 1], middle, j * 25));
						Points.Add(Extend(Polygon.Points[i - 1], middle2, j * 25));
					}
				}

			}
			Points.Add(Polygon.Points[0]);
			float dists = Distance(target->Position, ToVec3(Polygon.Points.back()), ToVec3(Polygon.Points[0]));
			Vector2 middle = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular(ToVec3((Polygon.Points.front() - Polygon.Points.back()))).To2D(),
				Polygon.Points.front(), Polygon.Points.back()), dists + 50);
			Vector2 middle2 = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular2(ToVec3((Polygon.Points.front() - Polygon.Points.back()))).To2D(),
				Polygon.Points.front(), Polygon.Points.back()), dists + 50);
			Points.Add(middle);
			Points.Add(middle2);
			float dist = Distance(ToVec3(Polygon.Points.back()), ToVec3(Polygon.Points[0]));
			// more maybe no need ?
			if (Polygon.Points.size() < 6)
			{
				for (int j = -4; j < 5; j = j + 1)
				{
					Points.Add(Extend(Polygon.Points.back(), middle, j * 25));
					Points.Add(Extend(Polygon.Points.back(), middle2, j * 25));
				}
			}
		}
		else
		{
			for (int i = 1; i < Polygon.Points.size(); i = i + 1)
			{
				float dists = Distance(target->Position, ToVec3(Polygon.Points[i - 1]), ToVec3(Polygon.Points[i]));
				Vector2 middle = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular(ToVec3((Polygon.Points[i - 1] - Polygon.Points[i]))).To2D(),
					Polygon.Points[i - 1], Polygon.Points[i]), dists + 50);
				Vector2 middle2 = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular2(ToVec3((Polygon.Points[i - 1] - Polygon.Points[i]))).To2D(),
					Polygon.Points[i - 1], Polygon.Points[i]), dists + 50);
				Points.Add(middle);
				Points.Add(middle2);
			}
			//float dists = Distance(target->Position, ToVec3(Polygon.Points.back()), ToVec3(Polygon.Points[0]));
			//Vector2 middle = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular(ToVec3((Polygon.Points.front() - Polygon.Points.back()))).To2D(),
			//	Polygon.Points.front(), Polygon.Points.back()), dists + 50);
			//Vector2 middle2 = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular2(ToVec3((Polygon.Points.front() - Polygon.Points.back()))).To2D(),
			//	Polygon.Points.front(), Polygon.Points.back()), dists + 50);
			//Points.Add(middle);
			//Points.Add(middle2);
		}

		std::vector<Vector3> waypoint = ObjectManager::Player->GetWaypointList();
		Draw.Line(waypoint[0], waypoint[1], 1, IM_COL32(255, 69, 0, 255));
		if (Points.Any())
			EvadePoint = Points.MinOrDefault<float>([&](Vector2 i) {return DistanceSqr(ToVec3(i), target->Position); });
			EvadePoint = Points.MinOrDefault<float>([&](Vector2 i) {return DistanceSqr(ToVec3(i), Extend(waypoint[0], waypoint[1], 50)) ; });
		//kiem diem di bo ne phu hop
		DetectedSKillShot SShot;
		Points = Points.Where([&](Vector2 i) {return !NavGrid::IsWall(ToVec3(i)) && GetDangerousLevel(i, detected, SShot) < dangerouslevel ; });
		if (Points.Any())
		{
			Points = Points.OrderBy<float>([&](Vector2 i) {return Distance(i, target->Position.To2D()); });
			Points = Points.OrderBy<float>([&](Vector2 i) {return Distance(i, ToVec2(Extend(waypoint[0], waypoint[1], 50))) ; });
			EvadePoint = Points.FirstOrDefault();
			Points = Points.Where([&](Vector2 i) {return IsAbleWalkEvade(i, skillshot, target) ; });
			if (Points.Any())
			{
				Points = Points.OrderBy<float>([&](Vector2 i) {return Distance(i, HudManager::Instance->CursorTargetLogic->CursorPosition.To2D()) ; });
				EvadePoint = Points.FirstOrDefault();
			}
		}
		
		return  EvadePoint;
		return Vector2(0, 0);*/



		SArray<DetectedSKillShot> detected = Detected.Where([&](DetectedSKillShot i) {return GetDodgeStage(i, Dangerouslevel); });
		DetectedSKillShot skillshot;
		int dangerouslevel = GetDangerousLevel(target->Position.To2D(), detected, skillshot);
		// an toan ne
		if (dangerouslevel == 0)
			return Vector2(0, 0);
		// nguy hiem ne
		//float distance = 10000000000000000;
		Vector2 EvadePoint = Vector2(0, 0);
		Geometry::IPolygon Polygon = skillshot.Polygon;

		// tim tat ca cac diem quen thuoc
		SArray<Vector2> Points;
		for (int i = 1; i < Polygon.Points.size(); i = i + 1)
		{
			float dists = Distance(target->Position, ToVec3(Polygon.Points[i - 1]), ToVec3(Polygon.Points[i]));
			Vector2 middle = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular(ToVec3((Polygon.Points[i - 1] - Polygon.Points[i]))).To2D(),
				Polygon.Points[i - 1], Polygon.Points[i]), dists + 50);
			Vector2 middle2 = Extend(target->Position.To2D(), GetLineLineIntersections(target->Position.To2D(), target->Position.To2D() + Pendicular2(ToVec3((Polygon.Points[i - 1] - Polygon.Points[i]))).To2D(),
				Polygon.Points[i - 1], Polygon.Points[i]), dists + 50);
			Points.Add(middle);
			Points.Add(middle2);
		}
		//float dists = Distance(target->GetPos(), ToVec3(Polygon.Points.back()), ToVec3(Polygon.Points[0]));
		//Vec2 middle = Extend(target->GetPos()->To2D(), GetLineLineIntersections(target->GetPos()->To2D(), target->GetPos()->To2D() + Pendicular(ToVec3((Polygon.Points.front() - Polygon.Points.back()))).To2D(),
		//	Polygon.Points.front(), Polygon.Points.back()), dists + 50);
		//Points.Add(middle);
		//}

		if (Points.Any())
			EvadePoint = Points.MinOrDefault<float>([&](Vector2 i) {return DistanceSqr(ToVec3(i), target->Position); });
		//kiem diem di bo ne phu hop
		DetectedSKillShot SShot;
		Points = Points.Where([&](Vector2 i) {return !NavGrid::IsWall(ToVec3(i)) && GetDangerousLevel(i, detected, SShot) < dangerouslevel; });
		if (Points.Any())
		{
			Points = Points.OrderBy<float>([&](Vector2 i) {return Distance(i, target->Position.To2D()); });
			EvadePoint = Points.FirstOrDefault();
			Points = Points.Where([&](Vector2 i) {return IsAbleWalkEvade(i, skillshot, target); });
			if (Points.Any())
			{
				Points = Points.OrderBy<float>([&](Vector2 i) {return Distance(i, HudManager::Instance->CursorTargetLogic->CursorPosition.To2D()); });
				EvadePoint = Points.FirstOrDefault();
			}
		}
		return  EvadePoint;
		return Vector2(0, 0);
	}

	inline bool IsGettingHit(int milisecond, DetectedSKillShot skillshot, GameObject* target)
	{
		if (!skillshot.Polygon.IsInside(target->Position.To2D()))
			return false;
		if (skillshot.IsProcess)
		{
			float distance = Distance(target->Position, ToVec3(skillshot.Start));
			int time = skillshot.Data->delay + distance * 1000 / skillshot.Data->missile_speed;
			return ClockFacade::GameTickCount() + milisecond >= time + skillshot.DetectionTime;
		}
		if (skillshot.IsMissile && skillshot.MissileObject != nullptr)
		{
			float distance = Distance(skillshot.MissileObject, target);
			int time = distance * 100 / skillshot.Data->missile_speed;
			return milisecond >= time;
		}
		return false;
	}
	inline bool ShouldHoldOn(SArray<DetectedSKillShot> Detected, GameObject* target, int Dangerouslevel)
	{
		vector<Vector3> waypoint = target->GetWaypointList();
		if (waypoint.size() <= 1)
			return false;
		SArray<DetectedSKillShot> detected = Detected.Where([&](DetectedSKillShot i) {return GetDodgeStage(i, Dangerouslevel); });
		DetectedSKillShot skillshot;
		if (GetDangerousLevel(ToVec2(waypoint[0]), detected, skillshot) != 0)
			return false;
		if (GetDangerousLevel(ToVec2(Extend(waypoint[0], waypoint[1], 50)), detected, skillshot) != 0)
			return true;
		if (GetDangerousLevel(ToVec2(Extend(waypoint[0], waypoint[1], 100)), detected, skillshot) != 0)
			return true;
		if (GetDangerousLevel(ToVec2(target->Position), detected, skillshot) != 0)
			return false;
		if (GetDangerousLevel(ToVec2(Extend(target->Position, waypoint[1], 50)), detected, skillshot) != 0)
			return true;
		if (GetDangerousLevel(ToVec2(Extend(target->Position, waypoint[1], 100)), detected, skillshot) != 0)
			return true;
		return false;
	}
	inline bool ShouldBlock(SArray<DetectedSKillShot> Detected, GameObject* target, Vector3 Position, int Dangerouslevel)
	{
		SArray<DetectedSKillShot> detected = Detected.Where([&](DetectedSKillShot i) {return GetDodgeStage(i, Dangerouslevel); });
		DetectedSKillShot skillshot;
		if (GetDangerousLevel(ToVec2(Extend(target->Position, Position, 50)), detected, skillshot) != 0)
			return true;
		if (GetDangerousLevel(ToVec2(Extend(target->Position, Position, 100)), detected, skillshot) != 0)
			return true;
		return false;
	}
	inline void TriggerEvader(Evader* evader, DetectedSKillShot skillshot)
	{
		switch (evader->Type)
		{
		case Movement:
		{

			Vector2 EvadePoint = GetEvadePosition(SArray<DetectedSKillShot>().Add(skillshot), ObjectManager::Player, EvaderOptions[evader->MenuName].DangerLevel->Value);
			if (EvadePoint != Vector2(0, 0))
				ObjectManager::Player->CastSpellPos(evader->Slot, (DWORD)ObjectManager::Player, ToVec3(EvadePoint));
		}
		break;
		case SpeedBuffer:
		{
			ObjectManager::Player->CastSpell(evader->Slot, (DWORD)ObjectManager::Player);
		}
		break;
		case Invulnerability:
		{
			auto Position = ObjectManager::Player->Position.To2D().Extended(skillshot.Start, evader->Range);
			ObjectManager::Player->CastSpellPos(evader->Slot, (DWORD)ObjectManager::Player, ToVec3(Position));
		}
		break;
		case Shield:
		{
			ObjectManager::Player->CastTargetSpell(evader->Slot, (DWORD)ObjectManager::Player, (DWORD)ObjectManager::Player, ObjectManager::Player->Position, ObjectManager::Player->Position, ObjectManager::Player->NetworkId);
		}
		break;
		case Wall:
		{
			if (skillshot.Data->missile_name != "" )
			{
				ObjectManager::Player->CastSpellPos(evader->Slot, (DWORD)ObjectManager::Player, ToVec3(skillshot.Start));
			}
		}
		break;
		}
	}
}