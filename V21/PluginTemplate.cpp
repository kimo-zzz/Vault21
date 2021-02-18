#include "stdafx.h"
#include "PluginTemplate.h"
#include "NavGrid.h"
#include "Draw.h"
#include "Geometry.h"

namespace V21 {
	namespace Plugins {

		using namespace V21::SDK;
	
		// a global variable
		std::vector<GameObject*> missleList;

		// a global instance of std::mutex to protect global variable
		std::mutex myMutex;

		Menu* menu;
		void V21::Plugins::ChampionName::Initialize()
		{
			menu = Menu::CreateMenu("Template", "Template");
			auto combo = menu->AddMenu("Combo", "Combo Settings");

			auto farm = menu->AddMenu("farm", "Farm Settings");


			auto misc = menu->AddMenu("misc", "Misc");

			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::AddEventHandler(LeagueEvents::OnPresent, OnDraw);
			EventManager::AddEventHandler(LeagueEvents::OnCreateMissile, OnCreateMissile);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteMissile, OnDeleteMissile);
			EventManager::AddEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::AddEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::AddEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::AddEventHandler(LeagueEvents::OnCastSpell, OnCastSpell);
			EventManager::AddEventHandler(LeagueEvents::OnSpellCast, OnSpellCast);
			EventManager::AddEventHandler(LeagueEvents::OnDoCast, OnDoCast);
			EventManager::AddEventHandler(LeagueEvents::OnDoCastDelayed, OnDoCastDelayed);
			EventManager::AddEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::AddEventHandler(LeagueEvents::OnPlayAnimation, OnPlayAnimation);
			EventManager::AddEventHandler(LeagueEvents::OnFinishCast, OnFinishCast);
			EventManager::AddEventHandler(LeagueEvents::OnStopCast, OnStopCast);
			EventManager::AddEventHandler(LeagueEvents::OnGapCloserSpell, OnGapCloserSpell);
			EventManager::AddEventHandler(LeagueEvents::OnInterruptibleSpell, OnInterruptibleSpell);
			EventManager::AddEventHandler(LeagueEvents::OnNewPath, OnNewPath);


			GameClient::PrintChat("Template Script Loaded~!", IM_COL32(255, 69, 255, 255));
		}

		void ChampionName::Dispose()
		{
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnGameUpdate);
			EventManager::RemoveEventHandler(LeagueEvents::OnPresent, OnDraw);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateMissile, OnCreateMissile);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteMissile, OnDeleteMissile);
			EventManager::RemoveEventHandler(LeagueEvents::OnCreateObject, OnCreateObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnDeleteObject, OnDeleteObject);
			EventManager::RemoveEventHandler(LeagueEvents::OnIssueOrder, OnIssueOrder);
			EventManager::RemoveEventHandler(LeagueEvents::OnCastSpell, OnCastSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnSpellCast, OnSpellCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnDoCast, OnDoCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnDoCastDelayed, OnDoCastDelayed);
			EventManager::RemoveEventHandler(LeagueEvents::OnProcessSpell, OnProcessSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnPlayAnimation, OnPlayAnimation);
			EventManager::RemoveEventHandler(LeagueEvents::OnFinishCast, OnFinishCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnStopCast, OnStopCast);
			EventManager::RemoveEventHandler(LeagueEvents::OnGapCloserSpell, OnGapCloserSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnInterruptibleSpell, OnInterruptibleSpell);
			EventManager::RemoveEventHandler(LeagueEvents::OnNewPath, OnNewPath);
			
		}


		void ChampionName::OnGameUpdate() //SAMPLE READ
		{
			std::lock_guard<std::mutex> guard(myMutex);

			for (GameObject* go : missleList) {
				if (go != nullptr)
				{
					menu->AddMenu(go->Name.c_str(), go->Name.c_str());
				}
			}
		}

		void ChampionName::OnDraw()
		{

		}

		//if OnCreateObject is missle, then OnCreateMissle is fired instead
		void ChampionName::OnCreateMissile(GameObject* unit) { //SAMPLE CREATE
			//GameClient::PrintChat("OnCreateMissile fired!", IM_COL32(255, 69, 0, 255));
			//GameClient::PrintChat(unit->Name.c_str(), IM_COL32(255, 69, 0, 255));

			std::lock_guard<std::mutex> guard(myMutex);
			
			missleList.push_back(unit);
		}

		//if OnDeleteObject is missle, then OnDeleteMissle is fired instead
		void ChampionName::OnDeleteMissile(GameObject* unit) { //SAMPLE DELETE. CAN BE USED ALSO IN UPDATE
			//GameClient::PrintChat("OnDeleteMissile fired!", IM_COL32(255, 69, 0, 255));
			//GameClient::PrintChat(unit->Name.c_str(), IM_COL32(255, 69, 0, 255));

			std::lock_guard<std::mutex> guard(myMutex);
			
			std::vector<GameObject*> _missleList;
			for (GameObject* go : missleList) {
				if (go != nullptr) {
					if (go->Id != unit->Id) {
						_missleList.push_back(go); // add if the element to delete is not equal to current iteration
					}
				}
			}
			missleList = _missleList;
		}

		//doesnt catch missle. Use OnCreateMissle instead
		void ChampionName::OnCreateObject(GameObject* unit) {
			//GameClient::PrintChat("OnCreateObject fired!", IM_COL32(255, 69, 0, 255));
			//GameClient::PrintChat(unit->Name.c_str(), IM_COL32(255, 69, 0, 255));
		}

		//doesnt catch missle. Use OnDeleteMissle instead
		void ChampionName::OnDeleteObject(GameObject* unit) {
			//GameClient::PrintChat("OnDeleteObject fired!", IM_COL32(255, 69, 0, 255));
			//GameClient::PrintChat(unit->Name.c_str(), IM_COL32(255, 69, 0, 255));
		}

		//untested //triggers only when localPlayer issues IssueOrder programatically
		void ChampionName::OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3* position, GameObject* target) {
			//GameClient::PrintChat("OnIssueOrder fired!", IM_COL32(255, 69, 0, 255));
			//GameClient::PrintChat(unit->Name.c_str(), IM_COL32(255, 69, 0, 255));
		}

		//triggers only when localPlayer issues CastSpell programatically
		void ChampionName::OnCastSpell(SpellbookClient* spellbook, SpellDataInst* pSpellInfo, kSpellSlot slot, Vector3* _end_position, Vector3* _start_position, DWORD netId) {
			//GameClient::PrintChat("OnCastSpell fired!", IM_COL32(255, 69, 0, 255));
			//auto caster = ObjectManager::Instance->ObjectsArray[spellbook->casterIndex];
			//GameClient::PrintChat(caster->Name.c_str(), IM_COL32(255, 69, 0, 255));
		}

		//trigger fired from Events.cpp //doesnt really get triggered 
		void ChampionName::OnSpellCast(kSpellSlot slot) {
			//GameClient::PrintChat("OnSpellCast fired!", IM_COL32(255, 69, 0, 255));
		}

		//triggered after OnProcessSpell if the caster is hero OR castInfo is auto attack
		void ChampionName::OnDoCast(SpellInfo* castInfo, SpellDataResource* spellData) {
			//GameClient::PrintChat("OnDoCast fired!", IM_COL32(255, 69, 0, 255));
		}

		//no equivalent hooks NOR triggers. maybe this is used by manually firing ???
		void ChampionName::OnDoCastDelayed(SpellInfo* castInfo, SpellDataResource* spellData) {
			//GameClient::PrintChat("OnDoCastDelayed fired!", IM_COL32(255, 69, 0, 255));
		}

		//always triggered for all OnProcessSpell
		void ChampionName::OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData) {
			//GameClient::PrintChat("OnProcessSpell fired!", IM_COL32(255, 69, 0, 255));
		}

		//untested because we have no OnPlayAnimation RVA
		void ChampionName::OnPlayAnimation(GameObject* ptr, char* name, float animationTime) {
			//GameClient::PrintChat("OnPlayAnimation fired!", IM_COL32(255, 69, 0, 255));
			//GameClient::PrintChat(ptr->Name.c_str(), IM_COL32(255, 69, 0, 255));
		}

		//always triggered for all OnFinishCast
		void ChampionName::OnFinishCast(SpellCastInfo* castInfo, GameObject* object) {
			//GameClient::PrintChat("OnFinishCast fired!", IM_COL32(255, 69, 0, 255));
		}

		//always triggered for all OnStopCast
		void ChampionName::OnStopCast(GameObject* unit, StopCast args) {
			//GameClient::PrintChat("OnStopCast fired!", IM_COL32(255, 69, 0, 255));
			//GameClient::PrintChat(unit->Name.c_str(), IM_COL32(255, 69, 0, 255));
			//GameClient::PrintChat((args.stopAnimation ? "stopAnim" : "!stopAnim"), IM_COL32(255, 69, 0, 255));
		}

		//triggered after OnProcessSpell check dllmain.cpp
		void ChampionName::OnGapCloserSpell(SpellInfo* castInfo, SpellDataResource* spellData) {
			//GameClient::PrintChat("OnGapCloserSpell fired!", IM_COL32(255, 69, 0, 255));
		}

		//triggered after OnProcessSpell check dllmain.cpp
		void ChampionName::OnInterruptibleSpell(SpellInfo* castInfo, SpellDataResource* spellData) {
			//GameClient::PrintChat("OnInterruptibleSpell fired!", IM_COL32(255, 69, 0, 255));
		}

		//triggered for all OnNewPath
		void ChampionName::OnNewPath(NewPath args) {
			//GameClient::PrintChat("OnNewPath fired!", IM_COL32(255, 69, 0, 255));
			//GameClient::PrintChat(args.sender->Name.c_str(), IM_COL32(255, 69, 0, 255));
		}
	}
}