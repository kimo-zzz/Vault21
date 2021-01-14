#pragma once
#include "stdafx.h"
#include "Utils.h"
#include "SpellData.h"

class SpellInfo
{
public:
	SpellData* GetSpellData()
	{
		return *(SpellData**)((DWORD)this + oSpellInfoSpellData);
	}

	short GetMissileSourceIndex()
	{
		return *(short*)((DWORD)this + oMissileSourceIndex);
	}

	Vector GetStartPos()
	{
		return *(Vector*)((DWORD)this + oMissileStartPos);
	}

	Vector GetEndPos()
	{
		return *(Vector*)((DWORD)this + oMissileEndPos);
	}
};
