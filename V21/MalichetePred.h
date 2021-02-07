/*#pragma once
#include "stdafx.h"
#include "NetClient.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Geometry.h"

namespace HACKUZAN {

	//std::map<int, UnitDash*> MalachitePredAllDashData;
	std::map <int, int> MalachitePredAllNewPathTicks;
	Vector3 MalachitePredLastYasuoWallCastPos;
	class MalachitePredOutPut
	{
	public:
		Vector3 CastPosition;
		Vector3 UnitPosition;
		vector<GameObject*> CollisionObjects;
		HitChance HitsChance = HitChance::Immobile;
		MalachitePredOutPut(Vector3 castposition, Vector3 unitposition, vector<GameObject*> collisionobjects, HitChance hitchance)
		{
			CastPosition = castposition;
			UnitPosition = unitposition;
			CollisionObjects = collisionobjects;
			HitsChance = hitchance;
		}
		MalachitePredOutPut() {};
	};
	inline Vector3 MalachitePredGetUnitPosition(GameObject* target, float delay)
	{
		delay = delay + NetClient::Instance->GetPing() / 1000;
		auto waypoint = target->GetWaypointList();
		if (waypoint.size() == 1)
			return waypoint.front();
		if (target->IsDashing())
		{
			auto data = MalachitePredAllDashData[target->NetworkId];
			float dashdistance = delay * data->Speed;
			return dashdistance >= Distance(target, data->EndPosition) ? data->EndPosition
				: Extend(target->Position, data->EndPosition, dashdistance);
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
	inline vector<GameObject*> MalachitePredGetCollisions(Vector3 From, Vector3 To, int ColiFlag, GameObject* target, float spellRadius, float spellSpeed, float spellDelay)
	{
		vector<GameObject*> Collisons;
		if (ColiFlag & kCollidesWithHeroes || ColiFlag & kCollidesWithMinions)
		{
			//GGame->PrintChat("a");
			SArray<GameObject*>* Heroes;
			SArray<GameObject*>* Minions;
			SArray<GameObject*>* AllUnits;
			if (ColiFlag & kCollidesWithHeroes)
			{
				Heroes = &HACKUZAN::GameObject::SArrayHeroes()->Where([&](GameObject* i) {return i != nullptr && i->Alive() && i->NetworkId != target->NetworkId; });
			}
			if (ColiFlag & kCollidesWithMinions)
			{
				//GGame->PrintChat("b");
				Minions = &HACKUZAN::GameObject::SArrayMinions()->Where([&](GameObject* i) {return i != nullptr && i->Alive() && i->IsAlly() && i->IsValidTarget(Distance(From, To) + 500, false, From); });
				Minions->RemoveAll([&](GameObject* i) {return i->NetworkId == target->NetworkId; });
				//GGame->PrintChat(std::to_string(Minions.Count()).c_str());
			}
			AllUnits->AddRange(*Heroes);
			AllUnits->AddRange(*Minions);
			for (GameObject* hero : AllUnits->elems)
			{
				float delay = spellDelay + Distance(From, hero->Position) / spellSpeed;
				Vector3 pred = MalachitePredGetUnitPosition(hero, delay);
				float mindistance = hero->GetBoundingRadius() + spellRadius / 2 + 25;
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
			SArray<GameObject*>* AllUnits;
			AllUnits = &HACKUZAN::GameObject::SArrayMinions()->Where([&](GameObject* i) {return i != nullptr && i->Alive() && i->IsAlly() && Contains(i->Name, "w_windwall_enemy_"); });
			if (AllUnits->Any())
			{
				Wall = AllUnits->FirstOrDefault();
				float length = 300 + 5 * 5;
				Vector3 direction = Pendicular(Normalize((Wall->Position - MalachitePredLastYasuoWallCastPos)));
				Vector3 WallStart = ToVec3((ToVec2(Wall->Position) + length * ToVec2(direction) / 2));
				Vector3 WallEnd = ToVec3((ToVec2(Wall->Position) - length * ToVec2(direction) / 2));
				float mindistance = 50 + spellRadius / 2 + 50;
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

	inline MalachitePredOutPut MalachitePredGetPrediction(ISpell2* spell, GameObject* target)
	{
		MalachitePredOutPut output;
		output.HitChance = kHitChanceImpossible;
		auto waypoint = target->GetWaypointList();
		Vector3 RangeCheckFrom = spell->GetRangeCheckFrom() == Vector3(0, 0, 0) ? GEntityList->Player()->GetPosition() : spell->GetRangeCheckFrom();
		if (waypoint.size() == 1)
		{
			output.CastPosition = waypoint[0];
			output.UnitPosition = waypoint[0];
			output.HitChance = kHitChanceHigh;
		}
		float speed = target->IsDashing() ? MalachitePredAllDashData[target->GetNetworkId()]->Speed : target->MovementSpeed();
		float realspelldelay = spell->GetDelay(); /*> spell->Radius() / 2.f / speed ? spell->GetDelay() - spell->Radius() / 2.f / speed : 0.f;
		float time = 0.f;
		for (int i = 1; i < waypoint.size(); i = i + 1)
		{
			float distance = Distance(waypoint[i - 1], waypoint[i]);
			for (float j = 0; j <= distance; j = j + 5)
			{
				Vector3 Position = Extend(waypoint[i - 1], waypoint[i], j);

				float spelldistance = Distance(RangeCheckFrom, Position);
				float targettime = time + j / speed;
				float spelltime = realspelldelay + spelldistance / spell->Speed();
				if (abs(targettime - spelltime) < 10 / target->MovementSpeed())
				{
					output.CastPosition = Position;
					output.UnitPosition = Position;
					output.HitChance = kHitChanceHigh;
					goto ABC;
				}
			}
			time = time + distance / target->MovementSpeed();
		}
	ABC:
		if (output.HitChance > kHitChanceImpossible)
		{
			if (GGame->TickCount() - MalachitePredAllNewPathTicks[target->GetNetworkId()] < 100)
				output.HitChance = kHitChanceVeryHigh;
			if (Distance(target, RangeCheckFrom) <= 300)
				output.HitChance = kHitChanceVeryHigh;
			if (target->IsDashing())
				output.HitChance = kHitChanceDashing;
			else if (waypoint.size() == 1 && IsCCed(target))
				output.HitChance = kHitChanceImmobile;
			if (Distance(output.CastPosition, RangeCheckFrom) > spell->Range() && spell->Range() != 0)
			{
				output.HitChance = kHitChanceOutOfRange;
			}
			auto collisionobjects = MalachitePredGetCollisions(RangeCheckFrom, output.CastPosition, spell->GetCollisionFlags(), target, spell);
			if (collisionobjects.size() != 0)
			{
				output.HitChance = kHitChanceCollision;
			}
			output.CollisionObjects = collisionobjects;
			return output;
		}
		output.CastPosition = waypoint.back();
		output.UnitPosition = waypoint.back();
		output.HitChance = kHitChanceImpossible;
		output.CollisionObjects = MalachitePredGetCollisions(RangeCheckFrom, output.CastPosition, spell->GetCollisionFlags(), target, spell);
		return output;
	}
	//cast
	inline bool MalachiteCast(ISpell2* spell, GameObject* target, ePredictionChance MinHitChane = kHitChanceHigh)
	{
		MalachitePredOutPut pred = MalachitePredGetPrediction(spell, target);
		if (pred.HitChance >= MinHitChane)
		{
			spell->CastOnPosition(pred.CastPosition);
			return true;
		}
		return false;
	}

	PLUGIN_EVENT(void) MalachitePredOnSpellCast(CastedSpell const& Args)
	{
		if (Args.Caster_ != nullptr && Args.Caster_->IsEnemy(GEntityList->Player()) && Contains(GSpellData->GetSpellName(Args.Data_), "YasuoWMovingWall", false))
		{
			MalachitePredLastYasuoWallCastPos = Args.Caster_->GetPosition();
		}
	}
	PLUGIN_EVENT(void) MalachitePredOnDash(UnitDash* Args)
	{
		if (Args->Source == nullptr)
			return;
		MalachitePredAllDashData[Args->Source->GetNetworkId()] = Args;
	}
	PLUGIN_EVENT(void) MalachitePredOnNewPath(GameObject* Source, const std::vector<Vector3>& Path)
	{
		if (Source == nullptr)
			return;
		MalachitePredAllNewPathTicks[Source->GetNetworkId()] = GGame->TickCount();
	}

	inline void MalachitePredOnload()
	{
		GEventManager->AddEventHandler(kEventOnDash, MalachitePredOnDash);
		GEventManager->AddEventHandler(kEventOnSpellCast, MalachitePredOnSpellCast);
		GEventManager->AddEventHandler(kEventOnNewPath, MalachitePredOnNewPath);
	}
	inline void MalachitePredUnload()
	{
		GEventManager->RemoveEventHandler(kEventOnDash, MalachitePredOnDash);
		GEventManager->RemoveEventHandler(kEventOnSpellCast, MalachitePredOnSpellCast);
		GEventManager->RemoveEventHandler(kEventOnNewPath, MalachitePredOnNewPath);
	}
}





_DWORD *__thiscall sub_6D56A0(_DWORD *this)
{
  _DWORD *v1; // edi
  int v2; // esi
  int v3; // edx
  int v4; // eax
  int v5; // eax
  int v6; // eax
  int v7; // eax
  int v8; // eax
  int v9; // eax
  int v10; // eax
  int v11; // eax
  __m128i *v12; // eax
  __m128i *v13; // esi
  int v14; // eax
  int v15; // eax
  int v16; // eax
  _DWORD *v17; // eax
  _DWORD *v18; // eax
  _DWORD *v19; // eax
  _DWORD *v20; // eax
  _DWORD *v21; // eax
  _DWORD *v22; // eax
  _DWORD *v23; // eax
  int v24; // esi
  int v25; // eax
  int v26; // eax
  int v27; // eax
  _DWORD *v28; // eax
  _DWORD *v29; // eax
  _DWORD *v30; // eax
  int v31; // eax
  int v32; // eax
  int v33; // eax
  _DWORD *v34; // eax
  _DWORD *v35; // esi
  int v36; // eax
  int v37; // eax
  int v38; // eax
  _DWORD *v39; // eax
  _DWORD *v40; // eax
  _DWORD *v41; // esi
  int v42; // eax
  _DWORD *v43; // eax
  _DWORD *v44; // eax
  _DWORD *v45; // eax
  int v46; // esi
  int v47; // eax
  _DWORD *v48; // eax
  int v49; // esi
  int v50; // eax
  _OWORD *v51; // eax
  int v52; // ecx
  unsigned __int64 v53; // rax
  unsigned int v54; // edx
  unsigned __int64 v55; // rax
  unsigned int v56; // edx  
  int v57; // eax
  int v58; // eax
  int v59; // eax
  int v60; // eax
  int v61; // eax
  int v62; // eax
  _DWORD *v63; // eax
  _DWORD *v64; // esi
  int v65; // eax
  int v66; // eax
  _DWORD *v67; // eax
  _DWORD *v68; // esi
  int v69; // eax
  int v70; // eax
  int v71; // eax
  int v72; // eax
  int v73; // eax
  int v74; // eax
  int v75; // eax
  int v76; // eax
  int v77; // eax
  int v78; // eax
  int v79; // eax
  int v80; // eax
  int v81; // eax
  int v82; // eax
  int v83; // eax
  int v85; // eax
  int v86; // eax
  int v87; // eax
  int v88; // eax
  int v89; // edx
  int v90; // eax
  int v91; // eax
  int v92; // eax
  int v93; // eax
  float v94; // xmm0_4
  char v95; // [esp+Fh] [ebp-85h]
  int v96; // [esp+10h] [ebp-84h]
  int v97; // [esp+14h] [ebp-80h]
  int v98; // [esp+18h] [ebp-7Ch]
  int v99; // [esp+1Ch] [ebp-78h]
  int v100; // [esp+20h] [ebp-74h]
  int v101; // [esp+24h] [ebp-70h]
  int v102; // [esp+28h] [ebp-6Ch]
  int v103; // [esp+2Ch] [ebp-68h]
  int v104; // [esp+30h] [ebp-64h]
  char v105; // [esp+3Ch] [ebp-58h]
  int v106; // [esp+4Ch] [ebp-48h]
  const char *v107; // [esp+50h] [ebp-44h]
  char v108; // [esp+68h] [ebp-2Ch]
  char v109; // [esp+8Ch] [ebp-8h]

  v1 = this;
  *this = off_1901B9C;
  dword_38CA828 = (int)this;
  sub_6D4720(this + 2);
  *v1 = &off_1901BA0;
  v1[2] = off_1901BA4;
  v1[44] = 0;
  *((_BYTE *)v1 + 180) = 0;
  v2 = dword_38CA1A4;
  if ( *(_BYTE *)(dword_38CA1A4 + 100) )
  {
    if ( *(char **)(dword_38CA1A4 + 28) != "gameplay" )
    {
      *(_DWORD *)(v2 + 8) = sub_E58C30();
      *(_DWORD *)(v2 + 12) = v3;
      sub_E56680("gameplay");
      *(_DWORD *)(v2 + 28) = "gameplay";
      sub_E56B70(v2 + 32);
      *(_DWORD *)(v2 + 44) = 0;
      sub_E56B70(v2 + 48);
      *(_DWORD *)(v2 + 60) = 0;
      v2 = dword_38CA1A4;
    }
    if ( *(_BYTE *)(v2 + 100) && *(char **)(v2 + 44) != "game_start" )
    {
      sub_E56680("game_start");
      *(_DWORD *)(v2 + 44) = "game_start";
      sub_E56B70(v2 + 48);
      *(_DWORD *)(v2 + 60) = 0;
    }
  }
  sub_6E02C0("Gameplay.bin", "Gameplay.SceneData.bin", 0);
  v4 = sub_E55D10(304);
  if ( v4 )
    v5 = sub_8CBD40(v4);
  else
    v5 = 0;
  MissionInfo_Instance = v5;
  sub_8D9090(v5);
  v6 = sub_E55D10(252);
  if ( v6 )
    v7 = sub_672560(v6);
  else
    v7 = 0;
  dword_38C90DC = v7;
  sub_9D2F00();
  v8 = sub_E55D10(128);
  if ( v8 )
    v9 = sub_703D40(v8);
  else
    v9 = 0;
  dword_202D614 = v9;
  sub_7077E0();
  sub_CC3D40(dword_202D64C);
  v10 = sub_E55D10(280);
  if ( v10 )
    v11 = sub_5FFD90(v10);
  else
    v11 = 0;
  dword_38C1A38 = v11;
  v12 = (__m128i *)sub_E55D10(24);
  v13 = v12;
  if ( v12 )
  {
    *v12 = 0i64;
    _mm_storel_epi64(v12 + 1, (__m128i)0i64);
    sub_C8DED0(v12);
    v13->m128i_i32[0] = (__int32)&off_1902AC8;
  }
  else
  {
    v13 = 0;
  }
  LOBYTE(v96) = sub_713DE0();
  if ( sub_E55D10(200) )
    v14 = sub_5FFD70(1);
  else
    v14 = 0;
  sub_C91150(v14, v13, v96);
  v15 = sub_E55D10(128);
  if ( v15 )
    v16 = sub_7033B0(v15);
  else
    v16 = 0;
  dword_202AF38 = v16;
  v17 = (_DWORD *)sub_E55D10(48);
  if ( v17 )
  {
    v17[2] = 0;
    v17[3] = 0;
    v17[1] = 0;
    v17[2] = 0;
    v17[3] = 0;
    *v17 = &off_1903580;
    v17[4] = 1034147594;
    v17[5] = 1040187392;
    v17[6] = 0;
    v17[7] = -1;
    v17[8] = 0;
    v17[9] = 0;
    v17[10] = 0;
    v17[11] = 0;
  }
  else
  {
    v17 = 0;
  }
  dword_2C79C04 = (int)v17;
  v18 = (_DWORD *)sub_E55D10(16);
  if ( v18 )
  {
    v18[2] = 0;
    v18[3] = 0;
    v18[1] = 0;
    v18[2] = 0;
    v18[3] = 0;
    *v18 = &off_1903608;
  }
  else
  {
    v18 = 0;
  }
  Inhib_List_38CA89C = (int)v18;
  v19 = (_DWORD *)sub_E55D10(16);
  if ( v19 )
  {
    v19[2] = 0;
    v19[3] = 0;
    v19[1] = 0;
    v19[2] = 0;
    v19[3] = 0;
    *v19 = &off_1903620;
  }
  else
  {
    v19 = 0;
  }
  dword_2C7AE88 = (int)v19;
  v20 = (_DWORD *)sub_E55D10(16);
  if ( v20 )
  {
    v20[2] = 0;
    v20[3] = 0;
    v20[1] = 0;
    v20[2] = 0;
    v20[3] = 0;
    *v20 = &off_1903630;
  }
  else
  {
    v20 = 0;
  }
  Turret_List_38C12B8 = (int)v20;
  v21 = (_DWORD *)sub_E55D10(16);
  if ( v21 )
  {
    v21[2] = 0;
    v21[3] = 0;
    v21[1] = 0;
    v21[2] = 0;
    v21[3] = 0;
    *v21 = &off_19035A0;
  }
  else
  {
    v21 = 0;
  }
  dword_2C7AEE4 = (int)v21;
  v22 = (_DWORD *)sub_E55D10(16);
  if ( v22 )
  {
    v22[2] = 0;
    v22[3] = 0;
    v22[1] = 0;
    v22[2] = 0;
    v22[3] = 0;
    *v22 = &off_19035F8;
  }
  else
  {
    v22 = 0;
  }
  dword_202AF2C = (int)v22;
  v23 = (_DWORD *)sub_E55D10(12);
  v24 = (int)v23;
  if ( v23 )
  {
    v23[2] = 0;
    dword_2C79C34 = (int)v23;
    *v23 = off_19035D0;
    v23[1] = 0;
    v23[2] = 0;
    v25 = sub_E55D10(24);
    *(_DWORD *)v25 = v25;
    *(_DWORD *)(v25 + 4) = v25;
    *(_DWORD *)(v25 + 8) = v25;
    *(_WORD *)(v25 + 12) = 257;
    *(_DWORD *)(v24 + 4) = v25;
  }
  else
  {
    v24 = 0;
  }
  dword_2C79C34 = v24;
  v26 = sub_E55D10(60);
  if ( v26 )
    v27 = sub_588000(v26);
  else
    v27 = 0;
  RealTurret_List__38C12B4 = v27;
  v28 = (_DWORD *)sub_E55D10(48);
  if ( v28 )
    v29 = sub_51FB30(v28);
  else
    v29 = 0;
  Minion_List_2C79C24 = (int)v29;
  v30 = (_DWORD *)sub_E55D10(20);
  if ( v30 )
  {
    v30[2] = 0;
    v30[3] = 0;
    v30[1] = 0;
    v30[2] = 0;
    v30[3] = 0;
    *v30 = &off_19035B0;
    v30[4] = 0;
  }
  else
  {
    v30 = 0;
  }
  dword_2C79C28 = (int)v30;
  v31 = sub_E55D10(156);
  if ( v31 )
    v32 = sub_65BBC0(v31);
  else
    v32 = 0;
  dword_202D610 = v32;
  v33 = sub_E55D10(32);
  if ( v33 )
  {
    *(_DWORD *)(v33 + 8) = 0;
    *(_DWORD *)(v33 + 12) = 0;
    *(_WORD *)(v33 + 29) = 0;
    *(_BYTE *)(v33 + 31) = 0;
    *(_DWORD *)(v33 + 4) = 0;
    *(_DWORD *)(v33 + 8) = 0;
    *(_DWORD *)(v33 + 12) = 0;
    *(_DWORD *)v33 = &off_1903590;
    *(_DWORD *)(v33 + 16) = 1051361018;
    *(_DWORD *)(v33 + 20) = 0;
    *(_DWORD *)(v33 + 24) = 0;
    *(_BYTE *)(v33 + 28) = 0;
  }
  else
  {
    v33 = 0;
  }
  Hero_List_202B580 = v33;
  v34 = (_DWORD *)sub_E55D10(24);
  v35 = v34;
  if ( v34 )
  {
    v34[2] = 0;
    v34[3] = 0;
    v34[4] = 0;
    v34[5] = 0;
    LevelProp_202AF30 = (int)v34;
    *v34 = off_1903618;
    v34[1] = 0;
    v34[2] = 0;
    v36 = sub_E55D10(24);
    *(_DWORD *)v36 = v36;
    *(_DWORD *)(v36 + 4) = v36;
    *(_DWORD *)(v36 + 8) = v36;
    *(_WORD *)(v36 + 12) = 257;
    v35[1] = v36;
    v35[3] = 0;
    v35[4] = 0;
    v35[5] = 0;
  }
  else
  {
    v35 = 0;
  }
  LevelProp_202AF30 = (int)v35;
  v37 = sub_E55D10(20);
  if ( v37 )
    v38 = sub_A6AF80(v37);
  else
    v38 = 0;
  dword_38C1A34 = v38;
  v39 = (_DWORD *)sub_E55D10(16);
  if ( v39 )
  {
    v39[2] = 0;
    v39[3] = 0;
    v39[1] = 0;
    v39[2] = 0;
    v39[3] = 0;
    *v39 = &off_19035C0;
  }
  else
  {
    v39 = 0;
  }
  dword_2C79C2C = (int)v39;
  v40 = (_DWORD *)sub_E55D10(24);
  v41 = v40;
  if ( v40 )
  {
    v40[2] = 0;
    v40[3] = 0;
    v40[4] = 0;
    v40[5] = 0;
    dword_202B57C = (int)v40;
    *v40 = off_1903568;
    v40[1] = 0;
    v40[2] = 0;
    v42 = sub_E55D10(48);
    *(_DWORD *)v42 = v42;
    *(_DWORD *)(v42 + 4) = v42;
    *(_DWORD *)(v42 + 8) = v42;
    *(_WORD *)(v42 + 12) = 257;
    v41[1] = v42;
    v41[3] = 0;
    v41[4] = 0;
    v41[5] = 0;
  }
  else
  {
    v41 = 0;
  }
  dword_202B57C = (int)v41;
  v43 = (_DWORD *)sub_E55D10(16);
  if ( v43 )
  {
    v43[2] = 0;
    v43[3] = 0;
    v43[1] = 0;
    v43[2] = 0;
    v43[3] = 0;
    *v43 = &off_19035D8;
  }
  else
  {
    v43 = 0;
  }
  Missile_List_38C848C = (int)v43;
  v44 = (_DWORD *)sub_E55D10(16);
  if ( v44 )
  {
    v44[2] = 0;
    v44[3] = 0;
    v44[1] = 0;
    v44[2] = 0;
    v44[3] = 0;
    *v44 = &off_1903570;
  }
  else
  {
    v44 = 0;
  }
  dword_202D658 = (int)v44;
  v45 = (_DWORD *)sub_E55D10(12);
  v46 = (int)v45;
  if ( v45 )
  {
    v45[2] = 0;
    dword_38CA88C = (int)v45;
    *v45 = off_1902AB4;
    v45[1] = 0;
    v45[2] = 0;
    v47 = sub_E55D10(20);
    *(_DWORD *)v47 = v47;
    *(_DWORD *)(v47 + 4) = v47;
    *(_DWORD *)(v47 + 8) = v47;
    *(_WORD *)(v47 + 12) = 257;
    *(_DWORD *)(v46 + 4) = v47;
  }
  else
  {
    v46 = 0;
  }
  dword_38CA88C = v46;
  v48 = (_DWORD *)sub_E55D10(12);
  v49 = (int)v48;
  if ( v48 )
  {
    v48[2] = 0;
    dword_202D68C = (int)v48;
    *v48 = off_1902ABC;
    v48[1] = 0;
    v48[2] = 0;
    v50 = sub_E55D10(20);
    *(_DWORD *)v50 = v50;
    *(_DWORD *)(v50 + 4) = v50;
    *(_DWORD *)(v50 + 8) = v50;
    *(_WORD *)(v50 + 12) = 257;
    *(_DWORD *)(v49 + 4) = v50;
  }
  else
  {
    v49 = 0;
  }
  dword_202D68C = v49;
  v51 = (_OWORD *)sub_E55D10(24);
  v52 = (int)v51;
  if ( v51 )
  {
    v95 = 0;
    *v51 = 0i64;
    v53 = __rdtsc();
    _mm_storel_epi64((__m128i *)(v52 + 16), (__m128i)0i64);
    dword_38C8434 = v52;
    *(_DWORD *)v52 = off_1902AC4;
    *(_WORD *)(v52 + 5) = 256;
    *(_BYTE *)(v52 + 7) = v53;
    v99 = HIDWORD(v53);
    v54 = 0;
    *(_BYTE *)(v52 + 8) = 0;
    *(_BYTE *)(v52 + 4) = 1;
    do
    {
      *(&v95 + v54) ^= ~*(_BYTE *)(v54 + v52 + 7);
      ++v54;
    }
    while ( v54 < 1 );
    *(_BYTE *)(v52 + 10) = v95;
    *(_BYTE *)(v52 + 8) = 1;
    v55 = __rdtsc();
    *(_WORD *)(v52 + 14) = 256;
    *(_BYTE *)(v52 + 16) = v55;
    v99 = HIDWORD(v55);
    v56 = 0;
    *(_BYTE *)(v52 + 17) = 0;
    *(_BYTE *)(v52 + 13) = 1;
    v95 = 0;
    do
    {
      *(&v95 + v56) ^= ~*(_BYTE *)(v56 + v52 + 16);
      ++v56;
    }
    while ( v56 < 1 );
    *(_BYTE *)(v52 + 19) = v95;
    *(_BYTE *)(v52 + 17) = 1;
  }
  else
  {
    v52 = 0;
  }
  dword_38C8434 = v52;
  v57 = sub_E55D10(16);
  if ( v57 )
    v58 = sub_CD4330(v57);
  else
    v58 = 0;
  dword_38CA84C = v58;
  ++*(_DWORD *)(MissionInfo_Instance + 204);
  sub_618840();
  sub_637AB0();
  sub_713730();
  v59 = sub_E55D10(1712);
  if ( v59 )
    v60 = sub_8901A0(v59);
  else
    v60 = 0;
  SpellDatabaseClient_Instance = v60;
  v61 = sub_E55D10(396);
  if ( v61 )
    v62 = sub_5FF830(v61);
  else
    v62 = 0;
  dword_38C3464 = v62;
  v63 = (_DWORD *)sub_E55D10(20);
  v64 = v63;
  if ( v63 )
  {
    v63[2] = 0;
    v63[3] = 0;
    v63[4] = 0;
    dword_38C54BC = (int)v63;
    *v63 = off_19035F0;
    v63[1] = 0;
    v63[2] = 0;
    v65 = sub_E55D10(32);
    *(_DWORD *)v65 = v65;
    *(_DWORD *)(v65 + 4) = v65;
    *(_DWORD *)(v65 + 8) = v65;
    *(_WORD *)(v65 + 12) = 257;
    v64[1] = v65;
    v64[3] = 0;
    v64[4] = 0;
    v66 = sub_E55D10(32);
    *(_DWORD *)v66 = v66;
    *(_DWORD *)(v66 + 4) = v66;
    *(_DWORD *)(v66 + 8) = v66;
    *(_WORD *)(v66 + 12) = 257;
    v64[3] = v66;
  }
  else
  {
    v64 = 0;
  }
  dword_38C54BC = (int)v64;
  v67 = (_DWORD *)sub_E55D10(48);
  v68 = v67;
  if ( v67 )
  {
    v67[2] = 0;
    v67[3] = 0;
    v67[4] = 0;
    v67[5] = 0;
    v67[6] = 0;
    v67[7] = 0;
    v67[8] = 0;
    v67[9] = 0;
    v67[10] = 0;
    v67[11] = 0;
    dword_202B574 = (int)v67;
    *v67 = off_19035E8;
    v67[1] = 0;
    v67[2] = 0;
    v69 = sub_E55D10(24);
    *(_DWORD *)v69 = v69;
    *(_DWORD *)(v69 + 4) = v69;
    *(_DWORD *)(v69 + 8) = v69;
    *(_WORD *)(v69 + 12) = 257;
    v68[1] = v69;
    v68[3] = 0;
    v68[4] = 0;
    v70 = sub_E55D10(24);
    *(_DWORD *)v70 = v70;
    *(_DWORD *)(v70 + 4) = v70;
    *(_DWORD *)(v70 + 8) = v70;
    *(_WORD *)(v70 + 12) = 257;
    v68[3] = v70;
    v68[5] = 0;
    v68[6] = 0;
    v68[7] = 0;
    sub_DF84E0(v68 + 8);
    v68[10] = 0;
    v68[11] = 0;
  }
  else
  {
    v68 = 0;
  }
  dword_202B574 = (int)v68;
  sub_72DC50(v68);
  v71 = sub_E55D10(12);
  if ( v71 )
    v72 = sub_525F70(v71);
  else
    v72 = 0;
  dword_2C7A17C = v72;
  v73 = sub_E55D10(72);
  if ( v73 )
    v74 = sub_8E75B0(v73);
  else
    v74 = 0;
  dword_38CA1C8 = v74;
  v75 = sub_E55D10(24);
  if ( v75 )
    v76 = sub_5FDB60(v75);
  else
    v76 = 0;
  dword_38C3428 = v76;
  v77 = sub_E55D10(104);
  if ( v77 )
    v78 = sub_5C87F0(v77);
  else
    v78 = 0;
  dword_2C79C30 = v78;
  v79 = sub_E55D10(24);
  if ( v79 )
    v80 = sub_9CD100(v79);
  else
    v80 = 0;
  dword_38CA894 = v80;
  switch ( *(_DWORD *)(dword_2C79C3C + 116) )
  {
    case 0:
      goto LABEL_131;
    case 1:
      v83 = sub_7089C0();
      ((void (__thiscall *)(int))dword_765BE0[0])(v83 + 264);
LABEL_131:
      sub_7089C0();
      if ( !(unsigned __int8)sub_7076D0(
                               *(_DWORD *)(dword_2C79C3C + 4),
                               *(unsigned __int16 *)(dword_2C79C3C + 16),
                               *(_DWORD *)(dword_2C79C3C + 88),
                               *(_DWORD *)(dword_2C79C3C + 92),
                               dword_2C79C3C + 20) )
      {
        if ( byte_1FCCE88 )
          sub_5F6B50(3, "{:5s}| {#channel!} Error Initiating Game Server Connection", &off_1FCCE84, &off_1FCCE84);
        if ( (unsigned __int8)sub_4BEFD0(dword_2C79C3C) )
        {
          v106 = 0;
          v108 = 1;
          v107 = "Error Initiating Game Server Connection";
          v109 = 0;
          ((void (__thiscall *)(int, const char *, signed int, int *))dword_6EDC80[0])(
            dword_38CA1A4,
            "start_game",
            269732542,
            &v106);
          sub_625860(&v106);
        }
LABEL_136:
        *((_BYTE *)v1 + 180) = 1;
        return v1;
      }
      v85 = sub_7089C0();
      sub_71E330(v85);
      v86 = sub_7089C0();
      sub_708450(v86);
def_6D6182:
      if ( !(unsigned __int8)sub_713DE0() || (v87 = sub_7089C0(), *(_BYTE *)(v87 + 442)) && *(_BYTE *)(v87 + 444) )
      {
        if ( sub_6F53D0(v1) )
          goto LABEL_136;
      }
      if ( (unsigned __int8)sub_713DE0() && sub_6F50C0(v1) || !(unsigned __int8)sub_6F4BB0(v1) )
        goto LABEL_136;
      sub_4E7D70(3, 1, 0, (int)"Set focus to app\n");
      MEMORY[0x9670580](dword_38CA114);
      if ( !(unsigned __int8)sub_713DE0() )
      {
        v88 = sub_7089C0();
        sub_6C2070(*(_BYTE *)(v88 + 564));
      }
      sub_6B7730();
      sub_DE4470();
      if ( byte_1FCCE80 )
      {
        v96 = sub_DD7A90();
        v100 = sub_DD7860();
        v98 = sub_DD8870();
        v99 = v89;
        v101 = sub_DD7E90();
        v102 = sub_DD8500();
        v103 = sub_DD8410();
        v104 = sub_DD8E40();
        v97 = sub_DD8860();
        sub_6C3640(
          3,
          "{:5s}| {#channel!} OS Version: {OSVersion} CPU: {ProcessorName}, {CPUSpeed}Mhz ({CoreCount} cores) Instruction"
          " Set Level {InstructionSetLevel}Physical memory: {PhysicalMemory}Display info: {DisplayInfo} Driver: {GPUDriver}",
          &off_1FCCE7C,
          &off_1FCCE7C,
          &v97,
          &v104,
          &v103,
          &v102,
          &v101,
          &v98,
          &v100,
          &v96);
      }
      if ( (unsigned __int8)sub_DDEF50() && byte_1FCCE80 )
        sub_5F6B50(3, "{:5s}| {#channel!} Remote session detected.", &off_1FCCE7C, &off_1FCCE7C);
      sub_908D90(993830990);
      LOBYTE(v97) = sub_9132A0(-1667036753, 1);
      sub_5ECAE0(v97);
      v90 = sub_E55D10(52);
      if ( v90 )
        v91 = sub_933AA0(v90);
      else
        v91 = 0;
      dword_38CA1D4 = v91;
      sub_940E50(v91);
      v92 = sub_E55D10(48);
      if ( v92 )
        v93 = sub_5250C0(v92);
      else
        v93 = 0;
      dword_2C7A1C4 = v93;
      if ( (unsigned __int8)sub_4BEFD0(dword_2C79C3C) )
      {
        if ( *(_BYTE *)(dword_38CA1A4 + 100) )
        {
          v94 = *(float *)(dword_38CA1A4 + 108);
          if ( v94 == 1.0 || v94 > *(float *)(dword_38CA1A4 + 104) )
          {
            sub_6EEAE0("start_game", "success", &off_1FD5B00, 0, 0);
            sub_6DFDF0("start_game");
          }
        }
      }
      sub_D97950(&v105);
      return v1;
    case 2:
      v82 = sub_7089C0();
      sub_766050(v82 + 264);
      goto def_6D6182;
    case 3:
      if ( byte_1FCCE80 )
      {
        v96 = *(_DWORD *)(dword_2C79C3C + 120);
        sub_5F6D70(3, "{:5s}| {#channel!} Replay filepath: {ReplayFilePath:%s}", &off_1FCCE7C, &off_1FCCE7C, &v96);
      }
      v81 = sub_7089C0();
      ((void (__thiscall *)(int))dword_765DE0[0])(v81 + 264);
      goto def_6D6182;
  }
}*/