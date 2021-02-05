#include "stdafx.h"
#include "Menu.h"
#include "EventManager.h"
#include "SpellDB.h"
#include "EvaderDB.h"

namespace HACKUZAN {

	using namespace HACKUZAN::SDK;
	namespace Config {

		namespace EvadeMenu {
			CheckBox* EvadeUsePercentHP;
			KeyBind* EvadeToogleKey;
			KeyBind* EvadeDodgeOnlyDangerousKey;
			CheckBox* EvadeEnable;
			CheckBox* EvadeDrawStatus;
			CheckBox* EvadeDrawEvadePoint;
			Slider* EvadeTuner;
		}

		namespace EvadeSkillMenu {
			CheckBox* EvadeWithWalking;
			List* EvadeWithWalkingDanger;
			CheckBox* EvadeWithFlash;
			List* EvadeWithFLashDanger;
			CheckBox* EvadeWithHourglass;
			List* EvadeWithHourglassDanger;
		}

		namespace EvadeSkillShotMenu {
			CheckBox* Enable;
			Slider* PercentHP;
			List* DangerLevel;
			CheckBox* IsDangerous;
			CheckBox* Draw;
		}

		namespace Misc {
			Slider* EvadeDelay;
			Slider* EvadeBuffer;
			Slider* BoundingRadius;
			Slider* ExtraRange;
		}
	}

	class EvaderMenu
	{
	public:
		CheckBox* Enable;
		List* DangerLevel;
	};

	class SkillShotMenu
	{
	public:
		Menu* ParentMenu;
		CheckBox* Enable;
		Slider* PercentHP;
		List* DangerLevel;
		CheckBox* IsDangerous;
		CheckBox* Draw;
	};

	std::map<string, EvaderMenu> EvaderOptions;
	std::map<string, SkillShotMenu> EvadeSkillShotOptions;

	class ConfigMenu
	{
	public:

		static void Initialize()
		{

			auto EvadeMenu = Menu::CreateMenu("Evade", "Basic Evade");
			Config::EvadeMenu::EvadeEnable = EvadeMenu->AddCheckBox("EvadeEnable", "Enable Evade", true);
			Config::EvadeMenu::EvadeUsePercentHP = EvadeMenu->AddCheckBox("EvadeUsePercentHP", "Enable Evade only if % HP ", true);
			Config::EvadeMenu::EvadeToogleKey = EvadeMenu->AddKeyBind("EvadeToogleKey", "Toggle Key", 'K', true, true);
			Config::EvadeMenu::EvadeDodgeOnlyDangerousKey = EvadeMenu->AddKeyBind("EvadeDodgeOnlyDangerousKey", "Dodge Only Dangerous Key", 'J', false, false);
			Config::EvadeMenu::EvadeDrawEvadePoint = EvadeMenu->AddCheckBox("EvadeDrawEvadePoint", "Draw Evade Point", true);
			Config::EvadeMenu::EvadeDrawStatus = EvadeMenu->AddCheckBox("EvadeDrawStatus", "Draw Evade Status", true);

			auto Misc = EvadeMenu->AddMenu("Misc", "Misc");
			Config::Misc::EvadeDelay = Misc->AddSlider("EvadeDelay", "Evade Delay", 150, 0, 200, 1);
			Config::Misc::EvadeBuffer = Misc->AddSlider("EvadeBuffer", "Evade Buffer", 250, 0, 500, 1);
			Config::Misc::BoundingRadius = Misc->AddSlider("BoundingRadius", "Extra Hitbox Radius", 100, 0, 500, 10);
			Config::Misc::ExtraRange = Misc->AddSlider("ExtraRange", "Extra Evade Range", 250, 0, 500, 10);

			auto EvadeSkillMenu = EvadeMenu->AddMenu("Evader", "Evader");
			Config::EvadeSkillMenu::EvadeWithWalking = EvadeSkillMenu->AddCheckBox("EvadeWithWalking", "Enable Evade Walking", true);
			Config::EvadeSkillMenu::EvadeWithWalkingDanger = EvadeSkillMenu->AddList("EvadeWithWalkingDanger", "Min Walk Danger Level", { "Low", "Medium", "High", "Extreme" }, 0);
			Config::EvadeSkillMenu::EvadeWithFlash = EvadeSkillMenu->AddCheckBox("EvadeWithFlash", "Evade with Flash", true);
			Config::EvadeSkillMenu::EvadeWithFLashDanger = EvadeSkillMenu->AddList("EvadeWithFLashDanger", "Min Flash Danger Level", { "Low", "Medium", "High", "Extreme" }, 2);
			Config::EvadeSkillMenu::EvadeWithHourglass = EvadeSkillMenu->AddCheckBox("EvadeWithHourglass", "Evade with Zhonya's", true);
			Config::EvadeSkillMenu::EvadeWithHourglassDanger = EvadeSkillMenu->AddList("EvadeWithHourglassDanger", "Min Zhonya's Danger Level", { "Low", "Medium", "High", "Extreme" }, 1);
			for (Evader* spell : EvadersDB->EvadeSpells)
			{
				if (ObjectManager::Player && ObjectManager::Player->BaseCharacterData->SkinName == spell->ChampionName) {
					EvaderOptions[spell->MenuName].Enable = EvadeSkillMenu->AddCheckBox(spell->MenuName.c_str(), spell->MenuName.c_str(), true);
					EvaderOptions[spell->MenuName].DangerLevel = EvadeSkillMenu->AddList((spell->MenuName + " min danger level").c_str(), (spell->MenuName + " min danger level").c_str(), { "Low", "Medium", "High", "Extreme" }, 0);
				}
			}

			auto EvadeSkillShotMenu = EvadeMenu->AddMenu("SkillShot setting", "SkillShot setting");
			for (auto skillshot_data : SpellDb::spells)
			{
				auto hero_list = HACKUZAN::GameObject::GetHeroes();
				for (size_t i = 0; i < hero_list->size; i++)
				{
					auto hero = hero_list->entities[i];

					if (hero->IsEnemy() && hero->BaseCharacterData->SkinName == skillshot_data->champion_name) {


						EvadeSkillShotOptions[skillshot_data->MenuName()].ParentMenu = EvadeSkillShotMenu->AddMenu(skillshot_data->MenuName().c_str(), skillshot_data->MenuDisplayName().c_str());
						EvadeSkillShotOptions[skillshot_data->MenuName()].Enable = EvadeSkillShotOptions[skillshot_data->MenuName()].ParentMenu->AddCheckBox("Enable", "Enable", !skillshot_data->is_disabled_by_default);
						EvadeSkillShotOptions[skillshot_data->MenuName()].PercentHP = EvadeSkillShotOptions[skillshot_data->MenuName()].ParentMenu->AddSlider("% HP", "% HP", 101, 0, 101, 10);
						EvadeSkillShotOptions[skillshot_data->MenuName()].DangerLevel = EvadeSkillShotOptions[skillshot_data->MenuName()].ParentMenu->AddList("Danger Level", "Danger Level", { "Low", "Medium", "High", "Extreme" }, skillshot_data->danger_level);
						EvadeSkillShotOptions[skillshot_data->MenuName()].IsDangerous = EvadeSkillShotOptions[skillshot_data->MenuName()].ParentMenu->AddCheckBox("Is Dangerous", "Is Dangerous", skillshot_data->is_dangerous);
						EvadeSkillShotOptions[skillshot_data->MenuName()].Draw = EvadeSkillShotOptions[skillshot_data->MenuName()].ParentMenu->AddCheckBox("Draw", "Draw", true);
					}
				}
			}
		}
	};

	inline bool GetDodgeStage(DetectedSKillShot skillshot, int DangerousLevel)
	{
		if (EvadeSkillShotOptions.find(skillshot.Data->MenuName()) == EvadeSkillShotOptions.end())
			return false;
		if (!Config::EvadeMenu::EvadeEnable->Value)
			return false;
		if (DangerousLevel > EvadeSkillShotOptions[skillshot.Data->MenuName()].DangerLevel->Value)
			return false;
		if (!EvadeSkillShotOptions[skillshot.Data->MenuName()].Enable->Value)
			return false;
		if (Config::EvadeMenu::EvadeUsePercentHP->Value && ObjectManager::Player->Health >= (EvadeSkillShotOptions[skillshot.Data->MenuName()].PercentHP->Value / 100.0) * ObjectManager::Player->MaxHealth)
			return false;
		if (Config::EvadeMenu::EvadeDodgeOnlyDangerousKey->Value && !EvadeSkillShotOptions[skillshot.Data->MenuName()].IsDangerous->Value)
			return false;
		return true;
	}

	inline bool GetEvaderStage(Evader* spell, DetectedSKillShot skillshot)
	{
		if (EvaderOptions.find(spell->MenuName) == EvaderOptions.end())
			return false;
		if (!EvaderOptions[spell->MenuName].Enable->Value)
			return false;
		return GetDodgeStage(skillshot, EvaderOptions[spell->MenuName].DangerLevel->Value);
	}

}