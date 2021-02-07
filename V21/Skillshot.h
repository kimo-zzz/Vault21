#include "SkillshotData.h"

namespace HACKUZAN {

	class DetectedSKillShot
	{
	public:
		// step one
		SkillshotData* Data = nullptr;
		bool IsProcess = false;
		bool IsMissile = false;
		// step 2
		GameObject* MissileObject = nullptr;
		GameObject* Caster = nullptr;
		//CastedSpell ProcessInfo;
		//step 3
		Vector2 Start;
		Vector2 End;
		//step 4 
		int DetectionTime = 0;
		int MissileNWID = 0;
		DetectedSKillShot(SkillshotData* data, Vector2 start, Vector2 end, int detectiontime, bool isprocess, bool ismissile, GameObject* missileObject, int missileNWID = 0, GameObject* caster = nullptr)
		{
			Data = data, Start = start, End = end, DetectionTime = detectiontime, IsProcess = isprocess, IsMissile = ismissile, MissileObject = missileObject
				, MissileNWID = missileNWID, Caster = caster;
		};
		DetectedSKillShot() { IsNull = true; }
		bool IsNull = false;
		Geometry::IPolygon Polygon;
		void SetPolygon(Geometry::IPolygon poly)
		{
			Polygon = poly;
		}
	};

	SArray<DetectedSKillShot> DetectedSkillShots;
	SArray<DetectedSKillShot> DetectedSkillShotsAlly;
}