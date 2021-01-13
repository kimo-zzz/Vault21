#include "TargetSelector.h"

CObject* TargetSelector::GetLowestHpTarget()
{
	CObject holzer;
	auto obj = holzer.GetFirstObject();

	CObject* target = nullptr; // Our orb target
	while(obj)
	{
		if (!obj->IsDeletedObject())
		{
			if (obj->IsHero())
			{
				if (obj->GetDistance(Engine::GetLocalObject()) < Engine::GetLocalObject()->GetAttackRange()+Engine::GetLocalObject()->GetBoundingRadius())
				{
					if (obj->IsEnemyTo(Engine::GetLocalObject()))
					{
						if (obj->IsAlive() && obj->IsTargetable())
						{
							if (target == nullptr || obj->GetHealth() < target->GetHealth())
								target = obj;
						}
					}
				}

			}
		}
		
		obj = holzer.GetNextObject(obj);
	}

	return target;
}
