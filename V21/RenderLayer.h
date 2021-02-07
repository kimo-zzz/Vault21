#pragma once

#include "Offsets.h"
#include "Vector2.h"
#include "Vector3.h"

namespace HACKUZAN
{
	class RenderLayer
	{
	public:
		static RenderLayer* Instance;

		union
		{
			DEFINE_MEMBER_N(LONG ClientWidth, Offsets::RenderLayer::ClientWidth)
			DEFINE_MEMBER_N(LONG ClientHeight, Offsets::RenderLayer::ClientHeight)
		};

		static bool WorldToScreen(Vector3 position, Vector2& out);
	};
}