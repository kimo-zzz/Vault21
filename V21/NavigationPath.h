#pragma once

#include "Offsets.h"
#include "Vector3.h"

namespace V21
{
	class NavigationPath
	{
	public:

		union
		{
			DEFINE_MEMBER_0(unsigned int Index)
			DEFINE_MEMBER_N(Vector3 StartPosition, Offsets::NavigationPath::StartPosition)
			DEFINE_MEMBER_N(Vector3 EndPosition, Offsets::NavigationPath::EndPosition)
			DEFINE_MEMBER_N(std::vector<Vector3> Path, Offsets::NavigationPath::Path)
			DEFINE_MEMBER_N(float DashSpeed, Offsets::NavigationPath::DashSpeed)
			DEFINE_MEMBER_N(bool IsDashing, Offsets::NavigationPath::IsDashing)
		};

	};
}
