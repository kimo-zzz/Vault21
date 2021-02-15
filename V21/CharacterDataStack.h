#pragma once

#include "Offsets.h"
#include "stdafx.h"

namespace V21 {
	class CharacterDataStack {
	public:
		union {
			DEFINE_MEMBER_N(std::string SkinName, Offsets::CharacterDataStack::SkinName)
			DEFINE_MEMBER_N(unsigned int SkinId, Offsets::CharacterDataStack::SkinId)
		};
	};
}