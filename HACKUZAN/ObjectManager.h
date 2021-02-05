#pragma once

#include "Offsets.h"
#include "GameObject.h"

namespace HACKUZAN
{
	class ObjectManager
	{
	public:
		static ObjectManager* Instance;
		static GameObject* Player;

		union
		{
			DEFINE_MEMBER_N(GameObject** ObjectsArray, Offsets::ObjectManager::ObjectsArray)
			DEFINE_MEMBER_N(unsigned int MaxObjects, Offsets::ObjectManager::MaxObjects)
		};

		static GameObject* Get(unsigned short id);
	};

	
}