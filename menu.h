#pragma once

#include "stdafx.h"
#include "CObject.h"
#include "CObjectManager.h"
#include "Hooks.h"
#include "Engine.h"
#include "ImRenderer.h"

#include <cinttypes>
#include <map>

namespace DX11
{
	class Menu
	{
	public:
		static bool bIsOpen;
		static void Render11();
		static void Content();
		static void Render9();

		static void Log(const char* msg);
	};

	namespace config
	{
		extern int32_t current_combo_skin_index;
		extern int32_t current_combo_ward_index;
		extern int32_t current_combo_minion_index;

		extern int32_t current_ward_skin_index;
		extern int32_t current_minion_skin_index;

		extern std::map<uint32_t, int32_t> current_combo_ally_skin_index;
		extern std::map<uint32_t, int32_t> current_combo_enemy_skin_index;
		extern std::map<uint32_t, int32_t> current_combo_jungle_mob_skin_index;

		void save();
		void load();
		void reset();
	};
}

extern CFunctions Functions;
