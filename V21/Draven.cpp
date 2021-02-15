#include "stdafx.h"
#include "Draven.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"

namespace V21 {
	namespace Plugins {

		DelayAction* Delay = nullptr;

		using namespace V21::SDK;
		using namespace V21::SDK::Orbwalker;

		SArray<GameObject*> DravenAxesObject;
		SArray<GameObject*> DravenAxesMissileOne; 
		SArray<GameObject*> QReticles;

		class DravenantiGapCloser
		{
		public:
			Menu* ParentMenu;
			CheckBox* Enable;
		};

		std::map<string, DravenantiGapCloser> AntiGapCloserOptions;

		namespace DravenConfig {

			namespace DravenCombo {
				CheckBox* UseQ;
				CheckBox* UseW;
				Slider* WmaNa;
				CheckBox* UseE;
				Slider* EmaNa;
				CheckBox* UseR;
				Slider* enemiesInRange;
			}

			namespace DravenFarm {
				CheckBox* UseQ;
				CheckBox* UseE;
				Slider* QmaNa;
				Slider* EmaNa;
			}

			namespace DravenMisc {
				CheckBox* AutoE;
				CheckBox* AutoCatch;
				CheckBox* switchCatch;
				CheckBox* UseQforW;
				CheckBox* CatchUnderTurret;
				CheckBox* DrawAxe;
				Slider* DravenAxePickRange;
			}
		}

		void V21::Plugins::Draven::Initialize()
		{
			auto menu = Menu::CreateMenu("Draven", "Draven");

			auto combo = menu->AddMenu("Combo", "Combo Settings");
			DravenConfig::DravenCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
			DravenConfig::DravenCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
			DravenConfig::DravenCombo::WmaNa = combo->AddSlider("Wmana", "Minimum W mana", 10, 0, 100, 5);
			DravenConfig::DravenCombo::UseE = combo->AddCheckBox("Use E", "Use SpellSlot E", true);
			DravenConfig::DravenCombo::EmaNa = combo->AddSlider("EMana", "Minimum E mana", 50, 0, 100, 5);
			DravenConfig::DravenCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);

			auto farm = menu->AddMenu("farm", "Farm Settings");
			DravenConfig::DravenFarm::UseQ = farm->AddCheckBox("Use Q", "Use SpellSlot Q", true);
			DravenConfig::DravenFarm::QmaNa = farm->AddSlider("QMana", "Minimum Q mana", 50, 0, 100, 5);
			DravenConfig::DravenFarm::UseE = farm->AddCheckBox("Use E", "Use SpellSlot E", true);
			DravenConfig::DravenFarm::EmaNa = farm->AddSlider("EMana", "Minimum E mana", 50, 0, 100, 5);

			auto misc = menu->AddMenu("misc", "Misc");

			auto catchaxe = misc->AddMenu("catchaxe", "Axe Catch Settings");
			DravenConfig::DravenMisc::AutoCatch = catchaxe->AddCheckBox("AutoCatch", "Enable Auto Catch", true);
			DravenConfig::DravenMisc::switchCatch = catchaxe->AddCheckBox("switchCatch", "Position : True = HeroPos : False = MousePos", true);
			DravenConfig::DravenMisc::DravenAxePickRange = catchaxe->AddSlider("DravenAxePickRange", "Axe Pick Range", 700, 0, 1000, 10);
			DravenConfig::DravenMisc::UseQforW = catchaxe->AddCheckBox("UseQforW", "Use W to Catch", true);
			DravenConfig::DravenMisc::CatchUnderTurret = catchaxe->AddCheckBox("CatchUnderTurret", "Catch Under Turret", false);

			DravenConfig::DravenMisc::AutoE = misc->AddCheckBox("AutoE", "Enable Auto E", true);
			DravenConfig::DravenMisc::DrawAxe = misc->AddCheckBox("DrawAxe", "Enable Drawings", true);

			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			//EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);

			GameClient::PrintChat("Draven Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void Draven::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			//EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		void Draven::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3* position, GameObject* target) {
			if (unit == ObjectManager::Player)
			{
				if (Orbwalker::DisableNextMove && order == GameObjectOrder::MoveTo) {

					return;
				}
			}
			return;
		}

		void Draven::OnCreateObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;

			if (unit->Missile()) {

				if (unit->SourceID == ObjectManager::Player->Id)
				{
					auto data = unit->MissileSpellInfo->Resource->MissileName;
					if (Equals(data, "DravenSpinningReturn")) {
						DravenAxesObject.Add(unit);
					}
					if (Equals(data, "DravenSpinningAttack")) {
						DravenAxesMissileOne.Add(unit);
					}
				}
			}
			else
			{
				if (unit->TroyEnt()) {

					if (Equals(unit->Name, "Draven_Base_Q_reticle_self")) {
						QReticles.Add(unit);
						//GameClient::PrintChat(("LifeTimeTicks: " + to_string(unit->MissileStartTime)).c_str(), IM_COL32(255, 69, 0, 255));
					}
				}
			}
		}

		void Draven::OnDeleteObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;

			if (unit->Missile()) {

				if (unit->SourceID == ObjectManager::Player->Id)
				{
					auto data = unit->MissileSpellInfo->Resource->MissileName;
					if (Equals(data, "DravenSpinningReturn")) {
						DravenAxesObject.RemoveAll([&](GameObject* i) { return i->Id == unit->Id; });
					}
					if (Equals(data, "DravenSpinningAttack")) {
						DravenAxesMissileOne.RemoveAll([&](GameObject* i) { return i->Id == unit->Id; });
					}
				}
			}
			else
			{
				if (unit->TroyEnt()) {

					if (Equals(unit->Name, "Draven_Base_Q_reticle_self")) {
						QReticles.RemoveAll([&](GameObject* i) { return i->Id == unit->Id; });
					}
				}
			}
		}

		void Draven::OnDraw()
		{
			if (DravenConfig::DravenMisc::DrawAxe->Value)
			{
				auto catchPos = (DravenConfig::DravenMisc::switchCatch->Value ? ObjectManager::Player->Position : HudManager::Instance->CursorTargetLogic->CursorPosition);
				Renderer::AddCircle(catchPos, DravenConfig::DravenMisc::DravenAxePickRange->Value, 2.0f, IM_COL32(255, 0, 255, 155));

				auto bestReticle = QReticles.Where([&](GameObject* i) { return i->Position.Distance(catchPos) <= DravenConfig::DravenMisc::DravenAxePickRange->Value; }).
					//OrderBy<float>([&](GameObject* i) {return i->Position.Distance(ObjectManager::Player->Position); }).
					//OrderBy<float>([&](GameObject* i) {return i->Position.Distance(HudManager::Instance->CursorTargetLogic->CursorPosition); }).
					OrderBy<float>([&](GameObject* i) { return ClockFacade::GameTickCount() + 1800; }).FirstOrDefault();

				if (bestReticle != nullptr) {
					Renderer::AddCircle(bestReticle->Position, 150, 2.0f, Distance(bestReticle->Position, catchPos) < DravenConfig::DravenMisc::DravenAxePickRange->Value ? IM_COL32(255, 0, 255, 255) : IM_COL32(255, 69, 255, 255));
				}
			}
		}

		void Draven::OnGameUpdate()
		{
			auto target = TargetSelector::GetTarget(TargetType::TSTARGET_HEROES, 1000.0f, kDamageType::DamageType_Physical);

			if (!Orbwalker::OrbwalkerEvading) {

				if (ActiveMode != OrbwalkerMode_None)
				{
					Draven::CatchAxe();
				}
			}

			if (ActiveMode & OrbwalkerMode_Combo) {

				if (target && Orbwalker::CanCastAfterAttack()) {

					if (DravenConfig::DravenCombo::UseW->Value)
					{
						if (ObjectManager::Player->Resource >= (DravenConfig::DravenCombo::WmaNa->Value / 100.0) * ObjectManager::Player->MaxResource) {
							if (!DravenHasWAttackSpeed() && ObjectManager::Player->CountEnemiesInRange(ObjectManager::Player->GetAutoAttackRange()) > 0)
							{
								ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_W, (DWORD)ObjectManager::Player);
							}
							if (!DravenHasWMoveSpeed() && ObjectManager::Player->CountEnemiesInRange(ObjectManager::Player->GetAutoAttackRange()) == 0
								&& ObjectManager::Player->CountEnemiesInRange(900) > 0)
							{
								ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_W, (DWORD)ObjectManager::Player);
							}

							if (!DravenHasWAttackSpeed() && ObjectManager::Player->CountEnemiesInRange(900) > 0 && ObjectManager::Player->FindBuffType(BuffType::Slow)) {
								ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_W, (DWORD)ObjectManager::Player);
							}
						}
					}

					if (DravenConfig::DravenCombo::UseE->Value)
					{
						if (ObjectManager::Player->Resource >= (DravenConfig::DravenCombo::EmaNa->Value / 100.0) * ObjectManager::Player->MaxResource) {
							if (target->IsMelee() && ObjectManager::Player->Position.Distance(target->Position) <= target->AttackRange + 50) {
								ObjectManager::Player->CastPredictSpell(kSpellSlot::SpellSlot_E, ObjectManager::Player->Position, target->Position);
							}
						}
					}
				}
			}

			if (ActiveMode != OrbwalkerMode_None) {

				if (Orbwalker::CanCastAfterAttack()) {

					if (ActiveMode != OrbwalkerMode_Combo) {
						if (Orbwalker::LaneClearMinion != nullptr) {
							if (ObjectManager::Player->Resource >= (DravenConfig::DravenFarm::QmaNa->Value / 100.0) * ObjectManager::Player->MaxResource) {
								if (DravenConfig::DravenFarm::UseQ->Value && DravenAxesCount() <= 1) {
									ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_Q, (DWORD)ObjectManager::Player);
								}
							}
						}
					}
					else
					{
						if (DravenConfig::DravenCombo::UseQ->Value && DravenAxesCount() < 2) {
							if (ObjectManager::Player->CountEnemiesInRange(900) > 0)
							{
								//GameClient::PrintChat(("Axe Count: " + to_string(DravenAxesCount())).c_str(), IM_COL32(255, 69, 0, 255));
								ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_Q, (DWORD)ObjectManager::Player);
							}
						}
					}
				}
			}
		}

		void Draven::CatchAxe()
		{
			if (!DravenConfig::DravenMisc::AutoCatch)
				return;

			auto catchPos = (DravenConfig::DravenMisc::switchCatch->Value ? ObjectManager::Player->Position : HudManager::Instance->CursorTargetLogic->CursorPosition);
			auto bestReticle = QReticles.Where([&](GameObject* i) { return i->Position.Distance(catchPos) <= DravenConfig::DravenMisc::DravenAxePickRange->Value; }).
				//OrderBy<float>([&](GameObject* i) {return i->Position.Distance(ObjectManager::Player->Position); }).
				//OrderBy<float>([&](GameObject* i) {return i->Position.Distance(HudManager::Instance->CursorTargetLogic->CursorPosition); }).
				OrderBy<float>([&](GameObject* i) { return ClockFacade::GameTickCount() + 1800; }).FirstOrDefault();

			if (Orbwalker::CanMove() && !Orbwalker::CanAttack(nullptr)) {
				if (bestReticle != nullptr && bestReticle->Position.Distance(ObjectManager::Player->Position) > 100)
				{
					auto eta = 1000 * (ObjectManager::Player->Position.Distance(bestReticle->Position) / ObjectManager::Player->MoveSpeed);
					auto expireTime = (ClockFacade::GameTickCount() + 1800);
					//GameClient::PrintChat(("eta: " + to_string(ClockFacade::GameTickCount() + eta)).c_str(), IM_COL32(255, 69, 0, 255));
					//GameClient::PrintChat(("expireTime: " + to_string(expireTime)).c_str(), IM_COL32(255, 69, 0, 255));
					if (eta >= expireTime && DravenConfig::DravenMisc::UseQforW->Value)
					{
						ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_W, (DWORD)ObjectManager::Player);
					}

					if (!DravenConfig::DravenMisc::CatchUnderTurret) // debug this?
					{
						// If we're under the turret as well as the axe, catch the axe
						if (ObjectManager::Player->IsUnderEnemyTurret(900) && bestReticle->IsUnderEnemyTurret(900))
						{
							if (ClockFacade::GameTickCount() - Orbwalker::LastMoveCommandT >= 50)
							{
								Orbwalker::DisableNextMove = false;
								ObjectManager::Player->OverrideIssueOrder(GameObjectOrder::MoveTo, &bestReticle->Position);
								Orbwalker::DisableNextMove = true;
								Orbwalker::LastMoveCommandT = ClockFacade::GameTickCount();
							}

						}
						else if (!bestReticle->IsUnderEnemyTurret(900))
						{
							if (ClockFacade::GameTickCount() - Orbwalker::LastMoveCommandT >= 50)
							{
								Orbwalker::DisableNextMove = false;
								ObjectManager::Player->OverrideIssueOrder(GameObjectOrder::MoveTo, &bestReticle->Position);
								Orbwalker::DisableNextMove = true;
								Orbwalker::LastMoveCommandT = ClockFacade::GameTickCount();
							}
						}
					}
					else
					{
						if (ClockFacade::GameTickCount() - Orbwalker::LastMoveCommandT >= 50)
						{
							Orbwalker::DisableNextMove = false;
							ObjectManager::Player->OverrideIssueOrder(GameObjectOrder::MoveTo, &bestReticle->Position);
							Orbwalker::DisableNextMove = true;
							Orbwalker::LastMoveCommandT = ClockFacade::GameTickCount();
						}
					}
				}
			}
		}

		inline int  Draven::DravenAxesOnHand()
		{
			return ObjectManager::Player->GetBuffCount("DravenSpinningAttack");
		}
		inline int  Draven::DravenAxesCount()
		{
			return  Draven::DravenAxesOnHand() + DravenAxesObject.Count() + DravenAxesMissileOne.Count() + QReticles.Count();
		}
		inline bool  Draven::DravenHasWMoveSpeed()
		{
			return  ObjectManager::Player->FindBuffName("DravenFury");
		}
		inline bool  Draven::DravenHasWAttackSpeed()
		{
			return  ObjectManager::Player->FindBuffName("dravenfurybuff");
		}
	}
}