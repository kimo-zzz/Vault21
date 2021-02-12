#include "stdafx.h"
#include "SpellInfo.h"

namespace V21
{
	bool SpellInfo::IsAutoAttack()
	{
		auto slot = this->Slot;
		if (this->IsBasicAttack != 0 || (slot && slot == SpellSlot_SpecialAttack))
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