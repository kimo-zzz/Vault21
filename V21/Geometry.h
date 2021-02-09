#pragma once
#include "stdafx.h"
#include "Template.h"
#include "Vector3.h"
#include "Vector2.h"
#include "cmath"
#include <map>
#define PI 3.14159265f

namespace HACKUZAN {

	//lay khoang cach ne may ban
	inline Vector2 ToVec2(Vector3 vec)
	{
		return Vector2(vec.X, vec.Z);
	}
	inline Vector3 ToVec3(Vector2 vec)
	{
		return Vector3(vec.X, 0, vec.Y);
	}
	inline Vector3 Normalize(Vector3 x)
	{
		auto X1 = ToVec2(x);
		float sqr = X1.X * X1.X + X1.Y * X1.Y;
		return ToVec3((X1 * (1.0f / sqrt(sqr))));
	}
	inline Vector3 Pendicular(Vector3 x)
	{
		auto X1 = ToVec2(x);
		Vector2 X2;
		X2.X = -X1.Y;
		X2.Y = X1.X;
		return ToVec3(X2);
	}

	inline Vector3 Pendicular2(Vector3 x)
	{
		auto X1 = ToVec2(x);
		Vector2 X2;
		X2.X = X1.Y;
		X2.Y = -X1.X;
		return ToVec3(X2);
	}

	inline float DistanceSqr(Vector3 from, Vector3 to)
	{
		float distance = (from.To2D() - to.To2D()).LengthSquared();
		if (isnan(distance))
			return 0;
		return distance;
	}
	inline float Distance(Vector3 from, Vector3 to)
	{
		float distance = (from.To2D() - to.To2D()).Length();
		if (isnan(distance))
			return 0;
		return distance;
	}
	inline float AngleBetween(Vector3 a, Vector3 center, Vector3 c)
	{
		float a1 = Distance(c, center);
		float b1 = Distance(a, c);
		float c1 = Distance(center, a);
		if (a1 == 0 || c1 == 0) { return 0; }
		else
		{
			return acos((a1 * a1 + c1 * c1 - b1 * b1) / (2 * a1 * c1)) * (180 / PI);
		}
	}
	inline float AngleToRadian(float Angle)
	{
		return Angle * PI / 180.f;
	}
	inline Vector3 RotateAround(Vector3 pointToRotate3D, Vector3 centerPoint3D, float angleInDegree)
	{
		auto angleInRadians = AngleToRadian(angleInDegree);
		double cosTheta = cos(angleInRadians);
		double sinTheta = cos(angleInRadians);
		Vector2 pointToRotate = ToVec2(pointToRotate3D);
		Vector2 centerPoint = ToVec2(centerPoint3D);
		Vector2 vec2Return
		(
			(cosTheta * (pointToRotate.X - centerPoint.X) -
				sinTheta * (pointToRotate.Y - centerPoint.Y) + centerPoint.X),

			(sinTheta * (pointToRotate.X - centerPoint.X) +
				cosTheta * (pointToRotate.Y - centerPoint.Y) + centerPoint.Y)
		);
		return ToVec3(vec2Return);
	}

	inline bool InTheCone(Vector3 pos, Vector3 centerconePolar, Vector3 centerconeEnd, float coneAngle)

	{

		return AngleBetween(pos, centerconePolar, centerconeEnd) < coneAngle / 2

			&& Distance(pos, centerconePolar) < Distance(centerconePolar, centerconeEnd);

	}
	inline float Distance(Vector3 point, Vector3 segmentStart, Vector3 segmentEnd, bool onlyIfOnSegment = false)
	{
		auto a = ToVec2(point);
		auto b = ToVec2(segmentStart);
		auto c = ToVec2(segmentEnd);
		float STriangle = 0.5f * abs((b.X - a.X) * (c.Y - a.Y) - (c.X - a.X) * (b.Y - a.Y));
		float botLength = Distance(segmentStart, segmentEnd);
		float distance = 2 * STriangle / botLength;
		if (!onlyIfOnSegment)
			return distance;
		if (AngleBetween(point, segmentStart, segmentEnd) > 90 || AngleBetween(point, segmentEnd, segmentStart) > 90)
			return 100000.f;
		return distance;
	}
	inline float Distance(GameObject* from, GameObject* to)
	{
		return Distance(from->Position, to->Position);
	}
	inline float Distance(GameObject* from, Vector3 to)
	{
		return Distance(from->Position, to);
	}
	inline float Distance(Vector2 from, Vector2 to)
	{
		return Distance(ToVec3(from), ToVec3(to));
	}
	//dich chuyen ex- vector tend

	inline Vector2 Extend(Vector2 from, Vector2 to, float distance)
	{
		auto direction = (to - from).Normalized();
		return from + direction * distance;
	}
	inline Vector3 Extend(Vector3 from, Vector3 to, float distance)
	{
		return ToVec3(Extend(from.To2D(), to.To2D(), distance));
	}
	inline SArray<Vector3> GetCircleCircleIntersections(Vector3 center1, Vector3 center2, float radius1, float radius2)
	{
		SArray<Vector3> result;
		float D = Distance(center2, center1);
		//The Circles dont intersect:
		if (D > radius1 + radius2 || (D <= abs(radius1 - radius2)))
		{
			return result;
		}

		float A = (radius1 * radius1 - radius2 * radius2 + D * D) / (2 * D);
		float H = sqrt(radius1 * radius1 - A * A);
		Vector3 Direction = (center2 - center1).Normalized();
		Vector3 PA = ToVec3(ToVec2(center1) + A * ToVec2(Direction));
		Vector3 S1 = ToVec3(ToVec2(PA) + H * ToVec2(Pendicular(Direction)));
		Vector3 S2 = ToVec3((ToVec2(PA) - H * ToVec2(Pendicular(Direction))));
		result.Add(S1);
		result.Add(S2);
		return result;
	}

	inline bool GetSegmentSegmentIntersections(
		Vector2 lineSegment1Start,
		Vector2 lineSegment1End,
		Vector2 lineSegment2Start,
		Vector2 lineSegment2End,
		Vector2& intersectpoint)
	{
		float deltaACy = lineSegment1Start.Y - lineSegment2Start.Y;
		float deltaDCx = lineSegment2End.X - lineSegment2Start.X;
		float deltaACx = lineSegment1Start.X - lineSegment2Start.X;
		float deltaDCy = lineSegment2End.Y - lineSegment2Start.Y;
		float deltaBAx = lineSegment1End.X - lineSegment1Start.X;
		float deltaBAy = lineSegment1End.Y - lineSegment1Start.Y;

		float denominator = deltaBAx * deltaDCy - deltaBAy * deltaDCx;
		float numerator = deltaACy * deltaDCx - deltaACx * deltaDCy;

		if (abs(denominator) < std::numeric_limits<float>::epsilon())
		{
			if (abs(numerator) < std::numeric_limits<float>::epsilon())
			{
				// collinear. Potentially infinite intersection points.
				// Check and return one of them.
				if (lineSegment1Start.X >= lineSegment2Start.X && lineSegment1Start.X <= lineSegment2End.X)
				{
					intersectpoint = lineSegment1Start;
					return true;
				}
				if (lineSegment2Start.X >= lineSegment1Start.X && lineSegment2Start.X <= lineSegment1End.X)
				{
					intersectpoint = lineSegment2Start;
					return true;
				}
				return false;
			}
			// parallel
			return false;
		}

		float r = numerator / denominator;
		if (r < 0 || r > 1)
		{
			return false;
		}

		float s = (deltaACy * deltaBAx - deltaACx * deltaBAy) / denominator;
		if (s < 0 || s > 1)
		{
			return false;
		}
		intersectpoint.X = lineSegment1Start.X + r * deltaBAx;
		intersectpoint.Y = lineSegment1Start.Y + r * deltaBAy;
		return true;
	}
	inline bool GetSegmentSegmentIntersections(
		Vector3 lineSegment1Start,
		Vector3 lineSegment1End,
		Vector3 lineSegment2Start,
		Vector3 lineSegment2End)
	{
		Vector2 out;
		return GetSegmentSegmentIntersections(ToVec2(lineSegment1Start), ToVec2(lineSegment1End), ToVec2(lineSegment2Start), ToVec2(lineSegment2End), out);
	}
	inline Vector2 GetLineLineIntersections(Vector2 A1, Vector2 A2, Vector2 B1, Vector2 B2)
	{
		float a1 = A2.Y - A1.Y;
		float b1 = A1.X - A2.X;
		float c1 = a1 * A1.X + b1 * A1.Y;

		float a2 = B2.Y - B1.Y;
		float b2 = B1.X - B2.X;
		float c2 = a2 * B1.X + b2 * B1.Y;

		float det = a1 * b2 - a2 * b1;
		float x = (b2 * c1 - b1 * c2) / det;
		float y = (a1 * c2 - a2 * c1) / det;
		return Vector2(x, y);
	}
}