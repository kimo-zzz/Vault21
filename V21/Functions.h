#pragma once

#include "NetClient.h"
#include "CharacterDataStack.h"
#include "GameObject.h"

namespace HACKUZAN {
	typedef float(__thiscall* fnGetGameTime)(DWORD* frameClock);
	typedef const char* (__cdecl* fnTranslateString)(const char* str);
	typedef unsigned int(__thiscall* fnGetPing)(NetClient* netClient);
	typedef int(__thiscall* fnChangerCharacterData)(CharacterDataStack* characterDataStack, const char* skinName, int skinId, bool a4, bool a5, bool a6, bool a7, bool a8, bool a9);
	typedef int(__thiscall* fnIssueOrder)(GameObject* unit, GameObjectOrder order, Vector3& position, GameObject* target, bool attackMove, bool a6, bool a7);
	typedef void(__thiscall* fnCastSpell)(SpellbookClient* spellbook, SpellDataInst* spell, kSpellSlot slot, Vector3& endPosition, Vector3& startPosition, unsigned int target_network_id);
	typedef float(__cdecl* fnGetAttackCastDelay)(GameObject* unit, kSpellSlot slot);
	typedef float(__cdecl* fnGetAttackDelay)(GameObject* unit);
	typedef bool(__cdecl* fnWorldToScreen)(Vector3& position, Vector3& out);

	typedef NavGridCell* (__thiscall* fnGetCell)(NavGrid* navigationMesh, Vector3& position);
	typedef float(__thiscall* fnGetHeightForPosition)(NavGrid* navigationMesh, float x, float z);

	typedef SpellCastInfo* (__thiscall* fnGetBasicAttack)(GameObject* thisptr, int LeaveMeEmpty);

	typedef int(__thiscall* fnOnProcessSpellW)(void* spellBook, SpellInfo* spellData);
	typedef int(__cdecl* fnOnProcessSpell)(void* spellbookPtr, signed int spellslot, int a3, SpellDataResource* pSpellData, char IsBasicAttack, char a6);
	typedef int(__thiscall* fnOnFinishCast)(SpellCastInfo* ptr, GameObject* obj);
	typedef void(__thiscall* fnOnStopCast)(SpellCastInfo* spellCaster_Client, bool stopAnimation, bool* executeCastFrame, bool forceStop, bool destroyMissile, unsigned int missileNetworkID);

	typedef int(__thiscall* fnOnPlayAnimation)(GameObject* ptr, bool* ret, char* name, int unk1, unsigned int unk2, float animationTime, int unk4);

	typedef int(__thiscall* fnCreateObject)(GameObject* obj, unsigned int NetworkID);
	typedef int(__thiscall* fnDeleteObject)(void* thisPtr, GameObject* pObject);

	typedef int(__thiscall* fnCreatePath)(PathControllerCommon* thisPtr, NavigationPath* navpath, Vector3 from, Vector3 to);
	typedef int(__cdecl* fnOnNewPath)(GameObject* obj, Vector3* start, Vector3* end, Vector3* tail, int unk1,
		float* dashSpeed, unsigned dash, int unk3, char unk4, int unk5, int unk6,
		int unk7);

	typedef bool(__cdecl* fnIsHero)(GameObject* pObj);
	typedef bool(__cdecl* fnIsMissile)(GameObject* pObj);
	typedef bool(__cdecl* fnIsMinion)(GameObject* pObj);
	typedef bool(__cdecl* fnIsTurret)(GameObject* pObj);
	typedef bool(__cdecl* fnIsInhibitor)(GameObject* pObj);
	typedef bool(__cdecl* fnIsTroyEnt)(GameObject* pObj);
	typedef bool(__cdecl* fnIsNexus)(GameObject* pObj);

	typedef bool(__thiscall* fnIsAlive)(GameObject* pObj);
	typedef bool(__thiscall* fnIsTargetable)(GameObject* pObj);

	namespace Functions {
		extern fnGetGameTime GetGameTime;
		extern fnTranslateString TranslateString;
		extern fnGetPing GetPing;
		extern fnChangerCharacterData ChangerCharacterData;
		extern fnIssueOrder IssueOrder;
		extern fnCastSpell CastSpell;
		extern fnGetAttackCastDelay GetAttackCastDelay;
		extern fnGetAttackDelay GetAttackDelay;
		extern fnWorldToScreen WorldToScreen;

		extern fnGetCell GetCell;
		extern fnGetHeightForPosition GetHeightForPosition;

		extern fnGetBasicAttack GetBasicAttack;

		extern fnOnProcessSpellW OnProcessSpellW;
		extern fnOnProcessSpell OnProcessSpell;
		extern fnOnFinishCast OnFinishCast;
		extern fnOnStopCast OnStopCast;
		extern fnOnPlayAnimation OnPlayAnimation;
		extern fnCreateObject OnCreateObject;
		extern fnDeleteObject OnDeleteObject;

		extern fnCreatePath CreatePath;
		extern fnOnNewPath OnNewPath;

		extern fnIsTurret IsTurret;
		extern fnIsMinion IsMinion;
		extern fnIsHero IsHero;
		extern fnIsMissile IsMissile;
		extern fnIsTroyEnt IsTroyEnt;
		extern fnIsNexus IsNexus;
		extern fnIsInhibitor IsInhibitor;

		extern fnIsTargetable IsTargetable;
		extern fnIsAlive IsAlive;

		void Initialize();
	}
}