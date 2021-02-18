#include "stdafx.h"
#include "ManagerTemplate.h"

namespace V21
{

	std::vector<StopCast*>Args;
	std::vector<NewPath>path;

	std::map<int, NewPath> PredAllDashData;
	std::map <int, int> PredAllNewPathTicks;
	Vector3 PredLastYasuoWallCastPos;
}