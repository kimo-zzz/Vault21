/*#include "stdafx.h"
#include "Vector2.h"
#include "ObjectManager.h"

namespace HACKUZAN {
	__forceinline float Vector2::ToDegree(float Args)
	{
		const float flPi = 3.141592654f;
		return (Args * (180.f / flPi));
	}

	Vector2 Vector2::Zero = Vector2();

	Vector2::Vector2() {
		this->X = 0.0f;
		this->Y = 0.0f;
	}

	Vector2::Vector2(float x, float y) {
		this->X = x;
		this->Y = y;
	}

	bool Vector2::IsValid() {
		return *this != Vector2::Zero;
	}

	float Vector2::Length() {
		return sqrtf(this->X * this->X + this->Y * this->Y);
	}

	float Vector2::LengthSquared() {
		return this->X* this->X + this->Y * this->Y;
	}

	void Vector2::Normalize() {
		auto length = this->Length();
		this->X /= length;
		this->Y /= length;
	}

	Vector2 Vector2::Normalized() {
		//auto result = Vector2(this->X, this->Y);
		//result.Normalize();
		auto length = this->Length();
		this->X /= length;
		this->Y /= length;
		return *this;
	}

	void Vector2::Extend(Vector2 v, float distance) {
		auto normalized = (v - *this).Normalized();
		this->X = this->X + distance * normalized.X;
		this->Y = this->Y + distance * normalized.Y;
	}

	Vector2 Vector2::Extended(Vector2 v, float distance) {
		auto result = Vector2(this->X, this->Y);
		result.Extend(v, distance);
		return result;
	}

	void Vector2::Shorten(Vector2 v, float distance) {
		auto normalized = (v - *this).Normalized();
		this->X = this->X - distance * normalized.X;
		this->Y = this->Y - distance * normalized.Y;
	}

	Vector2 Vector2::Shortened(Vector2 v, float distance) {
		auto result = Vector2(this->X, this->Y);
		result.Shorten(v, distance);
		return result;
	}

	void Vector2::Rotate(float angle) {
		auto c = cosf(angle);
		auto s = sinf(angle);

		this->X = this->X * c - this->Y * s;
		this->Y = this->Y * c + this->X * s;
	}

	Vector2 Vector2::Rotated(float angle) {
		auto result = Vector2(this->X, this->Y);
		result.Rotate(angle);
		return result;
	}

	float Vector2::Distance(Vector2 v) {
		auto x = this->X - v.X;
		auto y = this->Y - v.Y;
		return sqrtf(x * x + y * y);
	}

	float Vector2::DistanceSquared(Vector2 v) {
		auto x = this->X - v.X;
		auto y = this->Y - v.Y;
		return x * x + y * y;
	}

	float Vector2::Dot(Vector2 v) {
		return this->X* v.X + this->Y * v.Y;
	}

	float Vector2::Cross(Vector2 v) {
		return this->X* v.Y - this->Y * v.X;
	}

	float Vector2::AngleBetween(Vector2 v) {
		auto vec = v - *this;
		return atan2f(vec.Y, vec.X) * 180.0f * M_1_PI;
	}

	Vector2 Vector2::Perpendicular() {
		return Vector2(-this->Y, this->X);
	}

	Vector2 Vector2::Perpendicular2() {
		return Vector2(this->Y, -this->X);
	}

	bool Vector2::IsInRange(Vector2 v, float range) {
		return this->DistanceSquared(v) < range* range;
	}
	
	bool Vector2::operator==(Vector2 v) {
		return this->X == v.X && this->Y == v.Y;
	}

	bool Vector2::operator!=(Vector2 v) {
		return this->X != v.X || this->Y != v.Y;
	}

	Vector2 operator+(Vector2 left, Vector2 right) {
		return Vector2(left.X + right.X, left.Y + right.Y);
	}

	Vector2 operator+(Vector2 v, float scalar) {
		return Vector2(v.X + scalar, v.Y + scalar);
	}

	Vector2 operator+(float scalar, Vector2 v) {
		return Vector2(scalar + v.X, scalar + v.Y);
	}

	Vector2 operator-(Vector2 v) {
		return Vector2(-v.X, -v.Y);
	}

	Vector2 operator-(Vector2 left, Vector2 right) {
		return Vector2(left.X - right.X, left.Y - right.Y);
	}

	Vector2 operator-(Vector2 v, float scalar) {
		return Vector2(v.X - scalar, v.Y - scalar);
	}

	Vector2 operator-(float scalar, Vector2 v) {
		return Vector2(scalar - v.X, scalar - v.Y);
	}

	Vector2 operator*(Vector2 left, Vector2 right) {
		return Vector2(left.X * right.X, left.Y * right.Y);
	}

	Vector2 operator*(Vector2 v, float scale) {
		return Vector2(v.X * scale, v.Y * scale);
	}

	Vector2 operator*(float scale, Vector2 v) {
		return Vector2(scale * v.X, scale * v.Y);
	}

	Vector2 operator/(Vector2 v, Vector2 scale) {
		return Vector2(v.X / scale.X, v.Y / scale.Y);
	}

	Vector2 operator/(Vector2 v, float scale) {
		return Vector2(v.X / scale, v.Y / scale);
	}

	Vector2 operator/(float scale, Vector2 v) {
		return Vector2(scale / v.X, scale / v.Y);
	}

	bool Vector2::Intersects(Vector2 const& Seg1End, Vector2 const& Seg2Start, Vector2 const& Seg2End, Vector2* IntersectionOut)
	{
		double deltaACy = Y - Seg2Start.Y;
		double deltaDCx = Seg2End.X - Seg2Start.X;
		double deltaACx = X - Seg2Start.X;
		double deltaDCy = Seg2End.Y - Seg2Start.Y;
		double deltaBAx = Seg1End.X - X;
		double deltaBAy = Seg1End.Y - Y;

		auto denominator = deltaBAx * deltaDCy - deltaBAy * deltaDCx;
		auto numerator = deltaACy * deltaDCx - deltaACx * deltaDCy;

		if (fabs(denominator) < FLT_EPSILON)
		{
			if (fabs(numerator) < FLT_EPSILON)
			{
				// collinear. Potentially infinite intersection points.
				// Check and return one of them.
				if (X >= Seg2Start.X && X <= Seg2End.X)
				{
					if (IntersectionOut)
						*IntersectionOut = *this;

					return true;
				}

				if (Seg2Start.X >= X && Seg2Start.X <= Seg1End.X)
				{
					if (IntersectionOut)
						*IntersectionOut = Seg2Start;

					return true;
				}

				return false;
			}

			// parallel
			return false;
		}

		auto r = numerator / denominator;

		if (r < 0 || r > 1)
			return false;

		auto s = (deltaACy * deltaBAx - deltaACx * deltaBAy) / denominator;

		if (s < 0 || s > 1)
			return false;

		if (IntersectionOut)
			*IntersectionOut = Vector2(X + r * deltaBAx, Y + r * deltaBAy);

		return true;
	}

	bool Vector2::Close(float a, float b, float eps) const
	{
		if (fabs(eps) < FLT_EPSILON)
			eps = (float)1e-9;

		return fabs(a - b) <= eps;
	}

	float Vector2::Polar() const
	{
		if (Close(X, 0, 0))
		{
			if (Y > 0)
				return 90;

			return Y < 0 ? 270.f : 0.f;
		}

		float flTheta = ToDegree(atanf(Y / X));

		if (X < 0)
			flTheta += 180.f;

		if (flTheta < 0)
			flTheta += 360.f;

		return flTheta;
	}

	float Vector2::Distance(Vector2 const& to) const
	{
		Vector2 delta;

		delta.X = X - to.X;
		delta.Y = Y - to.Y;

		return delta.Length();
	}

	bool Vector2::IsUnderEnemyTurret() const
	{
		auto turret_list = HACKUZAN::GameObject::GetTurrets();
		for (size_t i = 0; i < turret_list->size; i++)
		{
			auto turret = turret_list->entities[i];

			if (turret->IsValidTarget() && turret->IsEnemy() && turret->Position.To2D().Distance(*this) <= 900)
			{
				return true;
			}
		}
		return false;
	}


	int Vector2::CountEnemiesInRange(float range) const
	{
		auto count = 0;

		auto hero_list = HACKUZAN::GameObject::GetHeroes();
		for (size_t i = 0; i < hero_list->size; i++)
		{
			auto hero = hero_list->entities[i];
			if (hero->IsEnemy() && hero->IsValidTarget() && hero->Position.To2D().Distance(*this) < range)
			{
				count++;
			}
		}

		return count;
	}
}*/
