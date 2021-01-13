#pragma once
#include "Engine.h"

static std::vector<SpellInfo*> missileList;

class Drawings
{
public:
	static void EvadeDrawings(CObject*& objBuffer, list<CObject*> heroList);
	static void DrawInhibitors(CObject* &objBuffer);
	static void DrawTurrets(CObject* &objBuffer);
};
