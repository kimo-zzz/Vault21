#include "stdafx.h"
#include "ManagerTemplate.h"

namespace HACKUZAN
{

	SEntityList<GameObject>* ManagerTemplate::AIBases;
	SEntityList<GameObject>* ManagerTemplate::Missiles;
	SEntityList<GameObject>* ManagerTemplate::Turrets;
	SEntityList<GameObject>* ManagerTemplate::Inhibitor;
	SEntityList<GameObject>* ManagerTemplate::Minions;
	SEntityList<GameObject>* ManagerTemplate::Heroes;

	std::vector<StopCast*>Args;

}