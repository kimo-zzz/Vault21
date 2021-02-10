#include "stdafx.h"
#include "SpellInfo.h"

namespace HACKUZAN
{
	bool SpellInfo::IsAutoAttack()
	{
		auto slot = this->Slot;
		if (this->IsBasicAttack || (slot && slot == SpellSlot_SpecialAttack))
		{
			return true;
		}
		return false;
	}

	bool SpellInfo::IsChanneling()
	{
		return !this->IsInstantCast && this->SpellWasCast;
	}
}