#include "stdafx.h"
#include "SpellbookClient.h"


namespace V21
{

	SpellDataInst* SpellbookClient::GetSpell(kSpellSlot slot)
	{
		return *(SpellDataInst**)((DWORD)this + 0x478 + (0x4 * slot));
	}

	kSpellState SpellbookClient::GetSpellState(kSpellSlot slot)
	{
		return
			reinterpret_cast<kSpellState(__thiscall*)(void*, const kSpellSlot, const DWORD&)>
			(baseAddr + (DWORD)Offsets::SpellbookClient::GetSpellState)
			(this, slot, NULL);
	}

	kSpellSlot SpellbookClient::FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell summonerSpellToFind)
	{
		SpellDataResource* summoner1 = this->GetSpell(kSpellSlot::SpellSlot_Summoner1)->SpellData->Resource;
		SpellDataResource* summoner2 = this->GetSpell(kSpellSlot::SpellSlot_Summoner2)->SpellData->Resource;

		auto summonerSpell1 = ESummonerSpell::FromString(summoner1->GetSpellName());
		auto summonerSpell2 = ESummonerSpell::FromString(summoner2->GetSpellName());

		if (summonerSpell1 == summonerSpellToFind)
			return kSpellSlot::SpellSlot_Summoner1;
		if (summonerSpell2 == summonerSpellToFind)
			return kSpellSlot::SpellSlot_Summoner2;

		return kSpellSlot::SpellSlot_Unknown;
	}
}