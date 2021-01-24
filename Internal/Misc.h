#pragma once
#include <string>
#include "Engine.h"

extern std::string tiltMessages[];

class Misc
{
public:
	//Sends tilting messages when an enemy dies
	static void AutoTilt();
};
