#pragma once

class UnitComponentInfoProperties
{
public:
	float GetBaseAtkSpeed()
	{
		return *(float*)((DWORD)this + oBaseAtkSpeed);
	}
};