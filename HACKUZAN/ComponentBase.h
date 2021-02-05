#pragma once

#include "stdafx.h"
#include "GameObject.h"

#ifndef  COMPONENTBASE
#define COMPONENTBASE

namespace HACKUZAN {
	//team
	extern std::vector<DWORD>pComponents;

	class pComponent
	{
	public:
		pComponent() {};
		~pComponent() {};
		virtual void onStart() = 0;
		virtual void onUpdate() = 0;
		virtual void onRender() = 0;
		virtual void onMenu() = 0;
		virtual void OnProcessSpell(GameObject* sender, SpellCastInfo* spellcastinfo) = 0;
		void Add()
		{
			pComponents.push_back(reinterpret_cast<DWORD>(this));
		}

	private:
	};


#endif // ! COMPONENTBASE

}