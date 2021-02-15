#include "stdafx.h"
#include "Activator.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"

namespace V21 {
	namespace Plugins {

		using namespace V21::SDK;
		using namespace V21::SDK::Orbwalker;

		namespace Config {

			namespace Auto {
				CheckBox* EnableActivator;

				CheckBox* UseHeal;
				CheckBox* UseBarrier;
				CheckBox* UseIgnite;
				CheckBox* UseCleanse;
				CheckBox* UsePots;
				CheckBox* UseQSS;

				Slider* hpPots;
				Slider* hpHeal;
				Slider* hpBarrier;
			}
		}

		void Activator::Initialize()
		{
			auto menu = Menu::CreateMenu("Activator", "Activator");

			Config::Auto::EnableActivator = menu->AddCheckBox("EnableActivator", "Enable Activator", true);

			auto AutoHeal = menu->AddMenu("AutoHeal", "Auto Heal");
			Config::Auto::UsePots = AutoHeal->AddCheckBox("UsePots", "Use Item Potion", true);
			Config::Auto::hpPots = AutoHeal->AddSlider("hpHeal", "if hp% less than or equal ", 70, 1, 100, 5);
			Config::Auto::UseHeal = AutoHeal->AddCheckBox("UseHeal", "Use SummonerSpell Heal", true);
			Config::Auto::hpHeal = AutoHeal->AddSlider("hpHeal", "if hp% less than or equal ", 30, 1, 100, 5);
			auto AutoBarrier = menu->AddMenu("AutoBarrier", "Auto Barrier");
			Config::Auto::UseBarrier = AutoBarrier->AddCheckBox("UseBarrier", "Use SummonerSpell Barrier", true);
			Config::Auto::hpBarrier = AutoBarrier->AddSlider("hpBarrier", "if hp% less than or equal ", 30, 1, 100, 5);
			auto AntiCC = menu->AddMenu("Anti-CC", "Anti-CC");
			Config::Auto::UseQSS = AntiCC->AddCheckBox("UseQSS", "Use Item Cleanse x QSS x Mercurial", true);

			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);

			GameClient::PrintChat("Activator Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void Activator::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
		}

		void Activator::OnGameUpdate()
		{
			if (Config::Auto::EnableActivator->Value && ObjectManager::Player->Alive() && !ObjectManager::Player->InFountain() && ObjectManager::Player->CountEnemiesInRange(1000) > 0) {

				//ANTI-CC
				if (Config::Auto::UseQSS->Value) {
					if (ObjectManager::Player->FindItem(Item::Quicksilver_Sash) && ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->FindItemSpellSLot(Item::Quicksilver_Sash)) == SpellState_Ready) {
						if (ObjectManager::Player->FindBuffType(BuffType::Stun) || ObjectManager::Player->FindBuffType(BuffType::Knockup) || ObjectManager::Player->FindBuffType(BuffType::Knockback) || ObjectManager::Player->FindBuffType(BuffType::Charm) || ObjectManager::Player->FindBuffType(BuffType::Flee) || 
							ObjectManager::Player->FindBuffType(BuffType::Snare) || ObjectManager::Player->FindBuffType(BuffType::Fear) || ObjectManager::Player->FindBuffType(BuffType::Taunt) || ObjectManager::Player->FindBuffType(BuffType::Polymorph)) {
							GameClient::PrintChat("try cast Quicksilver_Sash~!", IM_COL32(255, 69, 255, 255));
							ObjectManager::Player->CastSpell(ObjectManager::Player->FindItemSpellSLot(Item::Quicksilver_Sash), (DWORD)ObjectManager::Player);
						}
					}
					if ( ObjectManager::Player->FindItem(Item::Mercurial_Scimitar) && ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->FindItemSpellSLot(Item::Mercurial_Scimitar)) == SpellState_Ready) {
						if (ObjectManager::Player->FindBuffType(BuffType::Stun) || ObjectManager::Player->FindBuffType(BuffType::Knockup) || ObjectManager::Player->FindBuffType(BuffType::Knockback) || ObjectManager::Player->FindBuffType(BuffType::Charm) || ObjectManager::Player->FindBuffType(BuffType::Flee) ||
							ObjectManager::Player->FindBuffType(BuffType::Snare) || ObjectManager::Player->FindBuffType(BuffType::Fear) || ObjectManager::Player->FindBuffType(BuffType::Taunt) || ObjectManager::Player->FindBuffType(BuffType::Polymorph)) {
							GameClient::PrintChat("try cast Mercurial_Scimitar~!", IM_COL32(255, 69, 255, 255));
							ObjectManager::Player->CastSpell(ObjectManager::Player->FindItemSpellSLot(Item::Mercurial_Scimitar), (DWORD)ObjectManager::Player);
						}
					}
					if (ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerCleanse)) == SpellState_Ready) {
						if (ObjectManager::Player->FindBuffType(BuffType::Stun) || ObjectManager::Player->FindBuffType(BuffType::Knockup) || ObjectManager::Player->FindBuffType(BuffType::Knockback) || ObjectManager::Player->FindBuffType(BuffType::Charm) || ObjectManager::Player->FindBuffType(BuffType::Flee) ||
							ObjectManager::Player->FindBuffType(BuffType::Snare) || ObjectManager::Player->FindBuffType(BuffType::Fear) || ObjectManager::Player->FindBuffType(BuffType::Taunt) || ObjectManager::Player->FindBuffType(BuffType::Polymorph)) {
							GameClient::PrintChat("try cast Cleanse~!", IM_COL32(255, 69, 255, 255));
							ObjectManager::Player->CastSpell(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerCleanse), (DWORD)ObjectManager::Player);
						}
					}
				}

				//HEALS && BARRIER
				if (Config::Auto::UsePots->Value) {
					if (ObjectManager::Player->FindItem(Item::Health_Potion) && ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->FindItemSpellSLot(Item::Health_Potion)) == SpellState_Ready) {
						if (ObjectManager::Player->Health <= (Config::Auto::hpPots->Value / 100.0) * ObjectManager::Player->MaxHealth) {
							if (!ObjectManager::Player->FindBuffType(BuffType::Invulnerability) && !ObjectManager::Player->FindBuffType(BuffType::SpellShield) && !ObjectManager::Player->FindBuffType(BuffType::Heal) && !ObjectManager::Player->FindBuffType(BuffType::SpellImmunity) && !ObjectManager::Player->FindBuffType(BuffType::PhysicalImmunity)) {
								ObjectManager::Player->CastSpell(ObjectManager::Player->FindItemSpellSLot(Item::Health_Potion), (DWORD)ObjectManager::Player);
							}
						}
					}
					if (ObjectManager::Player->FindItem(Item::Refillable_Potion) && ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->FindItemSpellSLot(Item::Refillable_Potion)) == SpellState_Ready) {
						if (ObjectManager::Player->Alive() && ObjectManager::Player->Health <= (Config::Auto::hpPots->Value / 100.0) * ObjectManager::Player->MaxHealth) {
							if (!ObjectManager::Player->FindBuffType(BuffType::Invulnerability) && !ObjectManager::Player->FindBuffType(BuffType::SpellShield) && !ObjectManager::Player->FindBuffType(BuffType::Heal) && !ObjectManager::Player->FindBuffType(BuffType::SpellImmunity) && !ObjectManager::Player->FindBuffType(BuffType::PhysicalImmunity)) {
								ObjectManager::Player->CastSpell(ObjectManager::Player->FindItemSpellSLot(Item::Refillable_Potion), (DWORD)ObjectManager::Player);
							}
						}
					}
				}

				if (Config::Auto::UseHeal->Value) {
					if (ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerHeal)) == SpellState_Ready) {
						if (ObjectManager::Player->Health <= (Config::Auto::hpHeal->Value / 100.0) * ObjectManager::Player->MaxHealth) {
							if (!ObjectManager::Player->FindBuffType(BuffType::Invulnerability) && !ObjectManager::Player->FindBuffType(BuffType::SpellShield) && !ObjectManager::Player->FindBuffType(BuffType::Heal) && !ObjectManager::Player->FindBuffType(BuffType::SpellImmunity) && !ObjectManager::Player->FindBuffType(BuffType::PhysicalImmunity)) {
								ObjectManager::Player->CastSpell(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerHeal), (DWORD)ObjectManager::Player);
							}
						}
						//if (ObjectManager::Player->Health <= (Config::Auto::hpHeal->Value / 100.0) * ObjectManager::Player->MaxHealth) {
						//	if (!ObjectManager::Player->FindBuffType(BuffType::Invulnerability) && !ObjectManager::Player->FindBuffType(BuffType::SpellShield) && ObjectManager::Player->FindBuffType(BuffType::Heal) && !ObjectManager::Player->FindBuffType(BuffType::SpellImmunity) && !ObjectManager::Player->FindBuffType(BuffType::PhysicalImmunity)) {
						//		ObjectManager::Player->CastSpell(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerHeal), (DWORD)ObjectManager::Player);
						//	}
						//}
					}
				}

				if (Config::Auto::UseBarrier->Value) {
					if (ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerBarrier)) == SpellState_Ready) {
						if (ObjectManager::Player->Health <= (Config::Auto::hpHeal->Value / 100.0) * ObjectManager::Player->MaxHealth) {
							if (!ObjectManager::Player->FindBuffType(BuffType::Invulnerability) && !ObjectManager::Player->FindBuffType(BuffType::SpellShield) && !ObjectManager::Player->FindBuffType(BuffType::Heal) && !ObjectManager::Player->FindBuffType(BuffType::SpellImmunity) && !ObjectManager::Player->FindBuffType(BuffType::PhysicalImmunity)) {
								ObjectManager::Player->CastSpell(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerBarrier), (DWORD)ObjectManager::Player);
							}
						}
						//if (ObjectManager::Player->Health <= (Config::Auto::hpHeal->Value / 100.0) * ObjectManager::Player->MaxHealth) {
						//	if (!ObjectManager::Player->FindBuffType(BuffType::Invulnerability) && !ObjectManager::Player->FindBuffType(BuffType::SpellShield) && ObjectManager::Player->FindBuffType(BuffType::Heal) && !ObjectManager::Player->FindBuffType(BuffType::SpellImmunity) && !ObjectManager::Player->FindBuffType(BuffType::PhysicalImmunity)) {
						//		ObjectManager::Player->CastSpell(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerBarrier), (DWORD)ObjectManager::Player);
						//	}
						//}
					}
				}
			}
		}
	}
}
