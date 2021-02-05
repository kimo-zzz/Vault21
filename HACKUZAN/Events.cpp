#include "stdafx.h"
#include "Events.h"
#include "MenuSettings.h"
#include "EventManager.h"
#include "Instances.h"
#include "Menu.h"
#include "HealthPrediction.h"
#include "TargetSelector.h"
#include "HashFunctions.h"
#include "Orbwalker.h"
#include "Plugins.h"
#include "HudManager.h"
#include "Renderlayer.h"
#include "Renderer.h"
#include "ClockFacade.h"
#include "ManagerTemplate.h"
#include "ObjectManager.h"
#include "Draw.h"
#include "NavGrid.h"

namespace HACKUZAN {

	CConsole Console;

	namespace SDK {

		void Events::Initialize() {
			GameStart::Initialize();
		}

		namespace Events {
			namespace GameStart {
				void Initialize() {
					EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
				}

				void OnGameUpdate() {
					if (*(DWORD*)DEFINE_RVA(Offsets::ObjectManager::Player) && *(float*)(baseAddr + (DWORD)Offsets::ClockFacade::GameTime) > 0) {

						Instances::Initialize();

						//Objects::Initialize();
					//	GameTick::Initialize();
						//SpellCast::Initialize();
					//	NewPath::Initialize();

						Menu::Initialize();
					//	HealthPrediction::Initialize();
					//	TargetSelector::Initialize();
					//	Orbwalker::Initialize();
					//	Plugins::Initialize();

						Globals::InGame = true;
						EventManager::Trigger(LeagueEvents::OnGameStart);
						EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
					}
				}
			}

			namespace Objects {
				GameObject* CachedGameObjects[10000];
				std::unordered_map<GameObject*, int> mActiveMissileMap;

				void Initialize() {

					mActiveMissileMap = std::unordered_map<GameObject*, int>();

					//auto minion_list = HACKUZAN::GameObject::GetMinions();
					//for (size_t i = 0; i < minion_list->size; i++)
					//{
					/*auto aibase_list = HACKUZAN::GameObject::GetAIBases();
					for (size_t i = 0; i < aibase_list->size; i++)
					{
						auto gameObject = aibase_list->entities[i];
						CachedGameObjects[i] = gameObject;
						if (gameObject) {
							EventManager::Trigger(LeagueEvents::OnCreateObject, gameObject);
						}
					}

					auto missile_list = HACKUZAN::GameObject::GetMissiles();
					for (size_t i = 0; i < missile_list->size; i++)
					{
						auto gameObject = missile_list->entities[i];

						CachedGameObjects[i] = gameObject;
						EventManager::Trigger(LeagueEvents::OnCreateMissile, gameObject);
					}*/

					EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
				}

				void AddMissile(GameObject* missile)
				{
					std::pair<GameObject*, int> newMissile(missile, 1000 * (missile->MissileStartTime));
					mActiveMissileMap.insert(newMissile);
				}

				void OnGameUpdate() {

					//auto minion_list = HACKUZAN::GameObject::GetMinions();
					//for (size_t i = 0; i < minion_list->size; i++)
					//{
					/*auto aibase_list = HACKUZAN::GameObject::GetAIBases();
					for (size_t i = 0; i < aibase_list->size; i++)
					{
						auto gameObject = aibase_list->entities[i];
						auto& cachedGameObject = CachedGameObjects[i];

						if (!cachedGameObject && gameObject) {
							cachedGameObject = gameObject;
							EventManager::Trigger(LeagueEvents::OnCreateObject, gameObject);
						}
						else if (cachedGameObject && gameObject != cachedGameObject) {
							EventManager::Trigger(LeagueEvents::OnDeleteObject, cachedGameObject);
							cachedGameObject = gameObject;
							if (gameObject) {
								EventManager::Trigger(LeagueEvents::OnCreateObject, gameObject);
							}
						}
					}*/

					/*auto missile_list = HACKUZAN::GameObject::GetMissiles();
					for (size_t i = 0; i < missile_list->size; i++)
					{
						auto gameObject = missile_list->entities[i];
						auto& cachedGameObject = CachedGameObjects[i];

						if (!cachedGameObject && gameObject) {
							cachedGameObject = gameObject;

							Draw.DrawCircle3D(gameObject->Position, 50, 50, IM_COL32(255, 255, 69, 69));

							EventManager::Trigger(LeagueEvents::OnCreateMissile, gameObject);
						}
						else if (cachedGameObject && gameObject != cachedGameObject) {
							EventManager::Trigger(LeagueEvents::OnDeleteMissile, cachedGameObject);
							cachedGameObject = gameObject;
							if (gameObject) {
								EventManager::Trigger(LeagueEvents::OnCreateMissile, gameObject);
							}
						}
					}*/
				}
			}

			namespace GameTick {
				float LastTick = 0.0f;
				float LastMove = 0.0f;

				void Initialize() {
					EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
				}

				void OnGameUpdate() {
					auto time = ClockFacade::GetGameTime();

					if (time - LastTick > 1.0f / MenuSettings::TicksPerSecond) {
						EventManager::Trigger(LeagueEvents::OnGameTick);
						LastTick = time;

						if (MenuSettings::AntiAFK) {
							if (ObjectManager::Player->GetPathController()->HasNavigationPath) {
								LastMove = time;
							}
							else {
								if (time - LastMove > 160) {
									ObjectManager::Player->IssueOrder(GameObjectOrder::Stop, &ObjectManager::Player->Position);
									LastMove = time;
								}
							}
						}
					}

					if (MenuSettings::DeveloperMode) {

						/*auto unknown_list = HACKUZAN::GameObject::GetUnknownList();
						for (size_t i = 0; i < unknown_list->size; i++)
						{
							auto gameObject = unknown_list->entities[i];
							if (gameObject) {

								Vector2 pos2D;
								if (!RenderLayer::WorldToScreen(gameObject->Position, pos2D))
									continue;
								Renderer::AddText(10, pos2D + Vector2(0, i), 0xFFFFA500, gameObject->Name.c_str());
								Draw.DrawCircle3D(gameObject->Position, 50, 50, IM_COL32(255, 255, 69, 69));
							}
						}*/

						auto cursorPos = HudManager::Instance->CursorTargetLogic->CursorPosition;
						Renderer::AddText(10, HudManager::CursorPos2D + Vector2(40, 0), 0xFFFFA500, "Screen Position: X:%d Y:%d", (int)HudManager::CursorPos2D.X, (int)HudManager::CursorPos2D.Y);
						Renderer::AddText(10, HudManager::CursorPos2D + Vector2(40, 20), 0xFFFFA500, "Game Position: X:%f Y:%f Z:%f", cursorPos.X, cursorPos.Y, cursorPos.Z);
						//auto navMeshCell = NavigationMesh::Instance->WorldToCell(cursorPos.X, cursorPos.Z);
						//Renderer::AddText(10, HudManager::CursorPos2D + Vector2(40, 40), 0xFFFFA500, "NavigationMesh Position: X:%f Y:%f", navMeshCell.X, navMeshCell.Y);
						Renderer::AddText(10, HudManager::CursorPos2D + Vector2(40, 60), 0xFFFFDEAD, "Collision flags: %d", NavGrid::Instance->GetCell(&cursorPos)->m_Flags);

						auto aibase_list = HACKUZAN::GameObject::GetAIBases();
						for (size_t i = 0; i < aibase_list->size; i++)
						{
							auto gameObject = aibase_list->entities[i];

							if (!gameObject || gameObject->Position.Distance(cursorPos) > 500.0f)
								continue;

							if (!gameObject->Alive())
								continue;

							Vector2 pos2D;
							if (!RenderLayer::WorldToScreen(gameObject->Position, pos2D))
								continue;

							if (gameObject->Hero() || gameObject->Minion()) {
								Renderer::AddText(10, pos2D + Vector2(0, i), 0xFFFFA500, "Buffs");
								i += 20;
								auto k = 0;
								for (auto buffManager : gameObject->Buffs) {
									auto buff = buffManager.Buff;
									if (buff->IsActive()) {
										Renderer::AddText(10, pos2D + Vector2(0, i), 0xFFFFDEAD, "[%d] CasterId : %hu | BuffName : %s (0x%X) | BuffType : %hu | BuffCount : %d | BuffCounter : %d | BuffRemainingTime : %s", k, buff->GetScriptInstance()->CasterId, buff->Script->Name, buff->Script->Hash, buff->Type, buff->GetCount(), buff->Counter, buff->ExpireTime - buff->StartTime > 20000 ? "Infinite" : std::to_string(buff->ExpireTime - ClockFacade::GetGameTime()).c_str());
										i += 20;
									}
									k++;
								}
							}
			
							if (gameObject->Hero()) {

								Renderer::AddText(10, pos2D + Vector2(0, i), 0xFFFFA500, "Items");
								i += 20;
								for (auto k = 0; k < 7; k++) {
									auto inventorySlot = gameObject->HeroInventory.Slots[k];
									if (inventorySlot->ItemInfo) {
										Renderer::AddText(10, pos2D + Vector2(0, i), 0xFFFFDEAD, "[%d] ItemId : %d | DisplayName : %s | Price : %d | Ammo : %d | Stacks : %d | MaxStacks : %d", k, inventorySlot->ItemInfo->ItemData->ItemId, HashFunctions::TranslateString(inventorySlot->ItemInfo->ItemData->DisplayNameLocalizationKey), inventorySlot->ItemInfo->ItemData->Price, (int)inventorySlot->ItemInfo->Ammo, (int)inventorySlot->Stacks, inventorySlot->ItemInfo->ItemData->MaxStacks);
										i += 20;
									}
								}
							}
						}
					}
				}
			}

			namespace SpellCast {
				SpellInfo* CachedSpells[10000];
				float CachedSpellCastEndTime[10000];
				kSpellState CachedSpellStates[SpellSlot_Trinket];

				void Initialize() {

					for (auto slot = SpellSlot_Q; slot < SpellSlot_Item6; slot = (kSpellSlot)(slot + 1)) {
						CachedSpellStates[slot] = ObjectManager::Player->Spellbook.GetSpellState(slot);
					}

					EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
				}

				void OnGameUpdate() {

					for (auto slot = SpellSlot_Q; slot < SpellSlot_Item6; slot = (kSpellSlot)(slot + 1)) {
						auto& cachedSpellState = CachedSpellStates[slot];
						auto spellState = ObjectManager::Player->Spellbook.GetSpellState(slot);

						if (cachedSpellState != spellState) {
							if (!(cachedSpellState & SpellState_Supressed) && !(cachedSpellState & SpellState_Cooldown)) {
								if (spellState & SpellState_Supressed || spellState & SpellState_Cooldown) {
									EventManager::Trigger(LeagueEvents::OnSpellCast, slot);
								}
							}
							cachedSpellState = spellState;
						}
					}
				}
			}

			namespace NewPath {
				Vector3 CachedEndPositions[10000];

				void Initialize() {

					auto aibase_list = HACKUZAN::GameObject::GetAIBases();
					for (size_t i = 0; i < aibase_list->size; i++)
					{
						auto unit = aibase_list->entities[i];
						if (unit->Hero() || unit->Minion()) {
							 CachedEndPositions[unit->Id] = unit->GetPathController()->GetNavigationPath()->EndPosition;
						}
					}

					EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
				}

				
				void OnGameUpdate() {

					auto aibase_list = HACKUZAN::GameObject::GetAIBases();
					for (size_t i = 0; i < aibase_list->size; i++)
					{
						auto unit = aibase_list->entities[i];
						if (unit->Hero() || unit->Minion()) {
							auto navigationPath = unit->GetPathController()->GetNavigationPath();
							auto endPosition = navigationPath->EndPosition;
							auto& cachedEndPosition = CachedEndPositions[unit->Id];
							if (endPosition != cachedEndPosition) {
								cachedEndPosition = endPosition;
								EventManager::Trigger(LeagueEvents::OnNewPath, unit, navigationPath);
							}
						}
					}
				}
			}
		}
	}
}