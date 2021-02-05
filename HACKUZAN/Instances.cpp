#include "stdafx.h"
#include "Instances.h"
#include "Offsets.h"
#include "ClockFacade.h"
#include "HudManager.h"
#include "ManagerTemplate.h"
#include "NetClient.h"
#include "NavGrid.h"
#include "ObjectManager.h"

namespace HACKUZAN {
	void Instances::Initialize() {

		//ClockFacade::Instance = *(ClockFacade**)DEFINE_RVA(Offsets::ClockFacade::Instance);
		//NavGrid::Instance = *(NavGrid**)DEFINE_RVA(Offsets::NavGrid::Instance);
		HudManager::Instance = *(HudManager**)DEFINE_RVA(Offsets::HudManager::Instance);
		NetClient::Instance = *(NetClient**)DEFINE_RVA(Offsets::NetClient::Instance);
		ObjectManager::Instance = (ObjectManager*)(*(DWORD*)(baseAddr + (DWORD)Offsets::ObjectManager::Instance));
		ObjectManager::Player = *(GameObject**)DEFINE_RVA(Offsets::ObjectManager::Player);

		ManagerTemplate::AIBases = *reinterpret_cast<SEntityList<GameObject>**>(baseAddr + (DWORD)Offsets::ManagerTemplate::AIBases);
		ManagerTemplate::Turrets = *reinterpret_cast<SEntityList<GameObject>**>(baseAddr + (DWORD)Offsets::ManagerTemplate::Turrets);
		ManagerTemplate::Inhibitor = *reinterpret_cast<SEntityList<GameObject>**>(baseAddr + (DWORD)Offsets::ManagerTemplate::Inhibitor);
		ManagerTemplate::Minions = *reinterpret_cast<SEntityList<GameObject>**>(baseAddr + (DWORD)Offsets::ManagerTemplate::Minions);
		ManagerTemplate::Heroes = *reinterpret_cast<SEntityList<GameObject>**>(baseAddr + (DWORD)Offsets::ManagerTemplate::Heroes);
	}
}