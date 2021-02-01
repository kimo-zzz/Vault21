#pragma once
#include "stdafx.h"

struct Vector
{
	float X, Y, Z;

	Vector(void)
	{
	}

	Vector(const float x, const float y, const float z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	Vector operator +(const Vector& A) const
	{
		return Vector(X + A.X, Y + A.Y, Z + A.Z);
	}

	Vector operator +(const float A) const
	{
		return Vector(X + A, Y + A, Z + A);
	}

	Vector operator *(const float A) const
	{
		return Vector(A * X, A * Y, A * Z);
	}

	Vector operator *(const Vector& A) const
	{
		return Vector(A.X * X, A.Y * Y, A.Z * Z);
	}

	Vector operator -(const float A) const
	{
		return Vector(A * X, A * Y, A * Z);
	}

	Vector operator -(const Vector& A) const
	{
		return Vector(A.X - X, A.Y - Y, A.Z - Z);
	}

	Vector operator /(const float A) const
	{
		return Vector(A / X, A / Y, A / Z);
	}

	Vector operator /(const Vector& A) const
	{
		return Vector(A.X / X, A.Y / Y, A.Z / Z);
	}

	float dot(const Vector& vec) const
	{
		return X * vec.X + Y * vec.Y + Z * vec.Z;
	}

	float lengthSquared()
	{
		return X * X + Y * Y + Z * Z;
	}

	float length()
	{
		return static_cast<float>(sqrt(lengthSquared()));
	}

	Vector perpendicularTo()
	{
		return Vector(Z, Y, -X);
	}

	Vector Normalize()
	{
		float length = this->length();
		if (length != 0)
		{
			float inv = 1.0f / length;
			X *= inv;
			Y *= inv;
			Z *= inv;
		}
		return Vector(X, Y, Z);
	}

	float DistTo(const Vector& A)
	{
		float out = sqrtf(powf(X - A.X, 2) + powf(Y - A.Y, 2) + powf(Z - A.Z, 2));
		return out < 0 ? out * -1 : out;
	}
};