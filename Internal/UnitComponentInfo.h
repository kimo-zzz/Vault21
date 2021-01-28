#pragma once
#include "UnitComponentInfoProperties.h"

class UnitComponentInfo
{
public:
	UnitComponentInfoProperties* GetUCIProperties()
	{
		return (UnitComponentInfoProperties*)((DWORD)this + oUCIProperties);
	}
};
