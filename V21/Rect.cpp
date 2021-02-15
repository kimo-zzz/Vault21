#include "stdafx.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"

namespace V21 {
	Rect Rect::Empty = Rect();

	Rect::Rect() {
		this->Position = Vector2(0, 0);
		this->Width = 0.0f;
		this->Height = 0.0f;
	}

	Rect::Rect(Vector2 position, float width, float height)
	{
		this->Position = position;
		this->Width = width;
		this->Height = height;
	}

	Rect::Rect(float x, float y, float width, float height) {
		this->Position = Vector2(x, y);
		this->Width = width;
		this->Height = height;
	}

	bool Rect::Contains(Vector2 point) {
		return point.X >= this->Position.X && point.X < this->Position.X + this->Width && point.Y >= this->Position.Y && point.Y < this->Position.Y + this->Height;
	}

	bool Rect::Contains(Rect rect) {
		return rect.Position.X >= this->Position.X && rect.Position.X + rect.Width <= this->Position.X + this->Width && rect.Position.Y >= this->Position.Y && rect.Position.Y + rect.Height <= this->Position.Y + this->Height;
	}

	bool Rect::Intersects(Rect rect, Rect* out) {
		auto newLeft = std::max(this->Position.X, rect.Position.X);
		auto newTop = std::max(this->Position.Y, rect.Position.Y);
		auto newRight = std::min(this->Position.X + this->Width, rect.Position.X + rect.Width);
		auto newBottom = std::min(this->Position.Y + this->Height, rect.Position.Y + rect.Height);

		if (newRight > newLeft && newBottom > newTop) {
			if (out) {
				*out = Rect(newLeft, newTop, newRight - newLeft, newBottom - newTop);
			}
			return true;
		}

		return false;
	}

	Rect Rect::Union(Rect rect) {
		auto left = std::min(this->Position.X, rect.Position.X);
		auto top = std::min(this->Position.Y, rect.Position.Y);
		auto right = std::max(this->Position.X + this->Width, rect.Position.X + rect.Width);
		auto bottom = std::max(this->Position.Y + this->Height, rect.Position.Y + rect.Height);
		return Rect(left, top, right - left, bottom - top);
	}

	bool Rect::operator==(Rect rect) {
		return this->Position.X == rect.Position.X && this->Position.Y == rect.Position.Y && this->Width == rect.Width && this->Height == rect.Height;
	}

	bool Rect::operator!=(Rect rect) {
		return this->Position.X != rect.Position.X || this->Position.Y != rect.Position.Y || this->Width != rect.Width || this->Height != rect.Height;
	}
}
