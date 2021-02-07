#include "stdafx.h"
#include "RenderLayer.h"
#include "Functions.h"

namespace HACKUZAN
{
	RenderLayer* RenderLayer::Instance;

	bool RenderLayer::WorldToScreen(Vector3 position, Vector2& out) {
		Vector3 temp;
		const auto result = Functions::WorldToScreen(position, temp);
		out.X = temp.X;
		out.Y = temp.Y;
		return result;
	}
}