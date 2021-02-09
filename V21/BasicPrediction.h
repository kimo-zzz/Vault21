#pragma once
#include "ObjectManager.h"
#include "stdafx.h"


enum ESpellCastType
{
	Cone = 1,
	Line = 2,
	MissileLine = 3
};

namespace HACKUZAN
{
	struct PredictionInput
	{
		HACKUZAN::Vector3 from = HACKUZAN::ObjectManager::Player->Position;
		float delay = 0;
		float radius = 1.f;
		float range = FLT_MAX;
		float speed = FLT_MAX;
		ESpellCastType castType;
		HACKUZAN::GameObject* target;
	};

	class PredictionOutput
	{
	public:
		PredictionOutput(PredictionInput input) {
			this->input = input;
			this->castPosition = HACKUZAN::ObjectManager::Player->Position;
		}

		PredictionInput input;
		HACKUZAN::Vector3 castPosition;
		HACKUZAN::Vector3 predictedPosition;
	};

	class Prediction
	{
	public:
		static PredictionOutput GetPositionOnPath(PredictionInput input);
		static std::tuple<float, Vector2> VectorMovementCollision(Vector2 startPoint1, Vector2 endPoint1, float v1, Vector2 startPoint2, float v2, float delay);

		static Vector3 BasicPrediction(GameObject* target, float spellRadius, float spellRange, float missileSpeed, float castDelay);
	};
}
