#pragma once
#include "stdafx.h"
#include "Polygons.h"
#include "GameObject.h"
#include "EvadeConstants.h"
#include "ObjectManager.h"

#define SET_VAR(TYPE, NAME) SkillshotData* _##NAME(TYPE const& VALUE) { this->NAME = VALUE; return this;  }
#define ADD_VAR(TYPE, NAME) TYPE NAME = { }; SET_VAR(TYPE, NAME)


namespace V21 {

	class SkillshotData
	{
	public:
			ADD_VAR(std::string, champion_name)

			ADD_VAR(std::string, spell_name)
			ADD_VAR(std::string, spellbook_spell_name)
			ADD_VAR(std::string, menu_name)
			ADD_VAR(std::string, menu_display_name)
			ADD_VAR(std::string, toggle_name)
			ADD_VAR(std::string, start_from_particle)
			ADD_VAR(std::string, start_from_object)
			ADD_VAR(std::string, end_at_object)
			ADD_VAR(std::string, end_at_particle)

			ADD_VAR(std::vector<std::string>, extra_spell_names)
			ADD_VAR(std::string, missile_name)
			ADD_VAR(std::string, object_name)

			ADD_VAR(kSpellSlot, spell_slot)
			ADD_VAR(SpellType, skillshot_type)
			ADD_VAR(CrowdControllFlags, crowd_controll_flags)
			ADD_VAR(DangerLevel, danger_level)

			ADD_VAR(int, collision_flags)
			ADD_VAR(int, raw_range)
			ADD_VAR(int, minimum_range)
			ADD_VAR(int, extra_range)
			ADD_VAR(int, raw_radius)
			ADD_VAR(int, radius_ex)
			ADD_VAR(int, delay)
			ADD_VAR(int, extra_delay)
			ADD_VAR(int, extra_duration)
			ADD_VAR(int, missile_length)
			ADD_VAR(int, infront_start)
			ADD_VAR(int, behind_start)
			ADD_VAR(int, missile_speed)
			ADD_VAR(int, missile_min_speed)
			ADD_VAR(int, missile_max_speed)
			ADD_VAR(int, missile_accel)
			ADD_VAR(int, multiple_number)

			ADD_VAR(float, multiple_angle)

			ADD_VAR(bool, is_fixed_range)
			ADD_VAR(bool, missile_delayed)
			ADD_VAR(bool, invert)
			ADD_VAR(bool, perpendicular)
			ADD_VAR(bool, missile_follows_unit)
			ADD_VAR(bool, missile_connected_to_unit)
			ADD_VAR(bool, dont_add_bounding_radius)
			ADD_VAR(bool, is_dangerous)
			ADD_VAR(bool, dont_cross)
			ADD_VAR(bool, can_be_deleted)
			ADD_VAR(bool, dont_add_extra_duration)
			ADD_VAR(bool, removes_on_collision_end)
			ADD_VAR(bool, is_disabled_by_default)
			ADD_VAR(bool, dont_scan_for_duplicates)

			SkillshotData();

		std::string MenuName() const;
		std::string MenuDisplayName() const;

		unsigned int Range() const;
		unsigned int Radius() const;

		bool CollidesOnlyWithYasuoWall() const;
	};

	SkillshotData* InitializeSpell(std::string const& champion);

}