#include "stdafx.h"
#include "Plugins.h"
#include "Draven.h"
#include "Vayne.h"
#include "SkinChanger.h"
#include "Activator.h"
#include "Evade.h"

namespace HACKUZAN {
	void Plugins::Initialize() {

		//SkinChanger::Initialize();
		Activator::Initialize();
		Evade::Initialize();

		switch (ObjectManager::Player->BaseCharacterData->SkinHash) {
		case Character::Vayne:
			Vayne::Initialize();
			break;
		case Character::Draven:
			Draven::Initialize();
			break;
		}
	}

	void Plugins::Dispose() {

		Activator::Dispose();
		Evade::Dispose();

		switch (ObjectManager::Player->BaseCharacterData->SkinHash) {
		case Character::Vayne:
			Vayne::Dispose();
			break;
		case Character::Draven:
			Draven::Dispose();
			break;
		}
	}
}