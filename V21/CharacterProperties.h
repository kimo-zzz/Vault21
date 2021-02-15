#pragma once

#include "Offsets.h"

namespace V21 {
	class CharacterProperties {
	public:
		union {
			DEFINE_MEMBER_N(float AttackRange, Offsets::CharacterProperties::AttackRange)
		};
	};
}