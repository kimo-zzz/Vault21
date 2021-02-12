#pragma once

#include "stdafx.h"
#include "GameObject.h"
#include "ManagerTemplate.h"

namespace HACKUZAN {
	namespace SDK {
		enum kOrbwalkerMode {
			OrbwalkerMode_None = 0,
			OrbwalkerMode_Combo = 1 << 0,
			OrbwalkerMode_Harass = 1 << 1,
			OrbwalkerMode_LastHit = 1 << 2,
			OrbwalkerMode_LaneClear = 1 << 3,
			OrbwalkerMode_JungleClear = 1 << 4,
			OrbwalkerMode_Flee = 1 << 5
		};

		enum class OrbwalkerTargetType {
			Hero,
			Monster,
			Minion,
			Structure
		};

		namespace Orbwalker {
			extern GameObject* LastHitMinion;
			extern GameObject* AlmostLastHitMinion;
			extern GameObject* LaneClearMinion;
			extern GameObject* turretMinion;
			extern GameObject* inhibitorMinion;
			extern GameObjectOrder LastOrder;
			extern int LastAttackCommandT;
			extern int LastMoveCommandT;
			extern bool _missileLaunched;
			extern bool DisableNextAttack;
			extern bool DisableNextMove;
			extern bool OrbwalkerEvading;
			extern bool OverrideOrbwalker;
			extern GameObject* LastTarget;
			extern Vector3 LastMovePosition;
			extern int LastAATick;
			extern unsigned int ActiveMode;

			//special stuff for champions
			extern bool IsAzir;
			extern std::vector<GameObject*> AzirSoldiers;
			extern bool IsAshe;
			extern bool IsGraves;
			extern bool IsRengar;
			extern bool AttackUsesAmmo;
			extern kSpellSlot AttackResetSlot;
			extern unsigned int AttackResetHash;
			extern bool IsDashAttackReset;

			//stuff people can set
			extern GameObject* ForcedTarget;
			extern Vector3 ForcedPosition;

			void Initialize();
			void Dispose();

			bool IsAutoAttack(string name);
			bool IsAutoAttackReset(string name);

			bool OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position, GameObject* target);
			void OnSpellCast(kSpellSlot slot);
			void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);
			void OnDoCast(SpellInfo* castInfo, SpellDataResource* spellData);
			void OnDoCastDelayed(SpellInfo* castInfo, SpellDataResource* spellData);
			void OnStopCast(GameObject* unit, StopCast* args);
			void OnNewPath(NewPath* args);
			void OnCreateObject(GameObject* unit);
			void OnDeleteObject(GameObject* unit);
			void OnGameUpdate();
			void OnDraw();

			void LasthitLogic();
			void LaneclearLogic();
			
			void ResetAutoAttack();
			Vector3 GetOrbwalkPosition();
			GameObject* GetTarget();
			GameObject* GetTargetByType(OrbwalkerTargetType targetType);
			bool ShouldWait();
			bool ShouldWaitUnderTurret(GameObject* noneKillableMinion);
			void OrbwalkTo(Vector3 position);
			void MoveTo(Vector3 position);
			float GetAttackCastDelay(GameObject* target = nullptr);
			float GetAttackDelay(GameObject* target = nullptr);
			float GetAttackMissileSpeed();
			bool CanAttack(GameObject* target);
			bool CanMove();
			bool CanCastAfterAttack();
		}
	}
}