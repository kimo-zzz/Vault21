#pragma once

#include "Offsets.h"
#include "Vector2.h"
#include "HudCursorTargetLogic.h"

namespace HACKUZAN
{
	class HudManager
	{
	public:
		static HudManager* Instance;
		static Vector2 CursorPos2D;

		union
		{
			DEFINE_MEMBER_N(HudCursorTargetLogic* CursorTargetLogic, Offsets::HudManager::CursorTargetLogic)
		};
	};
}