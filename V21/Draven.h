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

		class QRecticle
		{
		public:

			GameObject* Object = nullptr;
			float ExpireTime;
			Vector3 Position;

			QRecticle(GameObject* rectice, int expireTime);
		};

		namespace Draven {

			void Initialize();
			void Dispose();
			void OnGameUpdate();
			void OnDraw();
			bool OnIssueOrder(GameObject* unit, GameObjectOrder order, Vector3 position);
			void OnCreateObject(GameObject* unit);
			void OnDeleteObject(GameObject* unit);
			
			inline int DravenAxesOnHand();
			inline int DravenAxesCount();
			inline bool DravenHasWMoveSpeed();
			inline bool DravenHasWAttackSpeed();

			void CatchAxe();
			void addReticles(GameObject* rectice);
		}
	}
}