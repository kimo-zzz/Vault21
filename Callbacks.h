#pragma once
#include "Engine.h"

	int __fastcall hk_OnProcessSpell(void* spellBook, void* edx, SpellInfo* spellInfo);
	int __fastcall hk_OnCreateObject(CObject* obj, void* edx, unsigned id);
	int __fastcall hk_OnDeleteObject(void* thisPtr, void* edx, CObject* obj);
	int __cdecl hk_OnNewPath(CObject* obj, Vector* start, Vector* end, Vector* tail, int unk1, float* dashSpeed,
		unsigned dash, int unk3, char unk4, int unk5, int unk6, int unk7);

