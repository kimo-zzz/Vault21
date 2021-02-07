#pragma once

#include "GameObject.h"
#include "TeleportEventArgs.h"

namespace HACKUZAN {
	namespace SDK {
		namespace Events {

			void Initialize();

			namespace GameStart {
				void Initialize();
				void OnGameUpdate();
			}

			namespace LeagueHooks {
				void Initialize();
				void OnGameUpdate();
			}

			namespace GameTick {
				extern float LastTick;

				void Initialize();
				void OnGameUpdate();
			}

			namespace Objects {
				extern GameObject* CachedGameObjects[10000];

				void Initialize();
				void AddMissile(GameObject* missile);
				void OnGameUpdate();
			}

			namespace SpellCast {
				extern SpellInfo* CachedSpells[10000];
				extern float CachedSpellCastEndTime[10000];
				extern kSpellState CachedSpellStates[SpellSlot_Trinket];

				void Initialize();
				void OnGameUpdate();
			}

			namespace LevelUp {
				extern unsigned int CachedLevels[10000];

				void Initialize();
				void OnGameUpdate();
			}

			namespace Teleport {
				extern TeleportEventArgs CachedTeleports[10000];

				TeleportType GetTeleportType(unsigned int teleportTypeHash);
				float GetTeleportDuration(TeleportType teleportType, unsigned int teleportNameHash);
				void Initialize();
				void OnGameUpdate();
			}

			namespace NewPath {
				extern Vector3 CachedEndPositions[10000];

				void Initialize();
				void OnGameUpdate();
			}
		}
	}
}