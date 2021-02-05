#include "stdafx.h"
#include "EventManager.h"

namespace HACKUZAN {
	std::vector<void*> EventManager::EventCallbacks[(unsigned int)LeagueEvents::Size];

	void HACKUZAN::EventManager::AddEventHandler(LeagueEvents eventId, void* callback) {
		EventCallbacks[(unsigned int)eventId].push_back(callback);
	}

	void HACKUZAN::EventManager::RemoveEventHandler(LeagueEvents eventId, void* callback) {
		auto& callbacks = EventCallbacks[(unsigned int)eventId];

		for (auto it = callbacks.begin(); it != callbacks.end(); it++) {
			if (*it == callback) {
				callbacks.erase(it);
				break;
			}
		}
	}
}