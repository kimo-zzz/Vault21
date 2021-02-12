#pragma once

#include "Offsets.h"

namespace V21
{
	class ClockFacade
	{
	public:
		//static ClockFacade* Instance;

		union
		{
			//DEFINE_MEMBER_N(DWORD* FrameClock, Offsets::ClockFacade::FrameClock)
		};

		static float GetGameTime();
		static int GameTickCount();
	};
}