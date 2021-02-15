#pragma once
#include "NavGridCell.h"
#include "Offsets.h"
#include "Vector3.h"

namespace V21
{
	class NavGrid
	{
		typedef NavGridCell* (__thiscall* fnGetCell)(void*, Vector3*);
		typedef int(__vectorcall* fnGetNearestGrassGroupGridSpace)(void*, float, float, float, float);
		typedef int(__vectorcall* fnGetPositionHeight)(void*, float, float, float);
		typedef BYTE(__thiscall* fnLineOfSightTest)(void*, Vector3*, Vector3*, DWORD, DWORD*, int);

	public:
		static NavGrid* Instance;

		int						GetNearestGrassGroupGridSpace(float x, float y, float range);
		BOOL					IsGrass(float x, float y);
		//BOOL					IsWall(float x, float y);
		BOOL					IsWall(Vector3* position);
		NavGridCell*			GetCell(float x, float y);
		NavGridCell*			GetCell(Vector3* position);
		float					GetHeightForPosition(float x, float y);
		BOOL					LineOfSightTest(Vector3* start, Vector3* end);

		static bool IsWall(Vector3 worldPosition);
	};
}