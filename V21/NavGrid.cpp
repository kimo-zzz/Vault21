#include "stdafx.h"
#include "NavGrid.h"
#include "ObjectManager.h"
#include "Functions.h"

namespace V21
{
	NavGrid* NavGrid::Instance;

	int NavGrid::GetNearestGrassGroupGridSpace(float x, float y, float range)
	{
		static auto dwFuncAddress = reinterpret_cast<DWORD>(GetModuleHandle(nullptr)) + 0x3307B0;
		static auto dwFunc = reinterpret_cast<fnGetNearestGrassGroupGridSpace>(dwFuncAddress);
		return LOBYTE(dwFunc(this, 0, x, y, range));
	}

	BOOL NavGrid::IsGrass(float x, float y)
	{
		return false;
	}

	//BOOL NavGrid::IsWall(float x, float y)
	//{
	//	auto cell = this->GetCell(x, y);
	//	return cell != nullptr && (WallFlag & cell->m_Flags) == WallFlag;
	//}

	BOOL NavGrid::IsWall(Vector3* position)
	{
		return false;
	}

	NavGridCell* NavGrid::GetCell(float x, float y)
	{
		auto position = Vector3(x, y, 0);
		return this->GetCell(&position);
	}

	NavGridCell* NavGrid::GetCell(Vector3* position)
	{
		static auto dwFuncAddress = reinterpret_cast<DWORD>(GetModuleHandle(nullptr)) + (DWORD)Offsets::NavGrid::GetCell;
		static auto dwFunc = reinterpret_cast<fnGetCell>(dwFuncAddress);
		return dwFunc(this, position);
	}

	float NavGrid::GetHeightForPosition(float x, float z)
	{
		return Functions::GetHeightForPosition(this, x, z);
	}

	BOOL NavGrid::LineOfSightTest(Vector3* start, Vector3* end)
	{
		static auto dwFuncAddress = reinterpret_cast<DWORD>(GetModuleHandle(nullptr)) + 0x733000;
		static auto dwFunc = reinterpret_cast<fnLineOfSightTest>(dwFuncAddress);

		static auto player = *reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(GetModuleHandle(nullptr)) + 0x013B0D6C);
		static auto aiManager = *reinterpret_cast<DWORD*>(player + 0x2CC4);
		static auto actor = aiManager + 0x70;

		DWORD collision;
		return dwFunc(this, start, end, actor, &collision, 0);
	}

	bool NavGrid::IsWall(Vector3 worldPosition)
	{
		return false;
	}
}