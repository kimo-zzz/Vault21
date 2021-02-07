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
#include "EventManager.h"
#include "Functions.h"
#include "ManagerTemplate.h"
#include "HeavensGateHook.h"
#include "hideModule.h"
#include "LeagueFunctions.h"

CConsole Console;
using namespace HACKUZAN;
using namespace HACKUZAN::SDK;

bool Ready = true;

LeagueHooksHWBP LeagueHookHWBP;		

DWORD WINAPI InitThread(LPVOID);
std::atomic_bool running;
uintptr_t initThreadHandle;
HMODULE g_hModule;

void OnExit() noexcept;

DWORD oOnProcessSpell_addr, oOnCreateObject_addr, oOnDeleteObject_addr, oOnFinishCast_addr;

namespace LeagueHook {

	int __fastcall hk_OnCreateObject(GameObject* thisPtr, void* edx, unsigned int netId) {

		//GameClient::PrintChat("hk_OnCreateObject hooked!", IM_COL32(255, 69, 0, 255));

		//if (thisPtr != nullptr && !thisPtr->Missile()) {
			EventManager::Trigger(LeagueEvents::OnCreateObject, thisPtr, netId);
		//}

		if (thisPtr != nullptr && thisPtr->Missile()) {
			//MessageBoxA(0, ("Missile Adress " + hexify<DWORD>((DWORD)thisPtr)).c_str(), "", 0);
			EventManager::Trigger(LeagueEvents::OnCreateMissile, thisPtr, netId);
		}

		return Functions::OnCreateObject(thisPtr, netId);;
	}

	int __fastcall hk_OnDeleteObject(void* thisPtr, void* edx, GameObject* object) {

		//if (object == nullptr)
		//	return 0;
		//GameClient::PrintChat("hk_OnDeleteObject hooked!", IM_COL32(255, 69, 0, 255));

		//if (object != nullptr && !object->Missile()) {
			EventManager::Trigger(LeagueEvents::OnDeleteObject, object);
		//}

		if (object != nullptr && object->Missile()) {
			EventManager::Trigger(LeagueEvents::OnDeleteMissile, object);
		}

		return Functions::OnDeleteObject(thisPtr, object);;
	}

	int _fastcall hk_OnPlayAnimation(GameObject* ptr, void* edx, bool* ret, char name, int unk1, unsigned int unk2, float animationTime, int unk4) {

		EventManager::Trigger(LeagueEvents::OnPlayAnimation, ptr);

		return Functions::OnPlayAnimation(ptr, ret, name, unk1, unk2, animationTime, unk4);
	}
		
	int __fastcall hk_OnProcessSpell(void* spellBook, void* edx, SpellInfo* CastInfo) {
		//auto casterIndex = *(short*)((DWORD)spellBook + 0x1C);
		auto caster = ObjectManager::Instance->ObjectsArray[CastInfo->SourceId];

		auto spelldata = (SpellDataResource*)*(DWORD*)(*(DWORD*)CastInfo + (DWORD)Offsets::SpellData::Resource);

		//GameClient::PrintChat(("IsWindingUp" + to_string(CastInfo->IsWindingUp)).c_str(), IM_COL32(255, 69, 0, 255));
		//GameClient::PrintChat(("IsBasicAttack" + to_string(CastInfo->IsBasicAttack)).c_str(), IM_COL32(255, 69, 0, 255));
		//GameClient::PrintChat(("IsSpecialAttack" + to_string(CastInfo->IsSpecialAttack)).c_str(), IM_COL32(255, 69, 0, 255));
		//GameClient::PrintChat(("IsSpecialAttack" + to_string(CastInfo->IsInstantCast)).c_str(), IM_COL32(255, 69, 0, 255));
		//GameClient::PrintChat(("CantCancelWhileChanneling" + to_string(spelldata->CantCancelWhileChanneling)).c_str(), IM_COL32(255, 69, 0, 255));
		//GameClient::PrintChat(("ChannelIsInterruptedByAttacking" + to_string(spelldata->ChannelIsInterruptedByAttacking)).c_str(), IM_COL32(255, 69, 0, 255));

		EventManager::Trigger(LeagueEvents::OnProcessSpell, CastInfo, spelldata);

		if (caster->Hero()) {
			EventManager::Trigger(LeagueEvents::OnDoCast, CastInfo, spelldata);
		}
		if (!caster->Hero() && CastInfo->IsBasicAttack) {
			EventManager::Trigger(LeagueEvents::OnDoCast, CastInfo, spelldata);
		}

		//GameClient::PrintChat("hk_OnProcessSpell running~!", IM_COL32(255, 69, 0, 255));

		return Functions::OnProcessSpellW(spellBook, CastInfo);;
	}

	int __fastcall hk_OnFinishCast(SpellCastInfo* castInfo, void* edx, GameObject* object) {

		EventManager::Trigger(LeagueEvents::OnFinishCast, object, castInfo);

		return Functions::OnFinishCast(castInfo, object);
	}


	void __fastcall hk_OnStopCast(SpellCastInfo* spellCaster_Client, void* edx, bool stopAnimation, bool* executeCastFrame,
		bool forceStop, bool destroyMissile, unsigned int missileNetworkID) {

		auto caster = ObjectManager::Instance->ObjectsArray[spellCaster_Client->SourceId];
		//GameClient::PrintChat(("forceStop " + to_string(forceStop)).c_str(), IM_COL32(255, 69, 0, 255));

		auto sInfo = new StopCast;
		sInfo->stopAnimation = stopAnimation;
		sInfo->forceStop = forceStop;
		sInfo->executeCastFrame = executeCastFrame;
		sInfo->destroyMissile = destroyMissile;
		sInfo->missileNetworkID = missileNetworkID;
		//MessageBoxA(0, ("spellCaster_Client " + hexify<DWORD>((DWORD)spellCaster_Client)).c_str(), "", 0);
		//GameClient::PrintChat(caster->BaseCharacterData->SkinName, IM_COL32(255, 69, 0, 255));
		EventManager::Trigger(LeagueEvents::OnStopCast, caster, sInfo);

		//GameClient::PrintChat("hk_OnStopCast running~!", IM_COL32(255, 69, 0, 255));

		return Functions::OnStopCast(spellCaster_Client, stopAnimation, executeCastFrame,
			forceStop, destroyMissile, missileNetworkID);
	}

	void LoadHooks() {

		if (Ready) {
			oOnProcessSpell_addr = DEFINE_RVA(Offsets::Functions::OnProcessSpellW);
			oOnCreateObject_addr = DEFINE_RVA(Offsets::Functions::OnCreateObject);
			oOnDeleteObject_addr = DEFINE_RVA(Offsets::Functions::OnDeleteObject);
			oOnFinishCast_addr = DEFINE_RVA(Offsets::Functions::OnStopCast);

			LeagueDecrypt::_RtlDispatchExceptionAddress = find_RtlDispatchExceptionAddress();

			while (!LeagueDecrypt::_RtlDispatchExceptionAddress) {
				GameClient::PrintChat("Cannot find _RtlDispatchExceptionAddress. Retrying...", IM_COL32(255, 69, 0, 255));
				LeagueDecrypt::_RtlDispatchExceptionAddress = find_RtlDispatchExceptionAddress();
				Sleep(1000);
			}

			LeagueDecryptData ldd = LeagueDecrypt::decrypt(nullptr);

			//PIDManager _PIDManager;
			//Process::GetAllModules(_PIDManager.GetAowProcId());

			LeagueDecrypt::IsMemoryDecrypted((PVOID)oOnProcessSpell_addr);
			LeagueDecrypt::IsMemoryDecrypted((PVOID)oOnCreateObject_addr);
			LeagueDecrypt::IsMemoryDecrypted((PVOID)oOnDeleteObject_addr);
			LeagueDecrypt::IsMemoryDecrypted((PVOID)oOnFinishCast_addr);
			Sleep(1000);
			std::vector<PVECTORED_EXCEPTION_HANDLER> PVECTORED_EXCEPTION_HANDLER_list = Process::GetAllHandlers();
			Process::RemoveAllHandlers();
			DWORD leoAddr = LeagueHooks::init();
			Process::ReAddAllHandlers(PVECTORED_EXCEPTION_HANDLER_list);
			int i = 0;
			for (PVECTORED_EXCEPTION_HANDLER handler : PVECTORED_EXCEPTION_HANDLER_list) {
				//AppLog.AddLog(("Handler[" + to_string(i) + "]: " + hexify<DWORD>((DWORD)handler) + "\n").c_str());
				i++;
			}

			////////////////////////////////////////
			// PATCHING THE ISSUE ORDER RETCHECKS
			////////////////////////////////////////
			DWORD IssueOrderAddr = DEFINE_RVA(Offsets::Functions::IssueOrder);
			LeagueDecrypt::IsMemoryDecrypted((PVOID)IssueOrderAddr);
			Sleep(1000);

			size_t sizeIssueOrder = 0xFB0;
			DWORD EndIssueOrderAddr = IssueOrderAddr + 0xFB0;
			DWORD NewIssueOrder = LeagueFunctions::VirtualAllocateFunction(LeagueFunctions::NewIssueOrder, IssueOrderAddr, sizeIssueOrder);
			LeagueFunctions::CopyFunction((DWORD)LeagueFunctions::NewIssueOrder, IssueOrderAddr, sizeIssueOrder);
			LeagueFunctions::FixRellocation(IssueOrderAddr, EndIssueOrderAddr, (DWORD)LeagueFunctions::NewIssueOrder, sizeIssueOrder);
			LeagueFunctions::HookStartAndEndFunction(NewIssueOrder, sizeIssueOrder, 6, (DWORD)LeagueFunctions::NewIssueOrderStartHook, (DWORD)LeagueFunctions::NewIssueOrderEndHook, LeagueFunctions::IssueOrderStartHookGateway, LeagueFunctions::IssueOrderEndHookGateway);
			LeagueFunctions::IsDonePatchingIssueOrder = true;
			GameClient::PrintChat("IssueOrder is now patched", IM_COL32(255, 69, 0, 255));
			//////////////////////////////////////////
			// END PATCHING THE ISSUE ORDER RETCHECKS
			//////////////////////////////////////////

			//////////////////////////////////////////
			// PATCHING THE CAST SPELL RETCHECKS
			//////////////////////////////////////////
			DWORD CastSpellAddr = DEFINE_RVA(Offsets::Functions::CastSpell);
			LeagueDecrypt::IsMemoryDecrypted((PVOID)CastSpellAddr);
			Sleep(1000);

			size_t sizeCastSpell = 0x370;
			DWORD EndCastSpellAddr = CastSpellAddr + 0x370;
			DWORD NewCastSpell = LeagueFunctions::VirtualAllocateFunction(LeagueFunctions::NewCastSpell, CastSpellAddr, sizeCastSpell);
			LeagueFunctions::CopyFunction((DWORD)LeagueFunctions::NewCastSpell, CastSpellAddr, sizeCastSpell);
			LeagueFunctions::FixRellocation(CastSpellAddr, EndCastSpellAddr, (DWORD)LeagueFunctions::NewCastSpell, sizeCastSpell);
			LeagueFunctions::HookStartAndEndFunction(NewCastSpell, sizeCastSpell, 5, (DWORD)LeagueFunctions::NewCastSpellStartHook, (DWORD)LeagueFunctions::NewCastSpellEndHook, LeagueFunctions::CastSpellStartHookGateway, LeagueFunctions::CastSpellEndHookGateway);
			LeagueFunctions::IsDonePatchingCastSpell = true;
			GameClient::PrintChat("CastSpell is now patched", IM_COL32(255, 69, 0, 255));
			//////////////////////////////////////////
			// END PATCHING THE CAST SPELL RETCHECKS
			//////////////////////////////////////////

			GameClient::PrintChat("Client worker trying to hook...", IM_COL32(220, 69, 0, 255));
			Sleep(2000);

			bool isOnProcessHooked = LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnProcessSpellW), (DWORD)hk_OnProcessSpell, 0);
			while (!isOnProcessHooked) {
				GameClient::PrintChat("OnProcessSpell Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnProcessHooked = LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnProcessSpellW), (DWORD)hk_OnProcessSpell, 0);
				Sleep(1000);
			}
			GameClient::PrintChat("OnProcessSpell Hook success!.", IM_COL32(255, 69, 0, 255));

			bool isOnStopCastHooked = LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnStopCast), (DWORD)hk_OnStopCast, 1);
			while (!isOnStopCastHooked) {
				GameClient::PrintChat("OnStopCast Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnStopCastHooked = LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnStopCast), (DWORD)hk_OnStopCast, 1);
				Sleep(1000);
			}
			GameClient::PrintChat("OnStopCast Hook success!.", IM_COL32(255, 69, 0, 255));

			bool isOnCreateObjectHooked = LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnCreateObject), (DWORD)hk_OnCreateObject, 2);
			while (!isOnCreateObjectHooked) {
				GameClient::PrintChat("OnCreateObject Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnCreateObjectHooked = LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnCreateObject), (DWORD)hk_OnCreateObject, 2);
				Sleep(1000);
			}
			GameClient::PrintChat("OnCreateObject Hook success!.", IM_COL32(255, 69, 0, 255));

			bool isOnDeleteObjectHooked = LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnDeleteObject), (DWORD)hk_OnDeleteObject, 3);
			while (!isOnDeleteObjectHooked) {
				GameClient::PrintChat("OnDeleteObject Hook Failed! Retrying...", IM_COL32(255, 69, 0, 255));
				isOnDeleteObjectHooked = LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnDeleteObject), (DWORD)hk_OnDeleteObject, 3);
				Sleep(1000);
			}
			GameClient::PrintChat("OnDeleteObject Hook success!.", IM_COL32(255, 69, 0, 255));
			GameClient::PrintChat("Hooks Initialized~! : All credits goes to Vault21 Team <3", IM_COL32(255, 69, 0, 255));

			EnableHeavensGateHook(); // WEAPONIZING THE HEAVEN'S GATE
			Ready = false;
		}
	}
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{

	g_hModule = hModule;
	DisableThreadLibraryCalls(hModule);
	if (dwReason == DLL_PROCESS_ATTACH)
	{

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
	RestoreZwQueryInformationProcess();
	UnlinkModuleFromPEB(g_hModule);
	ErasePEHeader(g_hModule);
	EraseHeaders(g_hModule);

	while (!(*(DWORD*)DEFINE_RVA(Offsets::ObjectManager::Player)) && (*(float*)(baseAddr + (DWORD)Offsets::ClockFacade::GameTime) < 1))
		Sleep(1);

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

	LeagueHooks::deinit();
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