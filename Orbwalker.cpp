#include "Orbwalker.h"
#include "Globals.h"
bool DisableNextAttack = false;

/// <summary>
///     <c>true</c> if the auto attack missile was launched from the player.
/// </summary>
bool _missileLaunched;

/// <summary>
///     The last target
/// </summary>
int _lastTarget;


bool Orbwalker::Orbwalk(CObject* target, float extraWindup = 90.f)
{
	if (IsChatBoxOpen)
		return false;


	if (CanAttack() && target != nullptr)
	{
		Engine::AttackTarget(target);
		LastAttackCommandT = float(GetTickCount()) + 30;
	}
	else if (CanMove(extraWindup) && LastMoveCommandT < GetTickCount())
	{
		Engine::MoveTo(&Engine::GetMouseWorldPosition());
		LastMoveCommandT = GetTickCount() + 20;
	}
	return true;
}

/// <summary>
/// Returns true if the LocalPlayer can Attack.
/// </summary>
/// <returns></returns>
bool Orbwalker::CanAttack()
{
	//return Engine::GetGameTimeTickCount() + 30 / 2 >= LastAttackCommandT + Engine::GetLocalObject()->GetAttackDelay() * 1000;
	return float(GetTickCount()) + 30 / 2.f  >= LastAttackCommandT +me->GetAttackDelay() * 1000.f;
}

/// <summary>
/// Returns true if the LocalPlayer can Move.
/// </summary>
/// <returns></returns>
bool Orbwalker::CanMove(float extraWindup)
{
	return me->GetChampionName() == "Kalista" ||  float(GetTickCount()) + 30 * 0.5f >= LastAttackCommandT + me->GetAttackCastDelay() * 1000.f + (30 * 1.5f) + g_orbwalker_windup;
}
