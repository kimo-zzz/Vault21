#include "stdafx.h"
#include "Globals.h"
#include "Functions.h"
#include "DeviceHandler.h"
#include "RenderLayer.h"
#include "Instances.h"
#include "Renderer.h"
#include "Hooks.h"
#include "Events.h"
#include "Plugins.h"
#include "Menu.h"
#include "SpellDB.h"
#include "LeagueHooks.h"
#include "EventManager.h"
#include "Functions.h"
#include "ManagerTemplate.h"
#include "HeavensGateHook.h"

CConsole Console;
using namespace HACKUZAN;
using namespace HACKUZAN::SDK;

bool Triggered = true;
bool Ready = false;
FuncHook GetThreadContextHook;
LeagueHooksVEH LeagueHookVEH;
LeagueHooksHWBP LeagueHookHWBP;		

DWORD WINAPI InitThread(LPVOID);
std::atomic_bool running;
uintptr_t initThreadHandle;
HMODULE g_hModule;

void OnExit() noexcept;

__declspec(dllexport, naked) void __stdcall unpackman() {
	__asm pushfd
	__asm pushad
	__asm push 0
	__asm push 1
	__asm push unpackman
	__asm call unpackman
	__asm popad
	__asm popfd
	__asm ret
}

/*template< typename T >
std::string hexify(T i)
{
	std::stringbuf buf;
	std::ostream os(&buf);


	os << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;

	return buf.str().c_str();
}*/


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

		if (Triggered) {
			Triggered = false;

			//RestoreZwQueryInformationProcess();

			for (int i = 5; i > 0; i--) { // give some time to finish loading client
				Sleep(1000);
				GameClient::PrintChat(("Hooks ready in: " + std::to_string(i)).c_str(), IM_COL32(255, 69 + (i * 100), 0, 255));
			}
			Sleep(2000);
			GameClient::PrintChat("Recalling to decrypt OnProcessSpell...", IM_COL32(122, 69, 0, 122));
			//ObjectManager::Player->CastSpellPos(kSpellSlot::SpellSlot_Recall, (DWORD)ObjectManager::Player, ObjectManager::Player->Position);// recall to trigger onprocessspell 1 time before hooking

			GameClient::PrintChat("Processing the recall....", IM_COL32(134, 69, 0, 134));
			Sleep(2000);
			ObjectManager::Player->IssueOrder(GameObjectOrder::MoveTo, &Vector3(ObjectManager::Player->Position.X, ObjectManager::Player->Position.Y - 10, ObjectManager::Player->Position.Z));
			GameClient::PrintChat("Getting ready for Hooks...", IM_COL32(200, 69, 0, 255));
			Sleep(2000);
			Ready = true;
			GameClient::PrintChat("Hooks ready...", IM_COL32(220, 69, 0, 255));
			Sleep(2000);

			//EnableHeavensGateHook(); // WEAPONIZING THE HEAVEN'S GATE
		}

		auto _player = (GameObject*)*(DWORD*)DEFINE_RVA(Offsets::ObjectManager::Player);
		if (Ready && _player->Alive() && ClockFacade::GetGameTime() > 0) {
			if (!Triggered) {

				GameClient::PrintChat("Client worker trying to hook...", IM_COL32(220, 69, 0, 255));
				Sleep(2000);

				//OnPlayAnimation
				//if (LeagueHookVEH.addHook(DEFINE_RVA(Offsets::Functions::OnPlayAnimation), (DWORD)hk_OnPlayAnimation)) {
				//	GameClient::PrintChat("OnPlayAnimation1 Hook success!.", IM_COL32(255, 69, 0, 255));
				//	Sleep(3000);
					//OnProcessSpellW
					if (LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnProcessSpellW), (DWORD)hk_OnProcessSpell, 0)) {
						GameClient::PrintChat("OnProcessSpell Hook success!.", IM_COL32(255, 69, 0, 255));
						Sleep(3000);
						//Ready = false;
						//OnStopCast
						if (LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnStopCast), (DWORD)hk_OnStopCast, 1)) {
							GameClient::PrintChat("OnStopCast Hook success!.", IM_COL32(255, 69, 0, 255));
							Sleep(3000);
							//OnCreateObject
							if (LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnCreateObject), (DWORD)hk_OnCreateObject, 2)) {
								GameClient::PrintChat("OnCreateObject Hook success!.", IM_COL32(255, 69, 0, 255));
								Sleep(3000);
								//OnDeleteObject
								if (LeagueHookHWBP.addHook(DEFINE_RVA(Offsets::Functions::OnDeleteObject), (DWORD)hk_OnDeleteObject, 3)) {
									GameClient::PrintChat("OnDeleteObject Hook success!.", IM_COL32(255, 69, 0, 255));
									Sleep(3000);
									Ready = false;
									GameClient::PrintChat("Hooks Initialized~! : All credits goes to pakeke80 <3", IM_COL32(255, 69, 0, 255));
								}
								else
								{
									GameClient::PrintChat("OnDeleteObject Hook failed.", IM_COL32(255, 69, 0, 255));
								}
							}
							else
							{
								GameClient::PrintChat("OnCreateObject Hook failed.", IM_COL32(255, 69, 0, 255));
							}
						}
						else
						{
							GameClient::PrintChat("OnStopCast Hook failed.", IM_COL32(255, 69, 0, 255));
						}
					}
					else
					{
						GameClient::PrintChat("OnProcessSpell Hook failed.", IM_COL32(255, 69, 0, 255));
					}
				//}
				//else
				//{
				//	GameClient::PrintChat("OnPlayAnimation Hook failed.", IM_COL32(255, 69, 0, 255));
				//}
				//OnFinishCast
				//LeagueHookHWBP.addHook(oOnFinishCast_addr, (DWORD)hk_OnFinishCast, 1);
			}
		}
	}

	void OnGameUpdate() {


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
	if (!(*(DWORD*)DEFINE_RVA(Offsets::ObjectManager::Player)) && *(float*)(baseAddr + (DWORD)Offsets::ClockFacade::GameTime) < 1)
		return 0;

	Globals::Initialize();
	Functions::Initialize();
	DeviceHandler::Instance = *(DeviceHandler**)DEFINE_RVA(Offsets::DeviceHandler::Instance);
	GameClient::Instance = *(GameClient**)DEFINE_RVA(Offsets::GameClient::Instance);
	RenderLayer::Instance = *(RenderLayer**)DEFINE_RVA(Offsets::RenderLayer::Instance);
	Events::Initialize();
	SpellDb::Initialize();
	//GapClosersDB = new GapCloserDB;

	//LeagueDecrypt::IsMemoryDecrypted((PVOID)DEFINE_RVA(Offsets::Functions::OnProcessSpellW));
	//LeagueDecrypt::IsMemoryDecrypted((PVOID)DEFINE_RVA(Offsets::Functions::OnStopCast));
	//LeagueDecrypt::IsMemoryDecrypted((PVOID)DEFINE_RVA(Offsets::Functions::OnCreateObject));
	//LeagueDecrypt::IsMemoryDecrypted((PVOID)DEFINE_RVA(Offsets::Functions::OnDeleteObject));

	std::vector<PVECTORED_EXCEPTION_HANDLER> PVECTORED_EXCEPTION_HANDLER_list = Process::GetAllHandlers();
	Process::RemoveAllHandlers();
	DWORD leoAddr = LeagueHooks::init();
	Process::ReAddAllHandlers(PVECTORED_EXCEPTION_HANDLER_list);

	int count = 0;
	running = true;
	HMODULE hModule = (HMODULE)module;

	//MessageBoxA(0, "Capturing Module...", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
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
	//MessageBoxA(0, "Try Capture...", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	do
	{
		hr = capture->TryCapture();
		if (FAILED(hr)) {
			//MessageBoxA(0, "Cannot Capture", "Fatal Error", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
		else if (SUCCEEDED(hr)) {
			//MessageBoxA(0, "Capture Success", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
	} while (FAILED(hr));

	while (running)
	{
		LeagueHook::LoadHooks();
	}

	LeagueHooks::deinit();
	capture->FreeCapture();
	return 0;
}

void OnExit() noexcept
{
	if (running)
	{
		running = false;
		WaitForSingleObject((HANDLE)initThreadHandle, INFINITE);
		CloseHandle((HANDLE)initThreadHandle);

		try
		{
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