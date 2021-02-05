#pragma once

#include "stdafx.h"
#include "SkillshotData.h"


namespace HACKUZAN {
	class ISpecialSpell;

	namespace SpellDb
	{
		void Initialize();

		extern std::vector<SkillshotData*> spells;
		extern std::vector<ISpecialSpell*> special_spells;
	}
}