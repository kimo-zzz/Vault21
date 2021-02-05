#include "SkillshotData.h"

namespace HACKUZAN {

	SkillshotData::SkillshotData()
	{
		this->spell_slot = kSpellSlot::SpellSlot_Q;
		this->skillshot_type = kMissileLine;
		this->danger_level = kLow;
		this->delay = 250;
		this->missile_speed = INT_MAX;
		this->can_be_deleted = true;
	}

	std::string SkillshotData::MenuName() const
	{
		if (this->menu_name.empty())
		{
			auto result = this->champion_name;

			result += ".";
			result += this->spell_name;

			return result;
		}

		return this->menu_name;
	}

	std::string SkillshotData::MenuDisplayName() const
	{
		if (this->menu_display_name.empty())
		{
			auto result = this->spell_name;
			std::string spell_slot_name;

			switch (this->spell_slot)
			{
			case kSpellSlot::SpellSlot_Unknown: spell_slot_name = "Invalid"; break;
			case kSpellSlot::SpellSlot_Q: spell_slot_name = "Q"; break;
			case kSpellSlot::SpellSlot_W: spell_slot_name = "W"; break;
			case kSpellSlot::SpellSlot_E: spell_slot_name = "E"; break;
			case kSpellSlot::SpellSlot_R: spell_slot_name = "R"; break;
			case kSpellSlot::SpellSlot_Summoner1: spell_slot_name = "Summoner1"; break;
			case kSpellSlot::SpellSlot_Summoner2: spell_slot_name = "Summoner2"; break;
			case kSpellSlot::SpellSlot_Item1: spell_slot_name = "Item_1"; break;
			case kSpellSlot::SpellSlot_Item2: spell_slot_name = "Item_2"; break;
			case kSpellSlot::SpellSlot_Item3: spell_slot_name = "Item_3"; break;
			case kSpellSlot::SpellSlot_Item4: spell_slot_name = "Item_4";	break;
			case kSpellSlot::SpellSlot_Item5: spell_slot_name = "Item_5";	break;
			case kSpellSlot::SpellSlot_Item6: spell_slot_name = "Item_6";	break;
			case kSpellSlot::SpellSlot_Trinket: spell_slot_name = "Trinket"; break;
			case kSpellSlot::SpellSlot_Recall: spell_slot_name = "Recall"; break;
			default: spell_slot_name = "Unknown"; break;
			}

			result += " [";
			result += spell_slot_name;
			result += "]";

			return result;
		}

		return this->menu_display_name;
	}

	unsigned int SkillshotData::Range() const
	{
		if (this->skillshot_type == kMissileLine ||
			this->skillshot_type == kLine ||
			this->skillshot_type == kCone ||
			this->skillshot_type == kMissileCone)
		{
			return this->raw_range;
		}

		return this->raw_range;
	}

	unsigned int SkillshotData::Radius() const
	{
		if (!this->dont_add_bounding_radius)
		{
			return this->raw_radius;
		}

		return this->raw_radius + static_cast<unsigned int>(ObjectManager::Player->GetBoundingRadius());
	}

	bool SkillshotData::CollidesOnlyWithYasuoWall() const
	{
		return (this->collision_flags ^ kYasuoWall) == kYasuoWall;
	}

	SkillshotData* InitializeSpell(std::string const& champion)
	{
		auto result = new SkillshotData();
		result->_champion_name(champion);

		return result;
	}
}