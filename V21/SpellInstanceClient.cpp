#include "stdafx.h"
#include "SpellInstanceClient.h"

namespace V21 {
	SpellCastInfo* SpellInstanceClient::GetCastInfo() {
		return (SpellCastInfo*)((DWORD)this + (DWORD)Offsets::SpellInstanceClient::CastInfo);
	}
}