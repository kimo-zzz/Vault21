#include "Riven.h"

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
				List* ComboType;
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
			}
		}


		void Riven::Initialize()
		{
			auto menu = Menu::CreateMenu("riven", "Riven");
			auto combo = menu->AddMenu("combo", "Combo Settings");
			auto farm = menu->AddMenu("farming", "Farm Setting");
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

			RivenConfig::RivenFarm::UseQ = farm->AddCheckBox("Riven Farm Q", "Use Q", true);
			RivenConfig::RivenFarm::UseW = farm->AddCheckBox("Riven Farm W", "Use W", false);
			RivenConfig::RivenFarm::UseE = farm->AddCheckBox("Riven Farm E", "Use E", false);

			RivenConfig::RivenMisc::AutoE = misc->AddCheckBox("Riven Misc Auto E", "Auto E", true);

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

		}

		void Riven::OnDraw()
		{


		}

		bool Riven::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position)
		{
			return false;
		}

		void Riven::OnCreateObject(GameObject* unit)
		{
			if (unit == nullptr)
				return;
			GameClient::PrintChat(("[Riven Debug] Created Object: " + std::string(unit->Name)).c_str(), IM_COL32(255, 69, 255, 255));
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

			if (castInfo->SourceId == ObjectManager::Player->SourceID) // Check if it's our Spell Cast
			{

			}
			else if (castInfo->TargetId == ObjectManager::Player->SourceID) // Check if we're the Target of the Spell
			{

			}
		}
	}
}