#pragma once
#include "stdafx.h"
#include <Windows.h>
#include "Utils.h"
#include "Offsets.h"
#include "Vector.h"
#include "VectorLong.h"
#include "CObjectManager.h"
#include "Hooks.h"
#include "CObject.h"
#define me Engine::GetLocalObject()

class Engine
{
public:
	static void MoveTo(Vector* pos);
	static void AttackTarget(CObject* obj);

	static void CastSpellSelf(int SlotID);
	static void CastSpellTargetted(int SlotID, CObject* obj);

	static Vector GetMouseWorldPosition()
	{
		DWORD MousePtr = baseAddr + oHudInstance;
		auto aux1 = *(DWORD*)MousePtr;
		aux1 += 0x14;
		auto aux2 = *(DWORD*)aux1;
		aux2 += 0x1C;

		float X = *(float*)(aux2 + 0x0);
		float Y = *(float*)(aux2 + 0x4);
		float Z = *(float*)(aux2 + 0x8);

		return Vector{X, Y, Z};
	}

	static int GetPing()
	{
		return Functions.GetPing(*(void**)((DWORD)GetModuleHandle(nullptr) + oNetClient));
	}

	static float GetGameTime()
	{
		return *(float*)(baseAddr + oGameTime);
	}

	static int GetGameTimeTickCount()
	{
		return static_cast<int>(GetGameTime() * 1000);
	}

	static CObject* FindObjectByIndex(std::list<CObject*> heroList, short casterIndex)
	{
		for (CObject* a : heroList)
		{
			if (casterIndex == a->GetIndex())
				return a;
		}
		return nullptr;
	}

	static bool IsChatBoxOpen()
	{
		DWORD ChatClient = *(DWORD*)(baseAddr + oChatClientPtr);
		DWORD ChatClientStruct = *(DWORD*)(ChatClient + oChatClientStruct);

		return *(bool*)(static_cast<DWORD>(ChatClientStruct) + oIsChatOpen);
	}

	static std::string SecondsToClock(int value)
	{
		int hour;
		int seconds;
		int minutes;

		hour = value / 3600;
		minutes = (value % 3600) / 60;
		seconds = value % 60;

		char str_seconds[MAXIMUM_TEXT_SIZE];
		snprintf(str_seconds, MAXIMUM_TEXT_SIZE, "%d", seconds);
		std::string str_seconds_String(str_seconds);

		char str_minutes[MAXIMUM_TEXT_SIZE];
		snprintf(str_minutes, MAXIMUM_TEXT_SIZE, "%d", minutes);
		std::string str_minutes_String(str_minutes);

		return (minutes < 10 ? "0" : "") + str_minutes_String + ":" + (seconds < 10 ? "0" : "") + str_seconds_String;
	}

	static CObject* GetLocalObject()
	{
		auto retaddr = *(DWORD*)(baseAddr + oLocalPlayer);
		if (retaddr == NULL)
			return nullptr;

		return (CObject*)retaddr;
	}

	static void PrintChat(const char* Message)
	{
		typedef void (__thiscall* tPrintChat)(DWORD ChatClient, const char* Message, int Color);
		tPrintChat fnPrintChat = (tPrintChat)(baseAddr + oPrintChat);
		fnPrintChat(*(DWORD*)(baseAddr + oChatClientPtr), Message, 1);
	}

	static void SendChat(const char* Message)
	{
		typedef void (__thiscall* tSendChat)(DWORD MenuGUI, const char* Message, int Color);
		tSendChat fnSendChat = (tSendChat)(baseAddr + oSendChat);
		fnSendChat(*(DWORD*)(baseAddr + oMenuGUI), Message, 1);
	}

	static CObject* Engine::GetObjectByID(int ID)
	{
		CObject holzer;
		auto obj = holzer.GetFirstObject();
		while (obj)
		{
			if (obj->GetIndex() == ID)
				return obj;
			obj = holzer.GetNextObject(obj);
		}
		return nullptr;
		if (ObjManager != nullptr && ID >= 0 && ID <= 10000)
		{
			//return ObjManager->objectArray[ID];
		}
	}

	static void Engine::DrawCircle(Vector* position, float range, int* color, int a4, float a5, int a6, float alpha)
	{
		DWORD SpoofAddress = baseAddr + oDrawCircleRetAddr;
		DWORD DrawCircleAddr = baseAddr + oDrawCircle;

		__asm
			{
			push retnHere
			push alpha
			push a6
			push a5
			push a4
			push color
			push range
			push position
			push SpoofAddress
			jmp DrawCircleAddr
			retnHere :
			}
	}

	static float Engine::GetTimerExpiry(CObject* obj)
	{
		return Functions.GetTimerExpiry(obj);
	}

	static char* GetGameVersion()
	{
		return Functions.GetGameVer();
	}

	static void ChangeMaximumZoom(float amount)
	{
		DWORD* zoomInstance = reinterpret_cast<DWORD*>(baseAddr + oZoomClass);
		float* zoomAmount = reinterpret_cast<float*>(*zoomInstance + oMaxZoom);
		float newAmount = *zoomAmount + amount;

		*zoomAmount = amount;
	}

	static float* GetMaximumZoomAmount()
	{
		DWORD* zoomInstance = reinterpret_cast<DWORD*>(baseAddr + oZoomClass);
		float* zoomAmount = reinterpret_cast<float*>(*zoomInstance + oMaxZoom);
		return zoomAmount;
	}
};
