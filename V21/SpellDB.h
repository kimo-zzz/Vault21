#pragma once

#include "stdafx.h"
#include "SkillshotData.h"


namespace V21 {
	class ISpecialSpell;

	namespace SpellDb
	{
		void Initialize();

		extern std::vector<SkillshotData*> spells;
		extern std::vector<ISpecialSpell*> special_spells;
	}
}