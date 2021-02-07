#pragma once

#include "Offsets.h"

namespace HACKUZAN {
	class CharacterProperties {
	public:
		union {
			DEFINE_MEMBER_N(float AttackRange, Offsets::CharacterProperties::AttackRange)
		};
	};
}