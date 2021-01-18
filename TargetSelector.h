#pragma once
#include "Engine.h"

class TargetSelector
{
public:
	static CObject* GetOrbwalkerTarget();
	static CObject* GetLowestHpTarget();
	static CObject* GetLasthitTarget();
private:
	static CObject* GetWaveclearTarget();
};
