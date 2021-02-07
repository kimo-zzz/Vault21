#include "stdafx.h"
#include "SpellCastInfo.h"

namespace HACKUZAN
{
	bool SpellCastInfo::IsAutoAttack()
	{
		if (this->IsBasicAttack || this->IsSpecialAttack)// || this->Slot >= SpellSlot_SpecialAttack)
		{
			return this->TargetId > 0;
		}
		return false;
	}

	bool SpellCastInfo::IsChanneling()
	{
		return !this->IsInstantCast && this->SpellWasCast;
	}
}