
#include "PathManager.h"

namespace V21 {

	class Navigation
	{
	public:
		char pad_0004[16]; //0x0000
		Vector3 mGoingTo; //0x0010
		char pad_001C[380]; //0x001C
		bool mIsMoving; //0x0198
		char pad_0199[3]; //0x0199
		PathManager mPathManager; //0x019C
		char pad_01D0[236]; //0x01D0
		Vector3 mCurrPosition; //0x02BC
		float mVelocity; //0x2C8
		Vector3 mNavBegin; //0x1BC
		Vector3 mNavEnd; //0x1C0
		char pad_02D4[144]; //0x02D4
		Vector3 mDashEndPosition; //0x0364
		char pad_0370[52]; //0x0370
		bool mIsDashing; //0x1EC
		char pad_03A5[1243]; //0x03A5
		float mDashingSpeed; //0x1D0
		char pad_03A4[1242]; //0x03A4
		int* NavMesh; //0x100

	};
}