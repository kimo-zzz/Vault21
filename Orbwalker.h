#pragma once
#include "Engine.h"

class Orbwalker
{
public:
	bool Orbwalk(CObject* target, float extraWindup);
	
private:
	bool CanAttack();
	bool CanMove(float extraWindup);
};
