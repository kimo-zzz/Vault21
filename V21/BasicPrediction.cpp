#include "BasicPrediction.h"

#include "Geometry.h"
#include "NetClient.h"
#include "Draw.h"

namespace HACKUZAN
{

	GameObject* me = ObjectManager::Player;
	PredictionOutput Prediction::GetPositionOnPath(PredictionInput input)
	{
		GameObject* player = me;
		Navigation* navigation = input.target->AIManager();
		if (!navigation) input.target->Position;
		PathManager pathManager = navigation->mPathManager;

		float speed = input.target->MoveSpeed;

		if (Distance(input.target->Position, me->Position) <= 250)
			speed /= 1.5f;

		if (pathManager.GetSize() <= 1 || (input.target->ActiveSpell() && input.target->ActiveSpell()->IsBasicAttack && !navigation->mIsDashing))
		{
			PredictionOutput result = PredictionOutput(input);
			result.predictedPosition = input.target->Position;
			return result;
		}


		//Skillshots with only a delay
		float pathLength = pathManager.GetPathLength();
		if (pathLength >= input.delay * speed - input.radius && std::abs(input.speed - FLT_MAX) < FLT_EPSILON)
		{
			float tDistance = input.delay * speed - input.radius;

			for (int i = 0; i < pathManager.GetSize(); i++)
			{
				Vector3 a = pathManager.mPath[i];
				Vector3 b = pathManager.mPath[i + 1];
				float d = a.Distance(b);

				if (d >= tDistance)
				{
					Vector3 direction = (b - a).Normalized();

					Vector3 cp = a + direction * tDistance;
					Vector3 p = a + direction * ((i == pathManager.GetSize() - 2) ? min(tDistance + input.radius, d) : (tDistance + input.radius));

					PredictionOutput result = PredictionOutput(input);
					result.castPosition = result.castPosition = cp;
					result.predictedPosition = p;
					return result;
				}

				tDistance -= d;
			}
		}

		PredictionOutput result = PredictionOutput(input);
		result.predictedPosition = Vector3(0, 0, 0);
		return result;

		/*
		//Skillshot with a delay and speed.
		float pathLength = pathManager.GetPathLength();
		if (pathLength >= input.delay * speed - input.radius && std::abs(input.speed - FLT_MAX) > FLT_EPSILON)
		{
			float d = input.delay * speed - input.radius;
			if (input.castType == ESpellCastType::Line || input.castType == ESpellCastType::Cone || input.castType == ESpellCastType::MissileLine)
			{

				if (DistanceSqr(input.from, input.target->Position) < 200 * 200)
				{
					d = input.delay * speed;
				}
			}

			auto path = pathManager.CutPath(d);
			float tT = 0.f;
			for (int i = 0; i < path.size() - 1; i++)
			{
				Vector3 a = path[i];
				Vector3 b = path[i + 1];
				float tB = Distance(a, b) / speed;
				Vector3 direction = (b - a).Normalized();
				a = a - speed * tT * direction;

				auto sol = VectorMovementCollision(a, b, speed, input.from.To2D(), input.speed, tT); //add this func.
				auto t = (float)sol[0];
				auto pos = (Vector2)sol[1];

				if (pos.IsValid() && t >= tT && t <= tT + tB)
				{
					if (pos.Distance(b, true) < 20)
						break;
					auto p = pos + input.radius * direction;

					if (input.castType == ESpellCastType::MissileLine)
					{
						auto alpha = (input.from.To2D() - p).AngleBetween(a - b);
						if (alpha > 30 && alpha < 180 - 30)
						{

							auto beta = (float)std::asin(input.radius / Distance(p, input.from));
							auto cp1 = input.from.To2D() + (p - input.from.To2D()).Rotated(beta);
							auto cp2 = input.from.To2D() + (p - input.from.To2D()).Rotated(-beta);

							pos = cp1.Distance(pos, true) < cp2.Distance(pos, true) ? cp1 : cp2;
						}
					}

					return PredictionOutput{ Input = input, CastPosition = pos.To3D(), UnitPosition = p.To3D(), Hitchance = HitChanceEx.High };
				}
				tT += tB;
			}

		}
		*/
	}

	/*
	Vector3 Prediction::BasicPrediction(GameObject* target, float spellRadius, float spellRange, float missileSpeed, float castDelay)
	{
		GameObject* player = ObjectManager::Player;
		Navigation* navigation = target->AIManager(); //RECHECK ||  target->GetNavigation();
		if (!navigation) target->Position;
		PathManager pathManager = navigation->mPathManager;


		return Vector3();
	}
	*/
	Vector3 Prediction::BasicPrediction(GameObject* target, float spellRadius, float spellRange, float missileSpeed, float castDelay)
	{
		GameObject* player = me;
		auto navigation = target->AIManager();
		if (!navigation) target->Position;
		auto pathManager = target->AIManager()->mPathManager;

		if (!navigation->mIsMoving)
			return target->Position;

		/*
		int ping = 40;
		float flyTimeMax = spellRange / missileSpeed;
		float t_min = castDelay + ping / 1000.f;
		float t_max = flyTimeMax + castDelay + ping / 1000.f;
		*/

		int ping = NetClient::Instance->GetPing();
		float flytime_max = (missileSpeed != 0) ? spellRange / missileSpeed : 0.f;
		float t_min = castDelay + ping / 1000.f;
		float t_max = flytime_max + castDelay + ping / 1000.f;
		float path_time = 0.f;


		int path_bounds_index[2] = { -1, -1 };

		int pathSize = pathManager.GetSize();
		for (int i = 0; i < pathSize; ++i)
		{
			Vector3 cur = pathManager.mPath[i].toGround();			//skills hit calculating in 2d space
			Vector3 next = pathManager.mPath[i + 1].toGround();		//because height is not neccessary but when calculating the distance in 3d
			float t = next.Distance(cur);								//that causes a little difference in calculating so it will likely miss the target

			if (path_time <= t_min && path_time + t >= t_min)
				path_bounds_index[0] = i;
			if (path_time <= t_max && path_time + t >= t_max)
				path_bounds_index[1] = i;
			if (path_bounds_index[0] >= 0 && path_bounds_index[1] >= 0)
				break;

			path_time += t;
		}

		//if skill will hit to the target within the boundaries
		if (path_bounds_index[0] >= 0 && path_bounds_index[1] >= 0)
		{
			int curr_path_index = path_bounds_index[0];
			while (true)
			{
				Vector3 cur = pathManager.mPath[curr_path_index].toGround();
				Vector3 next = pathManager.mPath[curr_path_index + 1].toGround();

				Vector3 direction = (next - cur).Normalized();

				float extender = target->GetBoundingRadius();
				float distance = spellRadius;

				int steps = (int)floor(cur.Distance(next) / distance);

				if (steps > 0 && steps < 1000)
				{
					for (int i = 1; i < steps - 1; i++)
					{
						Vector3 center = cur + direction * distance * i;
						Vector3 pt_a = center - direction * extender;
						Vector3 pt_b = center + direction * extender;

						float flytime = (missileSpeed != 0) ? Distance(player->Position, target->Position) / missileSpeed : 0.f;
						float t = flytime + castDelay + ping / 1000.f;

						float arrive_time_a = Distance(target->Position, pt_a) / target->MoveSpeed;
						float arrive_time_b = Distance(target->Position, pt_b) / target->MoveSpeed;

						Vector3 predicted_pos = center;
						if (GetKeyState(VK_F10) & 1)
						{
							//printf("step : %d \n", i);
							//printf("path_bound_index : %d, %d \n", path_bounds_index[0], path_bounds_index[1]);
							//printf("predicted_pos : %.0f, %.0f, %.0f \n", predicted_pos.x, predicted_pos.y, predicted_pos.z);
							//printf("arrive_time_a : %.3f \n", arrive_time_a);
							//printf("arrive_time_b : %.3f \n", arrive_time_b);
							//printf("t : %.3f \n", t);
						}

						//Draw.Line(me->GetPos(), predicted_pos, 3, YELLOW_A(50));
						if (min(arrive_time_a, arrive_time_b) <= t && max(arrive_time_a, arrive_time_b) >= t)
						{
							return center;
						}
					}
				}

				if (curr_path_index == path_bounds_index[1])
					break;

				++curr_path_index;
			}
		}

		return Vector3(0, 0, 0);
	}
}

