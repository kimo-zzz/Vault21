#include "stdafx.h"
#include "ObjectManager.h"

namespace V21
{
	ObjectManager* ObjectManager::Instance;
	GameObject* ObjectManager::Player;

	/*GameObject* ObjectManager::Get(unsigned short id) {
		if (ObjectManager::Instance && id >= 1 && id <= ObjectManager::Instance->MaxObjects) {
			return ObjectManager::Instance->ObjectsArray[id];
		}

		return  nullptr;
	}*/
}