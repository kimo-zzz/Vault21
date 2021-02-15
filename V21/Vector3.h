#pragma once
#include "Vector2.h"
#include "NavGrid.h"
namespace V21 {

	class Vector2;

	class Vector3
	{
	public:
		float X;
		float Y;
		float Z;

		Vector3()
		{
		}

		explicit Vector3(const float x, const float y, const float z)
		{
			this->X = x;
			this->Y = y;
			this->Z = z;
		}

		void Set(const float x, const float y, const float z)
		{
			this->X = x;
			this->Y = y;
			this->Z = z;
		}

		void Zero()
		{
			this->X = 0;
			this->Y = 0;
			this->Z = 0;
		}

		bool IsValid() {
			return *this != Vector3(0, 0, 0);
		}

		float operator[](int index) const
		{
			return (&X)[index];
		}

		float& operator[](int index)
		{
			return (&X)[index];
		}

		Vector3 operator-() const
		{
			return Vector3(-X, -Y, -Z);
		}

		float operator*(Vector3& a) const
		{
			return ((X * a.X) + (Y * a.Y) + (Z * a.Z));
		}

		Vector3 operator*(const float a) const
		{
			return Vector3(X * a, Y * a, Z * a);
		}

		Vector3 operator/(const float a) const
		{
			float inva = 1.f / a;
			return Vector3(X * inva, Y * inva, Z * inva);
		}

		Vector3 operator+(const Vector3& a) const
		{
			return Vector3(X + a.X, Y + a.Y, Z + a.Z);
		}

		Vector3 operator-(const Vector3& a) const
		{
			return Vector3(X - a.X, Y - a.Y, Z - a.Z);
		}

		Vector3& operator+=(const Vector3& a)
		{
			X += a.X;
			Y += a.Y;
			Z += a.Z;
			return *this;
		}

		Vector3& operator-=(const Vector3& a)
		{
			X -= a.X;
			Y -= a.Y;
			Z -= a.Z;
			return *this;
		}

		Vector3& operator/=(const Vector3& a)
		{
			X /= a.X;
			Y /= a.Y;
			Z /= a.Z;
			return *this;
		}

		Vector3& operator/=(const float a)
		{
			X /= a;
			Y /= a;
			Z /= a;
			return *this;
		}

		Vector3& operator*=(const float a)
		{
			X *= a;
			Y *= a;
			Z *= a;
			return *this;
		}

		friend Vector3 operator*(const float a, const Vector3 b)
		{
			return Vector3(b.X * a, b.Y * a, b.Z * a);
		}

		bool operator==(const Vector3& a) const
		{
			return Compare(a) == true;
		}

		bool operator!=(const Vector3& a) const
		{
			return Compare(a) == false;
		}

		bool Compare(const Vector3& a) const
		{
			return ((X == a.X) && (Y == a.Y) && (Z == a.Z));
		}

		Vector3& Normalized()
		{
			float len = Length();

			*this /= len;

			return *this;
		}

		float Length()
		{
			return sqrtf(X * X + Y * Y + Z * Z);
		}

		float Length2D()
		{
			return sqrtf(X * X + Z * Z);
		}

		float DotProduct(const Vector3& vecOther)
		{
			return ((X * vecOther.X) + (Y * vecOther.Y) + (Z * vecOther.Z));
		}

		Vector3& toGround()
		{
			//this->Z = NavGrid::Instance->GetHeightForPosition(this->X, this->Y);
			return Vector3(this->X, this->Z, 0);
			return *this;
		}

		Vector2 To2D() const
		{
			return Vector2(X, Z);
		}

		Vector3 Rotated(float Angle)
		{
			float c = cosf(Angle);
			float s = sinf(Angle);

			return Vector3(X * c - Z * s, Y, Z * c + X * s);
		}

		float Distance2D(const Vector3 vecOther)
		{
			return ((vecOther - *this).To2D().Length());
		}

		Vector3 Extended(Vector3 const Other, float Len)
		{
			Vector3 vecThis = *this;
			return (vecThis + (Other - vecThis).Normalized() * Len);
		}

		float Distance(const Vector3 vecOther)
		{
			return ((vecOther - *this).Length());
		}

		bool Intersects(Vector3 const& Seg1End, Vector3 const& Seg2Start, Vector3 const& Seg2End, Vector3* IntersectionOut)
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
				*IntersectionOut = Vector3(X + r * deltaBAx, Y + r * deltaBAy, Z + r * deltaBAy);

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
		float AngleBetween(Vector3 const& Other) const
		{
			float flTheta = Polar() - Other.Polar();

			if (flTheta < 0)
				flTheta += 360.f;

			if (flTheta > 180.f)
				flTheta = 360.f - flTheta;

			return flTheta;
		}

		Vector3 Perpendicular()
		{
			return Vector3(Z, Y, -X);
		}
		Vector3 Perpendicular2()
		{
			return Vector3(-Z, Y, X);
		}

		float DistanceSquared(Vector3 v) {
			auto x = this->X - v.X;
			auto z = this->Z - v.Z;
			return x * x + z * z;
		}

		bool IsInRange(Vector3 v, float range) {
			return this->DistanceSquared(v) < range * range;
		}
	};
}
