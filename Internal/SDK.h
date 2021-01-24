#pragma once
#include "LUA.h"
#include "Orbwalker.h"
#include "TargetSelector.h"
namespace SDK
{

	static int GetLocalPlayer(lua_State* L)
	{
		int idx = Engine::GetLocalObject()->GetIndex();

		lua_pushnumber(L, idx);
		return 1;
	}

	static int GetMousePos(lua_State* L)
	{
		lua_newtable(L);
		Vector mousePos = Engine::GetMouseWorldPosition();
		lua_pushnumber(L, mousePos.X);
		lua_setfield(L, -2, "X");

		lua_pushnumber(L, mousePos.Y);
		lua_setfield(L, -2, "Y");

		lua_pushnumber(L, mousePos.Z);
		lua_setfield(L, -2, "Z");

		return 3;
	}

	static int GetHealth(lua_State* L)
	{
		int index = (int)lua_tonumber(L, 1);

		CObject* obj = Engine::GetObjectByID(index);

		float health = obj->GetHealth();

		lua_pushnumber(L, health);

		return 1;
	}

	static int GetChampionName(lua_State* L)
	{
		int index = (int)lua_tonumber(L, 1);
		CObject* obj = Engine::GetObjectByID(index);

		lua_pushstring(L, obj->GetChampionName());

		return 1;
	}

	namespace TS
	{
		static int GetLowestTarget(lua_State* L)
		{
			CObject* target = TargetSelector::GetLowestHpTarget();
			int targetidx = 0;

			if (target != nullptr)
			{
				targetidx = target->GetIndex();
			}

			lua_pushnumber(L, targetidx);

			return 1;
		}
	}

	namespace LeagueEngine
	{
		static int GetLeagueTickCount(lua_State* L)
		{
			lua_pushnumber(L, (float)GetTickCount());
			return 1;
		}

		static int GetAttackDelay(lua_State* L)
		{
			lua_pushnumber(L, Engine::GetLocalObject()->GetAttackDelay());
			return 1;
		}

		static int GetAttackCastDelay(lua_State* L)
		{
			lua_pushnumber(L, Engine::GetLocalObject()->GetAttackCastDelay());
			return 1;
		}

		static int SendChat(lua_State* L)
		{
			std::string msg = (std::string)lua_tostring(L, 1);
			Engine::SendChat(msg.c_str());

			return 1;
		}
	}

	namespace Functions
	{
		static int IssueMove(lua_State* L)
		{
			Engine::MoveTo(&Engine::GetMouseWorldPosition());

			return 1;
		}

		static int AttackTo(lua_State* L)
		{
			int targetIdx = (int)lua_tonumber(L, 1);

			CObject* target = Engine::GetObjectByID(targetIdx);

			Engine::AttackTarget(target);

			return 1;
		}
	}

	namespace Utils
	{
		static int Print(lua_State* L)
		{
			std::string msg = "\n[DEBUG] " + (std::string)lua_tostring(L, 1);
			Menu::Log(msg.c_str());
		}

		static int IsSpacebarDown(lua_State* L)
		{
			bool state = GetAsyncKeyState(VK_SPACE) & 0x8000;

			lua_pushboolean(L, state);

			return 1;
		}
	}
};