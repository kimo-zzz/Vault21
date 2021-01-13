#pragma once
#include "Engine.h"

class TargetSelector
{
public:
	static CObject* GetOrbwalkerTarget();
	static CObject* GetLowestHpTarget();
private:
	static CObject* GetWaveclearTarget();
};
