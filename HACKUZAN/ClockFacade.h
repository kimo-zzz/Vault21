#pragma once

#include "Offsets.h"

namespace HACKUZAN
{
	class ClockFacade
	{
	public:
		//static ClockFacade* Instance;

		union
		{
			DEFINE_MEMBER_N(DWORD* FrameClock, Offsets::ClockFacade::FrameClock)
		};

		static float GetGameTime();
		static int GameTickCount();
	};
}