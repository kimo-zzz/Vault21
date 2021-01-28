#pragma once
#include "stdafx.h"
#include <Windows.h>
#include "Utils.h"
#include "Vector.h"
#include "Vectorlong.h"
#include "Offsets.h"
#include "SpellInfo.h"
#include "SpellSlot.h"
#include "BuffManager.h"
#include "fnv_hash.hpp"
#include "encryption.hpp"
#include "SpellCastInfo.h"
#include "UnitComponentInfo.h"


enum class ObjectTypeFlags
{
	GameObject = (1 << 0),
	//0x1
	NeutralCamp = (1 << 1),
	//0x2
	DeadObject = (1 << 4),
	//0x10
	InvalidObject = (1 << 5),
	//0x20
	AIBaseCommon = (1 << 7),
	//0x80
	AttackableUnit = (1 << 9),
	//0x200
	AI = (1 << 10),
	//0x400
	Minion = (1 << 11),
	//0x800
	Hero = (1 << 12),
	//0x1000
	Turret = (1 << 13),
	//0x2000
	Unknown0 = (1 << 14),
	//0x4000
	Missile = (1 << 15),
	//0x8000
	Unknown1 = (1 << 16),
	//0x10000
	Building = (1 << 17),
	//0x20000
	Unknown2 = (1 << 18),
	//0x40000
};

class riot_string
{
public:
	const char* str;
	size_t length;
	size_t capacity;
};

class character_data
{
public:
	riot_string model;
	int32_t skin;
	PAD(0x20);
	bool update_spells;
	bool dont_update_hud;
	bool change_particle;
	PAD(0x1);
	PAD(0xC);
};

class character_data_stack
{
public:
	std::vector<character_data> stack;
	character_data base_skin;

	void update(bool change);
	void push(const char* model, int32_t skin);
};

struct champion_skin_data
{
	int skin_id; //0x0000
	char* skin_name; //0x0004
};

class champion_skin
{
public:
	std::vector<champion_skin_data> list_skins()
	{
		std::vector<champion_skin_data> _temp;
		bool isEnd = false;
		DWORD i = 0x0;
		do
		{
			if (*(int*)((DWORD)this + i) == 0 && i != 0x0)
			{
				isEnd = true;
			}
			else
			{
				champion_skin_data skin_data = {};
				skin_data.skin_id = *(int*)((DWORD)this + i);
				skin_data.skin_name = GetStr((DWORD)this + (i + 0x4));
				_temp.push_back(skin_data);
			}
			i += 0x10;
		} while (!isEnd);
		return _temp;
	}
};

class champion_data
{
public:
	char* get_champion_name()
	{
		return GetStr((DWORD)this + oGetChampionName);
	}

	champion_skin* get_champion_skin()
	{
		return *(champion_skin**)((DWORD)this + oGetChampionSkins);
	}
};

class champion_manager
{
public:
	std::vector<champion_data*> get_champion_data_ptr()
	{
		std::vector<champion_data*> _temp;
		for (int i = 0; i <= oChampionCount; i++)
		{
			_temp.push_back(*(champion_data**)((DWORD)this + (i * 0x4)));
		}
		return _temp;
	}
};

class champion_manager_ptr
{
public:
	champion_manager* get_champion_manager()
	{
		return *(champion_manager**)((DWORD)this + oChampionManagerChampionList);
	}
};

class CObject
{
public:

	bool IsTurret();
	bool IsMinion();
	bool IsDragon();
	bool IsAlive();
	bool IsHero();
	bool IsMissile();
	bool IsNexus();

	bool IsInhibitor();
	bool IsTroyEnt();

	bool IsTargetable();
	bool IsDeletedObject();
	bool IsInvalidObject();

	Vector GetHpBarPosition();
	CObject* GetOwner();
	bool is_lane_minion();
	character_data_stack* get_character_data_stack();

	bool CObject::CompareObjectTypeFlags(int objectTypeFlag)
	{
		unsigned __int8* v2; // edi
		unsigned int v3; // edx
		unsigned int v4; // esi
		int* v5; // ecx
		int v6; // eax
		unsigned __int8 v7; // al
		unsigned int v8; // eax
		unsigned __int8* v9; // edx
		char v10; // cl
		int objectId; // [esp+8h] [ebp-4h]

		v2 = (unsigned __int8*)this;
		v3 = 0;
		v4 = *static_cast<BYTE*>(v2 + 81);

		objectId = *(DWORD*)&v2[4 * v2[88] + 92];
		if (v4)
		{
			v5 = (int*)(this + 84);
			do
			{
				v6 = *v5;
				++v5;
				*(&objectId + v3) ^= ~v6;
				++v3;
			} while (v3 < v4);
		}
		v7 = v2[82];
		if (v7)
		{
			v8 = 4 - v7;
			if (v8 < 4)
			{
				v9 = &v2[v8 + 84];
				do
				{
					v10 = *v9++;
					*((BYTE*)&objectId + v8++) ^= ~v10;
				} while (v8 < 4);
			}
		}

		return (objectId & objectTypeFlag) != 0;
	}

	CObject* GetFirstObject()
	{
		typedef CObject* (__thiscall* fnGetFirst)(void*);
		return ((fnGetFirst)(baseAddr + oGetFirstObject))(*(void**)(baseAddr + oObjManager));
	}

	CObject* GetNextObject(CObject* object)
	{
		typedef CObject* (__thiscall* fnGetNext)(void*, CObject*);
		return ((fnGetNext)(baseAddr + oGetNextObject))(*(void**)(baseAddr + oObjManager), object);
	}

	CObject* GetParent(std::list<CObject*> heroList)
	{
		for (CObject* a : heroList)
		{
			if (this->GetObjSourceIndex() == a->GetIndex())
				return a;
		}
		return nullptr;
	}

	bool IsValidHero(std::list<CObject*> heroList)
	{
		for (CObject* a : heroList)
		{
			if ((DWORD)this == (DWORD)a)
				return true;
		}
		return false;
	}

	bool IsValidSpell(DWORD spellInfo)
	{
		if (!spellInfo)
			return false;

		for (int i = 0; i <= 64; i++)
		{
			if ((DWORD)this->GetSpellSlotByID(i)->GetSpellInfo() == spellInfo)
			{
				return true;
			}
		}
		return false;
	}

	static CObject* GetSpellOwner(std::string spellName, std::list<CObject*> heroList)
	{
		transform(spellName.begin(), spellName.end(), spellName.begin(), ::tolower);
		for (CObject* a : heroList)
		{
			char* name = a->GetChampionName();
			std::string name_str = std::string(name);
			transform(name_str.begin(), name_str.end(), name_str.begin(), ::tolower);
			if (spellName.find(name_str.c_str()) != std::string::npos)
				return a;
		}
		return nullptr;
	}

	DWORD GetThis()
	{
		return (DWORD)this;
	}

	short GetObjSourceIndex()
	{
		return *(short*)((DWORD)this + oObjSourceIndex);
	}

	float GetObjMoveSpeed()
	{
		return *(float*)((DWORD)this + oObjMovementSpeed);
	}

	short GetIndex()
	{
		return *(short*)((DWORD)this + oObjIndex);
	}

	DWORD GetNetworkID()
	{
		return *(DWORD*)((DWORD)this + oObjNetworkID);
	}

	Vector GetPos()
	{
		return *(Vector*)((DWORD)this + oObjPos);
	}

	VectorLong GetPosL()
	{
		return *(VectorLong*)((DWORD)this + oObjPos);
	}

	int GetLevel()
	{
		return *(int*)((DWORD)this + oObjLevel);
	}

	bool IsOnScreen()
	{
		return reinterpret_cast<xor_value<bool>*>((DWORD)this + oObjIsOnScreen)->decrypt();
	}

	float GetHealth()
	{
		return *(float*)((DWORD)this + oObjHealth);
	}

	float GetBaseAttackDamage()
	{
		return *(float*)((DWORD)this + oObjBaseAtk);
	}


	float GetBonusAttackDamage()
	{
		return *(float*)((DWORD)this + oObjBonusAtk);
	}

	float GetTotalAttackDamage()
	{
		return this->GetBonusAttackDamage() + this->GetBaseAttackDamage();
	}

	float GetArmor()
	{
		return *(float*)((DWORD)this + oObjArmor);
	}

	float GetMaxHealth()
	{
		return *(float*)((DWORD)this + oObjHealth + 0x10);
	}

	float GetAttackRange()
	{
		return *(float*)((DWORD)this + oObjAtkRange);
	}

	bool IsVisible()
	{
		return *(bool*)((DWORD)this + oObjVisibility);
	}

	float GetBoundingRadius()
	{
		typedef float(__thiscall* OriginalFn)(PVOID);
		return CallVirtual<OriginalFn>(this, 36)(this);
	}

	bool IsEnemyTo(CObject* Obj)
	{
		if (Obj->GetTeam() == 100 && this->GetTeam() == 200)
			return true;
		if (Obj->GetTeam() == 200 && this->GetTeam() == 100)
			return true;

		return false;
	}

	bool IsTeammateTo(CObject* Obj)
	{
		if (Obj->GetTeam() == this->GetTeam())
			return true;

		return false;
	}

	bool Isjungle(CObject* Obj)
	{
		if (Obj->GetTeam() == 300 && this->GetTeam() == 100)
			return true;
		return false;
	}


	char* GetName()
	{
		return GetStr((DWORD)this + oObjName);
	}

	char* GetChampionName()
	{
		return GetStr((DWORD)this + oObjChampionName);
	}

	int GetTeam()
	{
		return *(int*)((DWORD)this + oObjTeam);
	}

	BuffManager* GetBuffMgr()
	{
		return (BuffManager*)((DWORD)this + oObjBuffMgr);
	}

	float GetAttackDelay();

	float GetAttackCastDelay();

	float GetDistance(CObject* target)
	{
		return sqrtf(
			powf(this->GetPos().X - target->GetPos().X, 2) + powf(this->GetPos().Y - target->GetPos().Y, 2) + powf(
				this->GetPos().Z - target->GetPos().Z, 2));
	}

	SpellSlot* GetSpellSlotByID(int ID)
	{
		return *(SpellSlot**)((DWORD)this + (oObjSpellBook + 0x478) + (0x4 * ID));
	}

	SpellInfo* GetMissileSpellInfo()
	{
		return *(SpellInfo**)((DWORD)this + oMissileSpellInfo);
	}

	SpellCastInfo* GetSpellCastInfo()
	{
		return *(SpellCastInfo**)((DWORD)this + oActive_Spell_Entry);
	}

	UnitComponentInfo* GetUnitComponentInfo()
	{
		return *(UnitComponentInfo**)((DWORD)this + oUnitComponentInfo);
	}

	float GetAtkSpeedMod()
	{
		return *(float*)((DWORD)this + oObjAtkSpeedMod);
	}

	float GetTotalAtkSpeed()
	{
		return this->GetAtkSpeedMod() * this->GetUnitComponentInfo()->GetUCIProperties()->GetBaseAtkSpeed();
	}

	Vector GetSpellEndPos()
	{
		Vector spellEndPos = *(Vector*)((DWORD)this + oMissileEndPos);
		spellEndPos.Y += 100;
		return spellEndPos;
	}

	Vector GetSpellStartPos()
	{
		Vector startPos = *(Vector*)((DWORD)this + oMissileStartPos);
		startPos.Y += 100;
		return startPos;
	}

	void change_skin(const char* model, int32_t skin)
	{
		//Update skinid in object class to appear name in scoreboard and fix for some things
		//
		reinterpret_cast<xor_value<int32_t>*>(std::uintptr_t(this) + SkinId)->encrypt(skin);

		this->get_character_data_stack()->base_skin.skin = skin;

		//Lux has same skinid but diff model we have to handle it, game receives packets and calls Push function to change skin we do same but don't pushing new class but modify existing
		//
		if (fnv::hash_runtime(this->get_character_data_stack()->base_skin.model.str) == FNV("Lux"))
		{
			if (skin == 7)
			{
				if (this->get_character_data_stack()->stack.empty())
				{
					this->get_character_data_stack()->push(model, skin);
					return;
				}
				auto& last = this->get_character_data_stack()->stack.back();
				last.skin = skin;
				last.model.str = model;
				last.model.length = strlen(model);
				last.model.capacity = last.model.length + 1;
			}
			else
			{
				//Make sure that stack for lux is cleared
				//
				this->get_character_data_stack()->stack.clear();
			}
		}

		this->get_character_data_stack()->update(true);
	}

	float GetInhiRemainingRespawnTime()
	{
		return *(float*)((DWORD)this + oInhiRemainingRespawnTime);
	}
};
