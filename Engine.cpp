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
	if (!(DWORD)LeagueFunctions::NewIssueOrder)
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

void Engine::AttackTarget2(CObject* obj)
{
	if (me->IsAlive())
	{
		void* LocalPlayer = me;
		CObject* AttackTo = obj;
		Vector* pos = &obj->GetPos();

		Functions.IssueOrder(LocalPlayer, 3, pos, AttackTo, 0, 0, 0);
	}
}
void Engine::MoveTo(Vector* pos)
{
	if (!(DWORD)LeagueFunctions::NewIssueOrder)
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