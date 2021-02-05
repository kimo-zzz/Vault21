#pragma once

#include "Offsets.h"

namespace HACKUZAN
{
	class DeviceHandler
	{
	public:
		static DeviceHandler* Instance;

		union
		{
			DEFINE_MEMBER_N(IDirect3DDevice9* Direct3DDevice, Offsets::DeviceHandler::Direct3DDevice)
			DEFINE_MEMBER_N(DeviceHandler* Deviceptr, Offsets::DeviceHandler::DevicePtr)
		};
	};
}