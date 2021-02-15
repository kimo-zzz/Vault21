#pragma once

#include "stdafx.h"

namespace V21 {
	enum class LeagueEvents {
		OnWndProc,
		OnPresent,

		OnGameStart,
		OnGameTick,

		OnCreateMissile,
		OnDeleteMissile,
		OnCreateObject,
		OnDeleteObject,
		OnIssueOrder,
		OnCastSpell,
		OnSpellCast,
		OnDoCast,
		OnDoCastDelayed,
		OnProcessSpell,
		OnPlayAnimation,
		OnFinishCast,
		OnStopCast,
		OnGapCloserSpell,
		OnInterruptibleSpell,
		OnNewPath,
		Size
	};

	namespace EventManager {
		extern std::vector<void*> EventCallbacks[(unsigned int)LeagueEvents::Size];

		void AddEventHandler(LeagueEvents eventId, void* callback);
		void RemoveEventHandler(LeagueEvents eventId, void* callback);

		template <typename... Args>
		void Trigger(LeagueEvents eventId, Args... args) {
			for (auto callback : EventCallbacks[(unsigned int)eventId]) {
				static_cast<void(*)(Args...)>(callback)(args...);
			}
		}

		template <typename... Args>
		bool TriggerProcess(LeagueEvents eventId, Args... args) {
			auto process = true;

			for (auto callback : EventCallbacks[(unsigned int)eventId]) {
				if (!static_cast<bool(*)(Args...)>(callback)(args...)) {
					process = false;
				}
			}

			return process;
		}
	}
}