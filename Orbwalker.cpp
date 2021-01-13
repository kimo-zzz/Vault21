#include "Orbwalker.h"

/// <summary>
	   /// The tick the most recent attack command was sent.
	   /// </summary>
int LastAttackCommandT;

/// <summary>
/// The tick the most recent move command was sent.
/// </summary>
float LastMoveCommandT;

/// <summary>
///     <c>true</c> if the orbwalker will disable the next attack.
/// </summary>
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
	if(CanAttack() && target != nullptr)
	{
		Engine::AttackTarget(target);
		LastAttackCommandT = Engine::GetGameTimeTickCount() - 30/ 2;
	}
	else if(CanMove(extraWindup))
	{
		Engine::MoveTo(&Engine::GetMouseWorldPosition());
	}
	return true;
}

/// <summary>
/// Returns true if the LocalPlayer can Attack.
/// </summary>
/// <returns></returns>
bool Orbwalker::CanAttack()
{
	return Engine::GetGameTimeTickCount() + 30 / 2 >= LastAttackCommandT + Engine::GetLocalObject()->GetAttackDelay() * 1000;
}

/// <summary>
/// Returns true if the LocalPlayer can Move.
/// </summary>
/// <returns></returns>
bool Orbwalker::CanMove(float extraWindup)
{
	return Engine::GetGameTimeTickCount() + 30 / 2 >= LastAttackCommandT + Engine::GetLocalObject()->GetAttackCastDelay() * 1000 + extraWindup;
}
