#pragma once
#include "stdafx.h"
#include <map>
#include "EvadeConstants.h"
#include "ObjectManager.h"

namespace HACKUZAN {

	enum EvaderType
	{
		Movement, Invulnerability, SpeedBuffer, Wall, Shield
	};
	class Evader
	{
	public:
		std::string ChampionName;
		std::string MenuName;
		EvaderType Type;
		int Delay;
		kSpellSlot Slot;
		int Range;
		Evader(std::string championname, std::string menuname, kSpellSlot slot, int range, EvaderType type, int delay)
			: ChampionName(championname), MenuName(menuname), Slot(slot), Range(range), Type(type), Delay(delay)
		{};
	};
	struct EvaderDB
	{
	public:
		std::vector<Evader*> EvadeSpells;
		EvaderDB();
	};
	EvaderDB* EvadersDB = nullptr;
	inline EvaderDB::EvaderDB()
	{
		EvadeSpells.push_back(new Evader("Aatrox", "Aatrox Q", kSpellSlot::SpellSlot_Q, 600, Movement, 500));
		EvadeSpells.push_back(new Evader("Ahri", "Ahri R", kSpellSlot::SpellSlot_R, 450, Movement, 500));
		EvadeSpells.push_back(new Evader("Braum", "Braum E", kSpellSlot::SpellSlot_E, 450, Wall, 300));
		EvadeSpells.push_back(new Evader("Caitlyn", "Caitlyn E", kSpellSlot::SpellSlot_E, -400, Movement, 500));
		EvadeSpells.push_back(new Evader("Corki", "Corki W", kSpellSlot::SpellSlot_W, 600, Movement, 500));
		EvadeSpells.push_back(new Evader("Draven", "Draven W", kSpellSlot::SpellSlot_W, 600, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Diana", "Diana W", kSpellSlot::SpellSlot_W, 600, Shield, 500));
		EvadeSpells.push_back(new Evader("Ekko", "Ekko E", kSpellSlot::SpellSlot_E, 325, Movement, 500));
		EvadeSpells.push_back(new Evader("Ekko", "Ekko R", kSpellSlot::SpellSlot_R, 325, Invulnerability, 300));
		EvadeSpells.push_back(new Evader("Evelynn", "Evelynn W", kSpellSlot::SpellSlot_W, 325, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Ezreal", "Ezreal E", kSpellSlot::SpellSlot_E, 475, Movement, 500));
		EvadeSpells.push_back(new Evader("Fiora", "Fiora Q", kSpellSlot::SpellSlot_Q, 400, Movement, 500));
		EvadeSpells.push_back(new Evader("Fiora", "Fiora W", kSpellSlot::SpellSlot_W, 750, Invulnerability, 300));
		EvadeSpells.push_back(new Evader("Fizz", "Fizz E", kSpellSlot::SpellSlot_E, 400, Invulnerability, 300));
		EvadeSpells.push_back(new Evader("Garen", "Garen Q", kSpellSlot::SpellSlot_Q, 400, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Garen", "Garen W", kSpellSlot::SpellSlot_W, 400, Shield, 500));
		EvadeSpells.push_back(new Evader("Gnar", "Gnar E", kSpellSlot::SpellSlot_E, 475, Movement, 500));
		EvadeSpells.push_back(new Evader("Gragas", "Gragas E", kSpellSlot::SpellSlot_E, 600, Movement, 500));
		EvadeSpells.push_back(new Evader("Graves", "Graves E", kSpellSlot::SpellSlot_E, 425, Movement, 500));
		EvadeSpells.push_back(new Evader("Hecarim", "Hecarim E", kSpellSlot::SpellSlot_E, 425, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Hecarim", "Hecarim R", kSpellSlot::SpellSlot_R, 1000, Movement, 500));
		EvadeSpells.push_back(new Evader("Ivern", "Ivern E", kSpellSlot::SpellSlot_E, 1000, Shield, 500));
		EvadeSpells.push_back(new Evader("Janna", "Janna E", kSpellSlot::SpellSlot_E, 1000, Shield, 500));
		EvadeSpells.push_back(new Evader("Karma", "Karma E", kSpellSlot::SpellSlot_E, 1000, Shield, 500));
		EvadeSpells.push_back(new Evader("Kassadin", "Kassadin R", kSpellSlot::SpellSlot_R, 500, Movement, 500));
		EvadeSpells.push_back(new Evader("Kayle", "Kayle W", kSpellSlot::SpellSlot_W, 500, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Kayle", "Kayle R", kSpellSlot::SpellSlot_R, 500, Invulnerability, 500));
		EvadeSpells.push_back(new Evader("Khazix", "Khazix E", kSpellSlot::SpellSlot_E, 700, Movement, 500));
		EvadeSpells.push_back(new Evader("Kindred", "Kindred Q", kSpellSlot::SpellSlot_Q, 340, Movement, 500));
		EvadeSpells.push_back(new Evader("Kled", "Kled E", kSpellSlot::SpellSlot_E, 550, Movement, 500));
		EvadeSpells.push_back(new Evader("Leblanc", "Leblanc W", kSpellSlot::SpellSlot_W, 700, Movement, 500));
		EvadeSpells.push_back(new Evader("Lissandra", "Lissandra R", kSpellSlot::SpellSlot_R, 700, Invulnerability, 500));
		EvadeSpells.push_back(new Evader("Lucian", "Lucian E", kSpellSlot::SpellSlot_E, 425, Movement, 500));
		EvadeSpells.push_back(new Evader("Lulu", "Lulu W", kSpellSlot::SpellSlot_W, 425, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Lulu", "Lulu E", kSpellSlot::SpellSlot_E, 425, Shield, 500));
		EvadeSpells.push_back(new Evader("Malphite", "Malphite R", kSpellSlot::SpellSlot_R, 1000, Movement, 500));
		EvadeSpells.push_back(new Evader("MasterYi", "MasterYi R", kSpellSlot::SpellSlot_R, 1000, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Morgana", "Morgana E", kSpellSlot::SpellSlot_E, 1000, Shield, 500));
		EvadeSpells.push_back(new Evader("Nautilus", "Nautilus W", kSpellSlot::SpellSlot_W, 1000, Shield, 500));
		EvadeSpells.push_back(new Evader("Nocturne", "Nocturne W", kSpellSlot::SpellSlot_W, 1000, Invulnerability, 500));
		EvadeSpells.push_back(new Evader("Nunu", "Nunu W", kSpellSlot::SpellSlot_W, 1000, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Orianna", "Orianna E", kSpellSlot::SpellSlot_W, 1000, Shield, 500));
		EvadeSpells.push_back(new Evader("Poppy", "Poppy W", kSpellSlot::SpellSlot_W, 1000, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Rakan", "Rakan W", kSpellSlot::SpellSlot_W, 600, Movement, 500));
		EvadeSpells.push_back(new Evader("Rakan", "Rakan R", kSpellSlot::SpellSlot_R, 600, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Renekton", "Renekton E", kSpellSlot::SpellSlot_E, 450, Movement, 500));
		EvadeSpells.push_back(new Evader("Riven", "Riven E", kSpellSlot::SpellSlot_E, 325, Movement, 500));
		EvadeSpells.push_back(new Evader("Rumble", "Rumble W", kSpellSlot::SpellSlot_W, 325, Shield, 500));
		EvadeSpells.push_back(new Evader("Sejuani", "Sejuani Q", kSpellSlot::SpellSlot_Q, 650, Movement, 500));
		EvadeSpells.push_back(new Evader("Shaco", "Shaco Q", kSpellSlot::SpellSlot_Q, 400, Movement, 500));
		EvadeSpells.push_back(new Evader("Shaco", "Shaco R", kSpellSlot::SpellSlot_R, 400, Invulnerability, 500));
		EvadeSpells.push_back(new Evader("Shen", "Shen E", kSpellSlot::SpellSlot_E, 600, Movement, 500));
		EvadeSpells.push_back(new Evader("Shyvana", "Shyvana W", kSpellSlot::SpellSlot_W, 300, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Shyvana", "Shyvana R", kSpellSlot::SpellSlot_R, 850, Movement, 500));
		EvadeSpells.push_back(new Evader("Sion", "Sion W", kSpellSlot::SpellSlot_W, 850, Shield, 500));
		EvadeSpells.push_back(new Evader("Sivir", "Sivir E", kSpellSlot::SpellSlot_E, 850, Invulnerability, 500));
		EvadeSpells.push_back(new Evader("Skarner", "Skarner W", kSpellSlot::SpellSlot_W, 850, Shield, 500));
		EvadeSpells.push_back(new Evader("Sona", "Sona E", kSpellSlot::SpellSlot_E, 850, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Tristana", "Tristana W", kSpellSlot::SpellSlot_W, 900, Movement, 500));
		EvadeSpells.push_back(new Evader("Tryndamere", "Tryndamere E", kSpellSlot::SpellSlot_E, 660, Movement, 500));
		EvadeSpells.push_back(new Evader("Urgot", "Urgot W", kSpellSlot::SpellSlot_W, 660, Shield, 500));
		EvadeSpells.push_back(new Evader("Vayne", "Vayne Q", kSpellSlot::SpellSlot_Q, 300, Movement, 500));
		EvadeSpells.push_back(new Evader("Vladimir", "Vladimir W", kSpellSlot::SpellSlot_W, 300, Invulnerability, 500));
		EvadeSpells.push_back(new Evader("Volibear", "Volibear Q", kSpellSlot::SpellSlot_Q, 300, SpeedBuffer, 500));
		EvadeSpells.push_back(new Evader("Xayah", "Xayah R", kSpellSlot::SpellSlot_R, 1100, Invulnerability, 500));
		EvadeSpells.push_back(new Evader("Yasuo", "Yasuo W", kSpellSlot::SpellSlot_W, 1100, Wall, 500));
		EvadeSpells.push_back(new Evader("Zilean", "Zilean E", kSpellSlot::SpellSlot_E, 1100, SpeedBuffer, 500));

		EvadeSpells.erase(std::remove_if(EvadeSpells.begin(), EvadeSpells.end(), [&](Evader* i)
			{
				return i->ChampionName != std::string(ObjectManager::Player->BaseCharacterData->SkinName);
			}), EvadeSpells.end());
	}
}