#include "TargetSelector.h"

#include "HealthPrediction.h"


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
	HealthPrediction prediction;
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
							auto max = (int)max(0, obj->GetDistance(Engine::GetLocalObject()) - Engine::GetLocalObject()->GetBoundingRadius());
							auto t = (Engine::GetLocalObject()->GetAttackCastDelay() * 1000) - 100 + 30 /*30 is ping*/ / 2 + 1000 * max;
							auto landtime = Engine::GetGameTimeTickCount() + 1000 * (int)max(0, obj->GetPos().DistTo(Engine::GetLocalObject()->GetPos()) - obj->GetBoundingRadius()) / Engine::GetLocalObject()->GetAttackCastDelay() * 1000;
							if (0 < Engine::GetGameTimeTickCount() + t)
							{
								if (GetEffectiveHP(obj->GetArmor(), obj->GetHealth()) < Engine::GetLocalObject()->GetTotalAttackDamage())
								{
									if (target == nullptr || obj->GetHealth() < target->GetHealth())
										target = obj;
								}
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