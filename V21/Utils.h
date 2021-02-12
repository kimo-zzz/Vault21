#pragma once
#include "stdafx.h"
#include "Vector3.h"
#include "NavGrid.h"

namespace V21 {

	Vector3 Get3DPoint(Vector2 const& Position)
	{
		return Vector3(Position.X, NavGrid::Instance->GetHeightForPosition(Position.X, Position.Y), Position.Y);
	}
}