#include "TargetSelector.h"


CObject* TargetSelector::GetOrbwalkerTarget()
{
	if (GetAsyncKeyState(VK_SPACE))
		return GetLowestHpTarget();
	if (GetAsyncKeyState(0x56))
		return GetWaveclearTarget();
	if (GetAsyncKeyState(0x58))
		return GetLasthitTarget();

	return nullptr;
}

CObject* TargetSelector::GetWaveclearTarget()
{
	CObject holzer;
	auto obj = holzer.GetFirstObject();

	CObject* target = nullptr; // Our orb target
	while (obj)
	{

		if (obj->GetDistance(Engine::GetLocalObject()) < Engine::GetLocalObject()->GetAttackRange() + Engine::GetLocalObject()->GetBoundingRadius())
		{
			if (obj->IsInhibitor() || obj->IsTurret() || obj->IsNexus())
			{
				if (obj->IsEnemyTo(me))
				{
					if (obj->IsAlive() && obj->IsTargetable())
						return obj;
				}

			}

			if (obj->IsMinion())
			{
				if (obj->GetTeam() == 300) //if neutral team 
				{
					if (obj->IsAlive() && obj->IsTargetable())
						return obj;
				}

				if (obj->IsEnemyTo(me) && obj->IsAlive() && obj->IsTargetable())
				{
					if (target == nullptr || obj->GetHealth() < target->GetHealth())
					{
						target = obj;
					}
				}
			}


		}

		obj = holzer.GetNextObject(obj);
	}

	return target;
}


CObject* TargetSelector::GetLowestHpTarget()
{
	CObject holzer;
	auto obj = holzer.GetFirstObject();

	CObject* target = nullptr; // Our orb target
	while (obj)
	{
		if (obj != nullptr)
		{
			if (obj->IsHero())
			{
				if (obj->GetDistance(Engine::GetLocalObject()) < Engine::GetLocalObject()->GetAttackRange() + Engine::GetLocalObject()->GetBoundingRadius())
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

CObject* TargetSelector::GetLasthitTarget()
{
	CObject holzer;
	auto obj = holzer.GetFirstObject();

	CObject* target = nullptr; // Our orb target
	while (obj)
	{
		if (obj != nullptr)
		{
			if (obj->IsMinion())
			{
				if (obj->GetDistance(Engine::GetLocalObject()) < Engine::GetLocalObject()->GetAttackRange() + Engine::GetLocalObject()->GetBoundingRadius())
				{
					if (obj->IsEnemyTo(Engine::GetLocalObject()))
					{
						if (obj->IsAlive() && obj->IsTargetable())
						{
							if (GetEffectiveHP(obj->GetArmor(), obj->GetHealth()) < me->GetTotalAttackDamage())
							{
								if (target == nullptr || obj->GetHealth() < target->GetHealth())
									target = obj;
							}

						}
					}
				}

			}
		}
		obj = holzer.GetNextObject(obj);
	}

	return target;
}