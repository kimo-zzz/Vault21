#pragma once

namespace HACKUZAN {

	enum NavGridCellFlags
	{
		None = 0,
		GrassFlag = 1 << 0,
		WallFlag = 1 << 1,
		Unknown2 = 1 << 2,
		Unknown3 = 1 << 3,
		Unknown4 = 1 << 4,
		Unknown5 = 1 << 5,
		Building = 1 << 6,
		Unknown6 = 1 << 7
	};
}