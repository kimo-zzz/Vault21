#pragma once


class SpellCastInfo
{
public:
	Vector GetSpellStartPos()
	{
		return *(Vector*)((DWORD)this + oSpellInfoStartPos);
	}

	Vector GetSpellCastPos()
	{
		return *(Vector*)((DWORD)this + oSpellInfoCastPos);
	}

	Vector GetSpellEndPos()
	{
		return *(Vector*)((DWORD)this + oSpellInfoEndPos);
	}
};