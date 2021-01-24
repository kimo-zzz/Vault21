#pragma once
#include "stdafx.h"
#include <Windows.h>
#include "Offsets.h"
#include "CObject.h"
#include "Engine.h"
#include "LeagueFunctions.h"

#define me Engine::GetLocalObject()

void Engine::AttackTarget(CObject* obj)
{
	if ((!(DWORD)LeagueFunctions::NewIssueOrder) || (!LeagueFunctions::IsDonePatchingIssueOrder))
		return;
	if (me->IsAlive())
	{
		DWORD SpoofAddress = baseAddr + oRetAddr; //retn instruction
		DWORD IssueOrderAddr = (DWORD)LeagueFunctions::NewIssueOrder; //IssueOrder
		void* LocalPlayer = GetLocalObject();
		CObject* AttackTo = obj;
		Vector* pos = &obj->GetPos();

		if (((*(DWORD*)SpoofAddress) & 0xFF) != 0xC3)
			return; //This isn't the instruction we're looking for

		__asm
		{
			push retnHere
			mov ecx, LocalPlayer
			push 0
			push 0
			push 0
			push AttackTo
			push pos
			push 3
			push SpoofAddress
			jmp IssueOrderAddr
			retnHere :
		}
	}
}

void Engine::MoveTo(Vector* pos)
{
	if ((!(DWORD)LeagueFunctions::NewIssueOrder) || (!LeagueFunctions::IsDonePatchingIssueOrder))
		return;
	DWORD SpoofAddress = (DWORD)GetModuleHandle(nullptr) + oRetAddr; //retn instruction
	DWORD IssueOrderAddr = (DWORD)LeagueFunctions::NewIssueOrder; //IssueOrder
	void* LocalPlayer = GetLocalObject();

	if (((*(DWORD*)SpoofAddress) & 0xFF) != 0xC3)
		return; //This isn't the instruction we're looking for

	__asm
	{
		push retnHere
		mov ecx, LocalPlayer
		push 0
		push 0
		push 0
		push 0
		push pos
		push 2
		push SpoofAddress
		jmp IssueOrderAddr
		retnHere :
	}
}

void Engine::CastSpellSelf(int SlotID) {
	if (me->IsAlive()) {
		if ((!(DWORD)LeagueFunctions::NewCastSpell) || (!LeagueFunctions::IsDonePatchingCastSpell))
			return;
		DWORD spellbook = (DWORD)me + (DWORD)oObjSpellBook;
		auto spellslot = me->GetSpellSlotByID(SlotID);
		Vector* objPos = &me->GetPos();
		Vector* mePos = &me->GetPos();
		DWORD objNetworkID = 0;
		DWORD SpoofAddress = baseAddr + (DWORD)oRetAddr;
		DWORD CastSpellAddr = (DWORD)LeagueFunctions::NewCastSpell; //CastSpell

		if (((*(DWORD*)SpoofAddress) & 0xFF) != 0xC3)
			return; //This isn't the instruction we're looking for

		__asm
		{
			push retnHere
			mov ecx, spellbook //If the function is a __thiscall don't forget to set ECX
			push objNetworkID
			push mePos
			push objPos
			push SlotID
			push spellslot
			push SpoofAddress
			jmp CastSpellAddr
			retnHere :
		}
	}
}

void Engine::CastSpellTargetted(int SlotID, CObject* obj) {
	if (me->IsAlive()) {
		if ((!(DWORD)LeagueFunctions::NewCastSpell) || (!LeagueFunctions::IsDonePatchingCastSpell))
			return;
		auto spellbook = (DWORD)me + oObjSpellBook;
		auto spellslot = me->GetSpellSlotByID(SlotID);

		Vector* objPos = &obj->GetPos();
		Vector* mePos = &me->GetPos();
		DWORD objNetworkID = obj->GetNetworkID();
		DWORD SpoofAddress = baseAddr + oRetAddr;
		DWORD CastSpellAddr = (DWORD)LeagueFunctions::NewCastSpell; //CastSpell

		if (((*(DWORD*)SpoofAddress) & 0xFF) != 0xC3)
			return; //This isn't the instruction we're looking for

		__asm
		{
			push retnHere
			mov ecx, spellbook //If the function is a __thiscall don't forget to set ECX
			push objNetworkID
			push mePos
			push objPos
			push SlotID
			push spellslot
			push SpoofAddress
			jmp CastSpellAddr
			retnHere :
		}
	}
}