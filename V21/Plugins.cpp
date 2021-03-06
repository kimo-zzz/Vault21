#include "stdafx.h"
#include "Plugins.h"
#include "Draven.h"
#include "MissFortune.h"
#include "Vayne.h"
#include "Lucian.h"
#include "SkinChanger.h"
#include "Activator.h"
#include "Evade.h"
#include "Twitch.h"
#include "Brand.h"
#include "Riven.h"
#include "Jax.h"
#include "Xerath.h"

namespace V21 {
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
		case Character::Lucian:
			Lucian::Initialize();
			break;
		case Character::MissFortune:
			MissFortune::Initialize();
			break;
		case Character::Twitch:
			Twitch::Initialize();
			break;
		case Character::Brand:
			Brand::Initialize();
			break;
		case Character::Riven:
			Riven::Initialize();
			break;
		case Character::Jax:
			Jax::Initialize();
			break;
		case Character::Xerath:
			Xerath::Initialize();
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
		case Character::Lucian:
			Lucian::Dispose();
			break;
		case Character::MissFortune:
			MissFortune::Dispose();
			break;
		case Character::Twitch:
			Twitch::Dispose();
			break;
		case Character::Brand:
			Brand::Dispose();
			break;
		case Character::Riven:
			Riven::Dispose();
			break;
		case Character::Jax:
			Jax::Dispose();
			break;
		case Character::Xerath:
			Xerath::Dispose();
			break;
		}
	}
}
