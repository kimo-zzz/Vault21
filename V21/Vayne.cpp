#include "stdafx.h"
#include "Vayne.h"
#include "NavGrid.h"
#include "Draw.h"
#include "GapCloserDB.h"
#include "Geometry.h"

namespace HACKUZAN {
	namespace Plugins {
		bool Vayne::LastAttackCastSpell = false;

		using namespace HACKUZAN::SDK;
		using namespace HACKUZAN::SDK::Orbwalker;

		class VayneantiGapCloser
		{
		public:
			Menu* ParentMenu;
			CheckBox* Enable;
		};

		std::map<string, VayneantiGapCloser> AntiGapCloserOptions;

		namespace VayneConfig {

			namespace VayneCombo {
				CheckBox* UseQ;
				CheckBox* SmartQ;
				CheckBox* UseE;
				CheckBox* UseR;
				Slider* enemiesInRange;
				Slider* lowHPUlt;
			}

			namespace VayneMisc {
				CheckBox* AutoE;
				Slider* PushDistance;
				Slider* HitChance;
			}
		}

		void HACKUZAN::Plugins::Vayne::Initialize()
		{
			auto menu = Menu::CreateMenu("Vayne", "Vayne");

			auto combo = menu->AddMenu("Combo", "Combo");
			VayneConfig::VayneCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
			VayneConfig::VayneCombo::SmartQ = combo->AddCheckBox("SmartQ", "Smart Q Spell Usage", false);
			VayneConfig::VayneCombo::UseE = combo->AddCheckBox("Use E", "Use SpellSlot E", true);
			VayneConfig::VayneCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);
			VayneConfig::VayneCombo::enemiesInRange = combo->AddSlider("enemiesInRange", "if x Enemies in range", 2, 1, 5, 1);
			VayneConfig::VayneCombo::lowHPUlt = combo->AddSlider("lowHPUlt", "if hp% less than or equal ", 50, 1, 100, 5);
			auto misc = combo->AddMenu("misc", "Condemn Logic");
			VayneConfig::VayneMisc::AutoE = misc->AddCheckBox("Auto Condemn", "Enable Auto Condemn", true);
			VayneConfig::VayneMisc::PushDistance = misc->AddSlider("PushDistance", "Push Distance", 475, 0, 1000, 10);
			//Config::Misc::HitChance = misc->AddSlider("HitChance", "Condemn HitChance", 50, 0, 100, 5);
			/*auto AntiGapCloser = misc->AddMenu("antiGapCloser", "Anti-GapCloser");
			auto hero_list = HACKUZAN::GameObject::GetHeroes();
			for (size_t i = 0; i < hero_list->size; i++) {
				auto hero = hero_list->entities[i];
				if (hero && hero->IsEnemy()) {
					for (GapCloser* gapcloser : GapClosersDB->GapCloserSpells) {
						if (gapcloser->ChampionName.c_str() == hero->BaseCharacterData->SkinName) {
							AntiGapCloserOptions[gapcloser->MenuName].ParentMenu = AntiGapCloser->AddMenu(gapcloser->ChampionName.c_str(), gapcloser->ChampionName.c_str());
							AntiGapCloserOptions[gapcloser->MenuName].Enable = AntiGapCloserOptions[gapcloser->MenuName].ParentMenu->AddCheckBox(gapcloser->MenuName.c_str(), gapcloser->MenuName.c_str(), true);
						}
					}
				}
			}*/

			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);

			GameClient::PrintChat("Vayne Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void Vayne::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
		}


		void Vayne::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			if (castInfo == nullptr)
				return;
			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];

			for (GapCloser* gapcloser : GapClosersDB->GapCloserSpells) {
				if (caster && caster->IsEnemy()) {
					if (gapcloser->ChampionName == caster->BaseCharacterData->SkinName) {
						if (gapcloser->Slot == castInfo->Slot) {
							if (VayneConfig::VayneMisc::AutoE->Value) {
								if (caster && ObjectManager::Player->Position.Distance(caster->Position) <= 1000) {
									if (ObjectManager::Player->Position.Distance(castInfo->EndPosition) <= 350 || castInfo->TargetId == ObjectManager::Player->Id) {
										//GameClient::PrintChat("OnGapCloserSpells detected!", IM_COL32(255, 69, 255, 255));
										ObjectManager::Player->CastTargetSpell(kSpellSlot::SpellSlot_E, (DWORD)ObjectManager::Player, (DWORD)caster, ObjectManager::Player->Position, caster->Position, caster->NetworkId);
									}
								}
							}
						}
					}
				}
			}
		}

		void Vayne::OnGameUpdate()
		{
			auto target = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 1000.0f, kDamageType::DamageType_Physical);

			if (ActiveMode & OrbwalkerMode_Combo) {

				if (target && Orbwalker::CanCastAfterAttack() && ObjectManager::Player->Position.Distance(target->Position) <= 550) {
					if (VayneConfig::VayneCombo::UseE->Value) {
						if (!target->FindBuffType(BuffType::SpellShield) && !target->FindBuffType(BuffType::SpellImmunity) && !target->IsInvulnerable() && !target->IsMagicImmune()) {
							if (IsCondemnable(target)) {
								ObjectManager::Player->CastTargetSpell(kSpellSlot::SpellSlot_E, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->Position, target->Position, target->NetworkId);
							}
							if (target->IsMelee() && ObjectManager::Player->Position.Distance(target->Position) <= target->AttackRange + 50) {
								ObjectManager::Player->CastTargetSpell(kSpellSlot::SpellSlot_E, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->Position, target->Position, target->NetworkId);
							}
						}
					}
				}
			}

			if (target != nullptr && Orbwalker::CanCastAfterAttack() && ObjectManager::Player->Position.Distance(target->Position) <= 550
				|| target != nullptr && Orbwalker::CanCastAfterAttack() && ObjectManager::Player->Position.Distance(target->Position) <= 650 && ObjectManager::Player->Position.Distance(target->Position) >= ObjectManager::Player->GetAutoAttackRange()) {

				if (ActiveMode & OrbwalkerMode_Combo) {

					if (VayneConfig::VayneCombo::UseQ->Value) {

						if (VayneConfig::VayneCombo::SmartQ->Value) {
							auto after = ObjectManager::Player->Position + (HudManager::Instance->CursorTargetLogic->CursorPosition - ObjectManager::Player->Position).Normalized() * 300;

							auto disafter = target->Position.DistanceSquared(after);

							if ((disafter < 630 * 630) && disafter > 150 * 150)
							{
								ObjectManager::Player->CastSpellPos(kSpellSlot::SpellSlot_Q, (DWORD)ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition);
							}
							if (ObjectManager::Player->Position.DistanceSquared(target->Position) > 630 * 630 && disafter < 630 * 630)
							{
								ObjectManager::Player->CastSpellPos(kSpellSlot::SpellSlot_Q, (DWORD)ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition);
							}
						}
						if (!VayneConfig::VayneCombo::SmartQ->Value) {
							if (ObjectManager::Player->IsInAutoAttackRange(target)) {
								ObjectManager::Player->CastSpellPos(kSpellSlot::SpellSlot_Q, (DWORD)ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition);
							}
						}
					}
					if (VayneConfig::VayneCombo::UseR->Value) {
						if (ObjectManager::Player->CountEnemiesInRange(600) >= VayneConfig::VayneCombo::enemiesInRange->Value) {
							ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_R, (DWORD)ObjectManager::Player);
						}
						if (ObjectManager::Player->Health <= (VayneConfig::VayneCombo::lowHPUlt->Value / 100.0) * ObjectManager::Player->MaxHealth) {
							ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_R, (DWORD)ObjectManager::Player);
						}
					}
				}
			}
			if (VayneConfig::VayneMisc::AutoE->Value) {
				if (ActiveMode != OrbwalkerMode_Combo) {
					if (target && ObjectManager::Player->Position.Distance(target->Position) <= 550) {
						if (!target->FindBuffType(BuffType::SpellShield) && !target->FindBuffType(BuffType::SpellImmunity) && !target->IsInvulnerable() && !target->IsMagicImmune()) {
							if (IsCondemnable(target)) {
								ObjectManager::Player->CastTargetSpell(kSpellSlot::SpellSlot_E, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->Position, target->Position, target->NetworkId);
							}
							if (target->IsMelee() && ObjectManager::Player->Position.Distance(target->Position) <= target->AttackRange + 50) {
								ObjectManager::Player->CastTargetSpell(kSpellSlot::SpellSlot_E, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->Position, target->Position, target->NetworkId);
							}
						}
					}
				}
			}
		}

		bool HACKUZAN::Plugins::Vayne::IsCondemnable(GameObject* target)
		{
			if (target == nullptr)
				return false;
			auto  pushDistance = VayneConfig::VayneMisc::PushDistance->Value;
			auto targetPosition = target->Position;
			float checkDistance = pushDistance / 40.0f;
			auto pushDirection = (targetPosition - ObjectManager::Player->Position).Normalized();

			for (int i = 0; i < 40; i++)
			{
				Vector3 finalPosition = targetPosition + (pushDirection * checkDistance * i);

				if (NavGrid::IsWall(finalPosition)) {

					Draw.Line(target->Position, finalPosition, 3, IM_COL32(255, 255, 69, 255));
					Draw.DrawCircle3D(finalPosition, 50, 50, IM_COL32(255, 255, 69, 255));

					return true;
				}
			}
			return false;
		}
	}
}
