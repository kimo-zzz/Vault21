#include "stdafx.h"
#include "Orbwalker.h"
#include "Menu.h"
#include "EventManager.h"
#include "Renderer.h"
#include "Draw.h"
#include "ObjectManager.h"
#include "ManagerTemplate.h"
#include "ClockFacade.h"
#include "HudManager.h"
#include "NetClient.h"
#include "TargetSelector.h"
#include "HealthPrediction.h"
#include "Evade.h"
#include "Utils.h"

namespace HACKUZAN {


	namespace SDK {

		DelayAction* Delay = nullptr;

		GameObject* Orbwalker::LastHitMinion = nullptr;
		GameObject* Orbwalker::AlmostLastHitMinion = nullptr;
		GameObject* Orbwalker::LaneClearMinion = nullptr;
		GameObjectOrder Orbwalker::LastOrder = GameObjectOrder::None;
		int Orbwalker::LastAttackCommandT;
		int Orbwalker::LastMoveCommandT;
		bool Orbwalker::_missileLaunched = false;
		bool Orbwalker::DisableNextAttack = false;
		bool Orbwalker::DisableNextMove = false;
		bool Orbwalker::OrbwalkerEvading = false;
		bool Orbwalker::OverrideOrbwalker = false;
		GameObject* Orbwalker::LastTarget = nullptr;
		Vector3 Orbwalker::LastMovePosition = Vector3(0, 0, 0);
		int Orbwalker::LastAATick;
		unsigned int Orbwalker::ActiveMode = OrbwalkerMode_None;
		bool Orbwalker::IsAzir = false;
		std::vector<GameObject*> Orbwalker::AzirSoldiers;
		bool Orbwalker::IsAshe = false;
		bool Orbwalker::IsGraves = false;
		bool Orbwalker::IsRengar = false;
		bool Orbwalker::AttackUsesAmmo = false;
		kSpellSlot Orbwalker::AttackResetSlot = SpellSlot_Unknown;
		unsigned int Orbwalker::AttackResetHash = 0;
		bool Orbwalker::IsDashAttackReset = false;
		GameObject* Orbwalker::ForcedTarget = nullptr;
		Vector3 Orbwalker::ForcedPosition = Vector3(0, 0, 0);

		namespace Orbwalker {
			namespace Config {
				namespace Hotkeys {
					KeyBind* Combo;
					KeyBind* Harass;
					KeyBind* LaneClear;
					KeyBind* JungleClear;
					KeyBind* LastHit;
					KeyBind* Flee;
				}

				namespace Configuration {
					CheckBox* MissileCheck;
					CheckBox* LaneClearHeroes;
					CheckBox* SupportMode;
					Slider* HoldRadius;
					Slider* MovementDelay;
					Slider* ExtraWindUpTime;
				}

				namespace Farming {
					CheckBox* LastHitPriority;
					CheckBox* PushPriority;
					Slider* ExtraFarmDelay;
				}

				namespace Melee {
					CheckBox* StickToTarget;
				}

				namespace Drawings {
					CheckBox* AttackRange;
					CheckBox* AzirSoldierAttackRange;
					CheckBox* EnemyAttackRange;
					CheckBox* HoldRadius;
					CheckBox* LasthittableMinions;
				}
			}
		}

		void Orbwalker::Initialize() {
			switch (ObjectManager::Player->BaseCharacterData->SkinHash) {
			case Character::Ashe:
				IsAshe = true;
				break;
			case Character::Azir:
				IsAzir = true;
				break;
			case Character::Blitzcrank:
				AttackResetSlot = SpellSlot_E;
				break;
			case Character::Camille:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Chogath:
				AttackResetSlot = SpellSlot_E;
				break;
			case Character::Darius:
				AttackResetSlot = SpellSlot_W;
				break;
			case Character::DrMundo:
				AttackResetSlot = SpellSlot_E;
				break;
			case Character::Elise:
				AttackResetSlot = SpellSlot_W;
				AttackResetHash = 0x211D8403; // "EliseSpiderW"
				break;
			case Character::Fiora:
				AttackResetSlot = SpellSlot_E;
				break;
			case Character::Fizz:
				AttackResetSlot = SpellSlot_W;
				break;
			case Character::Garen:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Graves:
				IsGraves = true;
				AttackUsesAmmo = true;
				AttackResetSlot = SpellSlot_E;
				IsDashAttackReset = true;
				break;
			case Character::Kassadin:
				AttackResetSlot = SpellSlot_W;
				break;
			case Character::Illaoi:
				AttackResetSlot = SpellSlot_W;
				break;
			case Character::Jax:
				AttackResetSlot = SpellSlot_W;
				break;
			case Character::Jayce:
				AttackResetSlot = SpellSlot_W;
				AttackResetHash = 0xE95D0E7; // "JayceHyperCharge"
				break;
			case Character::Jhin:
				AttackUsesAmmo = true;
				break;
			case Character::Leona:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Lucian:
				AttackResetSlot = SpellSlot_E;
				IsDashAttackReset = true;
				break;
			case Character::MasterYi:
				AttackResetSlot = SpellSlot_W;
				break;
			case Character::Mordekaiser:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Nasus:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Nautilus:
				AttackResetSlot = SpellSlot_W;
				break;
			case Character::Nidalee:
				AttackResetSlot = SpellSlot_Q;
				AttackResetHash = 0xCCD4C05E; // "Takedown"
				break;
			case Character::RekSai:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Rengar:
				IsRengar = true;
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Renekton:
				AttackResetSlot = SpellSlot_W;
				break;
			case Character::Riven:
				AttackResetSlot = SpellSlot_Q;
				IsDashAttackReset = true;
				break;
			case Character::Shyvana:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Sivir:
				AttackResetSlot = SpellSlot_W;
				break;
			case Character::Talon:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Trundle:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Vayne:
				AttackResetSlot = SpellSlot_Q;
				IsDashAttackReset = true;
				break;
			case Character::Vi:
				AttackResetSlot = SpellSlot_E;
				break;
			case Character::Volibear:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::MonkeyKing:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::XinZhao:
				AttackResetSlot = SpellSlot_Q;
				break;
			case Character::Yorick:
				AttackResetSlot = SpellSlot_Q;
				break;
			}

			auto menu = Menu::CreateMenu("Orbwalker", "Orbwalker");

			auto hoykeysMenu = menu->AddMenu("Hotkeys", "Hotkeys");
			Config::Hotkeys::Combo = hoykeysMenu->AddKeyBind("Combo", "Combo", ' ', false, false, [](KeyBind* self, bool newValue) {
				if (newValue) {
					ActiveMode |= OrbwalkerMode_Combo;
				}
				else {
					ActiveMode &= ~OrbwalkerMode_Combo;
				}
				});
			Config::Hotkeys::Harass = hoykeysMenu->AddKeyBind("Harass", "Harass", 'C', false, false, [](KeyBind* self, bool newValue) {
				if (newValue) {
					ActiveMode |= OrbwalkerMode_Harass;
				}
				else {
					ActiveMode &= ~OrbwalkerMode_Harass;
				}
				});
			Config::Hotkeys::LaneClear = hoykeysMenu->AddKeyBind("LaneClear", "LaneClear", 'V', false, false, [](KeyBind* self, bool newValue) {
				if (newValue) {
					ActiveMode |= OrbwalkerMode_LaneClear;
				}
				else {
					ActiveMode &= ~OrbwalkerMode_LaneClear;
				}
				});
			Config::Hotkeys::JungleClear = hoykeysMenu->AddKeyBind("JungleClear", "JungleClear", 'Y', false, false, [](KeyBind* self, bool newValue) {
				if (newValue) {
					ActiveMode |= OrbwalkerMode_JungleClear;
				}
				else {
					ActiveMode &= ~OrbwalkerMode_JungleClear;
				}
				});
			Config::Hotkeys::LastHit = hoykeysMenu->AddKeyBind("LastHit", "LastHit", 'X', false, false, [](KeyBind* self, bool newValue) {
				if (newValue) {
					ActiveMode |= OrbwalkerMode_LastHit;
				}
				else {
					ActiveMode &= ~OrbwalkerMode_LastHit;
				}
				});
			Config::Hotkeys::Flee = hoykeysMenu->AddKeyBind("Flee", "Flee", 'Z', false, false, [](KeyBind* self, bool newValue) {
				if (newValue) {
					ActiveMode |= OrbwalkerMode_Flee;
				}
				else {
					ActiveMode &= ~OrbwalkerMode_Flee;
				}
				});

			auto configurationMenu = menu->AddMenu("Configuration", "Configuration");
			Config::Configuration::LaneClearHeroes = configurationMenu->AddCheckBox("LaneClearHeroes", "Attack heroes in laneclear", true);
			Config::Configuration::LaneClearHeroes->AddTooltip("It will attack heroes when lane clearing");
			Config::Configuration::SupportMode = configurationMenu->AddCheckBox(("SupportMode" + ObjectManager::Player->SkinName).c_str(), "Support mode");
			Config::Configuration::HoldRadius = configurationMenu->AddSlider("HoldRadius", "Hold Radius", 100, 0, 100, 10);
			Config::Configuration::MovementDelay = configurationMenu->AddSlider("MovementDelay", "Movement delay", 50, 0, 500, 10);
			Config::Configuration::ExtraWindUpTime = configurationMenu->AddSlider("ExtraWindUpTime", "Extra Windup Time", 0, 0, 200, 10);
			Config::Configuration::MissileCheck = configurationMenu->AddCheckBox("MissileCheck", "Use Missile Check", true);

			auto farmingMenu = menu->AddMenu("Farming", "Farming");
			Config::Farming::LastHitPriority = farmingMenu->AddCheckBox("LastHitPriority", "Prioritize lasthit over harass", true);
			Config::Farming::PushPriority = farmingMenu->AddCheckBox("PushPriority", "Prioritize push over freeze", true);
			Config::Farming::ExtraFarmDelay = farmingMenu->AddSlider("ExtraFarmDelay", "Extra farm delay", 70, -80, 80, 10);

			auto meleeMenu = menu->AddMenu("Melee", "Melee");
			Config::Melee::StickToTarget = meleeMenu->AddCheckBox("StickToTarget", "Stick to target", false);

			auto drawingsMenu = menu->AddMenu("Drawings", "Drawings");
			Config::Drawings::AttackRange = drawingsMenu->AddCheckBox("AttackRange", "Attack range", true);
			if (IsAzir) {
				Config::Drawings::AzirSoldierAttackRange = drawingsMenu->AddCheckBox("AzirSoldierAttackRange", "Azir soldier attack range", true);
			}
			Config::Drawings::EnemyAttackRange = drawingsMenu->AddCheckBox("EnemyAttackRange", "Enemy attack range", true);
			Config::Drawings::HoldRadius = drawingsMenu->AddCheckBox("HoldRadius", "Hold radius", true);
			Config::Drawings::LasthittableMinions = drawingsMenu->AddCheckBox("LasthittableMinions", "Lasthittable minions", true);

			Delay = new DelayAction;

			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnSpellCast, OnSpellCast);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnDoCast, OnDoCast);
			EventManager::AddEventHandler(LeagueEvents::OnDoCastDelayed, OnDoCastDelayed);
			EventManager::AddEventHandler(LeagueEvents::OnStopCast, OnStopCast);
			EventManager::AddEventHandler(LeagueEvents::OnNewPath, OnNewPath);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);

			GameClient::PrintChat("Orbwalker Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void Orbwalker::Dispose() {
			EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::RemoveEventHandler(LeagueEvents::OnSpellCast, OnSpellCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnDoCast, OnDoCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnDoCastDelayed, OnDoCastDelayed);
			EventManager::RemoveEventHandler(LeagueEvents::OnStopCast, OnStopCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnNewPath, OnNewPath);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
		}

		std::vector<string> NoCancelChamps =
		{
			"Kalista"
		};

		std::vector<string> AttackResets =
		{
			"dariusnoxiantacticsonh", "fiorae", "garenq", "gravesmove",
			"hecarimrapidslash", "jaxempowertwo", "jaycehypercharge",
			"leonashieldofdaybreak", "luciane", "monkeykingdoubleattack",
			"mordekaisermaceofspades", "nasusq", "nautiluspiercinggaze",
			"netherblade", "gangplankqwrapper", "powerfist",
			"renektonpreexecute", "rengarq", "shyvanadoubleattack",
			"sivirw", "takedown", "talonnoxiandiplomacy",
			"trundletrollsmash", "vaynetumble", "vie", "volibearq",
			"xenzhaocombotarget", "yorickspectral", "reksaiq",
			"itemtitanichydracleave", "masochism", "illaoiw",
			"elisespiderw", "fiorae", "meditate", "sejuaninorthernwinds",
			"asheq", "camilleq", "camilleq2"
		};


		std::vector<string> Attacks =
		{
			"caitlynheadshotmissile", "frostarrow", "garenslash2",
			"kennenmegaproc", "masteryidoublestrike", "quinnwenhanced",
			"renektonexecute", "renektonsuperexecute",
			"rengarnewpassivebuffdash", "trundleq", "xenzhaothrust",
			"xenzhaothrust2", "xenzhaothrust3", "viktorqbuff",
			"lucianpassiveshot"
		};

		std::vector<string> NoAttacks =
		{
			"volleyattack", "volleyattackwithsound",
			"jarvanivcataclysmattack", "monkeykingdoubleattack",
			"shyvanadoubleattack", "shyvanadoubleattackdragon",
			"zyragraspingplantattack", "zyragraspingplantattack2",
			"zyragraspingplantattackfire", "zyragraspingplantattack2fire",
			"viktorpowertransfer", "sivirwattackbounce", "asheqattacknoonhit",
			"elisespiderlingbasicattack", "heimertyellowbasicattack",
			"heimertyellowbasicattack2", "heimertbluebasicattack",
			"annietibbersbasicattack", "annietibbersbasicattack2",
			"yorickdecayedghoulbasicattack", "yorickravenousghoulbasicattack",
			"yorickspectralghoulbasicattack", "malzaharvoidlingbasicattack",
			"malzaharvoidlingbasicattack2", "malzaharvoidlingbasicattack3",
			"kindredwolfbasicattack", "gravesautoattackrecoil"
		};

		bool Orbwalker::IsAutoAttack(string name) {

			for (auto noattacks : NoAttacks) {
				if (strcmp("attack", name.c_str()) && !GameClient::StringEquals(noattacks.c_str(), name.c_str(), TRUE)) {
					return true;
				}
			}
			for (auto attacks : Attacks) {
				if (GameClient::StringEquals(attacks.c_str(), name.c_str(), TRUE)) {
					return true;
				}
			}
			return false;
		}

		bool Orbwalker::IsAutoAttackReset(string name) {

			for (auto attacks : AttackResets) {
				if (GameClient::StringEquals(attacks.c_str(), name.c_str(), TRUE)) {
					return true;
				}
			}
			return false;
		}

		bool Orbwalker::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position, GameObject* target) {
			if (unit == nullptr)
				return false;

			if (unit == ObjectManager::Player) {
				switch (order) {
				case GameObjectOrder::MoveTo:
					LastOrder = GameObjectOrder::MoveTo;
					LastMovePosition = position;
					LastTarget = nullptr;
					break;
				case GameObjectOrder::AttackUnit:
					LastOrder = GameObjectOrder::AttackUnit;
					LastTarget = target;
					break;
				case GameObjectOrder::Stop:
					LastOrder = GameObjectOrder::Stop;
					LastTarget = nullptr;
					break;
				}
			}

			return true;
		}

		void Orbwalker::OnSpellCast(kSpellSlot slot) {
			if (!slot)
				return;

			if (slot == AttackResetSlot && !IsDashAttackReset) {
				if (IsAshe) {
					LastAATick -= ObjectManager::Player->GetAttackDelay() * 0.4f;
				}
				else if (IsRengar) {
					auto pathController = ObjectManager::Player->GetPathController();
					if (!pathController->HasNavigationPath || !pathController->GetNavigationPath()->IsDashing) {
						ResetAutoAttack();
					}
				}
				else if (!AttackResetHash || ObjectManager::Player->Spellbook.GetSpell(slot)->SpellData->Script->Hash == AttackResetHash) {
					ResetAutoAttack();
				}
			}
			else if (slot >= SpellSlot_Item1 && slot <= SpellSlot_Item6) {
				auto inventorySlot = ObjectManager::Player->HeroInventory.Slots[slot - SpellSlot_Item1];
				auto itemInfo = inventorySlot->ItemInfo;
				if (itemInfo && itemInfo->ItemData->ItemId == Item::Titanic_Hydra) {
					ResetAutoAttack();
				}
			}
		}

		void Orbwalker::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			if (castInfo == nullptr || spellData == nullptr)
				return;
			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];
			/*
			if (caster != nullptr && caster->IsEnemy() && Contains(spellData->SpellName, "YasuoWMovingWall", false))
			{
				HACKUZAN::PredLastYasuoWallCastPos = caster->Position;
			}
			*/
			if (caster == ObjectManager::Player) {

				LastOrder = GameObjectOrder::None;
				LastMovePosition = Vector3(0, 0, 0);

				if (IsAutoAttackReset(spellData->SpellName) && spellData->CastDelay == 0) {
					ResetAutoAttack();
				}

				if (!IsAutoAttack(spellData->SpellName))
					return;

				if (!castInfo->IsAutoAttack())
					return;

				auto targetId = castInfo->TargetId;
				if (targetId) {
					LastTarget = ObjectManager::Instance->ObjectsArray[targetId];
				}

				LastAATick = ClockFacade::GameTickCount() - NetClient::Instance->GetPing() / 2;
				LastMoveCommandT = 0;
				_missileLaunched = false;
			}
		}

		void Orbwalker::OnDoCast(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			auto caster = ObjectManager::Instance->ObjectsArray[castInfo->SourceId];
			if (caster->IsMe()) {

				if (NetClient::Instance->GetPing() <= 30) //First world problems kappa
				{
					EventManager::Trigger(LeagueEvents::OnDoCastDelayed, castInfo, spellData);
					return;
				}

				EventManager::Trigger(LeagueEvents::OnDoCastDelayed, castInfo, spellData);
			}
		}

		void Orbwalker::OnDoCastDelayed(SpellInfo* castInfo, SpellDataResource* spellData)
		{
			if (IsAutoAttackReset(spellData->SpellName))
			{
				//Delay->Add(500 - NetClient::Instance->GetPing(), []() { ResetAutoAttack(); });
			}
			if (IsAutoAttack(spellData->SpellName))
			{
				Delay->Add(350 - NetClient::Instance->GetPing(), []() { _missileLaunched = true; });
			}
		}

		void Orbwalker::OnStopCast(GameObject* unit, StopCast* args) {
			if (unit == nullptr || args == nullptr)
				return;
			if (unit == ObjectManager::Player && args->destroyMissile && args->stopAnimation) {
				ResetAutoAttack();
			}
		}

		void Orbwalker::OnNewPath(NewPath* args) {

			auto _LastTarget = LastTarget;

			if (args != nullptr) {

				PredAllNewPathTicks[args->sender->NetworkId] = ClockFacade::GameTickCount();

				if (args->dashSpeed != 0) {
					PredAllDashData[args->sender->NetworkId] = args;
				}
				if (args->sender == ObjectManager::Player) {
					if (Orbwalker::IsRengar && _LastTarget && args->dashSpeed == 1450.0f) {
						Orbwalker::LastAATick = ClockFacade::GameTickCount() - ObjectManager::Player->GetAttackCastDelay() - NetClient::Instance->GetPing() * 0.001f;
					}
				}
			}
		}

		void Orbwalker::OnCreateObject(GameObject* unit) {
			if (unit == nullptr)
				return;

			if (IsAzir && unit->Minion() && unit->BaseCharacterData->SkinHash == Character::AzirSoldier) {
				AzirSoldiers.push_back(unit);
			}
		}

		void Orbwalker::OnDeleteObject(GameObject* unit) {

			if (unit == nullptr)
				return;

			if (IsAzir) {
				for (auto it = AzirSoldiers.begin(); it != AzirSoldiers.end(); it++) {
					if (unit == *it) {
						AzirSoldiers.erase(it);
						break;
					}
				}
			}
			if (unit == LastTarget) {
				LastTarget = nullptr;
			}
			if (unit == ForcedTarget) {
				ForcedTarget = nullptr;
			}
		}

		void Orbwalker::OnGameUpdate() {


			Delay->OnGameUpdate();

			if (LastTarget && !LastTarget->IsValidTarget()) {
				LastTarget = nullptr;
			}

			LastHitMinion = nullptr;
			AlmostLastHitMinion = nullptr;
			LaneClearMinion = nullptr;

			if (ActiveMode != OrbwalkerMode_None || Config::Drawings::LasthittableMinions->Value) {
				auto minion_list = HACKUZAN::GameObject::GetMinions();
				for (size_t i = 0; i < minion_list->size; i++)
				{

					auto minion = minion_list->entities[i];

					if (!minion || minion->Team == GameObjectTeam_Neutral || minion->Team == ObjectManager::Player->Team || !ObjectManager::Player->IsInAutoAttackRange(minion) || !minion->IsValidTarget()) {
						continue;
					}


					auto attackCastDelay = GetAttackCastDelay(minion);
					auto attackMissileSpeed = GetAttackMissileSpeed();
					auto lastHitHealth = minion->Health;
					auto laneClearHealth = lastHitHealth;

					auto LastHitMinion_lastHitHealth = FLT_MAX;
					auto AlmostLastHitMinion_laneClearHealth = FLT_MAX;
					auto LaneClearMinion_laneClearHealth = 0;
					
					auto t = (int)(ObjectManager::Player->GetAttackCastDelay() * 1000) - 100 + NetClient::Instance->GetPing() / 2
						+ 1000 * (int)std::max(0.0f, ObjectManager::Player->Position.Distance(minion->Position) - ObjectManager::Player->GetBoundingRadius())
						/ (int)attackMissileSpeed;
					
					lastHitHealth = HealthPrediction::GetHealthPrediction(minion, t, Config::Farming::ExtraFarmDelay->Value);
					laneClearHealth = HealthPrediction::LaneClearHealthPrediction(minion, ObjectManager::Player->GetAttackDelay() * 1000 * 2.0f, Config::Farming::ExtraFarmDelay->Value);
					auto health = laneClearHealth; // lastHitHealth if turret is targetting
					auto attackDamage = Damage::CalculateAutoAttackDamage(ObjectManager::Player, minion);
					
					if (lastHitHealth > 0 && lastHitHealth < attackDamage) {
						if (!LastHitMinion || (minion->MaxHealth == LastHitMinion->MaxHealth ? lastHitHealth < LastHitMinion_lastHitHealth : minion->MaxHealth > LastHitMinion->MaxHealth)) {
							LastHitMinion = minion;
							LastHitMinion_lastHitHealth = lastHitHealth;
						}
					}
					else if (health <= (minion->IsSiegeMinion() ? 2.0f : 1.5f) * attackDamage && health < minion->Health) {
						if (!AlmostLastHitMinion || (minion->MaxHealth == AlmostLastHitMinion->MaxHealth ? laneClearHealth < AlmostLastHitMinion_laneClearHealth : minion->MaxHealth > AlmostLastHitMinion->MaxHealth)) {
							AlmostLastHitMinion = minion;
							AlmostLastHitMinion_laneClearHealth = laneClearHealth;
						}
					}
					else if (ActiveMode & OrbwalkerMode_LaneClear) {
						bool isLaneClearMinion = true;
						auto turret_list = HACKUZAN::GameObject::GetTurrets();
						for (size_t i = 0; i < turret_list->size; i++)
						{
							auto turret = turret_list->entities[i];
							if (turret->IsAlly() && turret->IsValidTarget() && turret->Position.Distance(minion->Position) <= 900) {
								if (laneClearHealth == minion->Health) {
									auto turretDamage = Damage::CalculateAutoAttackDamage(turret, minion);
									for (auto minionHealth = minion->Health; minionHealth > 0.0f && turretDamage > 0.0f; minionHealth -= turretDamage) {
										if (minionHealth <= attackDamage) {
											isLaneClearMinion = false;
											break;
										}
									}
									if (!LaneClearMinion || (Config::Farming::PushPriority->Value ? laneClearHealth < LaneClearMinion_laneClearHealth : laneClearHealth > LaneClearMinion_laneClearHealth)) {
										LaneClearMinion = minion;
										LaneClearMinion_laneClearHealth = laneClearHealth;
									}
								}
								isLaneClearMinion = false;
								break;
							}
						}

						if (!isLaneClearMinion) {
							continue;
						}

						if (laneClearHealth >= 2.0f * attackDamage || laneClearHealth == minion->Health) {
							if (!LaneClearMinion || (Config::Farming::PushPriority->Value ? laneClearHealth < LaneClearMinion_laneClearHealth : laneClearHealth > LaneClearMinion_laneClearHealth)) {
								LaneClearMinion = minion;
								LaneClearMinion_laneClearHealth = laneClearHealth;
							}
						}
					}
				}
			}

			if (ActiveMode != OrbwalkerMode_None) {
				OrbwalkTo(GetOrbwalkPosition());
			}
		}

		void Orbwalker::OnDraw() {

			if (Config::Drawings::AttackRange->Value && ObjectManager::Player->Alive()) {
				Renderer::AddCircle(ObjectManager::Player->Position, ObjectManager::Player->GetAutoAttackRange(), 2.0f, IM_COL32(144, 238, 144, 255));
			}

			if (Config::Drawings::HoldRadius->Value && ObjectManager::Player->Alive()) {
				Renderer::AddCircle(ObjectManager::Player->Position, Config::Configuration::HoldRadius->Value, 2.0f, IM_COL32(144, 238, 144, 255));
			}

			if (Config::Drawings::EnemyAttackRange->Value) {
				auto hero_list = HACKUZAN::GameObject::GetHeroes();
				for (size_t i = 0; i < hero_list->size; i++)
				{
					auto hero = hero_list->entities[i];
					if (hero->IsEnemy() && hero->IsValidTarget()) {
						auto range = hero->GetAutoAttackRange(ObjectManager::Player);
						Renderer::AddCircle(hero->Position, range, 2.0f, hero->Position.IsInRange(ObjectManager::Player->Position, range) ? IM_COL32(255, 69, 0, 255) : IM_COL32(144, 238, 144, 255));
					}
				}
			}

			if (Config::Drawings::LasthittableMinions->Value) {
				if (LastHitMinion) {
					Renderer::AddCircle(LastHitMinion->Position, LastHitMinion->GetBoundingRadius(), 2.0f, IM_COL32(255, 255, 255, 255));
				}
				if (AlmostLastHitMinion && AlmostLastHitMinion != LastHitMinion) {
					Renderer::AddCircle(AlmostLastHitMinion->Position, AlmostLastHitMinion->GetBoundingRadius(), 2.0f, IM_COL32(255, 69, 0, 255));
				}
			}

			if (IsAzir && Config::Drawings::AzirSoldierAttackRange->Value) {
				for (auto soldier : AzirSoldiers) {
					if (soldier->Alive() && ObjectManager::Player->Position.IsInRange(soldier->Position, 790.0f)) {
						auto pathController = soldier->GetPathController();
						if ((!pathController->HasNavigationPath || !pathController->GetNavigationPath()->IsDashing)) {
							Renderer::AddCircle(soldier->Position, soldier->GetAutoAttackRange(), 2.0f, IM_COL32(144, 238, 144, 255));
						}
					}
				}
			}
		}

		void Orbwalker::ResetAutoAttack() {
			LastAATick = 0;
		}

		Vector3 Orbwalker::GetOrbwalkPosition() {
			auto _LastTarget = LastTarget;
			if (ForcedPosition.IsValid()) {
				return ForcedPosition;
			}
			else if (ObjectManager::Player->IsMelee() && Config::Melee::StickToTarget->Value && !(ActiveMode & OrbwalkerMode_Flee) && _LastTarget) {
				auto pathController = _LastTarget->GetPathController();
				if ((_LastTarget->IsMonster() || _LastTarget->Hero()) && pathController->HasNavigationPath && ObjectManager::Player->Position.IsInRange(pathController->ServerPosition, ObjectManager::Player->GetAutoAttackRange(_LastTarget) + 150.0f)) {
					return pathController->GetNavigationPath()->EndPosition;
				}
			}

			return HudManager::Instance->CursorTargetLogic->CursorPosition;
		}

		GameObject* Orbwalker::GetTarget() {
			if (ForcedTarget && ForcedTarget->IsValidTarget()) {
				return ObjectManager::Player->IsInAutoAttackRange(ForcedTarget) ? ForcedTarget : nullptr;
			}

			if (ActiveMode & OrbwalkerMode_Combo) {
				auto hero = GetTargetByType(OrbwalkerTargetType::Hero);
				if (hero) {
					return hero;
				}
			}

			if (ActiveMode & OrbwalkerMode_Harass) {
				auto structure = GetTargetByType(OrbwalkerTargetType::Structure);

				if (structure) {
					if (!Config::Farming::LastHitPriority->Value) {
						return structure;
					}
					auto minion = GetTargetByType(OrbwalkerTargetType::Minion);
					if (minion) {
						return minion;
					}
					if (!ShouldWait()) {
						return structure;
					}
				}
				else {
					auto hero = GetTargetByType(OrbwalkerTargetType::Hero);
					if (hero && !Config::Farming::LastHitPriority->Value) {
						return hero;
					}
					auto minion = GetTargetByType(OrbwalkerTargetType::Minion);
					if (minion) {
						return minion;
					}
					if (hero && !ShouldWait()) {
						return hero;
					}
				}
			}

			if (ActiveMode & OrbwalkerMode_LastHit) {
				auto minion = GetTargetByType(OrbwalkerTargetType::Minion);
				if (minion) {
					return minion;
				}
			}

			if (ActiveMode & OrbwalkerMode_LaneClear) {
				auto structure = GetTargetByType(OrbwalkerTargetType::Structure);

				if (structure) {
					if (!Config::Farming::LastHitPriority->Value) {
						return structure;
					}
					auto minion = GetTargetByType(OrbwalkerTargetType::Minion);
					if (minion && minion == LastHitMinion) {
						return minion;
					}
					if (!ShouldWait()) {
						return structure;
					}
				}
				else {
					auto hero = GetTargetByType(OrbwalkerTargetType::Hero);
					if (hero && Config::Configuration::LaneClearHeroes->Value && !Config::Farming::LastHitPriority->Value) {
						return hero;
					}
					auto minion = GetTargetByType(OrbwalkerTargetType::Minion);
					if (minion && minion == LastHitMinion) {
						return minion;
					}
					if (hero && Config::Configuration::LaneClearHeroes->Value && !ShouldWait()) {
						return hero;
					}
					if (minion) {
						return minion;
					}
				}
			}

			if (ActiveMode & OrbwalkerMode_JungleClear) {
				return GetTargetByType(OrbwalkerTargetType::Monster);
			}

			return nullptr;
		}

		GameObject* Orbwalker::GetTargetByType(OrbwalkerTargetType targetType) {
			switch (targetType)
			{
			case OrbwalkerTargetType::Hero: {
				if (IsAzir) {
					std::vector<GameObject*> soldierTargets;
					auto hero_list = HACKUZAN::GameObject::GetHeroes();
					for (size_t i = 0; i < hero_list->size; i++)
					{
						auto hero = hero_list->entities[i];

						if (hero && hero->IsEnemy() && hero->IsValidTarget()) {
							for (auto soldier : AzirSoldiers) {
								if (soldier->Alive() && soldier->IsInAutoAttackRange(hero) && ObjectManager::Player->Position.IsInRange(soldier->Position, 790.0f)) {
									auto pathController = soldier->GetPathController();
									if ((!pathController->HasNavigationPath || !pathController->GetNavigationPath()->IsDashing)) {
										soldierTargets.push_back(hero);
										break;
									}
								}
							}
						}
					}
					auto target = TargetSelector::GetTarget(soldierTargets, DamageType_Magical);
					if (target && ObjectManager::Player->IsInAutoAttackRange(target)) {
						return target;
					}
				}

				std::vector<GameObject*> heroes;
				auto hero_list = HACKUZAN::GameObject::GetHeroes();
				for (size_t i = 0; i < hero_list->size; i++)
				{
					auto hero = hero_list->entities[i];

					if (hero->IsEnemy() && hero->IsValidTarget() && ObjectManager::Player->IsInAutoAttackRange(hero)) {
						heroes.push_back(hero);
					}
				}

				return TargetSelector::GetTarget(heroes, DamageType_Physical);
			}
			case OrbwalkerTargetType::Monster: {
				GameObject* monster = nullptr;
				float highestMaxHealth = 0.0f;
				auto minion_list = HACKUZAN::GameObject::GetMinions();
				for (size_t i = 0; i < minion_list->size; i++)
				{
					auto minion = minion_list->entities[i];
					if (minion && minion->IsMonster() && minion->IsValidTarget() && ObjectManager::Player->IsInAutoAttackRange(minion)) {
						if (minion->MaxHealth > highestMaxHealth) {
							monster = minion;
							highestMaxHealth = minion->MaxHealth;
						}
					}
				}
				return monster;
			}
			case OrbwalkerTargetType::Minion: {
				auto supportMode = false;
				if (Config::Configuration::SupportMode->Value) {
					auto hero_list = HACKUZAN::GameObject::GetHeroes();
					for (size_t i = 0; i < hero_list->size; i++)
					{
						auto hero = hero_list->entities[i];
						if (hero != ObjectManager::Player && hero->IsAlly() && hero->IsValidTarget(1050.0f)) {
							supportMode = true;
							break;
						}
					}
				}

				if (!supportMode || ObjectManager::Player->FindBuffHash(0xC406EAE0)) { // "TalentReaper"
					if (LastHitMinion) {
						if (AlmostLastHitMinion && AlmostLastHitMinion != LastHitMinion && AlmostLastHitMinion->IsSiegeMinion()) {
							return nullptr;
						}
						return LastHitMinion;
					}
					if (supportMode || ShouldWait()) {
						return nullptr;
					}
					if (ActiveMode & OrbwalkerMode_LaneClear) {
						return LaneClearMinion;
					}
				}
				break;
			}
			case OrbwalkerTargetType::Structure: {
				auto aibase_list = HACKUZAN::GameObject::GetAIBases();
				for (size_t i = 0; i < aibase_list->size; i++)
				{
					auto unit = aibase_list->entities[i];

					if (unit->IsEnemy() && unit->Alive() && unit->IsTargetable && unit->IsStructure() && ObjectManager::Player->GetPathController()->ServerPosition.IsInRange(unit->Position, ObjectManager::Player->GetAutoAttackRange(unit))) {
						//GameClient::PrintChat(unit->BaseCharacterData->SkinName, IM_COL32(255, 69, 0, 255));
						return unit;
					}
				}
			}
			}
			return nullptr;
		}

		bool Orbwalker::ShouldWait() {
			return /*ClockFacade::GetGameTime() - LastShouldWait <= 0.4f ||*/ AlmostLastHitMinion;
		}


		/*bool Orbwalker::ShouldWait() {

			auto minion_list = HACKUZAN::GameObject::GetMinions();
			for (size_t i = 0; i < minion_list->size; i++)
			{
				auto minion = minion_list->entities[i];

				if (minion != nullptr && minion->Team != GameObjectTeam_Neutral && minion->IsEnemy() && minion->IsValidTarget() && ObjectManager::Player->IsInAutoAttackRange(minion) && HealthPrediction::LaneClearHealthPrediction(
					minion,
					(int)(ObjectManager::Player->GetAttackDelay() * 1000 * 2.0f),
					Config::Farming::ExtraFarmDelay->Value) <= (minion->IsSiegeMinion() ? 2.0f : 1.5f) * ObjectManager::Player->GetAutoAttackDamage(minion)) {

					return true;
				}
			}
			return false;
		}*/

		bool Orbwalker::ShouldWaitUnderTurret(GameObject* noneKillableMinion)
		{
			auto minion_list = HACKUZAN::GameObject::GetMinions();
			for (size_t i = 0; i < minion_list->size; i++)
			{
				auto minion = minion_list->entities[i];

				return
					((noneKillableMinion != nullptr ? noneKillableMinion->Id != minion->Id : true)
						&& ObjectManager::Player->IsInAutoAttackRange(minion) && HealthPrediction::LaneClearHealthPrediction(
							minion,
							(int)
							(ObjectManager::Player->GetAttackDelay() * 1000
								+ (ObjectManager::Player->IsMelee()
									? ObjectManager::Player->GetAttackCastDelay() * 1000
									: ObjectManager::Player->GetAttackCastDelay() * 1000
									+ 1000 * (ObjectManager::Player->AttackRange + 2 * ObjectManager::Player->GetBoundingRadius())
									/ GetAttackMissileSpeed())),
							Config::Farming::ExtraFarmDelay->Value) <= (minion->IsSiegeMinion() ? 2.0f : 1.5f) * ObjectManager::Player->GetAutoAttackDamage(minion));
			}
		}

		void Orbwalker::OrbwalkTo(Vector3 position) {
			if (!ObjectManager::Player->Alive())
				return;
			if (ClockFacade::GameTickCount() - LastAttackCommandT < Config::Configuration::MovementDelay->Value + std::min(60, NetClient::Instance->GetPing())) {
				return;
			}

			try
			{
				auto target = GetTarget();
				if (target && target->IsValidTarget() && CanAttack(target))
				{
					DisableNextAttack = false;
					//FireBeforeAttack(target);

					if (!DisableNextAttack)
					{
						std::string _championName = ObjectManager::Player->BaseCharacterData->SkinName;
						for (auto champs : NoCancelChamps) {
							if (!GameClient::StringEquals(champs.c_str(), _championName.c_str(), TRUE)) {
								_missileLaunched = false;
							}
						}

						ObjectManager::Player->IssueOrder(GameObjectOrder::AttackUnit, target);
						LastAttackCommandT = ClockFacade::GameTickCount();
						LastTarget = target;

						return;
					}
				}

				if (CanMove())
				{
					DisableNextMove = false;
					OrbwalkerEvading = false;
					if (!DisableNextMove) {
						MoveTo(position);
					}
				}
			}
			catch (...)
			{
				// Console.WriteLine(e.ToString());
			}
		}

		void Orbwalker::MoveTo(Vector3 position) {
			if (ObjectManager::Player->IsImmovable() || !ObjectManager::Player->Alive() || Orbwalker::OrbwalkerEvading)
				return;
			if (ClockFacade::GameTickCount() - LastMoveCommandT < Config::Configuration::MovementDelay->Value + std::min(60, NetClient::Instance->GetPing())) {
				return;
			}

			auto playerPosition = ObjectManager::Player->Position;
			auto movePosition = playerPosition.DistanceSquared(position) < 10000.0f ? playerPosition.Extended(position, 100.0f) : position;
			auto order = GameObjectOrder::MoveTo;

			auto holdRadius = Config::Configuration::HoldRadius->Value;
			if (holdRadius > 0) {
				if (playerPosition.DistanceSquared(position) <= holdRadius * holdRadius) {
					movePosition = playerPosition;
					order = GameObjectOrder::Stop;
				}
			}

			if (order == LastOrder) {
				switch (LastOrder) {
				case GameObjectOrder::Stop:
					return;
				case GameObjectOrder::MoveTo:
					if (movePosition == LastMovePosition && ObjectManager::Player->GetPathController()->HasNavigationPath) {
						return;
					}
					break;
				}
			}

			ObjectManager::Player->IssueOrder(order, &movePosition);
			LastMoveCommandT = ClockFacade::GameTickCount();
		}

		float Orbwalker::GetAttackCastDelay(GameObject* target) {
			if (IsAzir && target) {
				for (auto soldier : AzirSoldiers) {
					if (soldier->IsInAutoAttackRange(target)) {
						return soldier->GetAttackCastDelay();
					}
				}
			}

			return ObjectManager::Player->GetAttackCastDelay();
		}

		float Orbwalker::GetAttackDelay(GameObject* target) {
			if (IsAzir && target) {
				for (auto soldier : AzirSoldiers) {
					if (soldier->IsInAutoAttackRange(target)) {
						return soldier->GetAttackDelay();
					}
				}
			}

			return ObjectManager::Player->GetAttackDelay();
		}

		float Orbwalker::GetAttackMissileSpeed() {
			switch (ObjectManager::Player->BaseCharacterData->SkinHash) {
			case Character::Thresh:
			case Character::Azir:
			case Character::Velkoz:
			case Character::Rakan:
				return FLT_MAX;
			case Character::Viktor:
				if (ObjectManager::Player->FindBuffName("ViktorPowerTransferReturn")) {
					return FLT_MAX;
				}
				break;
			case Character::Jinx:
				if (ObjectManager::Player->FindBuffName("JinxQ")) {
					return 2000.0f;
				}
				break;
			case Character::Poppy:
				if (ObjectManager::Player->FindBuffName("poppypassivebuff")) {
					return 1600.0f;
				}
				break;
			case Character::Ivern:
				if (ObjectManager::Player->FindBuffName("ivernwpassive")) {
					return 1600.0f;
				}
				break;
			case Character::Caitlyn:
				if (ObjectManager::Player->FindBuffName("caitlynheadshot")) {
					return 3000.0f;
				}
				break;
			case Character::Twitch:
				if (ObjectManager::Player->FindBuffName("TwitchFullAutomatic")) {
					return 4000.0f;
				}
				break;
			case Character::Jayce:
				if (ObjectManager::Player->FindBuffName("jaycestancegun")) {
					return 2000.0f;
				}
				return FLT_MAX;
			}

			return ObjectManager::Player->IsMelee() ? FLT_MAX : ObjectManager::Player->GetBasicAttack()->GetSpellData()->MissileSpeed;
		}

		bool Orbwalker::CanAttack(GameObject* target) {

			if ((ObjectManager::Player->IsRanged() && target->Hero() && target->IsMelee() && ObjectManager::Player->Position.Distance(target->Position) <= target->AttackRange + 75 &&
				ObjectManager::Player->Health <= (30 / 100.0) * ObjectManager::Player->MaxHealth) && target->Health >= ObjectManager::Player->Health) {
				return false;
			}
			if (ObjectManager::Player->BaseCharacterData->SkinName == "Vayne" && ObjectManager::Player->FindBuffType(BuffType::Invisibility)) {
				return false;
			}

			std::string _championName = ObjectManager::Player->BaseCharacterData->SkinName;
			if (ObjectManager::Player->FindBuffType(BuffType::Blind) || ObjectManager::Player->FindBuffType(BuffType::Blind) && _championName == "Kalista")
			{
				return false;
			}

			if (_championName == "Graves")
			{
				auto attackDelay = 1.0740296828 * 1000 * GetAttackDelay() - 716.2381256175;
				if (ClockFacade::GameTickCount() + NetClient::Instance->GetPing() / 2 + 25 >= LastAATick + attackDelay
					&& ObjectManager::Player->FindBuffName("GravesBasicAttackAmmo1"))
				{
					return true;
				}

				return false;
			}

			if (_championName == "Jhin")
			{
				if (ObjectManager::Player->FindBuffName("JhinPassiveReload"))
				{
					return false;
				}
			}

			return ClockFacade::GameTickCount() + NetClient::Instance->GetPing() / 2 + 25 >= LastAATick + GetAttackDelay() * 1000;
		}

		bool Orbwalker::CanMove() {

			std::string _championName = ObjectManager::Player->BaseCharacterData->SkinName;

			if (_missileLaunched && Config::Configuration::MissileCheck->Value)
			{
				return true;
			}

			auto localExtraWindup = 0;
			if (_championName == "Rengar" && (ObjectManager::Player->FindBuffName("rengarqbase") || ObjectManager::Player->FindBuffName("rengarqemp")))
			{
				localExtraWindup = 200;
			}

			for (auto champs : NoCancelChamps) {
				if (GameClient::StringEquals(champs.c_str(), _championName.c_str(), TRUE)) {
					return true;
				}
			}
			return (ClockFacade::GameTickCount() + NetClient::Instance->GetPing() / 2 >= LastAATick + GetAttackCastDelay() * 1000 + Config::Configuration::ExtraWindUpTime->Value + localExtraWindup);
		}

		bool Orbwalker::CanCastAfterAttack() {

			std::string _championName = ObjectManager::Player->BaseCharacterData->SkinName;

			if (_missileLaunched && Config::Configuration::MissileCheck->Value)
			{
				return true;
			}

			auto localExtraWindup = 0;
			if (_championName == "Rengar" && (ObjectManager::Player->FindBuffName("rengarqbase") || ObjectManager::Player->FindBuffName("rengarqemp")))
			{
				localExtraWindup = 200;
			}

			for (auto champs : NoCancelChamps) {
				if (GameClient::StringEquals(champs.c_str(), _championName.c_str(), TRUE)) {
					return true;
				}
			}
			return (!Config::Configuration::MissileCheck->Value && ClockFacade::GameTickCount() + NetClient::Instance->GetPing() / 2 >= LastAATick + GetAttackCastDelay() * 1000 + Config::Configuration::ExtraWindUpTime->Value + localExtraWindup);
		}
	}
}