#include "HealthPrediction.h"

std::vector<CObject*> HealthPrediction::GetActiveMinionAttacks()
{
	std::vector<CObject*> activeAttacks;

	CObject holzer;
	auto obj = holzer.GetFirstObject();

	CObject* target = nullptr; // Our orb target
	while (obj)
	{
		if (obj->IsMissile() && obj->GetDistance(Engine::GetLocalObject()) <= Engine::GetLocalObject()->GetAttackRange() + Engine::GetLocalObject()->GetBoundingRadius())
		{
			if (Engine::GetObjectByID(obj->GetIndex())->IsMinion())
			{
				activeAttacks.push_back(obj);
			}
		}

		obj = holzer.GetNextObject(obj);
	}


}
