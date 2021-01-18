#pragma once
#include "stdafx.h"
#include <Windows.h>
#include "Utils.h"
#include "Vector.h"
#include "VectorLong.h"
#include "CObject.h"

class Typedefs
{
public:
	typedef void (__thiscall* tPrintChat)(DWORD ChatClient, const char* Message, int Color);
	typedef void (__thiscall* tSendChat)(DWORD ChatClient, const char* Message, int Color);
	typedef int*(__thiscall* fnIssueOrder)(void* thisPtr, int Order, Vector* Loc, CObject* Target, bool IsAttackMove,
	                                       bool IsMinion, DWORD Unknown);
	typedef int(__fastcall* fnIssueOrderCheck)(int a1, int a2, int a3, DWORD* a4, char a5, int a6, int a7, int a8, int a9, DWORD* a10);
	typedef float (__cdecl* fnGetAttackCastDelay)(CObject* pObj);
	typedef float (__cdecl* fnGetAttackDelay)(CObject* pObj);
	typedef float (__thiscall* fnGetTimerExpiry)(CObject* camp);
	typedef void (__cdecl* fnDrawCircle)(Vector* position, float range, int* color, int a4, float a5, int a6,
	                                     float alpha);
	typedef int* (__thiscall* fnCastSpell)(DWORD spellbook_addr, SpellSlot* spellslot, int SlotID, Vector* targetpos,
	                                       Vector* startpos, DWORD NetworkID);
	typedef void (__thiscall* fnPrintChat)(DWORD ChatClient, const char* Message, int Color);
	typedef void (__thiscall* fnSendChat)(DWORD ChatClient, const char* Message, int Color);

	typedef bool (__cdecl* WorldToScreen)(Vector* vectorIn, Vector* vectorOut);
	typedef bool (__cdecl* WorldToScreenL)(VectorLong* vectorIn, VectorLong* vectorOut);

	typedef bool (__cdecl* fnIsHero)(CObject* pObj);
	typedef bool (__cdecl* fnIsMissile)(CObject* pObj);
	typedef bool (__cdecl* fnIsMinion)(CObject* pObj);
	typedef bool (__cdecl* fnIsDragon)(CObject* pObj);
	typedef bool (__cdecl* fnIsTurret)(CObject* pObj);
	typedef bool (__cdecl* fnIsInhibitor)(CObject* pObj);
	typedef bool (__cdecl* fnIsTroyEnt)(CObject* pObj);
	typedef bool (__cdecl* fnIsNexus)(CObject* pObj);
	typedef bool (__thiscall* fnIsAlive)(CObject* pObj);
	typedef bool (__thiscall* fnIsTargetable)(CObject* pObj);
	typedef bool (__thiscall* fnGetPing)(void* pObj);

	typedef char*(*fnGetGameVer)();

	typedef int (__thiscall* fnBaseDrawPosition)(CObject* object, Vector* position);
	typedef CObject* (__thiscall* fnGetObjectOwner)(void*);

	//events
	typedef int (__thiscall* event_OnProcessSpell_h)(void* spellBook, SpellCastInfo* spellInfo);
	typedef int (__thiscall* event_OnCreateObject_h)(CObject* obj, unsigned id);
	typedef int (__thiscall* event_OnDeleteObject_h)(void* thisPtr, CObject* obj);
	typedef int (__cdecl* event_OnNewPath_h)(CObject* obj, Vector* start, Vector* end, Vector* tail, int unk1,
	                                         float* dashSpeed, unsigned dash, int unk3, char unk4, int unk5, int unk6,
	                                         int unk7);
};

class CFunctions
{
public:
	Typedefs::fnIsTurret IsTurret;
	Typedefs::fnIsMinion IsMinion;
	Typedefs::fnIsDragon IsDragon;
	Typedefs::fnIsHero IsHero;
	Typedefs::fnIsMissile IsMissile;
	Typedefs::fnIsTroyEnt IsTroyEnt;
	Typedefs::fnIsNexus IsNexus;
	Typedefs::fnIsInhibitor IsInhibitor;
	Typedefs::WorldToScreen WorldToScreen;
	Typedefs::WorldToScreenL WorldToScreenL;
	Typedefs::fnIsTargetable IsTargetable;
	Typedefs::fnIsAlive IsAlive;

	Typedefs::fnGetPing GetPing;


	Typedefs::fnIssueOrder IssueOrder;
	Typedefs::fnIssueOrderCheck IssueOrderCheck;
	Typedefs::fnGetAttackCastDelay GetAttackCastDelay;
	Typedefs::fnGetAttackDelay GetAttackDelay;
	Typedefs::fnGetTimerExpiry GetTimerExpiry;
	Typedefs::fnDrawCircle DrawCircle;
	Typedefs::fnCastSpell CastSpell;
	Typedefs::fnPrintChat PrintChat;
	Typedefs::fnSendChat SendChat;

	Typedefs::fnGetGameVer GetGameVer;

	Typedefs::fnBaseDrawPosition GetBaseDrawPosition;
	Typedefs::fnGetObjectOwner GetObjectOwner;

	Typedefs::event_OnProcessSpell_h OnProcessSpell_h;
	Typedefs::event_OnCreateObject_h OnCreateObject_h;
	Typedefs::event_OnDeleteObject_h OnDeleteObject_h;
	Typedefs::event_OnNewPath_h OnNewPath_h;
};

extern CFunctions Functions;
