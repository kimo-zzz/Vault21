#pragma once

#include "NavigationPath.h"

namespace HACKUZAN
{
	class PathControllerCommon
	{
	public:

		union
		{
			DEFINE_MEMBER_N(bool HasNavigationPath, Offsets::PathControllerCommon::HasNavigationPath)
			DEFINE_MEMBER_N(Vector3 ServerPosition, Offsets::PathControllerCommon::ServerPosition)
		};

		NavigationPath* GetNavigationPath();
		void SmoothPath(NavigationPath* path);
		bool CreatePath(Vector3 start, Vector3 destination, NavigationPath* pathOut);
	};
}
