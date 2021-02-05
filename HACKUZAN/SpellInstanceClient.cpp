#include "stdafx.h"
#include "SpellInstanceClient.h"

namespace HACKUZAN {
	SpellCastInfo* SpellInstanceClient::GetCastInfo() {
		return (SpellCastInfo*)((DWORD)this + (DWORD)Offsets::SpellInstanceClient::CastInfo);
	}
}