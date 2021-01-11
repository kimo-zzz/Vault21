#pragma once
#include "Engine.h"

class Orbwalker
{
public:
	static bool Orbwalk(CObject* target, float extraWindup);

private:
	static bool CanAttack();
	static bool CanMove(float extraWindup);
};
