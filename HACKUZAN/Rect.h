#pragma once

#include "Vector2.h"

namespace HACKUZAN {
	class Vector3;
	class Vector2;

	class Rect {
	public:
		static Rect Empty;

		Vector2 Position;
		float Width;
		float Height;

		Rect();
		Rect(Vector2 position, float width, float height);
		Rect(float x, float y, float width, float height);

		bool Contains(Vector2 point);
		bool Contains(Rect rect);

		bool Intersects(Rect rect, Rect* out = nullptr);

		Rect Union(Rect rect);

		bool operator==(Rect rect);
		bool operator!=(Rect rect);
	};
}
