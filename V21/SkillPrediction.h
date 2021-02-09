#pragma once
#include "HudManager.h"
#include "List.h"
#include "ObjectManager.h"

namespace HACKUZAN
{
	namespace SkillPrediction
	{
		
		
		enum HitChance
		{
			/// <summary>
			///     The target is immobile.
			/// </summary>
			Immobile = 8,

			/// <summary>
			///     The unit is dashing.
			/// </summary>
			Dashing = 7,

			/// <summary>
			///     Very high probability of hitting the target.
			/// </summary>
			VeryHigh = 6,

			/// <summary>
			///     High probability of hitting the target.
			/// </summary>
			High = 5,

			/// <summary>
			///     Medium probability of hitting the target.
			/// </summary>
			Medium = 4,

			/// <summary>
			///     Low probability of hitting the target.
			/// </summary>
			Low = 3,

			/// <summary>
			///     Impossible to hit the target.
			/// </summary>
			Impossible = 2,

			/// <summary>
			///     The target is out of range.
			/// </summary>
			OutOfRange = 1,

			/// <summary>
			///     The target is blocked by other units.
			/// </summary>
			Collision = 0
		};

		enum SkillshotType
		{
			/// <summary>
			///     The skillshot is linear.
			/// </summary>
			SkillshotLine,

			/// <summary>
			///     The skillshot is circular.
			/// </summary>
			SkillshotCircle,

			/// <summary>
			///     The skillshot is conical.
			/// </summary>
			SkillshotCone
		};

		enum CollisionableObjects
		{
			/// <summary>
			///     Minions.
			/// </summary>
			Minions,

			/// <summary>
			///     Enemy heroes.
			/// </summary>
			Heroes,

			/// <summary>
			///     Yasuo's Wind Wall (W)
			/// </summary>
			YasuoWall,

			/// <summary>
			///     Walls.
			/// </summary>
			Walls,

			/// <summary>
			///     Ally heroes.
			/// </summary>
			Allies
		};

		class PredictInput
		{
		public:
			//If set to <c>true</c> the prediction will hit as many enemy heroes as posible.
			bool Aoe = false;
			CollisionableObjects CollisionObjects[2] = { CollisionableObjects::Minions, CollisionableObjects::YasuoWall };

			///     The skillshot delay in seconds.

			float Delay;

			/// <summary>
			///     The skillshot width's radius or the angle in case of the cone skillshots.
			/// </summary>
			float Radius = 1.f;

			/// <summary>
			///     The skillshot range in units.
			/// </summary>
			float Range = 0.f;

			/// <summary>
			///     The skillshot speed in units per second.
			/// </summary>
			float Speed = 0.f;

			/// <summary>
			///     The skillshot type.
			/// </summary>
			SkillshotType Type = SkillshotType::SkillshotLine;

			/// <summary>
			///     The unit that the prediction will made for.
			/// </summary>
			GameObject* Unit = ObjectManager::Player;

			/// <summary>
			///     Set to true to increase the prediction radius by the unit bounding radius.
			/// </summary>
			bool UseBoundingRadius = true;

			/// <summary>
			///     The position that the skillshot will be launched from.
			/// </summary>
			Vector3 _from;

			/// <summary>
			///     The position to check the range from.
			/// </summary>
			Vector3 _rangeCheckFrom;

			/// <summary>
	   ///     The position from where the skillshot missile gets fired.
	   /// </summary>
	   /// <value>From.</value>
			Vector3 From()
			{
				return this->_from.IsValid() ? this->_from : ObjectManager::Player->ServerPosition();
			}

			/// <summary>
	   ///     The position from where the range is checked.
	   /// </summary>
	   /// <value>The range check from.</value>
			Vector3 RangeCheckFrom()
			{
				return this->_rangeCheckFrom.To2D().IsValid()
					? this->_rangeCheckFrom
					: (this->From().To2D().IsValid() ? this->From() : ObjectManager::Player->ServerPosition());
			}

			/// <summary>
		///     Gets the real radius.
		/// </summary>
		/// <value>The real radius.</value>
			float RealRadius()
			{
				return this->UseBoundingRadius ? this->Radius + this->Unit->GetBoundingRadius() : this->Radius;
			}

		}; //PredictInput end


		class PredictOutput
		{
		public:
			/// <summary>
	   ///     The list of the targets that the spell will hit (only if aoe was enabled).
	   /// </summary>

			std::vector<GameObject*> AoeTargetsHit;

			/// <summary>
			///     The list of the units that the skillshot will collide with.
			/// </summary>
			std::vector<GameObject*> CollisionObjects;

			/// <summary>
			///     Returns the hitchance.
			/// </summary>
			HitChance Hitchance = HitChance::Impossible;

			/// <summary>
			///     The AoE target hit.
			/// </summary>
			int _aoeTargetsHitCount;

			/// <summary>
			///     The input
			/// </summary>
			PredictInput* Input;

			/// <summary>
			///     The calculated cast position
			/// </summary>
			Vector3 _castPosition;

			/// <summary>
			///     The predicted unit position
			/// </summary>
			Vector3 _unitPosition;

			/// <summary>
			///     The number of targets the skillshot will hit (only if aoe was enabled).
			/// </summary>
			/// <value>The aoe targets hit count.</value>
			int AoeTargetsHitCount()
			{
				return std::max(this->_aoeTargetsHitCount, (int)this->AoeTargetsHit.size());
			}

			/// <summary>
			///     The position where the skillshot should be casted to increase the accuracy.
			/// </summary>
			/// <value>The cast position.</value>
			Vector3 CastPosition()
			{
				auto test = this->Input->Unit->ServerPosition();

				if (this->_castPosition.IsValid() && this->_castPosition.To2D().IsValid())
				{
					this->_castPosition.Z = HudManager::Instance->CursorTargetLogic->CursorPosition.Z;
					return this->_castPosition;
				}
				this->Input->Unit->ServerPosition();
			}

			/// <summary>
			///     The position where the unit is going to be when the skillshot reaches his position.
			/// </summary>
			/// <value>The unit position.</value>
			Vector3 UnitPosition()
			{
				if (this->_unitPosition.To2D().IsValid())
				{
					this->_unitPosition.Z = HudManager::Instance->CursorTargetLogic->CursorPosition.Z;
					return this->_unitPosition;
				}
				return Input->Unit->ServerPosition();
			}

			class Prediction
			{
			public:

			};
		};


	}
}
