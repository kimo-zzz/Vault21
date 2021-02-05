#pragma once

#include "Offsets.h"

namespace HACKUZAN
{
	enum class GameState
	{
		PreGame,
		Spawn,
		GameLoop,
		EndGame,
		PreExit,
		Exit,
	};

	class GameClient
	{
	public:
		static GameClient* Instance;

		union
		{
			DEFINE_MEMBER_N(GameState GameState, Offsets::GameClient::GameState)
		};

		static void PrintChat(const char* Message, DWORD color);
		template<typename ...Args>
		static void PrintChat(const char* message, DWORD color, Args ... args);

		static bool StringEquals(const char* strA, const char* strB, bool ignore_case);
	};
}