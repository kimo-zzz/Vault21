#pragma once

#include "Offsets.h"
#include "stdafx.h"

namespace HACKUZAN {
	class CharacterDataStack {
	public:
		union {
			DEFINE_MEMBER_N(std::string SkinName, Offsets::CharacterDataStack::SkinName)
			DEFINE_MEMBER_N(unsigned int SkinId, Offsets::CharacterDataStack::SkinId)
		};
	};
}