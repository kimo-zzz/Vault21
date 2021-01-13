#include "Callbacks.h"


#include "ExampleAppLog.h"
#include "Globals.h"
#include "Orbwalker.h"
#include "TargetSelector.h"

ExampleAppLog AppLog;

/// <summary>
/// Recognizes all Created Objects.
/// </summary>
/// <param name="obj">Object which has been created.</param>
/// <returns></returns>
int __fastcall hk_OnCreateObject(CObject* obj, void* edx, unsigned id)
{

	
	if (obj == nullptr)
		return 0;
	
	if (g_debug_cacheOnCreate)
	{
		if (obj->IsMissile() || obj->IsHero() || obj->IsMinion() || obj->IsTurret() || obj->IsInhibitor())
		{
			AppLog.AddLog(("OnCreateObject: " + std::string(obj->GetName()) + " \n").c_str());
		}
	}
	
	return Functions.OnCreateObject_h(obj, id);
}

/// <summary>
/// Recognizes all Deleted Objects.
/// </summary>
/// <param name="obj">Object which will be deleted</param>
/// <returns></returns>
int __fastcall hk_OnDeleteObject(void* thisPtr, void* edx, CObject* obj)
{
	if (obj == nullptr)
		return 0;

	if (g_debug_cacheOnDelete)
		if (obj->IsMissile() || obj->IsHero() || obj->IsMinion() || obj->IsTurret() || obj->IsInhibitor())
		{
			AppLog.AddLog(("OnDeleteObject: " + std::string(obj->GetName()) + " \n").c_str());
		}

	return Functions.OnDeleteObject_h(thisPtr, obj);
}

//Recognizes all new upcoming paths
int hk_OnNewPath(CObject* obj, Vector* start, Vector* end, Vector* tail, int unk1, float* dashSpeed, unsigned dash, int unk3, char unk4, int unk5, int unk6, int unk7)
{
	if (obj == nullptr)
		return 0;

	if (obj->IsHero())
	{
		auto isDash = dash != 1;
		auto speed = *dashSpeed != 0.0f ? *dashSpeed : obj->GetObjMoveSpeed();
		auto isDash_str = (isDash ? "true" : "false");

		AppLog.AddLog(
			("OnNewPath: " + std::string(obj->GetName()) + ", Speed " + std::to_string(speed) + " \n").c_str());
		AppLog.AddLog(("\t\t\tisDash: " + std::string(isDash_str) + " \n").c_str());
		AppLog.AddLog(
			("\t\t\tVectorstart X: " + std::to_string(start->X) + " Y: " + std::to_string(start->Y) + " Z: " +
				std::to_string(start->Z) + " \n").c_str());

	}
	return Functions.OnNewPath_h(obj, start, end, tail, unk1, dashSpeed, dash, unk3, unk4, unk5, unk6, unk7);
}

/// <summary>
/// Callback which recognizes Spells/Missiles before they have been Created.
/// </summary>
/// <param name="spellInfo">SpellInfo Instance of currently processed Spell</param>
/// <returns></returns>
int __fastcall hk_OnProcessSpell(void* spellBook, void* edx, SpellInfo* spellInfo)
{
	if (spellInfo == nullptr)
		return 0;

	SpellInfo* derefSpellInfo = (SpellInfo*)*(DWORD*)(spellInfo);
	short casterIndex = *(short*)((DWORD)spellBook + oSpellBookOwner);
	CObject* caster = Engine::FindObjectByIndex(heroList, casterIndex);

	if (caster)
	{

		if (caster->IsValidHero(heroList))
		{
			if (g_debug_cacheOnProcessSpell)
			{
				AppLog.AddLog(
					(std::string(caster->GetChampionName()) + " : " + std::string(
						derefSpellInfo->GetSpellData()->GetSpellName2()) + " \n").c_str());
			}

		}

	}

	return Functions.OnProcessSpell_h(spellBook, spellInfo);
}
