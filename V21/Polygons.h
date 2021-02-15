#ifndef Polygons_h__
#define Polygons_h__

#include "GameObject.h"
#include "clipper.h"
#include "Renderer.h"
#include "RenderLayer.h"
#include "Geometry.h"
#include "Draw.h"

#define M_PI static_cast<float>(3.141592)


namespace V21 {

	// utils
	static void DrawLine(Vector2 const Start, Vector2 const End, DWORD Color)
	{
		Vector3 vecStart3D = ToVec3(Start);
		Vector3 vecEnd3D = ToVec3(End);
		
		Vector3 start, end;
		Draw.WorldToScreen(vecStart3D, &start);
		Draw.WorldToScreen(vecEnd3D, &end);
		if (start.Z < 1.0f && end.Z < 1.0f)
			Renderer::AddLine(Vector2(start.X, start.Y), Vector2(end.X, end.Y), 2, Color);
		//Draw.Line(vecStart3D, vecEnd3D, 3, Color);
	}


	namespace Geometry
	{
		static std::vector<Vector2> CircleCircleIntersection(Vector2 center1, Vector2 center2, float radius1, float radius2)
		{
			std::vector<Vector2> vecOut;
			auto D = (center2 - center1).Length();

			//The Circles dont intersect:
			if (D > radius1 + radius2 || (D <= fabs(radius1 - radius2)))
				return vecOut;

			auto A = (radius1 * radius1 - radius2 * radius2 + D * D) / (2 * D);
			auto H = (float)sqrt(radius1 * radius1 - A * A);
			auto Direction = (center2 - center1).Normalized();
			auto PA = center1 + A * Direction;
			auto S1 = PA + H * Direction.Perpendicular();
			auto S2 = PA - H * Direction.Perpendicular();

			vecOut.push_back(S1);
			vecOut.push_back(S2);

			return vecOut;
		}

		struct IPolygon
		{
			std::vector<Vector2> Points;

			void Clear()
			{
				Points.clear();
			}

			void Add(Vector2 Point)
			{
				Points.push_back(Point);
			}

			void Add(Vector3 Point)
			{
				Points.push_back(Point.To2D());
			}

			void Add(IPolygon Other)
			{
				for (auto i : Other.Points)
					Points.push_back(i);
			}

			void Draw(D3DCOLOR Color, int Width = 1)
			{
				for (size_t i = 0; i <= Points.size() - 1; i++)
				{
					auto iNextIdx = (Points.size() - 1 == i) ? 0 : (i + 1);

					DrawLine(Points[i], Points[iNextIdx], Color);
				}
			}

			bool IsInside(Vector2 point)
			{
				return !IsOutside(point);
			}

			bool IsInside(Vector3 point)
			{
				return !IsOutside(point.To2D());
			}

			bool IsInside(GameObject* point)
			{
				if (point == nullptr)
					return false;

				return !IsOutside(ToVec2(point->Position));
			}


			bool IsOutside(Vector2 point)
			{
				auto p = ClipperLib::IntPoint(static_cast<ClipperLib::long64>(point.X), static_cast<ClipperLib::long64>(point.Y));
				auto v = ToClipperPath();

				return (ClipperLib::PointInPolygon(p, v) == 0);
			}

			std::vector<ClipperLib::IntPoint> ToClipperPath()
			{
				std::vector<ClipperLib::IntPoint> result;

				for (auto i : Points)
					result.push_back(ClipperLib::IntPoint(static_cast<ClipperLib::long64>(i.X), static_cast<ClipperLib::long64>(i.Y)));

				return result;
			}
		};

		static std::vector<std::vector<ClipperLib::IntPoint>> ClipPolygons(std::vector<IPolygon> polygons)
		{
			auto subj = std::vector<std::vector<ClipperLib::IntPoint>>(polygons.size());
			auto clip = std::vector<std::vector<ClipperLib::IntPoint>>(polygons.size());

			for (auto i : polygons)
			{
				subj.push_back(i.ToClipperPath());
				clip.push_back(i.ToClipperPath());
			}

			auto solution = std::vector<std::vector<ClipperLib::IntPoint>>();

			ClipperLib::Clipper c;

			c.AddPaths(subj, ClipperLib::PolyType::ptSubject, true);
			c.AddPaths(clip, ClipperLib::PolyType::ptClip, true);
			c.Execute(ClipperLib::ClipType::ctUnion, solution, ClipperLib::pftPositive, ClipperLib::pftEvenOdd);

			return solution;
		}

		static IPolygon ToPolygon(std::vector<ClipperLib::IntPoint> Args)
		{
			IPolygon out;

			for (auto i : Args)
				out.Add(Vector2(i.X, i.Y));

			return out;
		}

		static std::vector<IPolygon> ToPolygons(std::vector<std::vector<ClipperLib::IntPoint>> Args)
		{
			std::vector<IPolygon> out;

			for (auto i : Args)
				out.push_back(ToPolygon(i));

			return out;
		}

		struct Arc : public IPolygon
		{
			float Angle;
			Vector2 EndPos;
			float Radius;
			Vector2 StartPos;
			int _quality;

			Arc() { }

			Arc(Vector2 start, Vector2 direction, float angle, float radius, int quality = 20)
			{
				StartPos = start;
				EndPos = (direction - start).Normalized();
				Angle = angle;
				Radius = radius;
				_quality = quality;

				UpdatePolygon();
			}

			void UpdatePolygon(int offset = 0)
			{
				Clear();

				auto outRadius = (Radius + offset) / cosf(2 * M_PI / _quality);
				auto side1 = EndPos.Rotated(-Angle * 0.5f);

				for (auto i = 0; i <= _quality; i++)
				{
					auto cDirection = side1.Rotated(i * Angle / _quality).Normalized();
					Add(Vector2(StartPos.X + outRadius * cDirection.X, StartPos.Y + outRadius * cDirection.Y));
				}
			}
		};

		struct Circle : public IPolygon
		{
			Vector2 Center;
			float Radius;
			int _quality;

			Circle() { }

			Circle(Vector2 center, float radius, int quality = 20)
			{
				Center = center;
				Radius = radius;
				_quality = quality;

				UpdatePolygon();
			}

			void UpdatePolygon(int offset = 0, float overrideWidth = -1)
			{
				Clear();

				auto outRadius = (overrideWidth > 0 ? overrideWidth : (offset + Radius) / cosf(2 * M_PI / _quality));

				for (auto i = 1; i <= _quality; i++)
				{
					auto angle = i * 2 * M_PI / _quality;
					auto point = Vector2(Center.X + outRadius * cosf(angle), Center.Y + outRadius * sinf(angle));

					Add(point);
				}
			}
		};

		struct Line : public IPolygon
		{
			Vector2 LineEnd;
			Vector2 LineStart;

			Line() { }

			Line(Vector2 start, Vector2 end, float length = -1)
			{
				LineStart = start;
				LineEnd = end;

				if (length > 0)
					SetLength(length);

				UpdatePolygon();
			}

			float GetLength()
			{
				return (LineEnd - LineStart).Length();
			}

			void SetLength(float Length)
			{
				LineEnd = (LineEnd - LineStart).Normalized() * Length + LineStart;
			}

			void UpdatePolygon()
			{
				Clear();
				Add(LineStart);
				Add(LineEnd);
			}
		};

		struct Rectangle : public IPolygon
		{
			Vector2 End;
			Vector2 Start;
			float Width;

			Rectangle() { }

			Rectangle(Vector2 start, Vector2 end, float width)
			{
				Start = start;
				End = end;
				Width = width;

				UpdatePolygon();
			}

			Vector2 GetDirection()
			{
				return (End - Start).Normalized();
			}

			Vector2 GetPerpendicular()
			{
				return GetDirection().Perpendicular();
			}

			void UpdatePolygon(int offset = 0, float overrideWidth = -1)
			{
				Clear();

				float flOffset = static_cast<float>(offset);

				Add(Start + (overrideWidth > 0 ? overrideWidth : Width + flOffset) * GetPerpendicular() - flOffset * GetDirection());
				Add(Start - (overrideWidth > 0 ? overrideWidth : Width + flOffset) * GetPerpendicular() - flOffset * GetDirection());
				Add(End - (overrideWidth > 0 ? overrideWidth : Width + flOffset) * GetPerpendicular() + flOffset * GetDirection());
				Add(End + (overrideWidth > 0 ? overrideWidth : Width + flOffset) * GetPerpendicular() + flOffset * GetDirection());
			}
		};

		struct Ring : public IPolygon
		{
			Vector2 Center;
			float InnerRadius;
			float OuterRadius;
			int _quality;

			Ring() { }

			Ring(Vector2 center, float innerRadius, float outerRadius, int quality = 20)
			{
				Center = center;
				InnerRadius = innerRadius;
				OuterRadius = outerRadius;
				_quality = quality;

				UpdatePolygon();
			}

			void UpdatePolygon(int offset = 0)
			{
				Points.clear();

				auto outRadius = (offset + InnerRadius + OuterRadius) / cosf(2 * M_PI / _quality);
				auto innerRadius = InnerRadius - OuterRadius - offset;

				for (auto i = 0; i <= _quality; i++)
				{
					auto angle = i * 2 * M_PI / _quality;
					auto point = Vector2(Center.X - outRadius * cosf(angle), Center.Y - outRadius * sinf(angle));

					Add(point);
				}

				for (auto i = 0; i <= _quality; i++)
				{
					auto angle = i * 2 * M_PI / _quality;
					auto point = Vector2(Center.X + innerRadius * cosf(angle), Center.Y - innerRadius * sinf(angle));

					Add(point);
				}
			}
		};

		struct Sector : public IPolygon
		{
			float Angle;
			Vector2 Center;
			Vector2 Direction;
			float Radius;
			int _quality;

			Sector() { }

			Sector(Vector2 center, Vector2 direction, float angle, float radius, int quality = 20)
			{
				Center = center;
				Direction = (direction - center).Normalized();
				Angle = angle;
				Radius = radius;
				_quality = quality;

				UpdatePolygon();
			}

			Vector2 RotateLineFromPoint(Vector2 point1, Vector2 point2, float value, bool radian = true)
			{
				auto angle = !radian ? value * M_PI / 180 : value;
				auto line = (point2 - point1);

				auto newline = Vector2(line.X * cosf(angle) - line.Y * sinf(angle), line.X * sinf(angle) + line.Y * cosf(angle));

				return (newline + point1);
			}

			void UpdatePolygon(int offset = 0)
			{
				Clear();

				auto outRadius = (Radius + offset) / cosf(2 * M_PI / _quality);
				auto side1 = Direction.Rotated(Angle * 0.5f);

				Add(Center);

				for (auto i = 0; i <= _quality; i++)
				{
					auto cDirection = side1.Rotated(i * Angle / _quality).Normalized();

					Add(Center + outRadius * cDirection);
				}
			}
		};
	}

	namespace Polygons
	{
		static const int Quality = 22;

		static std::vector<Vector2> GetIntersectPointsLine(Geometry::IPolygon poly, Vector2 start, Vector2 end)
		{
			std::vector<Vector2> points;

			for (size_t i = 0; i < poly.Points.size(); i++)
			{
				Vector2 vecIntersection;

				if (poly.Points[i].Intersects(poly.Points[i != poly.Points.size() - 1 ? i + 1 : 0], start, end, &vecIntersection))
					points.push_back(vecIntersection);
			}

			return points;
		}
		struct Ellipse
		{
			Vector2 Start;
			Vector2 End;
			Vector2 Center;
			int Radius;
			int RadiusEx;
			Ellipse() {}
			Ellipse(Vector2 start, Vector2 end, Vector2 center, int radius, int radiusex) : Start(start), End(end), Center(center), Radius(radius), RadiusEx(radiusex)
			{

			}
			Geometry::IPolygon ToPolygon(int extraRadius = 0)
			{
				Geometry::IPolygon result;

				const double Step = 2.f * M_PI / static_cast<float>(Quality);
				auto radius = (extraRadius + Radius) / cos(Step);
				auto radiusex = (extraRadius + RadiusEx) / cos(Step);
				auto angle = (double)Radius;
				Vector2 Direction = End - Start;
				float anglerotate = AngleBetween(ToVec3(Direction), Vector3(0, 0, 0), Vector3(0, 0, 1));
				float anglerad = AngleToRadian(anglerotate);
				for (auto i = -Quality; i <= Quality; i++)
				{
					angle += Step;

					result.Add(Vector2(Center.X + radius * cosf(angle + anglerad), Center.Y + radiusex * sinf(angle + anglerad)));
				}
				//for (auto i = 0; i <= Quality; i++)
				//{
				//	angle += Step;
				//	Vector2 point = Vector2(Center.x + radius * cosf(angle), Center.y + radiusex * sinf(angle));

				//	result.Add(RotateAround(ToVec3(point), ToVec3(Center),anglerotate).To2D());
				//}

				return result;
			}
		};
		struct Arc
		{
			float Distance;
			Vector2 End;
			int Radius;
			Vector2 Start;

			Arc() { }

			Arc(Vector2 start, Vector2 end, int radius)
			{
				Start = start;
				End = end;
				Radius = radius;
				Distance = (End - Start).Length();
			}

			Geometry::IPolygon ToPolygon(int extraRadius = 0)
			{
				extraRadius += Radius;

				Geometry::IPolygon result;

				auto outerRadius = (0.35256f * Distance + 133.f) / cos(2 * M_PI / Quality);
				auto innerRadius = -0.1562f * Distance + 687.31f;
				auto outerCenter = Geometry::CircleCircleIntersection(Start, End, outerRadius, outerRadius)[0];
				auto innerCenter = Geometry::CircleCircleIntersection(Start, End, innerRadius, innerRadius)[0];

				auto dir = (End - outerCenter).Normalized();
				auto step = -(float)(dir.AngleBetween((Start - outerCenter).Normalized()) * M_PI / 180) / Quality;

				for (auto i = 0; i < Quality; i++)
					result.Add(outerCenter + (outerRadius + 15 + extraRadius) * dir.Rotated(step * i));

				dir = (Start - innerCenter).Normalized();
				step = (float)(dir.AngleBetween((End - innerCenter).Normalized()) * M_PI / 180) / Quality;

				for (auto i = 0; i < Quality; i++)
					result.Add(innerCenter + std::max<float>(0.0, innerRadius - extraRadius - 100.f) * dir.Rotated(step * i));

				return result;
			}
		};

		struct Circle
		{
			Vector2 Center;
			int Radius;

			Circle() { }

			Circle(Vector2 center, int radius)
			{
				Center = center;
				Radius = radius;
			}

			Geometry::IPolygon ToPolygon(int extraRadius = 0, float overrideRadius = -1)
			{
				Geometry::IPolygon result;

				const double Step = 2.f * M_PI / static_cast<float>(Quality);
				auto radius = overrideRadius > 0 ? overrideRadius : (extraRadius + Radius) / cos(Step);
				auto angle = (double)Radius;

				for (auto i = 0; i <= Quality; i++)
				{
					angle += Step;

					result.Add(Vector2(Center.X + radius * cosf(angle), Center.Y + radius * sinf(angle)));
				}

				return result;
			}
		};

		struct Cone
		{
			Vector2 Center;
			Vector2 Direction;
			float Radius;
			int Range;

			Cone() { }

			Cone(Vector2 center, Vector2 direction, int radius, int range)
			{
				Center = center;
				Direction = direction;
				Radius = radius * M_PI / 180;
				Range = range;
			}

			Geometry::IPolygon ToPolygon(int extraRadius = 0)
			{
				Geometry::IPolygon result;

				result.Add(Center);

				auto radius = (Range + extraRadius) / cos(2 * M_PI / Quality);
				auto side = Direction.Rotated(-Radius * 0.5f);

				for (auto i = 0; i <= Quality; i++)
				{
					auto dir = side.Rotated(i * Radius / Quality).Normalized();
					result.Add(Vector2(Center.X + radius * dir.X, Center.Y + radius * dir.Y));
				}

				return result;
			}
		};

		struct Line
		{
			Vector2 Direction;
			Vector2 Perpendicular;
			Vector2 End;
			int Radius;
			Vector2 Start;

			Line() { }

			Line(Vector2 start, Vector2 end, int radius)
			{
				Start = start;
				End = end;
				Radius = radius;
				Direction = (End - Start).Normalized();
				Perpendicular = Direction.Perpendicular();
			}

			Geometry::IPolygon ToPolygon(int extraRadius = 0, float overrideRadius = -1)
			{
				Geometry::IPolygon result;
				auto radius = (overrideRadius > 0 ? overrideRadius : Radius + extraRadius) * Perpendicular;
				auto dir = (static_cast<float>(extraRadius) + 50) * Direction;
				result.Add(Start + radius - dir);
				result.Add(Start - radius - dir);
				result.Add(End - radius + dir);
				result.Add(End + radius + dir);

				return result;
			}
		};

		struct Ring
		{
			Vector2 Center;
			int InnerRadius;
			int OuterRadius;

			Ring() { }

			Ring(Vector2 center, int innerRadius, int outerRadius)
			{
				Center = center;
				InnerRadius = innerRadius;
				OuterRadius = outerRadius;
			}

			Geometry::IPolygon ToPolygon(int extraRadius = 0)
			{
				Geometry::IPolygon result;

				const double Step = 2 * M_PI / Quality;
				auto outerRadius = (extraRadius + OuterRadius) / cos(Step);
				auto innerRadius = (InnerRadius - extraRadius) / cos(Step);

				auto outerAngle = (double)OuterRadius;

				for (auto i = 0; i <= Quality; i++)
				{
					outerAngle += Step;
					result.Add(Vector2(Center.X + outerRadius * cos(outerAngle), Center.Y + outerRadius * sin(outerAngle)));
				}

				auto innerAngle = (double)InnerRadius;

				for (auto i = 0; i <= Quality; i++)
				{
					innerAngle += Step;
					result.Add(Vector2(Center.X + innerRadius * cos(innerAngle), Center.Y + innerRadius * sin(innerAngle)));
				}

				return result;
			}
		};
	}
}

#endif // Polygons_h__