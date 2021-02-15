#include "stdafx.h"
#include "Lucian.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"
#include "RenderLayer.h"

namespace V21 {
	namespace Plugins {



		using namespace V21::SDK;
		using namespace V21::SDK::Orbwalker;

		namespace LucianConfig {

			namespace LucianCombo {
				List* ComboMode;
			}

			namespace LucianFarm {
				CheckBox* UseQ;
				CheckBox* UseE;

			}

			namespace LucianMisc {
				CheckBox* AutoE;
			}

			namespace LucianDrawings {
				CheckBox* DrawQ;
				CheckBox* DrawW;
				CheckBox* DrawE;
			}

		}

		void V21::Plugins::Lucian::Initialize()
		{
			auto menu = Menu::CreateMenu("Lucian", "Lucian");

			auto combo = menu->AddMenu("Combo", "Combo Settings");
			LucianConfig::LucianCombo::ComboMode = combo->AddList("ComboMode", "ComboMode", std::vector<string>{"E->W->Q", "Q->E->W"}, 0);

			auto farm = menu->AddMenu("farm", "Farm Settings");
			LucianConfig::LucianFarm::UseQ = farm->AddCheckBox("UseQ Lasthit", "Lasthit Q", true);
			LucianConfig::LucianFarm::UseE = farm->AddCheckBox("Use E Lasthit", "Use E", true);

			auto drawings = menu->AddMenu("Drawings", "Drawings Menu");
			LucianConfig::LucianDrawings::DrawQ = drawings->AddCheckBox("DrawQ", "DrawQ", true);
			LucianConfig::LucianDrawings::DrawW = drawings->AddCheckBox("DrawW", "DrawW", true);
			LucianConfig::LucianDrawings::DrawE = drawings->AddCheckBox("DrawE", "DrawE", true);

			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnPlayAnimation, OnPlayAnimation);
			EventManager::AddEventHandler(LeagueEvents::OnFinishCast, OnFinishCast);
			EventManager::AddEventHandler(LeagueEvents::OnStopCast, OnStopCast);
			EventManager::AddEventHandler(LeagueEvents::OnNewPath, OnNewPath);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);


			GameClient::PrintChat("Lucian Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void Lucian::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnPlayAnimation, OnPlayAnimation);
			EventManager::RemoveEventHandler(LeagueEvents::OnFinishCast, OnFinishCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnStopCast, OnStopCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnNewPath, OnNewPath);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

#pragma region Logics
		void Lucian::Combo()
		{
			auto Qtarget = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 500.f, DamageType_Physical, ObjectManager::Player->Position, true);
			auto Etarget = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 425.f + ObjectManager::Player->GetAutoAttackRange(), DamageType_Physical);


			switch (LucianConfig::LucianCombo::ComboMode->Value)
			{
			case 0: //E->W->Q
				if (Etarget && !HasDoubleshotPassive())
				{
					auto posAfterE = ObjectManager::Player->Position.Extended(HudManager::Instance->CursorTargetLogic->CursorPosition, 425.f);
					if (ObjectManager::Player->CountEnemiesInRange(1000.f) > 3 || Distance(Etarget, posAfterE) <= ObjectManager::Player->GetAutoAttackRange() - 20)
					{
						ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, posAfterE);
					}
				}

				if (Qtarget)
				{
					if (!HasDoubleshotPassive())
						ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)Qtarget, ObjectManager::Player->ServerPosition(), Qtarget->ServerPosition(), Qtarget->NetworkId);

					if (!HasDoubleshotPassive())
						ObjectManager::Player->CastPredictSpell(SpellSlot_W, ObjectManager::Player->Position, Qtarget->Position);
				}
				break;

			case 1: //Q->E->W
				if (Qtarget)
				{
					if (!HasDoubleshotPassive())
						ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)Qtarget, ObjectManager::Player->ServerPosition(), Qtarget->ServerPosition(), Qtarget->NetworkId);
				}

				if (Etarget && !HasDoubleshotPassive())
				{
					auto posAfterE = ObjectManager::Player->Position.Extended(HudManager::Instance->CursorTargetLogic->CursorPosition, 425.f);
					if (ObjectManager::Player->CountEnemiesInRange(1000.f) > 3 || Distance(Etarget, posAfterE) >= ObjectManager::Player->GetAutoAttackRange())
					{
						ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, posAfterE);
					}
				}

				if (Qtarget && !HasDoubleshotPassive())
					ObjectManager::Player->CastPredictSpell(SpellSlot_W, ObjectManager::Player->Position, Qtarget->Position);
				break;
			}

		}

		void Lucian::Farm()
		{
			auto target = TargetSelector::GetTarget(TargetType::TSTARGET_MINION, 500.f, DamageType_Physical, ObjectManager::Player->ServerPosition(), true);

			if (target == nullptr)
				return;

			if (!HasDoubleshotPassive())
				ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)target, ObjectManager::Player->ServerPosition(), target->ServerPosition(), target->NetworkId);

			if (!HasDoubleshotPassive())
			{
				auto posAfterE = ObjectManager::Player->Position.Extended(HudManager::Instance->CursorTargetLogic->CursorPosition, 425.f);
				if (ObjectManager::Player->CountEnemiesInRange(1000.f) > 3 || Distance(target, posAfterE) >= ObjectManager::Player->GetAutoAttackRange())
				{
					ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, posAfterE);
				}
			}
			if (!HasDoubleshotPassive())
				ObjectManager::Player->CastSpellPos(SpellSlot_W, (DWORD)ObjectManager::Player, target->ServerPosition());
		}

		void Lucian::Lasthit()
		{

			if (!ObjectManager::Player->CanAttack())
			{
				if (Orbwalker::LastHitMinion != nullptr)
				{
					if (LucianConfig::LucianFarm::UseQ->Value && ObjectManager::Player->Spellbook.GetSpellState(SpellSlot_Q) == SpellState_Ready && ObjectManager::Player->Resource >= ObjectManager::Player->MaxResource / 2)
						ObjectManager::Player->CastTargetSpell(SpellSlot_Q, (DWORD)ObjectManager::Player, (DWORD)Orbwalker::LastHitMinion, ObjectManager::Player->ServerPosition(), Orbwalker::LastHitMinion->ServerPosition(), Orbwalker::LastHitMinion->NetworkId);
					else if (LucianConfig::LucianFarm::UseE->Value && ObjectManager::Player->Resource >= ObjectManager::Player->MaxResource / 4)
						ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, ObjectManager::Player->ServerPosition());
				}

			}
		}

		void Lucian::Jungle()
		{
		}

#pragma endregion Logics

		void Lucian::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3* position, GameObject* target) {
			if (unit == ObjectManager::Player)
			{
				if (Orbwalker::DisableNextMove && order == GameObjectOrder::MoveTo) {

					return;
				}
			}
			return ;
		}


		void Lucian::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{

			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];
		}

		void Lucian::OnPlayAnimation(GameObject* ptr, char* name, float animationTime)
		{

		}

		void Lucian::OnFinishCast(SpellCastInfo* castInfo, GameObject* object)
		{

		}

		void Lucian::OnStopCast(GameObject* unit, StopCast args)
		{

		}

		void Lucian::OnNewPath(NewPath* args)
		{

		}

		void Lucian::OnCreateObject(GameObject* unit)
		{


		}

		void Lucian::OnDeleteObject(GameObject* unit)
		{
		
		}

		void Lucian::OnDraw()
		{
			if (LucianConfig::LucianDrawings::DrawQ->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 500.f, 1, IM_COL32(51, 0, 102, 255));

			if (LucianConfig::LucianDrawings::DrawE->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 420.f, 1, IM_COL32(153, 51, 255, 255));

			if (LucianConfig::LucianDrawings::DrawW->Value == true)
				Renderer::AddCircle(ObjectManager::Player->Position, 900.f, 1, IM_COL32(0, 0, 153, 255));

			std::string currentMode = ("Style: " + LucianConfig::LucianCombo::ComboMode->Items[LucianConfig::LucianCombo::ComboMode->Value]);
			Vector2 Position;
			RenderLayer::WorldToScreen(ObjectManager::Player->Position, Position);

			auto size = ImGui::CalcTextSize(currentMode.c_str());
			Position.Y = Position.Y + 50;
			Position.X = Position.X - (size.x / 2);
			Renderer::AddText(currentMode.c_str(), 15, Position, IM_COL32(255, 0, 255, 255));
		}

		GameObject* Lucian::GetTarget(float radius)
		{
			std::vector<GameObject*> heroes;
			auto hero_list = V21::GameObject::GetHeroes();
			for (size_t i = 0; i < hero_list->size; i++)
			{
				auto hero = hero_list->entities[i];

				if (hero != nullptr && hero->IsEnemy() && hero->IsValidTarget(radius)) {
					heroes.push_back(hero);
				}
			}
			return TargetSelector::GetTarget(heroes, DamageType_Physical);
		}

		void Lucian::OnGameUpdate()
		{
			if (Orbwalker::OrbwalkerEvading || !Orbwalker::CanCastAfterAttack())
				return;

			switch (ActiveMode)
			{
			case OrbwalkerMode_Combo:
				Combo();
				break;
			case OrbwalkerMode_LaneClear:
				Farm();
				break;
			case OrbwalkerMode_JungleClear:
				Farm();
				break;
			case OrbwalkerMode_LastHit:
				Lasthit();
				break;
			case OrbwalkerMode_Flee:
				ObjectManager::Player->CastSpellPos(SpellSlot_E, (DWORD)ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition);
				break;
			case OrbwalkerMode_Harass:
				break;
			}
		}

		bool Lucian::HasDoubleshotPassive()
		{
			return ObjectManager::Player->FindBuffName("LucianPassiveBuff");
		}

	}
}
