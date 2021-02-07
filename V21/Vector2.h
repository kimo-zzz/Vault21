#pragma once

namespace HACKUZAN {
	class Vector3;

	__forceinline float ToDegree(float Args)
	{
		const float flPi = 3.141592654f;
		return (Args * (180.f / flPi));
	}

	class  Vector2
	{
	public:
		float X;
		float Y;

		Vector2()
		{
		}

		explicit Vector2(const float x, const float y)
		{
			this->X = x;
			this->Y = y;
		}

		void Set(const float x, const float y)
		{
			this->X = x;
			this->Y = y;
		}

		void Zero()
		{
			this->X = 0;
			this->Y = 0;
		}

		float operator[](int index) const
		{
			return (&X)[index];
		}

		float& operator[](int index)
		{
			return (&X)[index];
		}

		Vector2 operator-() const
		{
			return Vector2(-X, -Y);
		}

		float operator*(Vector2& a) const
		{
			return ((X * a.X) + (Y * a.Y));
		}

		Vector2 operator*(const float a) const
		{
			return Vector2(X * a, Y * a);
		}

		Vector2 operator/(const float a) const
		{
			float inva = 1.f / a;
			return Vector2(X * inva, Y * inva);
		}

		Vector2 operator+(const Vector2& a) const
		{
			return Vector2(X + a.X, Y + a.Y);
		}

		Vector2 operator-(const Vector2& a) const
		{
			return Vector2(X - a.X, Y - a.Y);
		}

		Vector2& operator+=(const Vector2& a)
		{
			X += a.X;
			Y += a.Y;
			return *this;
		}

		Vector2& operator-=(const Vector2& a)
		{
			X -= a.X;
			Y -= a.Y;
			return *this;
		}

		Vector2& operator/=(const Vector2& a)
		{
			X /= a.X;
			Y /= a.Y;
			return *this;
		}

		Vector2& operator/=(const float a)
		{
			X /= a;
			Y /= a;
			return *this;
		}

		Vector2& operator*=(const float a)
		{
			X *= a;
			Y *= a;
			return *this;
		}

		friend Vector2 operator*(const float a, const Vector2 b)
		{
			return Vector2(b.X * a, b.Y * a);
		}

		bool operator==(const Vector2& a) const
		{
			return Compare(a) == true;
		}

		bool operator!=(const Vector2& a) const
		{
			return Compare(a) == false;
		}

		bool Compare(const Vector2& a) const
		{
			return ((X == a.X) && (Y == a.Y));
		}

		Vector2& Normalized()
		{
			float len = Length();

			*this /= len;

			return *this;
		}

		float Length()
		{
			return sqrtf(X * X + Y * Y);
		}

		float DotProduct(const Vector2& vecOther)
		{
			return ((X * vecOther.X) + (Y * vecOther.Y));
		}

		Vector2 Rotated(float Angle)
		{
			float c = cosf(Angle);
			float s = sinf(Angle);

			return Vector2(X * c - Y * s, Y * c + X * s);
		}

		Vector2 Perpendicular()
		{
			return Vector2(Y, -X);
		}

		Vector2 Perpendicular2()
		{
			return Vector2(-Y, X);
		}

		bool Intersects(Vector2 const& Seg1End, Vector2 const& Seg2Start, Vector2 const& Seg2End, Vector2* IntersectionOut)
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

		bool Close(float a, float b, float eps) const
		{
			if (fabs(eps) < FLT_EPSILON)
				eps = (float)1e-9;

			return fabs(a - b) <= eps;
		}

		float Polar() const
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

		bool IsValid()
		{
			return (Length() != 0);
		}

		float Distance(Vector2 const& Other) const
		{
			return (Other - *this).Length();
		}

		float AngleBetween(Vector2 const& Other) const
		{
			float flTheta = Polar() - Other.Polar();

			if (flTheta < 0)
				flTheta += 360.f;

			if (flTheta > 180.f)
				flTheta = 360.f - flTheta;

			return flTheta;
		}

		float LengthSquared()
		{
			return (X * X + Y * Y);
		}

		float DistanceTo(const Vector2& vecOther)
		{
			return (vecOther - *this).Length();
		}

		float DistanceSquared(const Vector2& vecOther)
		{
			return (vecOther - *this).LengthSquared();
		}

		Vector2 Extended(Vector2 const Other, float Len)
		{
			Vector2 vecThis = *this;
			return (vecThis + (Other - vecThis).Normalized() * Len);
		}
	};

	/*Vector2 operator+(Vector2 left, Vector2 right);
	Vector2 operator+(Vector2 v, float scalar);
	Vector2 operator+(float scalar, Vector2 v);
	Vector2 operator-(Vector2 v);
	Vector2 operator-(Vector2 left, Vector2 right);
	Vector2 operator-(Vector2 v, float scalar);
	Vector2 operator-(float scalar, Vector2 v);
	Vector2 operator*(Vector2 left, Vector2 right);
	Vector2 operator*(Vector2 v, float scale);
	Vector2 operator*(float scale, Vector2 v);
	Vector2 operator/(Vector2 v, Vector2 scale);
	Vector2 operator/(Vector2 v, float scale);
	Vector2 operator/(float scale, Vector2 v);*/
}
