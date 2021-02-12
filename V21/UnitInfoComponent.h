#pragma once

#include "Offsets.h"

namespace V21 {
	class UnitInfoComponent {
	public:
		union {
			DEFINE_MEMBER_N(FloatingInfoBar* FloatingInfoBar, 0x14);
		};
	};
}