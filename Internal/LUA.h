#pragma once

extern "C"
{
#include "LUA/include/lua.h"
#include "LUA/include/lauxlib.h"
#include "LUA/include/lualib.h"
#include "LUA/include/luaconf.h"
}

#pragma comment(lib, "LUA/lua54.lib")
extern lua_State* LuaVM;
extern bool lua_init;
class LUA
{
public:
	static bool CheckLua(lua_State* L, int r);
	static void InitLuaScripts();
	static void ReloadScripts();
};

inline int PutOnLuaStack(lua_State*)
{
	return 0;
}

template< typename T >
inline int PutOnLuaStack(lua_State* L, T& toPutOnStack)
{
	rttr::type typeOfT = rttr::type::get<T>();
	if (typeOfT.is_class())
	{
		//pass-by-reference
		rttr::variant v(&toPutOnStack);
		return ToLua(L, v);
	}
	else
	{
		//pass-by-value
		rttr::variant v(toPutOnStack);
		return ToLua(L, v);
	}
}

template< typename T, typename... T2 >
//Pushes arguments onto the LUA Stack
inline int PutOnLuaStack(lua_State* L, T& toPutOnStack, T2&... moreArgs)
{
	return PutOnLuaStack(L, toPutOnStack) + PutOnLuaStack(L, moreArgs...);
}

template< typename... ARGS >
//Calls LUA function with the given Name
void CallScriptFunction(lua_State* L, const char* funcName, ARGS&... args)
{
	

	lua_getglobal(L, funcName);
	if (lua_type(L, -1) == LUA_TFUNCTION)
	{
		int numArgs = PutOnLuaStack(L, args...);
		if (lua_pcall(L, numArgs, 0, 0) != 0)
		{
			printf("unable to call script function '%s', '%s'\n", funcName, lua_tostring(L, -1));
			luaL_error(L, "unable to call script function '%s', '%s'", funcName, lua_tostring(L, -1));
		}
	}
	else
	{
		printf("unknown script function '%s'\n", funcName);
		luaL_error(L, "unknown script function '%s'", funcName);
	}
}