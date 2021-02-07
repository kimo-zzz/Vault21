#include "stdafx.h"
#include "GameClient.h"

namespace HACKUZAN {
	GameClient* GameClient::Instance;


	void GameClient::PrintChat(const char* Message, DWORD color)
	{
		typedef void(__thiscall* _fnPrintChat)(DWORD ChatClient, const char* Message, int Color);
		static _fnPrintChat PrintChat = (_fnPrintChat)(baseAddr + (DWORD)Offsets::GameClient::PrintChat);
		static DWORD chatClient = *(DWORD*)(baseAddr + (DWORD)Offsets::GameClient::ChatClientPtr);

		char messageBuffer[100];
		int size = sprintf(messageBuffer, "<font color='#%06X'>%s</font>", color & 0x00FFFFFF, Message);

		return PrintChat(chatClient, messageBuffer, 1);

	}

	template<typename ...Args>
	void GameClient::PrintChat(const char* message, DWORD color, Args ... args)
	{
		char messageBuffer[100];
		sprintf(messageBuffer, message, args...);
		GameClient::PrintChat(messageBuffer, color);
	}

	bool GameClient::StringEquals(const char* strA, const char* strB, bool ignore_case)
	{
		if (!ignore_case) {
			return strcmp(strA, strB) == 0;
		}		
		return strlen(strA) == strlen(strB) && _stricmp(strA, strB) == 0;
	}

}