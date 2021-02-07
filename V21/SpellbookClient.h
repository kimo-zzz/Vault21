#pragma once

#include "Offsets.h"
#include "SpellInstanceClient.h"
#include "SpellDataInst.h"
#include "EvadeConstants.h"
#include "SpellInfo.h"

namespace HACKUZAN
{
	enum kSpellState : int
	{
		SpellState_Ready = 0,
		SpellState_NotAvailable = 4,
		SpellState_Supressed = 8,
		SpellState_NotLearned = 12,
		SpellState_Disabled = 16,
		SpellState_Processing = 24,
		SpellState_Cooldown = 32,
		SpellState_NoMana = 64,
		Unknown
	};

	class SpellbookClient
	{
	public:
		union
		{
			DEFINE_MEMBER_0(DWORD* VTable)
			DEFINE_MEMBER_N(SpellCastInfo* ActiveSpellInstance, Offsets::SpellbookClient::ActiveSpellInstance)
			DEFINE_MEMBER_N(short casterIndex, 0x1c)
		};

		SpellDataInst* GetSpell(kSpellSlot slot);

		kSpellState GetSpellState(kSpellSlot slot);

		kSpellSlot FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell summonerSpellToFind);
	};
}