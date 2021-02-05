#include "stdafx.h"
#include "ClockFacade.h"
#include "Functions.h"

namespace HACKUZAN
{
	//ClockFacade* ClockFacade::Instance;

	float ClockFacade::GetGameTime()
	{
		return *(float*)(baseAddr + (DWORD)Offsets::ClockFacade::GameTime);
	}

	int ClockFacade::GameTickCount()
	{
		return (int)(GetGameTime() * 1000);
	}

}