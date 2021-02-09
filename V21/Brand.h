#pragma once
#include "ObjectManager.h"
#include "TargetSelector.h"
#include "Orbwalker.h"
#include "HudManager.h"
#include "EventManager.h"
#include "ClockFacade.h"
#include "Renderer.h"
#include "Functions.h"
#include "Menu.h"

#define PI 3.14159265f

namespace HACKUZAN {
	namespace Plugins {
		namespace Brand { //change champ name

			void Initialize();
			void Dispose();
			void OnGameUpdate();
			void OnDraw();
			void BrandCombo();
			void BrandHarass();
			GameObject* GetTarget(float range);
			bool HasBrandPassive(GameObject* unit);
		}
	}
}