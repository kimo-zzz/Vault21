#pragma once

#include "Offsets.h"
#include "Vector3.h"

namespace HACKUZAN
{
	class HudCursorTargetLogic
	{
	public:
		union
		{
			DEFINE_MEMBER_N(Vector3 CursorPosition, Offsets::HudCursorTargetLogic::CursorPosition)
			DEFINE_MEMBER_N(unsigned short HoveredUnitId, Offsets::HudCursorTargetLogic::HoveredUnitId)
		};
	};
}