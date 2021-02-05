#pragma once

#include "Offsets.h"

namespace HACKUZAN {
	enum class GameObjectType {
		NeutralMinionCamp,
		AIHeroClient,
		AIMarker,
		AIMinionClient,
		ObjectAttacher,
		LevelPropAIClient,
		AITurretClient,
		AITurretCommon,
		obj_GeneralParticleEmitter,
		GameObject,
		MissileClient,
		DrawFX,
		UnrevealedTarget,
		BarracksDampener,
		Barracks,
		AnimatedBuilding,
		BuildingClient,
		obj_Levelsizer,
		obj_NavPoint,
		obj_SpawnPoint,
		GrassObject,
		HQ,
		obj_InfoPoint,
		LevelPropGameObject,
		LevelPropSpawnerPoint,
		Shop,
		obj_Turret,
	};

	class ClassData {
	public:
		union {
			DEFINE_MEMBER_N(std::string Name, Offsets::ClassData::Name)
		};
		DEFINE_MEMBER_ENCRYPTED(GameObjectType, Type, Offsets::ClassData::Type)
	};
}