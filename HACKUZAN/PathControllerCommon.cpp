#include "stdafx.h"
#include "PathControllerCommon.h"
#include "Functions.h"

namespace HACKUZAN {
	NavigationPath* PathControllerCommon::GetNavigationPath() {
		return (NavigationPath*)((DWORD)this + (DWORD)Offsets::PathControllerCommon::NavigationPath);
	}

	void PathControllerCommon::SmoothPath(NavigationPath* path)
	{
		reinterpret_cast<void(__thiscall*)(void*, NavigationPath*)>
			DEFINE_RVA(Offsets::AIManagerClient::SmoothPath)
			(this, path);
	}

	bool PathControllerCommon::CreatePath(Vector3 start, Vector3 destination, NavigationPath* pathOut)
	{
		return Functions::CreatePath(this, pathOut, start, destination);
	}
}