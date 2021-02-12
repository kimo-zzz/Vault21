#include "stdafx.h"
#include "Riven.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"
#include "RenderLayer.h"

namespace HACKUZAN {
	namespace Plugins {

		/// <summary>
		/// Q:	RivenTriCleave
		///	W:	RivenMatyr
		/// E:	RivenFeint
		/// R1: RivenFengShuiEngine
		/// R2: RivenIzunaBlade
		/// </summary>

		using namespace HACKUZAN::SDK;
		using namespace HACKUZAN::SDK::Orbwalker;
		using namespace HACKUZAN::SDK::TargetSelector;

		namespace Riven
		{

			#pragma region Configuration Stuff

			namespace Config
			{


				namespace Combo
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

				namespace Draws
				{
					CheckBox* DrawQ;
					CheckBox* DrawW;
					CheckBox* DrawE;
					CheckBox* DrawR2;
					CheckBox* DrawFlash;
					CheckBox* DrawCombo;
				}

				namespace Farming
				{
					CheckBox* UseQ;
					CheckBox* UseW;
					CheckBox* UseE;
				}

				namespace Misc
				{
					CheckBox* AutoE;
					CheckBox* Debug;
				}


			}

			#pragma endregion

			#pragma region Logics

			namespace Logics
			{

				bool _CastQ = false;
				float _tickCastQ = 0;

				bool _CastW = false;
				float _tickCastW = 0;

				bool _CastE = false;
				float _tickCastE = 0;

			}

			#pragma endregion

		}

		void Riven::Initialize()
		{
			auto menu = Menu::CreateMenu("riven", "Riven");
			auto combo = menu->AddMenu("combo", "Combo Settings");
			auto farm = menu->AddMenu("farming", "Farm Setting");
			auto draw = menu->AddMenu("draw", "Draw Settings");
			auto misc = menu->AddMenu("misc", "Misc Settings");


			Config::Combo::UseQ = combo->AddCheckBox("Riven Combo Q", "Use Q", true);
			Config::Combo::UseW = combo->AddCheckBox("Riven Combo W", "Use W", true);
			Config::Combo::UseE = combo->AddCheckBox("Riven Combo E", "Use E", true);
			Config::Combo::UseR = combo->AddCheckBox("Riven Combo R", "Use R", true);
			combo->AddInfo("comboSpacer0", " ");
			Config::Combo::UseQAsGapcloser = combo->AddCheckBox("Riven Combo Use Q as Gapcloser", "Use Q as Gapcloser", true);
			Config::Combo::UseEAsGapcloser = combo->AddCheckBox("Riven Combo Use E as Gapcloser", "Use E as Gapcloser", true);
			combo->AddInfo("comboSpacer1", " ");
			Config::Combo::UseWAsInterupt = combo->AddCheckBox("Riven Combo Use W as Interupt", "Use W as Interupt", true);
			Config::Combo::InteruptMode = combo->AddList("Riven Combo Interupt Mode", "Interupt Mode", std::vector<std::string>{ "All", "Dangerous Spells", "Summoner Spells" }, 0u, nullptr);
			combo->AddInfo("comboSpacer2", " ");
			Config::Combo::UseFlash = combo->AddCheckBox("Riven Combo Use Flash", "Use Flash", false);
			Config::Combo::UseFlash = combo->AddCheckBox("Riven Combo Killsteal", "Use R2 to Killsteal", false);
			combo->AddInfo("comboSpacer3", " ");
			Config::Combo::ComboStyle = combo->AddList("Riven Combo Mode", "Combo Style", std::vector<std::string>{ "BoxBox", "Shy", "Adrien", "BRNA", "Vyper" }, 0u, nullptr);

			Config::Farming::UseQ = farm->AddCheckBox("Riven Farm Q", "Use Q", true);
			Config::Farming::UseW = farm->AddCheckBox("Riven Farm W", "Use W", false);
			Config::Farming::UseE = farm->AddCheckBox("Riven Farm E", "Use E", false);

			Config::Misc::AutoE = misc->AddCheckBox("Riven Misc Auto E", "Auto E", true);
			Config::Misc::Debug = misc->AddCheckBox("Riven Misc Debug", "Debug Mode", false);

			Config::Draws::DrawQ = draw->AddCheckBox("Riven Draws Draw Q", "Draw Q Range", true);
			Config::Draws::DrawW = draw->AddCheckBox("Riven Draws Draw W", "Draw W Range", true);
			Config::Draws::DrawE = draw->AddCheckBox("Riven Draws Draw E", "Draw E Range", true);
			Config::Draws::DrawR2 = draw->AddCheckBox("Riven Draws Draw R2", "Draw R2 Range", true);
			Config::Draws::DrawFlash = draw->AddCheckBox("Riven Draws Draw Flash", "Draw Flash Range", true);
			Config::Draws::DrawCombo = draw->AddCheckBox("Riven Draws Draw Combo Style", "Draw Combo Style", true);


			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnFinishCast, OnFinishCast);
			EventManager::AddEventHandler(LeagueEvents::OnStopCast, OnStopCast);
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
			EventManager::RemoveEventHandler(LeagueEvents::OnFinishCast, OnFinishCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnStopCast, OnStopCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		void Riven::OnGameUpdate()
		{
			auto target = GetTarget();
			if (!Orbwalker::OrbwalkerEvading)
			{
				if (Logics::_CastQ && (Logics::_tickCastQ - ClockFacade::GameTickCount()) <= 0)
				{
					auto _target = Orbwalker::GetTarget();
					if (_target)
					{
						if (Config::Misc::Debug->Value == true)
							GameClient::PrintChat("[Riven Debug] Casting queued Q Spell.", IM_COL32(255, 69, 255, 255));
						Logics::_CastQ = false;
						Logics::_tickCastQ = -1;
						ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)_target, ObjectManager::Player->Position, _target->Position, _target->NetworkId);
					}
				}

				switch (Orbwalker::ActiveMode)
				{
					case OrbwalkerMode_Combo:
					{

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
							ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition);
	
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
			if (Config::Draws::DrawQ->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 275);
			if (Config::Draws::DrawW->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 260);
			if (Config::Draws::DrawE->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 310);
			if (Config::Draws::DrawR2->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 850);
			if (Config::Draws::DrawFlash->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 450);
			if (Config::Draws::DrawCombo->Value == true)
			{
				std::string sCombo = ("Style: " + Config::Combo::ComboStyle->Items[Config::Combo::ComboStyle->Value]);
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

		void Riven::OnFinishCast(GameObject* object, SpellCastInfo* castInfo)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			#pragma region Debugging
			if (Config::Misc::Debug->Value == true)
				GameClient::PrintChat(("[Riven Debug] Finished Spell: " + castInfo->SpellData->Name).c_str(), IM_COL32(255, 69, 255, 255));
			#pragma endregion

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			#pragma region Logic Stuff (Combo, Farming)

			if (object->Id == ObjectManager::Player->Id)
			{
				switch (Orbwalker::ActiveMode)
				{
					case OrbwalkerMode_Combo:
					{
						if (castInfo->IsAutoAttack())
						{
							if (Config::Misc::Debug->Value == true)
								GameClient::PrintChat("[Riven Debug] Orbwalker Auto Attack detected.", IM_COL32(255, 69, 255, 255));
							Logics::_tickCastQ = ClockFacade::GameTickCount() + NetClient::Instance->GetPing();
							Logics::_CastQ = true;
						}
					}
					break;
				}
			}

			#pragma endregion 

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			#pragma region Misc Stuff (Auto E, Interupter)


			#pragma endregion

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		void Riven::OnStopCast(GameObject* caster, StopCast* args)
		{
		}

		void Riven::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			#pragma region Logic Stuff (Combo, Farming)
			if (caster->Id == ObjectManager::Player->Id)
			{
				switch (Orbwalker::ActiveMode)
				{
				case OrbwalkerMode_Combo:
				{
					if (strcmp(spellData->SpellName, "RivenTriCleave") == 0)
					{
						if (Config::Misc::Debug->Value == true)
							GameClient::PrintChat("[Riven Debug] Detected Q Spell and resetting Auto Timer.", IM_COL32(255, 69, 255, 255));
						Orbwalker::ResetAutoAttack();
					}
				}
				break;
				}
			}
			#pragma endregion

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			#pragma region Misc Stuff (Auto E, Interupter)
			if (castInfo->TargetId == ObjectManager::Player->Id)
			{
				if (strcmp(spellData->SpellName, "ChaosTurretWormBasicAttack") == 0 || strcmp(spellData->SpellName, "OrderTurretWormBasicAttack") == 0)
				{
					if (Config::Misc::AutoE->Value == true)
						ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition);
				}
			}
			#pragma endregion

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}