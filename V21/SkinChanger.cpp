#include "stdafx.h"
#include "SkinChanger.h"
#include "ObjectManager.h"
#include "Functions.h"
#include "Menu.h"

using namespace V21::SDK;

namespace V21 {
	namespace Plugins {
		

		void SkinChanger::Initialize() {
			std::vector<std::string> skins = { "Default" };
			std::unordered_map<std::string, unsigned int> chromas;

			for (unsigned int i = 1; ; i++) {
				char skinName[256];
				snprintf(skinName, sizeof(skinName), "game_character_skin_displayname_%s_%d", ObjectManager::Player->BaseCharacterData->SkinName, i);

				const char* skinDisplayName = Functions::TranslateString(skinName);

				if (skinDisplayName == skinName) {
					break;
				}

				chromas[skinDisplayName]++;
				if (chromas[skinDisplayName] > 1) {
					sprintf(skinName, "%s Chroma %d", skinDisplayName, chromas[skinDisplayName] - 1);
					skinDisplayName = skinName;
				}

				skins.push_back(skinDisplayName);
			}

			auto menu = Menu::CreateMenu("SkinChanger", "Skin Changer");
			menu->AddList("SelectedSkin", "Selected Skin", skins, ObjectManager::Player->CharacterDataStack.SkinId, [](List*, unsigned int value) {
				Functions::ChangerCharacterData(&ObjectManager::Player->CharacterDataStack, ObjectManager::Player->CharacterDataStack.SkinName.c_str(), value, false, false, false, false, false, false);
			})->Value = ObjectManager::Player->CharacterDataStack.SkinId;
		}
	}
}