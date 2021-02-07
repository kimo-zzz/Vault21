/*#include "stdafx.h"
#include "Vector3.h"
#include "Vector2.h"

namespace HACKUZAN {
	Vector3 Vector3::Zero = Vector3();

	Vector3::Vector3() {
		this->X = 0.0f;
		this->Y = 0.0f;
		this->Z = 0.0f;
	}

	Vector3::Vector3(float x, float y, float z) {
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

	bool Vector3::IsValid() {
		return *this != Vector3::Zero;
	}

	float Vector3::Length() {
		return sqrtf(this->X * this->X + this->Z * this->Z);
	}

	float Vector3::LengthSquared() {
		return this->X* this->X + this->Z * this->Z;
	}

	Vector2 Vector3::To2D() const
	{
		return Vector2(X, Y);
	}

	void Vector3::Normalize() {
		auto length = this->Length();
		this->X /= length;
		this->Z /= length;
	}

	Vector3 Vector3::Normalized() {
		auto result = Vector3(this->X, this->Y, this->Z);
		result.Normalize();
		return result;
	}

	void Vector3::Extend(Vector3 v, float distance) {
		auto normalized = (v - *this).Normalized();
		this->X = this->X + distance * normalized.X;
		this->Z = this->Z + distance * normalized.Z;
	}

	Vector3 Vector3::Extended(Vector3 v, float distance) {
		auto result = Vector3(this->X, this->Y, this->Z);
		result.Extend(v, distance);
		return result;
	}

	void Vector3::Shorten(Vector3 v, float distance) {
		auto normalized = (v - *this).Normalized();
		this->X = this->X - distance * normalized.X;
		this->Z = this->Z - distance * normalized.Z;
	}

	Vector3 Vector3::Shortened(Vector3 v, float distance) {
		auto result = Vector3(this->X, this->Y, this->Z);
		result.Shorten(v, distance);
		return result;
	}

	void Vector3::Rotate(float angle) {
		auto c = cosf(angle);
		auto s = sinf(angle);

		this->X = this->X * c - this->Z * s;
		this->Z = this->Z * c + this->X * s;
	}

	Vector3 Vector3::Rotated(float angle) {
		auto result = Vector3(this->X, this->Y, this->Z);
		result.Rotate(angle);
		return result;
	}

	float Vector3::Distance(Vector3 v) {
		float x = this->X - v.X;
		float z = this->Z - v.Z;
		return sqrtf(x * x + z * z);
	}

	float Vector3::Distance(Vector3 const& to) const
	{
		Vector3 delta;

		delta.X = X - to.X;
		delta.Y = Y - to.Y;

		return delta.Length();
	}

	float Vector3::DistanceSquared(Vector3 v) {
		auto x = this->X - v.X;
		auto z = this->Z - v.Z;
		return x * x + z * z;
	}

	float Vector3::Dot(Vector3 v) {
		return this->X * v.X + this->Z * v.Z;
	}

	float Vector3::Cross(Vector3 v) {
		return this->X * v.Z - this->Z * v.X;
	}

	float Vector3::AngleBetween(Vector3 v) {
		auto vec = v - *this;
		return atan2f(vec.Z, vec.X)* 180.0f* M_1_PI;
	}

	Vector3 Vector3::Perpendicular() {
		return Vector3(-this->Z, this->Y, this->X);
	}

	Vector3 Vector3::Perpendicular2() {
		return Vector3(this->Z, this->Y, -this->X);
	}

	bool Vector3::IsInRange(Vector3 v, float range) {
		return this->DistanceSquared(v) < range* range;
	}

	bool Vector3::operator==(Vector3 v) {
		return this->X == v.X && this->Y == v.Y && this->Z == v.Z;
	}

	bool Vector3::operator!=(Vector3 v) {
		return this->X != v.X || this->Y != v.Y || this->Z != v.Z;
	}

	Vector3 operator+(Vector3 left, Vector3 right) {
		return Vector3(left.X + right.X, left.Y, left.Z + right.Z);
	}

	Vector3 operator+(Vector3 v, float scalar) {
		return Vector3(v.X + scalar, v.Y + scalar, v.Z + scalar);
	}

	Vector3 operator+(float scalar, Vector3 v) {
		return Vector3(scalar + v.X, scalar + v.Y, scalar + v.Z);
	}

	Vector3 operator-(Vector3 v) {
		return Vector3(-v.X, -v.Y, -v.Z);
	}

	Vector3 operator-(Vector3 left, Vector3 right) {
		return Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
	}

	Vector3 operator-(Vector3 v, float scalar) {
		return Vector3(v.X - scalar, v.Y - scalar, v.Z - scalar);
	}

	Vector3 operator-(float scalar, Vector3 v) {
		return Vector3(scalar - v.X, scalar - v.Y, scalar - v.Z);
	}

	Vector3 operator*(Vector3 left, Vector3 right) {
		return Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
	}

	Vector3 operator*(Vector3 v, float scale) {
		return Vector3(v.X * scale, v.Y * scale, v.Z * scale);
	}

	Vector3 operator*(float scale, Vector3 v) {
		return Vector3(scale * v.X, scale * v.Y, scale * v.Z);
	}

	Vector3 operator/(Vector3 v, Vector3 scale) {
		return Vector3(v.X / scale.X, v.Y / scale.Y, v.Z / scale.Z);
	}

	Vector3 operator/(Vector3 v, float scale) {
		return Vector3(v.X / scale, v.Y / scale, v.Z / scale);
	}

	Vector3 operator/(float scale, Vector3 v) {
		return Vector3(scale / v.X, scale / v.Y, scale / v.Z);
	}
}*/