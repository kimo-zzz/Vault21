#include <string>
#include "menu.h"
#include "SDK.h"

bool lua_init = false;
lua_State* LuaVM = luaL_newstate();

static int  GetLocalPlayerIndex(int a)
{
	if (a > 1)
	{
		return Engine::GetLocalObject()->GetIndex();
	}
	return 0;
}

bool LUA::CheckLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::string error = lua_tostring(L, -1);
		Menu::Log(error.c_str());
		return false;
	}
	return true;
}



int wrapper(lua_State* L)
{
	int param1 = (int)lua_tonumber(L, 1); //push first param to LUA stack | 1 = first parameter

	int idx = GetLocalPlayerIndex(param1);

	lua_pushnumber(L, idx);
	return 1;
}



void LUA::InitLuaScripts()
{
	//TODO: Save all LUA files in folder into a list and execute them
	if (!lua_init)
	{
		luaL_openlibs(LuaVM);
	}

	//CObject
	lua_register(LuaVM, "GetLocalPlayer", SDK::GetLocalPlayer);
	lua_register(LuaVM, "GetHealth", SDK::GetHealth);
	lua_register(LuaVM, "GetChampionName", SDK::GetChampionName);

	//Engine
	lua_register(LuaVM, "SendChat", SDK::LeagueEngine::SendChat);
	lua_register(LuaVM, "IssueMove", SDK::Functions::IssueMove);
	lua_register(LuaVM, "AttackTo", SDK::Functions::AttackTo);
	lua_register(LuaVM, "GetAttackDelay", SDK::LeagueEngine::GetAttackDelay);
	lua_register(LuaVM, "GetAttackCastDelay", SDK::LeagueEngine::GetAttackCastDelay);
	lua_register(LuaVM, "GetMouseWorldPos", SDK::GetMousePos);


	//Orb
	lua_register(LuaVM, "GetLowestHPTarget", SDK::TS::GetLowestTarget);


	//Utils
	lua_register(LuaVM, "Debug", SDK::Utils::Print);
	lua_register(LuaVM, "IsSpacebarPressed", SDK::Utils::IsSpacebarDown);
	lua_register(LuaVM, "GetTickCount", SDK::LeagueEngine::GetLeagueTickCount);

	if (LUA::CheckLua(LuaVM, luaL_dofile(LuaVM, "Test.Lua")))
	{
		CallScriptFunction(LuaVM, "OnStart");
		lua_init = true;
	}
}

void LUA::ReloadScripts()
{
	if (GetAsyncKeyState(VK_F10))
	{
		lua_init = false;
		InitLuaScripts();
	}

}

