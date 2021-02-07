#pragma once
#include "stdafx.h"
#include "SpellInfo.h"
#include "ObjectManager.h"
#include <map>


namespace HACKUZAN {
	enum GapCloserType
	{
		skillshot, targeted
	};

	class GapCloser
	{
	public:
		std::string ChampionName;
		std::string MenuName;
		GapCloserType Type;
		int DangerLevel;
		kSpellSlot Slot;
		GapCloser(std::string championname, std::string menuname, kSpellSlot slot, GapCloserType type, int DangerLevel)
			: ChampionName(championname), MenuName(menuname), Slot(slot), Type(type), DangerLevel(DangerLevel)
		{};
	};
	struct GapCloserDB
	{
	public:
		std::vector<GapCloser*> GapCloserSpells;
		GapCloserDB();
	};
	GapCloserDB* GapClosersDB = nullptr;
	inline GapCloserDB::GapCloserDB()
	{
		GapCloserSpells.push_back(new GapCloser("Aatrox", "Aatrox Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Ahri", "Ahri R", kSpellSlot::SpellSlot_R, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Akali", "Akali R", kSpellSlot::SpellSlot_R, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Alistar", "Alistar W", kSpellSlot::SpellSlot_W, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Amumu", "Amumu Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Azir", "Azir E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Braum", "Braum W", kSpellSlot::SpellSlot_W, targeted, 1));
		GapCloserSpells.push_back(new GapCloser("Caitlyn", "Caitlyn E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Camille", "Camille E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Corki", "Corki W", kSpellSlot::SpellSlot_W, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Diana", "Diana R", kSpellSlot::SpellSlot_R, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Ekko", "Ekko E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Elise", "Elise Q", kSpellSlot::SpellSlot_Q, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Ezreal", "Ezreal E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Fiddlesticks", "Fiddlesticks R", kSpellSlot::SpellSlot_R, skillshot, 3));
		GapCloserSpells.push_back(new GapCloser("Fiora", "Fiora Q", kSpellSlot::SpellSlot_Q, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Fizz", "Fizz Q", kSpellSlot::SpellSlot_Q, targeted, 1));
		GapCloserSpells.push_back(new GapCloser("Fizz", "Ekko E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Galio", "Galio E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Gnar", "Gnar E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Gragas", "Gragas E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Hecarim", "Hecarim R", kSpellSlot::SpellSlot_R, skillshot, 3));
		GapCloserSpells.push_back(new GapCloser("Irelia", "Irelia Q", kSpellSlot::SpellSlot_Q, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Ivern", "Ivern Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Jarvan", "Jarvan Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Jax", "Jax Q", kSpellSlot::SpellSlot_Q, targeted, 1));
		GapCloserSpells.push_back(new GapCloser("Jayce", "Jayce Q", kSpellSlot::SpellSlot_Q, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Kaisa", "Kaisa E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Kassadin", "Kassadin R", kSpellSlot::SpellSlot_R, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Katarina", "Katarina E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Kayn", "Kayn Q", kSpellSlot::SpellSlot_Q, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Khazix", "Khazix E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Kindred", "Kindred Q", kSpellSlot::SpellSlot_Q, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Kled", "Kled Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Kled", "Kled E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Leblanc", "Leblanc W", kSpellSlot::SpellSlot_W, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("LeeSin", "LeeSin Q", kSpellSlot::SpellSlot_Q, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Leona", "Leona E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Lissandra", "Lissandra E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Lucian", "Lucian E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Malphite", "Malphite R", kSpellSlot::SpellSlot_R, skillshot, 3));
		GapCloserSpells.push_back(new GapCloser("Maokai", "Maokai W", kSpellSlot::SpellSlot_W, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("MasterYi", "MasterYi Q", kSpellSlot::SpellSlot_Q, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Nautilus", "Nautilus Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Nidalee", "Nidalee W", kSpellSlot::SpellSlot_W, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Nocturne", "Nocturne R", kSpellSlot::SpellSlot_R, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Ornn", "Ornn E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Pantheon", "Pantheon W", kSpellSlot::SpellSlot_W, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Poppy", "Poppy E", kSpellSlot::SpellSlot_E, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Pyke", "Pyke Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Pyke", "Pyke E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Quinn", "Quinn E", kSpellSlot::SpellSlot_E, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Rakan", "Rakan W", kSpellSlot::SpellSlot_W, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("RekSai", "RekSai E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Renekton", "Renekton E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Riven", "Riven Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Riven", "Riven E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Sejuani", "Sejuani Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Shaco", "Shaco Q", kSpellSlot::SpellSlot_Q, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Shen", "Shen E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Shyvana", "Shyvana R", kSpellSlot::SpellSlot_R, skillshot, 3));
		GapCloserSpells.push_back(new GapCloser("Taliyah", "Taliyah R", kSpellSlot::SpellSlot_R, skillshot, 3));
		GapCloserSpells.push_back(new GapCloser("Talon", "Talon Q", kSpellSlot::SpellSlot_Q, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Talon", "Talon E", kSpellSlot::SpellSlot_E, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Thresh", "Thresh Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Tristana", "Tristana W", kSpellSlot::SpellSlot_W, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Tryndamere", "Tryndamere E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Urgot", "Urgot E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Vayne", "Vayne Q", kSpellSlot::SpellSlot_Q, skillshot, 1));
		GapCloserSpells.push_back(new GapCloser("Vi", "Vi Q", kSpellSlot::SpellSlot_Q, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Warwick", "Warwick R", kSpellSlot::SpellSlot_R, skillshot, 3));
		GapCloserSpells.push_back(new GapCloser("MonkeyKing", "MonkeyKing E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Wukong", "Wukong E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("XinZhao", "XinZhao E", kSpellSlot::SpellSlot_E, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Yasuo", "Yasuo E", kSpellSlot::SpellSlot_E, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Zac", "Zac E", kSpellSlot::SpellSlot_E, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Zed", "Zed W", kSpellSlot::SpellSlot_W, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Zed", "Zed R", kSpellSlot::SpellSlot_R, targeted, 2));
		GapCloserSpells.push_back(new GapCloser("Ziggs", "Ziggs W", kSpellSlot::SpellSlot_W, skillshot, 2));
		GapCloserSpells.push_back(new GapCloser("Zoe", "Zoe E", kSpellSlot::SpellSlot_R, skillshot, 2));


		auto hero_list = HACKUZAN::GameObject::GetHeroes();
		for (size_t i = 0; i < hero_list->size; i++)
		{
			auto hero = hero_list->entities[i];

			GapCloserSpells.erase(std::remove_if(GapCloserSpells.begin(), GapCloserSpells.end(), [&](GapCloser* i)
				{
					return hero != nullptr && hero->IsEnemy() && i->ChampionName != std::string(hero->BaseCharacterData->SkinName);
				}), GapCloserSpells.end());
		}
	}
}