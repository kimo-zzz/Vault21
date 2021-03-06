#include "stdafx.h"
#include "Globals.h"
#include "Functions.h"
#include "DeviceHandler.h"
#include "RenderLayer.h"
#include "Instances.h"
#include "Renderer.h"
#include "Events.h"
#include "Plugins.h"
#include "Menu.h"
#include "SpellDB.h"
#include "LeagueHooks.h"
#include "UltimateHooks.h"
#include "EventManager.h"
#include "Functions.h"
#include "ManagerTemplate.h"
#include "HeavensGateHook.h"
#include "hideModule.h"
#include "LeagueFunctions.h"
#include "Orbwalker.h"
#include "CrashHandler.h"
#include "PIDManager.h"

CConsole Console;
using namespace V21;
using namespace V21::SDK;

bool Ready = true;

LeagueHooksHWBP LeagueHookHWBP;

DWORD WINAPI InitThread(LPVOID);
std::atomic_bool running;
uintptr_t initThreadHandle;
HMODULE g_hModule;

void OnExit() noexcept;

string dllPath;

PVOID NewOnProcessSpell, NewOnCreateObject, NewOnDeleteObject, NewOnFinishCast, NewOnStopCast, NewOnNewPath, NewCreatePath;

std::vector<string> gapcloser_targeted =
{
	"AkaliShadowDance", "Headbutt", "DianaTeleport", "IreliaGatotsu", "JaxLeapStrike", "JayceToTheSkies",
	"MaokaiUnstableGrowth", "MonkeyKingNimbus", "Pantheon_LeapBash", "PoppyHeroicCharge", "QuinnE",
	"XenZhaoSweep", "blindmonkqtwo", "FizzPiercingStrike", "RengarLeap"
};

std::vector<string> gapcloser_no_target =
{
	"AatroxQ", "GragasE", "GravesMove", "HecarimUlt", "JarvanIVDragonStrike", "JarvanIVCataclysm", "KhazixE",
	"khazixelong", "LeblancSlide", "LeblancSlideM", "LeonaZenithBlade", "UFSlash", "RenektonSliceAndDice",
	"SejuaniArcticAssault", "ShenShadowDash", "RocketJump", "slashCast"
};


std::vector<string> interruptible =
{
	"KatarinaR", "GalioIdolOfDurand", "Crowstorm", "Drain", "AbsoluteZero", "ShenStandUnited", "UrgotSwap2",
	"AlZaharNetherGrasp", "FallenOne", "Pantheon_GrandSkyfall_Jump", "VarusQ", "CaitlynAceintheHole",
	"MissFortuneBulletTime", "InfiniteDuress", "LucianR"
};

bool IsOnGapCloserSpell(string name) {

	for (auto targeted : gapcloser_targeted) {
		if (GameClient::StringEquals(targeted.c_str(), name.c_str(), TRUE)) {
			return true;
		}
	}

	for (auto nontargeted : gapcloser_no_target) {
		if (GameClient::StringEquals(nontargeted.c_str(), name.c_str(), TRUE)) {
			return true;
		}
	}

	return false;
}

bool IsOnInterruptibleSpell(string name) {

	for (auto interrupt : interruptible) {
		if (GameClient::StringEquals(interrupt.c_str(), name.c_str(), TRUE)) {
			return true;
		}
	}

	return false;
}

namespace LeagueHook {

	int __fastcall hk_OnCreateObject(GameObject* thisPtr, void* edx, unsigned int netId) {

		if (thisPtr == nullptr)
			return Functions::OnCreateObject(thisPtr, netId);
		//GameClient::PrintChat("hk_OnCreateObject hooked!", IM_COL32(255, 69, 0, 255));

		if (thisPtr->Missile()) {
			//MessageBoxA(0, ("Missile Adress " + hexify<DWORD>((DWORD)thisPtr)).c_str(), "", 0);
			EventManager::Trigger(LeagueEvents::OnCreateMissile, thisPtr);
		}
		else {
			EventManager::Trigger(LeagueEvents::OnCreateObject, thisPtr);
		}

		return Functions::OnCreateObject(thisPtr, netId);
	}

	int __fastcall hk_OnDeleteObject(void* thisPtr, void* edx, GameObject* object) {

		//GameClient::PrintChat("hk_OnDeleteObject hooked!", IM_COL32(255, 69, 0, 255));

		if (object == nullptr || thisPtr==nullptr)
			return Functions::OnDeleteObject(thisPtr, object);

		if (object->Missile()) {
			EventManager::Trigger(LeagueEvents::OnDeleteMissile, object);
		}
		else {
			EventManager::Trigger(LeagueEvents::OnDeleteObject, object);
		}

		return Functions::OnDeleteObject(thisPtr, object);
	}

	int _fastcall hk_OnPlayAnimation(GameObject* ptr, void* edx, bool* ret, char* name, int unk1, unsigned int unk2, float animationTime, int unk4) {

		if (ptr != nullptr)
			EventManager::Trigger(LeagueEvents::OnPlayAnimation, ptr, name, animationTime);

		return Functions::OnPlayAnimation(ptr, ret, name, unk1, unk2, animationTime, unk4);

	}

	int __fastcall hk_OnProcessSpell(void* spellBook, void* edx, SpellInfo* CastInfo) {

		//GameClient::PrintChat("hk_OnProcessSpell running~!", IM_COL32(255, 69, 0, 255));

		if (CastInfo == nullptr || spellBook == nullptr)
			return Functions::OnProcessSpellW(spellBook, CastInfo);

		auto caster = ObjectManager::Instance->ObjectsArray[CastInfo->SourceId];

		auto spelldata = (SpellDataResource*)*(DWORD*)(*(DWORD*)CastInfo + (DWORD)Offsets::SpellData::Resource);

		EventManager::Trigger(LeagueEvents::OnProcessSpell, CastInfo, spelldata);

		if (caster->Hero()) {
			EventManager::Trigger(LeagueEvents::OnDoCast, CastInfo, spelldata);
		}
		if (!caster->Hero() && CastInfo->IsAutoAttack()) {
			EventManager::Trigger(LeagueEvents::OnDoCast, CastInfo, spelldata);
		}

		if (CastInfo && caster && caster->IsEnemy()) {
			if (IsOnGapCloserSpell(spelldata->SpellName)) {
				if (CastInfo->TargetId == ObjectManager::Player->Id || Distance(CastInfo->EndPosition, ObjectManager::Player->Position) <= 300) {
					EventManager::Trigger(LeagueEvents::OnGapCloserSpell, CastInfo, spelldata);
				}
			}

			if (IsOnInterruptibleSpell(spelldata->SpellName)) {
				EventManager::Trigger(LeagueEvents::OnInterruptibleSpell, CastInfo, spelldata);
			}
		}

		return Functions::OnProcessSpellW(spellBook, CastInfo);
	}

	int __fastcall hk_OnFinishCast(SpellCastInfo* castInfo, void* edx, GameObject* object) {

		//GameClient::PrintChat("hk_OnFinishCast running~!", IM_COL32(255, 69, 0, 255));
		if (!object || !castInfo)
			return Functions::OnFinishCast(castInfo, object);

		EventManager::Trigger(LeagueEvents::OnFinishCast, castInfo, object);

		return Functions::OnFinishCast(castInfo, object);
	}


	void __fastcall hk_OnStopCast(SpellCastInfo* spellCaster_Client, void* edx, bool stopAnimation, bool* executeCastFrame,
		bool forceStop, bool destroyMissile, unsigned int missileNetworkID) {

		//GameClient::PrintChat("hk_OnStopCast running~!", IM_COL32(255, 69, 0, 255));

		if (spellCaster_Client == nullptr)
			return Functions::OnStopCast(spellCaster_Client, stopAnimation, executeCastFrame,
				forceStop, destroyMissile, missileNetworkID);

		auto caster = ObjectManager::Instance->ObjectsArray[spellCaster_Client->SourceId];
		//	GameClient::PrintChat(("forceStop " + to_string(forceStop)).c_str(), IM_COL32(255, 69, 0, 255));

		StopCast sInfo;
		sInfo.stopAnimation = stopAnimation;
		sInfo.forceStop = forceStop;
		sInfo.executeCastFrame = executeCastFrame;
		sInfo.destroyMissile = destroyMissile;
		sInfo.missileNetworkID = missileNetworkID;
		//MessageBoxA(0, ("spellCaster_Client " + hexify<DWORD>((DWORD)spellCaster_Client)).c_str(), "", 0);
		//GameClient::PrintChat(caster->BaseCharacterData->SkinName, IM_COL32(255, 69, 0, 255));
		EventManager::Trigger(LeagueEvents::OnStopCast, caster, sInfo);

		//GameClient::PrintChat("hk_OnStopCast running~!", IM_COL32(255, 69, 0, 255));

		return Functions::OnStopCast(spellCaster_Client, stopAnimation, executeCastFrame,
			forceStop, destroyMissile, missileNetworkID);
	}

	int hk_OnNewPath(GameObject* obj, Vector3* start, Vector3* end, Vector3* tail, int unk1, float* dashSpeed, unsigned dash, int unk3, char unk4, int unk5, int unk6, int unk7)
	{
		if (obj == nullptr)
			return Functions::OnNewPath(obj, start, end, tail, unk1, dashSpeed, dash, unk3, unk4, unk5, unk6, unk7);
		
		NewPath path;
		path.sender = obj;
		path.start = *start;
		path.end = *end;
		path.tail = *tail;
		path.dash = dash;
		path.dashSpeed = *dashSpeed;

		//	GameClient::PrintChat("hk_OnNewPath hooked!", IM_COL32(255, 69, 0, 255));
		EventManager::Trigger(LeagueEvents::OnNewPath, path);

		return Functions::OnNewPath(obj, start, end, tail, unk1, dashSpeed, dash, unk3, unk4, unk5, unk6, unk7);
	}

	void LoadHooks() {

		if (Ready) {

			//PIDManager _PIDManager;
			//Process::GetAllModules(_PIDManager.GetAowProcId());
#if _DEBUG
			CrashHandler::init();
#endif
			LeagueDecrypt::_RtlDispatchExceptionAddress = find_RtlDispatchExceptionAddress();

			while (!LeagueDecrypt::_RtlDispatchExceptionAddress) {
				GameClient::PrintChat("Cannot find _RtlDispatchExceptionAddress. Retrying...", IM_COL32(255, 69, 0, 255));
				LeagueDecrypt::_RtlDispatchExceptionAddress = find_RtlDispatchExceptionAddress();
				Sleep(1000);
			}

			LeagueDecryptData ldd = LeagueDecrypt::decrypt(nullptr);

			//////////////////////////////////////////
			// SETUP HOOKS
			//////////////////////////////////////////
			GameClient::PrintChat("Client worker trying to hook...", IM_COL32(220, 69, 0, 255));
			///////////////////////////////////////////////////////////
			DWORD NewOnDeleteObjectAddr = UltimateHooks::VirtualAllocateRegion(NewOnDeleteObject, DEFINE_RVA(Offsets::Functions::OnDeleteObject), 0x160);
			UltimateHooks::CopyRegion((DWORD)NewOnDeleteObject, DEFINE_RVA(Offsets::Functions::OnDeleteObject), 0x160);
			UltimateHooks::FixFuncRellocation(DEFINE_RVA(Offsets::Functions::OnDeleteObject), (DEFINE_RVA(Offsets::Functions::OnDeleteObject) + 0x160), (DWORD)NewOnDeleteObject, 0x160);
			Functions::OnDeleteObject = (fnDeleteObject)(NewOnDeleteObjectAddr);
			bool isOnDeleteObjectHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnDeleteObject), (DWORD)hk_OnDeleteObject, 5);
			while (!isOnDeleteObjectHooked) {
				GameClient::PrintChat("OnDeleteObject Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnDeleteObjectHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnDeleteObject), (DWORD)hk_OnDeleteObject, 5);
				Sleep(1000);
			}
			GameClient::PrintChat("OnDeleteObject Hook success!.", IM_COL32(255, 69, 0, 255));
			///////////////////////////////////////////////////////////
			DWORD NewOnNewPathAddr = UltimateHooks::VirtualAllocateRegion(NewOnNewPath, DEFINE_RVA(Offsets::Functions::OnNewPath), 0x300);
			UltimateHooks::CopyRegion((DWORD)NewOnNewPath, DEFINE_RVA(Offsets::Functions::OnNewPath), 0x300);
			UltimateHooks::FixFuncRellocation(DEFINE_RVA(Offsets::Functions::OnNewPath), (DEFINE_RVA(Offsets::Functions::OnNewPath) + 0x300), (DWORD)NewOnNewPath, 0x300);
			Functions::OnNewPath = (fnOnNewPath)(NewOnNewPathAddr);
			bool isOnNewPathHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnNewPath), (DWORD)hk_OnNewPath, 10);
			while (!isOnNewPathHooked) {
				GameClient::PrintChat("OnNewPath Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnNewPathHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnNewPath), (DWORD)hk_OnNewPath, 10);
				Sleep(1000);
			}
			GameClient::PrintChat("OnNewPath Hook success!.", IM_COL32(255, 69, 0, 255));
			///////////////////////////////////////////////////////////
			DWORD NewOnprocessSpellAddr = UltimateHooks::VirtualAllocateRegion(NewOnProcessSpell, DEFINE_RVA(Offsets::Functions::OnProcessSpellW), 0x60);
			UltimateHooks::CopyRegion((DWORD)NewOnProcessSpell, DEFINE_RVA(Offsets::Functions::OnProcessSpellW), 0x60);
			UltimateHooks::FixFuncRellocation(DEFINE_RVA(Offsets::Functions::OnProcessSpellW), (DEFINE_RVA(Offsets::Functions::OnProcessSpellW) + 0x60), (DWORD)NewOnProcessSpell, 0x60);
			Functions::OnProcessSpellW = (fnOnProcessSpellW)(NewOnprocessSpellAddr);
			bool isOnProcessSpellHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnProcessSpellW), (DWORD)hk_OnProcessSpell, 3);
			while (!isOnProcessSpellHooked) {
				GameClient::PrintChat("OnProcessSpell Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnProcessSpellHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnProcessSpellW), (DWORD)hk_OnProcessSpell, 3);
				Sleep(1000);
			}
			GameClient::PrintChat("OnProcessSpell Hook success!.", IM_COL32(255, 69, 0, 255));
			///////////////////////////////////////////////////////////
			DWORD NewOnFinishCastAddr = UltimateHooks::VirtualAllocateRegion(NewOnFinishCast, DEFINE_RVA(Offsets::Functions::OnFinishCast), 0x391060);
			UltimateHooks::CopyRegion((DWORD)NewOnFinishCast, DEFINE_RVA(Offsets::Functions::OnFinishCast), 0x391060);
			UltimateHooks::FixFuncRellocation(DEFINE_RVA(Offsets::Functions::OnFinishCast), (DEFINE_RVA(Offsets::Functions::OnFinishCast) + 0x391060), (DWORD)NewOnFinishCast, 0x391060);
			Functions::OnFinishCast = (fnOnFinishCast)(NewOnFinishCastAddr);
			bool isOnFinishCastHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnFinishCast), (DWORD)hk_OnFinishCast, 4);
			while (!isOnFinishCastHooked) {
				GameClient::PrintChat("OnFinishCast Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnFinishCastHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnFinishCast), (DWORD)hk_OnFinishCast, 4);
				Sleep(1000);
			}
			GameClient::PrintChat("OnFinishCast Hook success!.", IM_COL32(255, 69, 0, 255));
			///////////////////////////////////////////////////////////
			DWORD NewOnCreateObjectAddr = UltimateHooks::VirtualAllocateRegion(NewOnCreateObject, DEFINE_RVA(Offsets::Functions::OnCreateObject), 0xB0);
			UltimateHooks::CopyRegion((DWORD)NewOnCreateObject, DEFINE_RVA(Offsets::Functions::OnCreateObject), 0xB0);
			UltimateHooks::FixFuncRellocation(DEFINE_RVA(Offsets::Functions::OnCreateObject), (DEFINE_RVA(Offsets::Functions::OnCreateObject) + 0xB0), (DWORD)NewOnCreateObject, 0xB0);
			Functions::OnCreateObject = (fnCreateObject)(NewOnCreateObjectAddr);
			bool isOnCreateObjectHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnCreateObject), (DWORD)hk_OnCreateObject, 2);
			while (!isOnCreateObjectHooked) {
				GameClient::PrintChat("OnCreateObject Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnCreateObjectHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnCreateObject), (DWORD)hk_OnCreateObject, 2);
				Sleep(1000);
			}
			GameClient::PrintChat("OnCreateObject Hook success!.", IM_COL32(255, 69, 0, 255));
			///////////////////////////////////////////////////////////
			DWORD NewOnStopCastAddr = UltimateHooks::VirtualAllocateRegion(NewOnStopCast, DEFINE_RVA(Offsets::Functions::OnStopCast), 0x1A0);
			UltimateHooks::CopyRegion((DWORD)NewOnStopCast, DEFINE_RVA(Offsets::Functions::OnStopCast), 0x1A0);
			UltimateHooks::FixFuncRellocation(DEFINE_RVA(Offsets::Functions::OnStopCast), (DEFINE_RVA(Offsets::Functions::OnStopCast) + 0x1A0), (DWORD)NewOnStopCast, 0x1A0);
			Functions::OnStopCast = (fnOnStopCast)(NewOnStopCastAddr);
			bool isOnStopCastHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnStopCast), (DWORD)hk_OnStopCast, 3);
			while (!isOnStopCastHooked) {
				GameClient::PrintChat("OnStopCast Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnStopCastHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::OnStopCast), (DWORD)hk_OnStopCast, 3);
				Sleep(1000);
			}
			GameClient::PrintChat("OnStopCast Hook success!.", IM_COL32(255, 69, 0, 255));
			///////////////////////////////////////////////////////////
			/*
			DWORD NewCreatePathAddr = UltimateHooks::VirtualAllocateRegion(NewCreatePath, DEFINE_RVA(Offsets::Functions::CreatePath), 0x0);
			UltimateHooks::CopyRegion((DWORD)NewCreatePath, DEFINE_RVA(Offsets::Functions::CreatePath), 0x0);
			UltimateHooks::FixFuncRellocation(DEFINE_RVA(Offsets::Functions::CreatePath), (DEFINE_RVA(Offsets::Functions::CreatePath) + 0x0), (DWORD)NewCreatePath, 0x0);
			Functions::CreatePath = (fnCreatePath)(NewCreatePathAddr);
			bool isCreatePathHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::CreatePath), (DWORD)hk_CreatePath, 999);
			while (!isCreatePathHooked) {
				GameClient::PrintChat("CreatePath Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isCreatePathHooked = UltimateHooks::addHook(DEFINE_RVA(Offsets::Functions::CreatePath), (DWORD)hk_CreatePath, 999);
				Sleep(1000);
			}
			GameClient::PrintChat("CreatePath Hook success!.", IM_COL32(255, 69, 0, 255));
			*/
			//////////////////////////////////////////
			// END SETUP HOOKS
			//////////////////////////////////////////

			////////////////////////////////////////
			// PATCHING THE ISSUEORDER
			////////////////////////////////////////
			DWORD IssueOrderAddr = DEFINE_RVA(Offsets::Functions::IssueOrder);
			LeagueDecrypt::IsMemoryDecrypted((PVOID)IssueOrderAddr);

			size_t sizeIssueOrder = 0xFB0;
			DWORD EndIssueOrderAddr = IssueOrderAddr + 0xFB0;
			DWORD NewIssueOrder = LeagueFunctions::VirtualAllocateFunction(LeagueFunctions::NewIssueOrder, IssueOrderAddr, sizeIssueOrder);
			LeagueFunctions::CopyFunction((DWORD)LeagueFunctions::NewIssueOrder, IssueOrderAddr, sizeIssueOrder);
			UltimateHooks::FixFuncRellocation(IssueOrderAddr, EndIssueOrderAddr, (DWORD)LeagueFunctions::NewIssueOrder, sizeIssueOrder);
			LeagueFunctions::HookStartAndEndFunction(NewIssueOrder, sizeIssueOrder, 6, (DWORD)LeagueFunctions::NewIssueOrderStartHook, (DWORD)LeagueFunctions::NewIssueOrderEndHook, LeagueFunctions::IssueOrderStartHookGateway, LeagueFunctions::IssueOrderEndHookGateway);
			LeagueFunctions::IsDonePatchingIssueOrder = true;
			GameClient::PrintChat("IssueOrder is now patched", IM_COL32(255, 69, 0, 255));
			//////////////////////////////////////////
			// END PATCHING THE ISSUEORDER
			//////////////////////////////////////////

			//////////////////////////////////////////
			// PATCHING THE CASTSPELL
			//////////////////////////////////////////
			DWORD CastSpellAddr = DEFINE_RVA(Offsets::Functions::CastSpell);
			LeagueDecrypt::IsMemoryDecrypted((PVOID)CastSpellAddr);

			size_t sizeCastSpell = 0x370;
			DWORD EndCastSpellAddr = CastSpellAddr + 0x370;
			DWORD NewCastSpell = LeagueFunctions::VirtualAllocateFunction(LeagueFunctions::NewCastSpell, CastSpellAddr, sizeCastSpell);
			LeagueFunctions::CopyFunction((DWORD)LeagueFunctions::NewCastSpell, CastSpellAddr, sizeCastSpell);
			UltimateHooks::FixFuncRellocation(CastSpellAddr, EndCastSpellAddr, (DWORD)LeagueFunctions::NewCastSpell, sizeCastSpell);
			LeagueFunctions::HookStartAndEndFunction(NewCastSpell, sizeCastSpell, 5, (DWORD)LeagueFunctions::NewCastSpellStartHook, (DWORD)LeagueFunctions::NewCastSpellEndHook, LeagueFunctions::CastSpellStartHookGateway, LeagueFunctions::CastSpellEndHookGateway);
			LeagueFunctions::IsDonePatchingCastSpell = true;
			GameClient::PrintChat("CastSpell is now patched", IM_COL32(255, 69, 0, 255));
			//////////////////////////////////////////
			// END PATCHING THE CASTSPELL
			//////////////////////////////////////////

			//////////////////////////////////////////
			// PATCHING THE UpdateChargableSpell
			//////////////////////////////////////////
			DWORD UpdateChargableSpellAddr = DEFINE_RVA(Offsets::Functions::UpdateChargableSpell);
			LeagueDecrypt::IsMemoryDecrypted((PVOID)UpdateChargableSpellAddr);

			size_t sizeUpdateChargableSpell = 0x250;
			DWORD EndUpdateChargableSpellAddr = UpdateChargableSpellAddr + 0x250;
			DWORD NewUpdateChargableSpell = LeagueFunctions::VirtualAllocateFunction(LeagueFunctions::NewUpdateChargableSpell, UpdateChargableSpellAddr, sizeUpdateChargableSpell);
			LeagueFunctions::CopyFunction((DWORD)LeagueFunctions::NewUpdateChargableSpell, UpdateChargableSpellAddr, sizeUpdateChargableSpell);
			UltimateHooks::FixFuncRellocation(UpdateChargableSpellAddr, EndUpdateChargableSpellAddr, (DWORD)LeagueFunctions::NewUpdateChargableSpell, sizeUpdateChargableSpell);
			LeagueFunctions::HookStartAndEndFunction(NewUpdateChargableSpell, sizeUpdateChargableSpell, 4, (DWORD)LeagueFunctions::NewUpdateChargableSpellStartHook, (DWORD)LeagueFunctions::NewUpdateChargableSpellEndHook, LeagueFunctions::UpdateChargableSpellStartHookGateway, LeagueFunctions::UpdateChargableSpellEndHookGateway);
			LeagueFunctions::IsDonePatchingUpdateChargableSpell = true;
			GameClient::PrintChat("UpdateChargableSpell is now patched", IM_COL32(255, 69, 0, 255));
			//////////////////////////////////////////
			// END PATCHING THE UpdateChargableSpell
			//////////////////////////////////////////

			////////////////////////////////////////////////////////////
			GameClient::PrintChat("Hooks Initialized~! : All credits goes to Vault21 Team <3", IM_COL32(255, 69, 0, 255));
			Ready = false;

			ObjectManager::Player->CastSpell(kSpellSlot::SpellSlot_Recall, (DWORD)ObjectManager::Player);
		}
	}
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{

	g_hModule = hModule;
	DisableThreadLibraryCalls(hModule);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
#if _DEBUG
		char path[2048];
		GetModuleFileNameA(hModule, path, 2048);
		string s1(path);
		dllPath = s1.substr(0, s1.find_last_of("\\/"));
#endif

		std::atexit(OnExit);
		initThreadHandle = _beginthreadex(nullptr, 0, (_beginthreadex_proc_type)InitThread, hModule, 0, nullptr);

		FreeLibrary(g_hModule);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		OnExit();
	}

	return TRUE;
}


Capture* capture = nullptr;

extern DWORD WINAPI InitThread(LPVOID module);
__declspec(safebuffers)DWORD WINAPI InitThread(LPVOID module)
{
	//UnlinkModuleFromPEB(g_hModule);
	//ErasePEHeader(g_hModule);
	//EraseHeaders(g_hModule);

	while (!(*(DWORD*)DEFINE_RVA(Offsets::ObjectManager::Player)) && (*(float*)(baseAddr + (DWORD)Offsets::ClockFacade::GameTime) < 1))
		Sleep(1);

	RestoreRtlAddVectoredExceptionHandler();
	RestoreZwQueryInformationProcess();

	Globals::Initialize();
	Functions::Initialize();
	DeviceHandler::Instance = *(DeviceHandler**)DEFINE_RVA(Offsets::DeviceHandler::Instance);
	GameClient::Instance = *(GameClient**)DEFINE_RVA(Offsets::GameClient::Instance);
	RenderLayer::Instance = *(RenderLayer**)DEFINE_RVA(Offsets::RenderLayer::Instance);
	Events::Initialize();
	SpellDb::Initialize();

	int count = 0;
	running = true;
	HMODULE hModule = (HMODULE)module;

	Capture* captures[] = {
		Direct3D9Capture::singleton(),
		DXGICapture::singleton(),
	};

	do
	{
		capture = captures[count++ % _countof(captures)];
	} while (capture->GetCaptureModule() == nullptr && running);

	if (!running)
		return 0;
	HRESULT hr = 0;
	do
	{
		hr = capture->TryCapture();
		if (FAILED(hr)) {
		}
		else if (SUCCEEDED(hr)) {
		}
	} while (FAILED(hr));

	Sleep(3000); // Just give extra time after loading gui
	LeagueHook::LoadHooks();

	while (running)
	{

	}

	//LeagueHooks::deinit();
	UltimateHooks::deinit();
	capture->FreeCapture();
	return 0;
}

void OnExit() noexcept
{
	RelinkModuleToPEB(g_hModule);
	if (running)
	{
		running = false;
		WaitForSingleObject((HANDLE)initThreadHandle, INFINITE);
		CloseHandle((HANDLE)initThreadHandle);

		try
		{
			Direct3D9Capture::destroy_singleton();
			DXGICapture::destroy_singleton();
			Plugins::Dispose();
			Menu::Dispose();
			//Hooks::Dispose();
			Renderer::Dispose();
		}
		catch (std::exception&)
		{

		}
	}
}