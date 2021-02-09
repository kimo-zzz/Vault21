#include "stdafx.h"
#include "TargetSelector.h"
#include "Menu.h"
#include "ManagerTemplate.h"
#include "EventManager.h"
#include "HudManager.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "RenderLayer.h"
#include "MenuSettings.h"
#include "Damage.h"

namespace HACKUZAN {
	namespace SDK {
		std::vector<GameObject*> TargetSelector::Enemies;
		std::unordered_map<Character, unsigned int> TargetSelector::Priorities =
		{
			{ Character::Aatrox, 2 },
			{ Character::Ahri, 4 },
			{ Character::Akali, 3 },
			{ Character::Alistar, 1 },
			{ Character::Amumu, 1 },
			{ Character::Anivia, 4 },
			{ Character::Annie, 4 },
			{ Character::Ashe, 5 },
			{ Character::AurelionSol, 4 },
			{ Character::Azir, 4 },
			{ Character::Bard, 1 },
			{ Character::Blitzcrank, 1 },
			{ Character::Brand, 4 },
			{ Character::Braum, 1 },
			{ Character::Caitlyn, 5 },
			{ Character::Camille, 5 },
			{ Character::Cassiopeia, 4 },
			{ Character::Chogath, 1 },
			{ Character::Corki, 5 },
			{ Character::Darius, 2 },
			{ Character::Diana, 3 },
			{ Character::DrMundo, 1 },
			{ Character::Draven, 5 },
			{ Character::Ekko, 4 },
			{ Character::Elise, 2 },
			{ Character::Evelynn, 2 },
			{ Character::Ezreal, 5 },
			{ Character::Fiddlesticks, 3 },
			{ Character::Fiora, 3 },
			{ Character::Fizz, 3 },
			{ Character::Galio, 2 },
			{ Character::Gangplank, 2 },
			{ Character::Garen, 1 },
			{ Character::Gnar, 1 },
			{ Character::Gragas, 2 },
			{ Character::Graves, 5 },
			{ Character::Hecarim, 1 },
			{ Character::Heimerdinger, 2 },
			{ Character::Illaoi, 1 },
			{ Character::Irelia, 2 },
			{ Character::Ivern, 3 },
			{ Character::Janna, 2 },
			{ Character::JarvanIV, 1 },
			{ Character::Jax, 2 },
			{ Character::Jayce, 3 },
			{ Character::Jhin, 5 },
			{ Character::Jinx, 5 },
			{ Character::Kaisa, 5 },
			{ Character::Kalista, 5 },
			{ Character::Karma, 4 },
			{ Character::Karthus, 4 },
			{ Character::Kassadin, 3 },
			{ Character::Katarina, 4 },
			{ Character::Kayle, 3 },
			{ Character::Kayn, 3 },
			{ Character::Kennen, 4 },
			{ Character::Khazix, 3 },
			{ Character::Kindred, 3 },
			{ Character::Kled, 1 },
			{ Character::KogMaw, 5 },
			{ Character::Leona, 1 },
			{ Character::Lulu, 1 },
			{ Character::LeeSin, 2 },
			{ Character::Lissandra, 3 },
			{ Character::Leblanc, 4 },
			{ Character::Lux, 4 },
			{ Character::Lucian, 5 },
			{ Character::Malphite, 1 },
			{ Character::Maokai, 2 },
			{ Character::Morgana, 2 },
			{ Character::Mordekaiser, 3 },
			{ Character::Malzahar, 4 },
			{ Character::MasterYi, 4 },
			{ Character::MissFortune, 5 },
			{ Character::MonkeyKing, 1 },
			{ Character::Nasus, 1 },
			{ Character::Nautilus, 1 },
			{ Character::Nunu, 1 },
			{ Character::Nocturne, 2 },
			{ Character::Nami, 3 },
			{ Character::Nidalee, 3 },
			{ Character::Olaf, 1 },
			{ Character::Orianna, 4 },
			{ Character::Pantheon, 2 },
			{ Character::Poppy, 2 },
			{ Character::Quinn, 5 },
			{ Character::Rammus, 1 },
			{ Character::Renekton, 1 },
			{ Character::RekSai, 2 },
			{ Character::Rengar, 2 },
			{ Character::Rumble, 2 },
			{ Character::Ryze, 2 },
			{ Character::Rakan, 3 },
			{ Character::Riven, 3 },
			{ Character::Sejuani, 1 },
			{ Character::Shen, 1 },
			{ Character::Shyvana, 1 },
			{ Character::Singed, 1 },
			{ Character::Sion, 1 },
			{ Character::Skarner, 1 },
			{ Character::Swain, 2 },
			{ Character::Shaco, 3 },
			{ Character::Sona, 3 },
			{ Character::Soraka, 3 },
			{ Character::Syndra, 4 },
			{ Character::Sivir, 5 },
			{ Character::Taric, 1 },
			{ Character::Thresh, 1 },
			{ Character::Trundle, 2 },
			{ Character::Tryndamere, 2 },
			{ Character::TahmKench, 3 },
			{ Character::Taliyah, 4 },
			{ Character::Talon, 4 },
			{ Character::TwistedFate, 4 },
			{ Character::Teemo, 5 },
			{ Character::Tristana, 5 },
			{ Character::Twitch, 5 },
			{ Character::Udyr, 2 },
			{ Character::Urgot, 2 },
			{ Character::Volibear, 1 },
			{ Character::Vi, 2 },
			{ Character::Vladimir, 3 },
			{ Character::Veigar, 4 },
			{ Character::Velkoz, 4 },
			{ Character::Viktor, 4 },
			{ Character::Varus, 5 },
			{ Character::Vayne, 5 },
			{ Character::Warwick, 1 },
			{ Character::XinZhao, 2 },
			{ Character::Xerath, 5 },
			{ Character::Xayah, 5 },
			{ Character::Yorick, 1 },
			{ Character::Yasuo, 4 },
			{ Character::Zac, 1 },
			{ Character::Zilean, 3 },
			{ Character::Zyra, 3 },
			{ Character::Zed, 4 },
			{ Character::Ziggs, 4 },
			{ Character::Zoe, 5 },
		};
		TargetSelectorMode TargetSelector::ActiveMode = TargetSelectorMode::Auto;
		GameObject* TargetSelector::SelectedTarget = nullptr;

		namespace TargetSelector {
			namespace Config {
				Menu* Priorities;

				namespace Advanced {
					CheckBox* AttackSelectedTarget;
					CheckBox* AttackOnlySelectedTarget;
				}

				namespace Drawings {
					CheckBox* SelectedTarget;
				}
			}
		}

		void TargetSelector::Initialize() {
			auto hero_list = HACKUZAN::GameObject::GetHeroes();
			for (size_t i = 0; i < hero_list->size; ++i)
			{
				auto hero = hero_list->entities[i];
				if (hero->IsEnemy()) {
					Enemies.push_back(hero);
				}
			}

			auto menu = Menu::CreateMenu("TargetSelector", "Target Selector");

			if (!Enemies.empty()) {
				Config::Priorities = menu->AddMenu("Priorities", "Priorities");
				for (auto enemy : Enemies) {
					auto iterator = Priorities.find(enemy->BaseCharacterData->SkinHash);
					Config::Priorities->AddSlider(enemy->SkinName.c_str(), enemy->SkinName.c_str(), iterator != Priorities.end() ? iterator->second : 1, 1, 5, 1);
				}
				Config::Priorities->AddInfo("Info", "Higher value means higher priority");
				Config::Priorities->AddCheckBox("Reset", "Reset priorities", false, [](CheckBox * self, bool newValue) {
					if (newValue) {
						for (auto enemy : Enemies) {
							auto iterator = Priorities.find(enemy->BaseCharacterData->SkinHash);
							(*Config::Priorities)[enemy->SkinName]->Cast<Slider*>()->Value = iterator != Priorities.end() ? iterator->second : 1;
							self->Value = false;
						}
					}
					});
			}

			auto advancedMenu = menu->AddMenu("Advanced", "Advanced");
			Config::Advanced::AttackSelectedTarget = advancedMenu->AddCheckBox("AttackSelectedTarget", "Attack selected target", true);
			Config::Advanced::AttackOnlySelectedTarget = advancedMenu->AddCheckBox("AttackOnlySelectedTarget", "Attack only selected target");

			auto drawingsMenu = menu->AddMenu("Drawings", "Drawings");
			Config::Drawings::SelectedTarget = drawingsMenu->AddCheckBox("SelectedTarget", "Draw selected target", true);

			ActiveMode = (TargetSelectorMode)menu->AddList("Mode", "Mode", { "Auto",  "Least Health", "Most AD", "Most AP", "Closest", "Highest Priority", "Less Attack", "Less Cast", "Near Mouse" }, 0, [](List * self, unsigned int newValue) {
				ActiveMode = (TargetSelectorMode)newValue;
				})->Value;

			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);
			EventManager::AddEventHandler(LeagueEvents::OnWndProc, OnWndProc);
		}

		void TargetSelector::Dispose() {
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
			EventManager::RemoveEventHandler(LeagueEvents::OnWndProc, OnWndProc);
		}

		void TargetSelector::OnDraw() {
			if (Config::Drawings::SelectedTarget->Value && Config::Advanced::AttackSelectedTarget->Value && SelectedTarget && SelectedTarget->IsValidTarget()) {
				Renderer::AddCircle(SelectedTarget->Position, 80.0f, Config::Advanced::AttackOnlySelectedTarget->Value ? 5.0f : 2.0f, IM_COL32(255, 0, 0, 255));
			}
		}

		void TargetSelector::OnWndProc(UINT msg, WPARAM wparam) {
			if (msg == WM_LBUTTONDOWN && Config::Advanced::AttackSelectedTarget->Value && !MenuSettings::DrawMenu) {
				auto hoveredUnitId = HudManager::Instance->CursorTargetLogic->HoveredUnitId;
				if (hoveredUnitId) {
					auto obj = ObjectManager::Instance->ObjectsArray[hoveredUnitId];
					//if (obj != SelectedTarget && obj->IsEnemy() && obj->Flags() & GameObjectFlags_AIHeroClient) {
					if (obj != SelectedTarget && obj->IsEnemy() && obj->Hero()) {
						SelectedTarget = obj;
					}
					else {
						SelectedTarget = nullptr;
					}
				}
				else {
					SelectedTarget = nullptr;
				}
			}
		}

		float TargetSelector::GetReducedPriority(GameObject* hero) {
			switch (GetPriority(hero)) {
			case 5:
				return 2.5f;
			case 4:
				return 2.0f;
			case 3:
				return 1.75f;
			case 2:
				return 1.5f;
			default:
				return 1.0f;
			}
		}

		int TargetSelector::GetPriority(GameObject* hero) {
			if (hero->IsAlly()) {
				auto iterator = Priorities.find(hero->BaseCharacterData->SkinHash);
				if (iterator != Priorities.end()) {
					return iterator->second;
				}
			}

			for (auto enemy : Enemies) {
				if (enemy == hero) {
					return (*Config::Priorities)[enemy->SkinName]->Cast<Slider*>()->Value;
				}
			}

			return 1;
		}

		GameObject* TargetSelector::GetTarget(TargetType tsType, float range, kDamageType damageType, Vector3 source, bool addBoundingRadius) {
			auto sourcePosition = source.IsValid() ? source : ObjectManager::Player->Position;

			if (Config::Advanced::AttackSelectedTarget->Value && SelectedTarget && SelectedTarget->IsValidTarget()) {
				if (Config::Advanced::AttackOnlySelectedTarget->Value || sourcePosition.IsInRange(SelectedTarget->Position, range + (addBoundingRadius ? SelectedTarget->GetBoundingRadius() : 0.0f))) {
					return SelectedTarget;
				}
			}

			switch (tsType)
			{
			case TargetType::TSTARGET_HEROES: {
				std::vector<GameObject*> validTargets;

				auto hero_list = HACKUZAN::GameObject::GetHeroes();
				for (size_t i = 0; i < hero_list->size; ++i)
				{
					auto hero = hero_list->entities[i];
					if (hero->IsEnemy() && hero->IsValidTarget() && sourcePosition.IsInRange(hero->Position, range + (addBoundingRadius ? hero->GetBoundingRadius() : 0.0f))) {
						validTargets.push_back(hero);
					}
				}

				return GetTarget(validTargets, damageType);
			}
			case TargetType::TSTARGET_MINION: {
				std::vector<GameObject*> validTargets;

				auto minion_list = HACKUZAN::GameObject::GetMinions();
				for (size_t i = 0; i < minion_list->size; ++i)
				{
					auto minion = minion_list->entities[i];
					if (minion->IsEnemy() && minion->IsValidTarget() && sourcePosition.IsInRange(minion->Position, range + (addBoundingRadius ? minion->GetBoundingRadius() : 0.0f))) {
						validTargets.push_back(minion);
					}
				}

				return GetTarget(validTargets, damageType);
			}
			}
		}

		GameObject* TargetSelector::GetTarget(std::vector<GameObject*> targets, kDamageType damageType) {
			if (Config::Advanced::AttackOnlySelectedTarget->Value && Config::Advanced::AttackSelectedTarget->Value && SelectedTarget && SelectedTarget->IsValidTarget()) {
				return SelectedTarget;
			}

			switch (targets.size()) {
			case 0:
				return nullptr;
			case 1:
				return targets[0];
			}

			switch (ActiveMode) {
			case TargetSelectorMode::Auto: {
				GameObject* hero = nullptr;
				float highestPriority = 0.0f;
				for (auto target : targets) {
					auto priority = GetReducedPriority(target) * (damageType == DamageType_Magical ? Damage::CalculateMagicalDamage(ObjectManager::Player, target, 100.0f) : Damage::CalculatePhysicalDamage(ObjectManager::Player, target, 100.0f)) / target->TotalHealth();
					if (priority > highestPriority) {
						hero = target;
						highestPriority = priority;
					}
				}
				return hero;
			}
			case TargetSelectorMode::LeastHealth: {
				GameObject* hero = nullptr;
				float leastHealth = FLT_MAX;
				for (auto target : targets) {
					auto health = target->TotalHealth();
					if (health < leastHealth) {
						hero = target;
						leastHealth = health;
					}
				}
				return hero;
			}
			case TargetSelectorMode::MostAttackDamage: {
				GameObject* hero = nullptr;
				float mostAttackDamage = 0.0f;
				for (auto target : targets) {
					auto attackDamage = target->TotalAttackDamage();
					if (attackDamage > mostAttackDamage) {
						hero = target;
						mostAttackDamage = attackDamage;
					}
				}
				return hero;
			}
			case TargetSelectorMode::MostAbilityPower: {
				GameObject* hero = nullptr;
				float mostAbilityPower = 0.0f;
				for (auto target : targets) {
					auto abilityPower = target->TotalAbilityPower();
					if (abilityPower > mostAbilityPower) {
						hero = target;
						mostAbilityPower = abilityPower;
					}
				}
				return hero;
			}
			case TargetSelectorMode::Closest: {
				GameObject* hero = nullptr;
				float closest = FLT_MAX;
				for (auto target : targets) {
					auto distance = target->Position.DistanceSquared(ObjectManager::Player->Position);
					if (distance < closest) {
						hero = target;
						closest = distance;
					}
				}
				return hero;
			}
			case TargetSelectorMode::HighestPriority: {
				GameObject* hero = nullptr;
				int highestPriority = 0;
				for (auto target : targets) {
					auto priority = GetPriority(target);
					if (priority > highestPriority) {
						hero = target;
						highestPriority = priority;
					}
				}
				return hero;
			}
			case TargetSelectorMode::LessAttack: {
				GameObject* hero = nullptr;
				float highestPriority = 0.0f;
				for (auto target : targets) {
					auto priority = GetReducedPriority(target) * Damage::CalculatePhysicalDamage(ObjectManager::Player, target, 100.0f) / target->TotalHealth();
					if (priority > highestPriority) {
						hero = target;
						highestPriority = priority;
					}
				}
				return hero;
			}
			case TargetSelectorMode::LessCast: {
				GameObject* hero = nullptr;
				float highestPriority = 0.0f;
				for (auto target : targets) {
					auto priority = GetReducedPriority(target) * Damage::CalculateMagicalDamage(ObjectManager::Player, target, 100.0f) / target->TotalHealth();
					if (priority > highestPriority) {
						hero = target;
						highestPriority = priority;
					}
				}
				return hero;
			}
			case TargetSelectorMode::NearMouse: {
				GameObject* hero = nullptr;
				float closest = FLT_MAX;
				for (auto target : targets) {
					auto distance = target->Position.DistanceSquared(HudManager::Instance->CursorTargetLogic->CursorPosition);
					if (distance < closest) {
						hero = target;
						closest = distance;
					}
				}
				return hero;
			}
			}
		}
	}
}