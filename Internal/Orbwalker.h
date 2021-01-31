#pragma once
#include "Engine.h"

/// <summary>
	   /// The tick the most recent attack command was sent.
	   /// </summary>
static float LastAttackCommandT = 0;

/// <summary>
///     <c>true</c> if the orbwalker will disable the next attack.
/// </summary>
static float LastMoveCommandT = 0;

static Vector Original_Pos;

class Orbwalker
{
public:
	static bool Orbwalk(CObject* target, float extraWindup);
	static bool CanAttack();
	static bool CanMove(float extraWindup);
private:
};
