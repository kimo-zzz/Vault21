#include "stdafx.h"
#include "Riven.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"
#include "RenderLayer.h"

namespace HACKUZAN {
	namespace Plugins {



		using namespace HACKUZAN::SDK;
		using namespace HACKUZAN::SDK::Orbwalker;
		using namespace HACKUZAN::SDK::TargetSelector;

		namespace RivenConfig 
		{
			namespace RivenCombo 
			{
				CheckBox* UseQ;
				CheckBox* UseW;
				CheckBox* UseE;
				CheckBox* UseR;

				CheckBox* UseQAsGapcloser;
				CheckBox* UseEAsGapcloser;
				CheckBox* UseWAsInterupt;


				CheckBox* UseFlash;
				CheckBox* Killsteal;

				List* InteruptMode;
				List* ComboStyle;
			}

			namespace RivenDraws
			{
				CheckBox* DrawQ;
				CheckBox* DrawW;
				CheckBox* DrawE;
				CheckBox* DrawR2;
				CheckBox* DrawFlash;
				CheckBox* DrawCombo;
			}

			namespace RivenFarm
			{
				CheckBox* UseQ;
				CheckBox* UseW;
				CheckBox* UseE;
			}

			namespace RivenMisc 
			{
				CheckBox* AutoE;
				CheckBox* Debug;
			}
		}

		void Riven::Initialize()
		{
			auto menu = Menu::CreateMenu("riven", "Riven");
			auto combo = menu->AddMenu("combo", "Combo Settings");
			auto farm = menu->AddMenu("farming", "Farm Setting");
			auto draw = menu->AddMenu("draw", "Draw Settings");
			auto misc = menu->AddMenu("misc", "Misc Settings");


			RivenConfig::RivenCombo::UseQ = combo->AddCheckBox("Riven Combo Q", "Use Q", true);
			RivenConfig::RivenCombo::UseW = combo->AddCheckBox("Riven Combo W", "Use W", true);
			RivenConfig::RivenCombo::UseE = combo->AddCheckBox("Riven Combo E", "Use E", true);
			RivenConfig::RivenCombo::UseR = combo->AddCheckBox("Riven Combo R", "Use R", true);
			combo->AddInfo("comboSpacer0", " ");
			RivenConfig::RivenCombo::UseQAsGapcloser = combo->AddCheckBox("Riven Combo Use Q as Gapcloser", "Use Q as Gapcloser", true);
			RivenConfig::RivenCombo::UseEAsGapcloser = combo->AddCheckBox("Riven Combo Use E as Gapcloser", "Use E as Gapcloser", true);
			combo->AddInfo("comboSpacer1", " ");
			RivenConfig::RivenCombo::UseWAsInterupt = combo->AddCheckBox("Riven Combo Use W as Interupt", "Use W as Interupt", true);
			RivenConfig::RivenCombo::InteruptMode = combo->AddList("Riven Combo Interupt Mode", "Interupt Mode", std::vector<std::string>{ "All", "Dangerous Spells", "Summoner Spells" }, 0u, nullptr);
			combo->AddInfo("comboSpacer2", " ");
			RivenConfig::RivenCombo::UseFlash = combo->AddCheckBox("Riven Combo Use Flash", "Use Flash", false);
			RivenConfig::RivenCombo::UseFlash = combo->AddCheckBox("Riven Combo Killsteal", "Use R2 to Killsteal", false);
			combo->AddInfo("comboSpacer3", " ");
			RivenConfig::RivenCombo::ComboStyle = combo->AddList("Riven Combo Mode", "Combo Style", std::vector<std::string>{ "BoxBox", "Shy", "Adrien", "BRNA", "Vyper" }, 0u, nullptr);

			RivenConfig::RivenFarm::UseQ = farm->AddCheckBox("Riven Farm Q", "Use Q", true);
			RivenConfig::RivenFarm::UseW = farm->AddCheckBox("Riven Farm W", "Use W", false);
			RivenConfig::RivenFarm::UseE = farm->AddCheckBox("Riven Farm E", "Use E", false);

			RivenConfig::RivenMisc::AutoE = misc->AddCheckBox("Riven Misc Auto E", "Auto E", true);
			RivenConfig::RivenMisc::Debug = misc->AddCheckBox("Riven Misc Debug", "Debug Mode", false);

			RivenConfig::RivenDraws::DrawQ = draw->AddCheckBox("Riven Draws Draw Q", "Draw Q Range", true);
			RivenConfig::RivenDraws::DrawW = draw->AddCheckBox("Riven Draws Draw W", "Draw W Range", true);
			RivenConfig::RivenDraws::DrawE = draw->AddCheckBox("Riven Draws Draw E", "Draw E Range", true);
			RivenConfig::RivenDraws::DrawR2 = draw->AddCheckBox("Riven Draws Draw R2", "Draw R2 Range", true);
			RivenConfig::RivenDraws::DrawFlash = draw->AddCheckBox("Riven Draws Draw Flash", "Draw Flash Range", true);
			RivenConfig::RivenDraws::DrawCombo = draw->AddCheckBox("Riven Draws Draw Combo Style", "Draw Combo Style", true);


			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);

			GameClient::PrintChat("Riven Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void Riven::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		void Riven::OnGameUpdate()
		{
			auto target = GetTarget();
			if (!Orbwalker::OrbwalkerEvading)
			{
				switch (Orbwalker::ActiveMode)
				{
					case OrbwalkerMode_Combo:
					{
						if (target && Orbwalker::CanCastAfterAttack() && !Orbwalker::CanAttack(target) && ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_Q) == kSpellState::SpellState_Ready)
						{
							if (Distance(target, ObjectManager::Player) <= 260)
							{
								ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->Position, target->Position, target->NetworkId);
								return;
							}
						}
						if (target && Orbwalker::CanCastAfterAttack() && !target->IsImmovable() && ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_W) == kSpellState::SpellState_Ready)
						{
							ObjectManager::Player->CastSpell(SpellSlot_W, (DWORD)ObjectManager::Player);
							return;
						}
					}
					break;
					case OrbwalkerMode_LastHit:
					{

					}
					break;
					case OrbwalkerMode_LaneClear:
					case OrbwalkerMode_JungleClear:
					{

					}
					break;
					case OrbwalkerMode_Flee:
					{
						if (ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_Q) == kSpellState::SpellState_Ready)
							ObjectManager::Player->CastSpellPos(SpellSlot_Q, (DWORD)ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition);
						else if (ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_E) == kSpellState::SpellState_Ready)
							ObjectManager::Player->CastSpellPos(SpellSlot_Q, (DWORD)ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition);
	
					}
					break;
					default:
					{

					}
					break;
				}
			}
		}

		void Riven::OnDraw()
		{
			if (RivenConfig::RivenDraws::DrawQ->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 275);
			if (RivenConfig::RivenDraws::DrawW->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 260);
			if (RivenConfig::RivenDraws::DrawE->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 310);
			if (RivenConfig::RivenDraws::DrawR2->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 850);
			if (RivenConfig::RivenDraws::DrawFlash->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 450);
			if (RivenConfig::RivenDraws::DrawCombo->Value == true)
			{
				std::string sCombo = ("Style: " + RivenConfig::RivenCombo::ComboStyle->Items[RivenConfig::RivenCombo::ComboStyle->Value]);
				auto Combo = sCombo.c_str();
				Vector2 Position;
				RenderLayer::WorldToScreen(ObjectManager::Player->Position, Position);
				auto size = ImGui::CalcTextSize(Combo);
				Position.Y = Position.Y + 50;
				Position.X = Position.X - (size.x / 2);
				Renderer::AddText(Combo, 15, Position, IM_COL32(255, 0, 255, 255));
			}
		}

		bool Riven::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position)
		{
			return true;
		}

		void Riven::OnCreateObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;
		}

		void Riven::OnDeleteObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;

		}

		void Riven::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			if (castInfo == nullptr || spellData == nullptr)
				return;

			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];

#pragma region Debugging
			if (RivenConfig::RivenMisc::Debug->Value == true)
			{
				std::string x = "Target Id: " + std::to_string(castInfo->TargetId);
				std::string z = "Source Id: " + std::to_string(castInfo->SourceId);

				auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];

				GameClient::PrintChat(("[Riven Debug] Casted Spell: " + std::string(castInfo->GetSpellData()->SpellName)).c_str(), IM_COL32(255, 69, 255, 255));
				GameClient::PrintChat(x.c_str(), IM_COL32(255, 69, 255, 255));
				GameClient::PrintChat(z.c_str(), IM_COL32(255, 69, 255, 255));
				GameClient::PrintChat(caster->Name.c_str(), IM_COL32(255, 69, 255, 255));
			}
#pragma endregion

			if (caster->Id == ObjectManager::Player->Id)
			{
				GameClient::PrintChat("Cast by Me!!!", IM_COL32(255, 69, 255, 255));
				if (strcmp(spellData->SpellName, "RivenTriCleave") == 0)
				{
					GameClient::PrintChat("Reset Auto Attack", IM_COL32(255, 69, 255, 255));
					Orbwalker::ResetAutoAttack();
				}

			}

			if (castInfo->TargetId == ObjectManager::Player->Id) // Check if we're the Target of the Spell
			{
				if (strcmp(spellData->SpellName, "ChaosTurretWormBasicAttack") == 0 || strcmp(spellData->SpellName, "OrderTurretWormBasicAttack") == 0)
				{
					if(RivenConfig::RivenMisc::AutoE->Value == true)
						ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition);
				}
			}
		}

		GameObject* Riven::Target()
		{
			std::vector<GameObject*> heroes;
			auto hero_list = HACKUZAN::GameObject::GetHeroes();
			for (size_t i = 0; i < hero_list->size; i++)
			{
				auto hero = hero_list->entities[i];

				if (hero && hero->IsEnemy() && hero->IsValidTarget(900)) {
					heroes.push_back(hero);
				}
			}
			return TargetSelector::GetTarget(heroes, DamageType_Physical);
		}
	}
}