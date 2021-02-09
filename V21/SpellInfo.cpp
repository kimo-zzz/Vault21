#include "stdafx.h"
#include "SpellInfo.h"

namespace HACKUZAN
{
	bool SpellInfo::IsAutoAttack()
	{
		if (this->IsBasicAttack || this->IsSpecialAttack || this->Slot >= SpellSlot_SpecialAttack)
		{
			return true;// this->TargetId > 0;
		}
		return false;
	}

	bool SpellInfo::IsChanneling()
	{
		return !this->IsInstantCast && this->SpellWasCast;
	}
}