#pragma once
#include "Vector3.h"
#include "SpellData.h"

namespace HACKUZAN
{
	enum class SpellToggleState
	{
		None,
		NotToggled,
		Toggled
	};

	class SpellDataInst
	{
	public:
		union
		{
			DEFINE_MEMBER_N(unsigned int Level, Offsets::SpellDataInst::Level)
			DEFINE_MEMBER_N(float fEffect1, Offsets::SpellDataInst::fEffect1)
			DEFINE_MEMBER_N(float fEffect2, Offsets::SpellDataInst::fEffect2)
			DEFINE_MEMBER_N(float fEffect3, Offsets::SpellDataInst::fEffect3)
			DEFINE_MEMBER_N(float fEffect4, Offsets::SpellDataInst::fEffect4)
			DEFINE_MEMBER_N(float fEffect5, Offsets::SpellDataInst::fEffect5)
			DEFINE_MEMBER_N(float fEffect6, Offsets::SpellDataInst::fEffect6)
			DEFINE_MEMBER_N(float fEffect7, Offsets::SpellDataInst::fEffect7)
			DEFINE_MEMBER_N(float fEffect8, Offsets::SpellDataInst::fEffect8)
			DEFINE_MEMBER_N(float fEffect9, Offsets::SpellDataInst::fEffect9)
			DEFINE_MEMBER_N(float fEffect10, Offsets::SpellDataInst::fEffect10)
			DEFINE_MEMBER_N(float fEffect11, Offsets::SpellDataInst::fEffect11)
			DEFINE_MEMBER_N(HACKUZAN::SpellData* SpellData, Offsets::SpellDataInst::SpellData)
		};

		struct SpellTargetingClient
		{
			DWORD unk;
			DWORD unk2;
			DWORD Caster;
			DWORD Target;
			Vector3 StartPos;
			Vector3 TargetPos;
			Vector3 TargetPos2;
			Vector3 StartPos2;
		};


		SpellTargetingClient* GetSpellTargetingClient()
		{
			return *(SpellTargetingClient**)((DWORD)this + 0x138);
		}
	};
}