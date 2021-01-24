#include "Misc.h"
#include "Engine.h"
#include "Globals.h"
#include "Misc.h"

std::string tiltMessages[] =
{
	"lol",
	"xd",
	":^ )",
	"Why",
	"????",
	"Elo? XD",
	"ok",
	"Almost",
	"How can you be in Ranked with under lvl 30?",
	"jajajajajajajaja",
	"Is this Bronze??"

};

void Misc::AutoTilt()
{
	if (g_autoTilt)
	{
		if (GetAsyncKeyState(0x59))
		{
			auto rndmMsg = rand() % 11;

			std::string msgString = "/all " + tiltMessages[rndmMsg];

			Engine::SendChat(msgString.c_str());
		}
	}

}
