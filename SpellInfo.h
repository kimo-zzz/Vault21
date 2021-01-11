#pragma once
#include "stdafx.h"
#include "Utils.h"
#include "SpellData.h"

class SpellInfo
{
public:
	SpellData* GetSpellData() {
		return *(SpellData**)((DWORD)this + oSpellInfoSpellData);
	}
};