#include "stdafx.h"
#include "Evade.h"
#include "EvadeMath.h"
#include "ManagerTemplate.h"

namespace V21 {

	namespace Plugins {

		namespace Evade {
			Vector3 Evade::RenderPos1;
			Vector3 Evade::RenderPos2;
			int Evade::LastEvadeTick = 0;
			Vector2 Evade::EvadeWalkingPoint;
			float Evade::LastPolygonTick = 0;

			void Evade::Initialize()
			{
				EvadersDB = new EvaderDB;
				ConfigMenu::Initialize();

				EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
				EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
				EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
				EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
				EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
				EventManager::AddEventHandler(LeagueEvents::OnCreateMissile, OnCreateMissile);
				EventManager::AddEventHandler(LeagueEvents::OnDeleteMissile, OnDeleteMissile);
				EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);

				GameClient::PrintChat("Basic Evade Loaded~!", IM_COL32(255, 69, 255, 255));
			}

			void Evade::Dispose()
			{
				EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
				EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
				EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
				EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
				EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
				EventManager::RemoveEventHandler(LeagueEvents::OnCreateMissile, OnCreateMissile);
				EventManager::RemoveEventHandler(LeagueEvents::OnDeleteMissile, OnDeleteMissile);
				EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
			}

			bool Evade::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position) {
				if (unit == ObjectManager::Player && Config::EvadeSkillMenu::EvadeWithWalking->Value)
				{
					if (EvadeWalkingPoint != Vector2(0, 0))
					{
						if (!ObjectManager::Player->IsMagicImmune()) {
							//GameClient::PrintChat("movement blocked! #1", IM_COL32(255, 69, 255, 0));
							Orbwalker::DisableNextAttack = true;
							Orbwalker::OrbwalkerEvading = true;
							return false;
						}
					}
				}
				if (unit == ObjectManager::Player && order == GameObjectOrder::MoveTo && ShouldBlock(DetectedSkillShots, ObjectManager::Player, HudManager::Instance->CursorTargetLogic->CursorPosition, Config::EvadeSkillMenu::EvadeWithWalkingDanger->Value))
				{
					if (!ObjectManager::Player->IsMagicImmune()) {
						//GameClient::PrintChat("movement blocked! #2", IM_COL32(255, 69, 255, 0));
						Orbwalker::DisableNextAttack = true;
						Orbwalker::OrbwalkerEvading = true;
						return false;
					}
					Orbwalker::DisableNextAttack = false;
					Orbwalker::OrbwalkerEvading = false;
				}

				return  true;
			}

			void Evade::OnCreateObject(GameObject* unit)
			{
				if (unit == nullptr)
					return;

				for (auto skillshot_data : SpellDb::spells) {
					if (skillshot_data->object_name == unit->Name)
					{
						GetDetectedSSOnCreateObject(skillshot_data, unit);
					}
				}
			}

			void Evade::OnDeleteObject(GameObject* unit) {

				if (unit == nullptr)
					return;

				std::vector<SkillshotData*>::iterator data = std::find_if(SpellDb::spells.begin(), SpellDb::spells.end(), [&](SkillshotData* i)
					{
						return i->object_name == unit->Name;
					});

				if (data != SpellDb::spells.end())
				{
					DetectedSkillShots.RemoveAll(
						[&](DetectedSKillShot i)
						{
							return i.MissileNWID == unit->Id;
						});
				}
			}

			void Evade::OnCreateMissile(GameObject* unit)
			{
				if (unit == nullptr)
					return;

				auto caster = ObjectManager::Instance->ObjectsArray[unit->SourceID];
				//if (caster == nullptr || !caster->Hero() || !caster->IsEnemy() || unit->MissileIsBasicAttack)
				if (unit->MissileIsBasicAttack)
					return;

				for (auto skillshot_data : SpellDb::spells) {
					if (skillshot_data->champion_name == caster->BaseCharacterData->SkinName) {

						//if (skillshot_data->missile_name == unit->Name || skillshot_data->missile_name == unit->MissileSpellInfo->Resource->SpellName || skillshot_data->missile_name == unit->MissileSpellInfo->Name || skillshot_data->missile_name == unit->MissileSpellInfo->Resource->MissileName)
						if (skillshot_data->missile_name == unit->MissileSpellInfo->Resource->MissileName
							|| std::find(skillshot_data->extra_spell_names.begin(), skillshot_data->extra_spell_names.end(), unit->MissileSpellInfo->Resource->SpellName) != skillshot_data->extra_spell_names.end())
						{
							GetDetectedSSOnCreateMissile(skillshot_data, unit);
						}
					}
				}
			}

			void Evade::OnDeleteMissile(GameObject* unit)
			{
				if (unit == nullptr)
					return;

				//auto caster = ObjectManager::Instance->ObjectsArray[unit->SourceID];
				//if (caster == nullptr || !caster->Hero())
				//	return;
				//

				std::vector<SkillshotData*>::iterator data = std::find_if(SpellDb::spells.begin(), SpellDb::spells.end(), [&](SkillshotData* i)
					{
						return i->missile_name == unit->MissileSpellInfo->Resource->MissileName
							|| std::find(i->extra_spell_names.begin(), i->extra_spell_names.end(), unit->MissileSpellInfo->Resource->SpellName) != i->extra_spell_names.end();
					});

				if (data != SpellDb::spells.end())
				{
					DetectedSkillShots.RemoveAll(
						[&](DetectedSKillShot i)
						{
							return i.MissileNWID == unit->Id;
						});
				}
			}

			void Evade::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
			{
				if (!castInfo)
					return;

				auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];
				if (caster != nullptr && caster->Hero() && caster != ObjectManager::Player && caster->IsEnemy()) {
					if (!castInfo->IsAutoAttack()) {
						for (auto skillshot_data : SpellDb::spells) {
							if (skillshot_data->champion_name == caster->BaseCharacterData->SkinName) {
								if (skillshot_data->spell_name == spellData->GetSpellName() || skillshot_data->spell_name == spellData->SpellName
									|| std::find(skillshot_data->extra_spell_names.begin(), skillshot_data->extra_spell_names.end(), spellData->GetSpellName()) != skillshot_data->extra_spell_names.end()
									|| std::find(skillshot_data->extra_spell_names.begin(), skillshot_data->extra_spell_names.end(), spellData->SpellName) != skillshot_data->extra_spell_names.end())
								{
									GetDetectedSSOnCast(skillshot_data, castInfo, caster, Config::Misc::BoundingRadius->Value);
								}
							}
						}
					}
				}
			}

			void Evade::OnGameUpdate()
			{
				// destroy process

				DetectedSkillShots.RemoveAll(
					[&](DetectedSKillShot i)
					{
						//GameClient::PrintChat(i.Data->missile_names == (std::vector<string>)'L' ? "yes " : "no", IM_COL32(255, 69, 0, 255));

						if (i.Data->missile_name == "")
						{
							//GameClient::PrintChat("remove 1", IM_COL32(255, 69, 0, 255));
							float distance = Distance(i.Start, i.End);
							return !i.IsMissile && ClockFacade::GameTickCount() - i.DetectionTime >= i.Data->delay + i.Data->extra_duration
								+ distance * 1000 / i.Data->missile_speed;
						}
						else
						{
							//GameClient::PrintChat("remove 2", IM_COL32(255, 69, 0, 255));
							return  !i.IsMissile && ClockFacade::GameTickCount() - i.DetectionTime >= i.Data->delay + i.Data->extra_duration;// +(2 * Config::Misc::EvadeBuffer->Value);;
						}
					});

				// 
				if (ClockFacade::GameTickCount() - LastPolygonTick >= 100)
				{
					LastPolygonTick = ClockFacade::GameTickCount();
					for (int i = 0; i < DetectedSkillShots.ToVector().size(); ++i)
					{
						DetectedSkillShots.elems[i].Polygon = GetPolygon(DetectedSkillShots.elems[i], DetectedSkillShots.elems[i].Data->dont_add_bounding_radius, Config::Misc::BoundingRadius->Value);
					}
				}

				//initialize evading point 
				EvadeWalkingPoint = GetEvadePosition(DetectedSkillShots, ObjectManager::Player, Config::EvadeSkillMenu::EvadeWithWalkingDanger->Value);

				// evadeing
				if (Config::EvadeSkillMenu::EvadeWithWalking->Value /*&& GGame->TickCount() >= LastEvadeTick + 150*/)
				{
					if (EvadeWalkingPoint != Vector2(0, 0))
					{
						if (ClockFacade::GameTickCount() - LastEvadeTick >= 1 + Config::Misc::EvadeDelay->Value)
						{
							//ObjectManager::Player->EvadeIssueOrder(GameObjectOrder::MoveTo, &ToVec3(EvadeWalkingPoint));
							ObjectManager::Player->EvadeIssueOrder(GameObjectOrder::MoveTo, &Extend(ObjectManager::Player->Position, ToVec3(EvadeWalkingPoint), Config::Misc::ExtraRange->Value));
							LastEvadeTick = ClockFacade::GameTickCount();
						}

						if (ShouldHoldOn(DetectedSkillShots, ObjectManager::Player, Config::EvadeSkillMenu::EvadeWithWalkingDanger->Value))
						{
							if (ClockFacade::GameTickCount() - LastEvadeTick >= 1 + Config::Misc::EvadeDelay->Value)
							{
								//ObjectManager::Player->EvadeIssueOrderHoldPos(GameObjectOrder::HoldPosition, &ObjectManager::Player->Position);
								ObjectManager::Player->EvadeIssueOrder(GameObjectOrder::MoveTo, &Extend(ObjectManager::Player->Position, HudManager::Instance->CursorTargetLogic->CursorPosition.Perpendicular(), Config::Misc::ExtraRange->Value));
								LastEvadeTick = ClockFacade::GameTickCount();
							}
						}
					}


				}
				// HOURGLASS
				if (Config::EvadeSkillMenu::EvadeWithHourglass->Value)
				{
					SArray<DetectedSKillShot> detected = DetectedSkillShots.Where([&](DetectedSKillShot i) {return GetDodgeStage(i, Config::EvadeSkillMenu::EvadeWithHourglassDanger->Value); });
					for (DetectedSKillShot skillshot : detected.elems)
					{
						if (!IsAbleWalkEvade(EvadeWalkingPoint, skillshot, ObjectManager::Player) || !Config::EvadeSkillMenu::EvadeWithWalking->Value)
						{
							if (IsGettingHit(NetClient::Instance->GetPing() + 100, skillshot, ObjectManager::Player))
							{
								if (ObjectManager::Player->FindItem(Item::Zhonyas_Hourglass) && ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->FindItemSpellSLot(Item::Zhonyas_Hourglass)) == SpellState_Ready) {
									ObjectManager::Player->CastSpell(ObjectManager::Player->FindItemSpellSLot(Item::Zhonyas_Hourglass), (DWORD)ObjectManager::Player);
								}
								if (ObjectManager::Player->FindItem(Item::Stopwatch) && ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->FindItemSpellSLot(Item::Stopwatch)) == SpellState_Ready) {
									ObjectManager::Player->CastSpell(ObjectManager::Player->FindItemSpellSLot(Item::Stopwatch), (DWORD)ObjectManager::Player);
								}
							}
						}
					}
				}
				//FLASH 
				if (Config::EvadeSkillMenu::EvadeWithFlash->Value)
				{
					SArray<DetectedSKillShot> detected = DetectedSkillShots.Where([&](DetectedSKillShot i) {return GetDodgeStage(i, Config::EvadeSkillMenu::EvadeWithFLashDanger->Value); });
					for (DetectedSKillShot skillshot : detected.elems)
					{
						if (!IsAbleWalkEvade(EvadeWalkingPoint, skillshot, ObjectManager::Player) || !Config::EvadeSkillMenu::EvadeWithWalking->Value)
						{
							if (IsGettingHit(NetClient::Instance->GetPing() + 100, skillshot, ObjectManager::Player) && ObjectManager::Player->Spellbook.GetSpellState(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerFlash)) == SpellState_Ready)
							{
								Vector2 EvadePoint = GetEvadePosition(SArray<DetectedSKillShot>().Add(skillshot), ObjectManager::Player, Config::EvadeSkillMenu::EvadeWithFLashDanger->Value);
								Vector2 Point = Extend(ObjectManager::Player->Position.To2D(), EvadePoint, 424);
								ObjectManager::Player->CastSpellPos(ObjectManager::Player->Spellbook.FindSummonerSpellSlotFromEnum(ESummonerSpell::ESummonerSpell::SummonerFlash), (DWORD)ObjectManager::Player, ToVec3(Point));
							}
						}
					}
				}
				// EVADER
				for (Evader* evader : EvadersDB->EvadeSpells)
				{
					for (DetectedSKillShot skillshot : DetectedSkillShots.elems)
					{
						if (GetEvaderStage(evader, skillshot) && EvadeWalkingPoint != Vector2(0, 0))
						{
							if (!IsAbleWalkEvade(Extend(EvadeWalkingPoint, ObjectManager::Player->Position.To2D(), 50), skillshot, ObjectManager::Player) || !Config::EvadeSkillMenu::EvadeWithWalking->Value)
							{
								if (IsGettingHit(evader->Delay + NetClient::Instance->GetPing(), skillshot, ObjectManager::Player))
								{
									TriggerEvader(evader, skillshot);
								}
							}
						}
					}
				}
			}
			void Evade::OnDraw()
			{

				// draw evade point
				if (Config::EvadeMenu::EvadeDrawEvadePoint && EvadeWalkingPoint != Vector2(0, 0)) {
					Draw.Line(ObjectManager::Player->Position, Extend(ObjectManager::Player->Position, ToVec3(EvadeWalkingPoint), Config::Misc::ExtraRange->Value), 3, IM_COL32(255, 69, 255, 255));
					Draw.DrawCircle3D(Extend(ObjectManager::Player->Position, ToVec3(EvadeWalkingPoint), Config::Misc::ExtraRange->Value), 50, 50, IM_COL32(255, 69, 255, 255));

					Draw.Line(ObjectManager::Player->Position, Extend(ObjectManager::Player->Position, HudManager::Instance->CursorTargetLogic->CursorPosition.Perpendicular(), Config::Misc::ExtraRange->Value), 3, IM_COL32(255, 69, 255, 255));
					Draw.DrawCircle3D(Extend(ObjectManager::Player->Position, ToVec3(EvadeWalkingPoint), Config::Misc::ExtraRange->Value), 50, 50, IM_COL32(255, 69, 255, 255));
				}

				// draw skill shot
				for (DetectedSKillShot skillshot : DetectedSkillShots.elems)
				{
					if (EvadeSkillShotOptions.find(skillshot.Data->MenuName()) == EvadeSkillShotOptions.end())
						continue;
					if (EvadeSkillShotOptions[skillshot.Data->MenuName()].Draw->Value)
					{
						DWORD Color;
						if (GetDodgeStage(skillshot, Config::EvadeSkillMenu::EvadeWithWalking->Value))
						{
							Color = IM_COL32(255, 255, 255, 255);
						}
						else
						{
							Color = IM_COL32(255, 69, 0, 255);
						}
						skillshot.Polygon.Draw(Color);
					}
				}
				// draw status
				if (Config::EvadeMenu::EvadeDrawStatus->Value)
				{
					Vector2 Position;
					RenderLayer::WorldToScreen(ObjectManager::Player->Position, Position);
					Position.X = Position.X - 25;
					Position.Y = Position.Y + 25;
					DWORD Color =
						Config::EvadeMenu::EvadeEnable->Value ? IM_COL32(255, 255, 255, 255) : IM_COL32(255, 69, 0, 255);
					string Text = Config::EvadeMenu::EvadeToogleKey->Value ? "Evade : ON" : "Evade : OFF";
					Config::EvadeMenu::EvadeEnable->Value = Config::EvadeMenu::EvadeToogleKey->Value;
					Renderer::AddText(Text.c_str(), 15, Position, Color);
				}
			}
		}
	}
}
