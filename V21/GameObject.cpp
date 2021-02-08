#include "stdafx.h"
#include "GameObject.h"
#include "Functions.h"
#include "EventManager.h"
#include "ObjectManager.h"
#include "HashFunctions.h"
#include "HudManager.h"
#include "Damage.h"
#include "Orbwalker.h"
#include "GameClient.h"
#include "ClockFacade.h"
#include "LeagueFunctions.h"

namespace HACKUZAN
{
	int LastCastSpellTick = 0;

	bool GameObject::InFountain()
	{
		if (this->Position.X < 1276 && this->Position.Z < 1344) { return true; }
		if (this->Position.X > 13540 && this->Position.Z > 13470) { return true; }

		return false;
	}

	bool GameObject::IsUnderEnemyTurret(float range)
	{
		auto turret_list = HACKUZAN::GameObject::GetTurrets();
		for (size_t i = 0; i < turret_list->size; i++)
		{
			auto turret = turret_list->entities[i];

			if (turret->IsValidTarget() && turret->IsEnemy() && turret->Position.Distance(this->Position) <= range)
			{
				return true;
			}
		}
		return false;
	}

	float GameObject::GetBoundingRadius()
	{
		return reinterpret_cast<float(__thiscall*)(GameObject*)>(this->VTable[(DWORD)Offsets::GameObject::Virtual_GetBoundingRadius])(this);
		//typedef float(__thiscall* OriginalFn)(PVOID);
		//return CallVirtual<OriginalFn>(this, 36)(this);
	}

	PathControllerCommon* GameObject::GetPathController()
	{
		return reinterpret_cast<PathControllerCommon * (__thiscall*)(GameObject*)>(this->VTable[(DWORD)Offsets::GameObject::Virtual_GetPathController])(this);
		//typedef PathControllerCommon* (__thiscall* OriginalFn)(PVOID);
		//return CallVirtual<OriginalFn>(this, (DWORD)Offsets::GameObject::Virtual_GetPathController)(this);
	}

	Navigation* GameObject::AIManager() {
		typedef Navigation* (__thiscall* OriginalFn)(PVOID);
		return CallVirtual<OriginalFn>(this, (DWORD)Offsets::GameObject::Virtual_GetPathController)(this);
	}

	float GameObject::GetAttackCastDelay()
	{
		return Functions::GetAttackCastDelay(this, kSpellSlot::SpellSlot_BasicAttack);
	}

	float GameObject::GetAttackDelay()
	{
		return Functions::GetAttackDelay(this);
	}


	GameObject* GameObject::GetFirstObject()
	{
		typedef GameObject* (__thiscall* fnGetFirst)(void*);
		return ((fnGetFirst)(baseAddr + (DWORD)Offsets::ObjectManager::GetFirstObject))(*(void**)(baseAddr + (DWORD)Offsets::ObjectManager::Instance));
	}

	GameObject* GameObject::GetNextObject(GameObject* object)
	{
		typedef GameObject* (__thiscall* fnGetNext)(void*, GameObject*);
		return ((fnGetNext)(baseAddr + (DWORD)Offsets::ObjectManager::GetNextObject))(*(void**)(baseAddr + (DWORD)Offsets::ObjectManager::Instance), object);
	}


	void GameObject::CastSpell(kSpellSlot slot, DWORD Caster)
	{
		if ((!(DWORD)LeagueFunctions::NewCastSpell) || (!LeagueFunctions::IsDonePatchingCastSpell))
			return;

		if (this->Spellbook.GetSpellState(slot) == SpellState_Ready && ClockFacade::GameTickCount() - LastCastSpellTick >= 100) {

			SpellbookClient* spellbook = &this->Spellbook;
			auto pSpellInfo = this->Spellbook.GetSpell(slot);

			DWORD SpoofAddress = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress;
			DWORD CastSpellAddr = (DWORD)LeagueFunctions::NewCastSpell; //CastSpell

			Vector3* mePos = &this->Position;

			if (EventManager::TriggerProcess(LeagueEvents::OnCastSpell, spellbook, pSpellInfo, slot, mePos, mePos, 0x0)) {

				if (((*(DWORD*)SpoofAddress) & 0xFF) != 0xC3)
					return; //This isn't the instruction we're looking for

				__asm
				{
					push retnHere //address of our function,  
					mov ecx, spellbook //If the function is a __thiscall don't forget to set ECX
					push 0
					push mePos
					push mePos
					push slot
					push pSpellInfo
					push SpoofAddress
					jmp CastSpellAddr
					retnHere :
				}
			}
			LastCastSpellTick = ClockFacade::GameTickCount();
		}
	}


	void GameObject::CastSpellPos(kSpellSlot slot, DWORD Caster, Vector3 TargetPos)
	{
		if ((!(DWORD)LeagueFunctions::NewCastSpell) || (!LeagueFunctions::IsDonePatchingCastSpell))
			return;

		if (this->Spellbook.GetSpellState(slot) == SpellState_Ready && ClockFacade::GameTickCount() - LastCastSpellTick >= 100) {

			DWORD spellbook = Caster + (DWORD)Offsets::GameObject::Spellbook;
			SpellDataInst* pSpellInfo = this->Spellbook.GetSpell(slot);
			Vector3* mePos = &this->Position;
			Vector3* targetPos = &TargetPos;

			DWORD SpoofAddress = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress;
			DWORD CastSpellAddr = (DWORD)LeagueFunctions::NewCastSpell; //CastSpell

			if (((*(DWORD*)SpoofAddress) & 0xFF) != 0xC3)
				return; //This isn't the instruction we're looking for

			__asm
			{
				push retnHere //address of our function,  
				mov ecx, spellbook //If the function is a __thiscall don't forget to set ECX
				push 0
				push mePos
				push targetPos
				push slot
				push pSpellInfo
				push SpoofAddress
				jmp CastSpellAddr
				retnHere :
			}

			LastCastSpellTick = ClockFacade::GameTickCount();
		}
	}

	void GameObject::CastPredictSpell(kSpellSlot slot, Vector3 start_position, Vector3 end_position)
	{
		if ((!(DWORD)LeagueFunctions::NewCastSpell) || (!LeagueFunctions::IsDonePatchingCastSpell))
			return;

		if (this->Spellbook.GetSpellState(slot) == SpellState_Ready && ClockFacade::GameTickCount() - LastCastSpellTick >= 100) {

			SpellbookClient* spellbook = &this->Spellbook;
			auto pSpellInfo = this->Spellbook.GetSpell(slot);

			Vector3* _start_position = &start_position;
			Vector3* _end_position = &end_position;

			DWORD SpoofAddress = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress;
			DWORD CastSpellAddr = (DWORD)LeagueFunctions::NewCastSpell; //CastSpell

			if (EventManager::TriggerProcess(LeagueEvents::OnCastSpell, spellbook, pSpellInfo, slot, _end_position, _start_position, 0x0)) {

				if (((*(DWORD*)SpoofAddress) & 0xFF) != 0xC3)
					return; //This isn't the instruction we're looking for

				__asm
				{
					push retnHere //address of our function,  
					mov ecx, spellbook //If the function is a __thiscall don't forget to set ECX
					push 0
					push _start_position
					push _end_position
					push slot
					push pSpellInfo
					push SpoofAddress
					jmp CastSpellAddr
					retnHere :
				}
			}
			LastCastSpellTick = ClockFacade::GameTickCount();
		}
	}


	void GameObject::CastTargetSpell(kSpellSlot slot, DWORD Caster, DWORD Target, Vector3 StartPos, Vector3 TargetPos, DWORD netid)
	{
		if ((!(DWORD)LeagueFunctions::NewCastSpell) || (!LeagueFunctions::IsDonePatchingCastSpell))
			return;

		if (this->Spellbook.GetSpellState(slot) == SpellState_Ready && ClockFacade::GameTickCount() - LastCastSpellTick >= 100) {

			SpellbookClient* spellbook = &this->Spellbook;
			auto pSpellInfo = this->Spellbook.GetSpell(slot);

			DWORD SpoofAddress = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress;
			DWORD CastSpellAddr = (DWORD)LeagueFunctions::NewCastSpell; //CastSpell

			Vector3* mePos = &this->Position;

			GameObject* targetGO = (GameObject*)Target;
			Vector3* targetPos = &targetGO->Position;

			if (EventManager::TriggerProcess(LeagueEvents::OnCastSpell, spellbook, pSpellInfo, slot, targetPos, mePos, 0x0)) {

				if (((*(DWORD*)SpoofAddress) & 0xFF) != 0xC3)
					return; //This isn't the instruction we're looking for

				__asm
				{
					push retnHere //address of our function,  
					mov ecx, spellbook //If the function is a __thiscall don't forget to set ECX
					push netid
					push mePos
					push targetPos
					push slot
					push pSpellInfo
					push SpoofAddress
					jmp CastSpellAddr
					retnHere :
				}
			}
			LastCastSpellTick = ClockFacade::GameTickCount();
		}
	}


	void GameObject::IssueOrder(GameObjectOrder order, GameObject* target)
	{
		if ((!(DWORD)LeagueFunctions::NewIssueOrder) || (!LeagueFunctions::IsDonePatchingIssueOrder))
			return;

		DWORD retInstruction = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress; //Address of a return instruction
		DWORD IssueOrderAddr = (DWORD)LeagueFunctions::NewIssueOrder; //IssueOrder
		Vector3* pos = &target->Position;
		DWORD networkID = target->NetworkId;

		if (EventManager::TriggerProcess(LeagueEvents::OnIssueOrder, this, order, pos, target))
		{
			if (((*(DWORD*)retInstruction) & 0xFF) != 0xC3)
				return; //This isn't the instruction we're looking for

			__asm {
				push retnHere; //address of our function

				mov ecx, this; //If the function is a __thiscall don't forget to set ECX
				push 0 //Push args to league function, typically left to right 
					push 0  //Push args to league function, typically left to right 
					push 1  //Push args to league function, typically left to right 
					push target  //Push args to league function, typically left to right 
					push pos  //Push args to league function, typically left to right 
					push order  //Push args to league function, typically left to right 
					push retInstruction;
				jmp IssueOrderAddr; //Jump to league function
			retnHere:
			}
		}
	}

	void GameObject::IssueOrder(GameObjectOrder order, Vector3* position)
	{
		if ((!(DWORD)LeagueFunctions::NewIssueOrder) || (!LeagueFunctions::IsDonePatchingIssueOrder))
			return;

		DWORD retInstruction = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress; //Address of a return instruction
		DWORD IssueOrderAddr = (DWORD)LeagueFunctions::NewIssueOrder; //IssueOrder

		if (EventManager::TriggerProcess(LeagueEvents::OnIssueOrder, this, order, position, nullptr))
		{
			if (((*(DWORD*)retInstruction) & 0xFF) != 0xC3)
				return; //This isn't the instruction we're looking for

			__asm {
				push retnHere; //address of our function

				mov ecx, this; //If the function is a __thiscall don't forget to set ECX
				push 0  //Push args to league function, typically left to right 
					push 0  //Push args to league function, typically left to right 
					push 0  //Push args to league function, typically left to right 
					push 0  //Push args to league function, typically left to right 
					push position  //Push args to league function, typically left to right 
					push order  //Push args to league function, typically left to right 
					push retInstruction;
				jmp IssueOrderAddr; //Jump to league function
			retnHere:
			}
		}
	}

	void GameObject::IssueOrderHoldPos(GameObjectOrder order, Vector3* position)
	{
		if ((!(DWORD)LeagueFunctions::NewIssueOrder) || (!LeagueFunctions::IsDonePatchingIssueOrder))
			return;

		DWORD retInstruction = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress; //Address of a return instruction
		DWORD IssueOrderAddr = (DWORD)LeagueFunctions::NewIssueOrder; //IssueOrder

		if (EventManager::TriggerProcess(LeagueEvents::OnIssueOrder, this, order, position, nullptr))
		{
			if (((*(DWORD*)retInstruction) & 0xFF) != 0xC3)
				return; //This isn't the instruction we're looking for

			__asm {
				push retnHere; //address of our function

				mov ecx, this; //If the function is a __thiscall don't forget to set ECX
				push 0  //Push args to league function, typically left to right 
					push 0  //Push args to league function, typically left to right 
					push 0  //Push args to league function, typically left to right 
					push 0  //Push args to league function, typically left to right 
					push position  //Push args to league function, typically left to right 
					push order  //Push args to league function, typically left to right 
					push retInstruction;
				jmp IssueOrderAddr; //Jump to league function
			retnHere:
			}
		}
	}

	void GameObject::OverrideIssueOrder(GameObjectOrder order, Vector3* position)
	{
		if ((!(DWORD)LeagueFunctions::NewIssueOrder) || (!LeagueFunctions::IsDonePatchingIssueOrder))
			return;

		DWORD retInstruction = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress; //Address of a return instruction
		DWORD IssueOrderAddr = (DWORD)LeagueFunctions::NewIssueOrder; //IssueOrder

		if (((*(DWORD*)retInstruction) & 0xFF) != 0xC3)
			return; //This isn't the instruction we're looking for

		__asm {
			push retnHere; //address of our function

			mov ecx, this; //If the function is a __thiscall don't forget to set ECX
			push 0  //Push args to league function, typically left to right 
				push 0  //Push args to league function, typically left to right 
				push 0  //Push args to league function, typically left to right 
				push 0  //Push args to league function, typically left to right 
				push position  //Push args to league function, typically left to right 
				push order  //Push args to league function, typically left to right 
				push retInstruction;
			jmp IssueOrderAddr; //Jump to league function
		retnHere:
		}
	}

	void GameObject::EvadeIssueOrder(GameObjectOrder order, Vector3* position)
	{
		if ((!(DWORD)LeagueFunctions::NewIssueOrder) || (!LeagueFunctions::IsDonePatchingIssueOrder))
			return;

		DWORD retInstruction = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress; //Address of a return instruction
		DWORD IssueOrderAddr = (DWORD)LeagueFunctions::NewIssueOrder; //IssueOrder

		if (((*(DWORD*)retInstruction) & 0xFF) != 0xC3)
			return; //This isn't the instruction we're looking for

		__asm {
			push retnHere; //address of our function

			mov ecx, this; //If the function is a __thiscall don't forget to set ECX
			push 0  //Push args to league function, typically left to right 
				push 0  //Push args to league function, typically left to right 
				push 0  //Push args to league function, typically left to right 
				push 0  //Push args to league function, typically left to right 
				push position  //Push args to league function, typically left to right 
				push order  //Push args to league function, typically left to right 
				push retInstruction;
			jmp IssueOrderAddr; //Jump to league function
		retnHere:
		}
	}

	void GameObject::EvadeIssueOrderHoldPos(GameObjectOrder order, Vector3* position)
	{
		if ((!(DWORD)LeagueFunctions::NewIssueOrder) || (!LeagueFunctions::IsDonePatchingIssueOrder))
			return;

		DWORD retInstruction = (DWORD)GetModuleHandle(NULL) + (DWORD)Offsets::Functions::RetAddress; //Address of a return instruction
		DWORD IssueOrderAddr = (DWORD)LeagueFunctions::NewIssueOrder; //IssueOrder

		if (((*(DWORD*)retInstruction) & 0xFF) != 0xC3)
			return; //This isn't the instruction we're looking for

		__asm {
			push retnHere; //address of our function

			mov ecx, this; //If the function is a __thiscall don't forget to set ECX
			push 0  //Push args to league function, typically left to right 
				push 0  //Push args to league function, typically left to right 
				push 1  //Push args to league function, typically left to right 
				push 0  //Push args to league function, typically left to right 
				push position  //Push args to league function, typically left to right 
				push order  //Push args to league function, typically left to right 
				push retInstruction;
			jmp IssueOrderAddr; //Jump to league function
		retnHere:
		}
	}

	bool GameObject::IsMe()
	{
		return this == ObjectManager::Player;
	}

	bool GameObject::IsMoving()
	{
		return this->GetPathController()->HasNavigationPath;
	}

	unsigned int GameObject::PathIndex()
	{
		return this->GetPathController()->GetNavigationPath()->Index;
	}

	unsigned int GameObject::PathCount()
	{
		return this->GetPathController()->GetNavigationPath()->Path.size();
	}

	Vector3 GameObject::GetPath(unsigned int index)
	{
		return this->GetPathController()->GetNavigationPath()->Path[index];
	}

	std::vector<Vector3> GameObject::CreatePath(Vector3 start, Vector3 end)
	{
		std::vector<Vector3> vecList = std::vector<Vector3>();
		auto player = ObjectManager::Player;
		if (player != nullptr)
		{
			for each (auto vec in player->GetPath(start, end))
				vecList.push_back(vec);
		}
		return vecList;
	}

	std::vector<Vector3> GameObject::GetPath(Vector3 start, Vector3 end)
	{
		return this->GetPath(start, end, false);
	}

	std::vector<Vector3> GameObject::GetPath(Vector3 start, Vector3 end, bool smoothPath)
	{
		std::vector<Vector3> vecList = std::vector<Vector3>();

		auto ptr = ObjectManager::Player;//ObjectManager::Instance->ObjectsArray[this->Id];
		if (ptr != nullptr)
		{
			auto AIManager = ptr->GetPathController();
			if (AIManager != nullptr)
			{
				auto startVec = Vector3(start.X, start.Z, start.Y);
				auto dstVec = Vector3(end.X, end.Z, end.Y);
				auto navPath = AIManager->GetNavigationPath();

				if (AIManager->CreatePath(startVec, dstVec, navPath))
				{
					if (smoothPath)
					{
						AIManager->SmoothPath(navPath);
					}

					Vector3* begin = &navPath->StartPosition;
					Vector3* end = &navPath->EndPosition;

					while (begin != end)
					{
						vecList.push_back(Vector3(begin->X, begin->Z, begin->Y));
						begin += 12 / sizeof(Vector3);
					}
				}
			}
		}

		return vecList;
	}

	bool GameObject::IsDashing()
	{
		return this->GetPathController()->GetNavigationPath()->IsDashing;
	}

	float GameObject::DashSpeed()
	{
		return this->GetPathController()->GetNavigationPath()->DashSpeed;
	}

	Vector3 GameObject::ServerPosition()
	{
		return this->GetPathController()->ServerPosition;
	}

	std::vector<Vector3> GameObject::GetWaypointList() {
		std::vector<Vector3> result = std::vector<Vector3>();
		int size = this->PathCount();
		for (int i = this->PathIndex() - 1; i < size; ++i)
			result.push_back(this->GetPath(i));

		return result;
	}

	unsigned int GameObject::RuneCount()
	{
		return 6;
	}

	GameObjectType GameObject::Type()
	{
		return this->GetClassData()->Type();
	}

	bool GameObject::IsStructure()
	{
		/*switch (this->GetClassData()->Type())
		{
		case GameObjectType::AITurretClient:
		case GameObjectType::BarracksDampener:
		case GameObjectType::HQ:
			return true;
		default:
			return false;
		}*/
		if (this->Inhibitor() || this->Turret() || this->Nexus()) {
			return true;
		}
		return false;
	}

	bool GameObject::IsAlly()
	{
		return this->Team == ObjectManager::Player->Team;
	}

	bool GameObject::IsEnemy()
	{
		return this->Team != ObjectManager::Player->Team;
	}

	bool GameObject::IsNeutral()
	{
		return this->Team == GameObjectTeam_Neutral;
	}

	bool GameObject::IsMonster() {
		return this->Team == GameObjectTeam_Neutral && this->MaxHealth > 6.0f;
	}

	bool GameObject::IsBaseAI()
	{
		//return this->Flags() & GameObjectFlags_AIBaseClient;
		return true;
	}

	bool GameObject::IsMinion()
	{
		//return this->Flags() & GameObjectFlags_AIMinionClient;
		return Minion();
	}

	bool GameObject::IsHero()
	{
		//return this->Flags() & GameObjectFlags_AIHeroClient;
		return Hero();
	}

	bool GameObject::IsTurret()
	{
		//return this->Flags() & GameObjectFlags_AITurretClient;
		return Turret();
	}

	bool GameObject::IsMissile()
	{
		//return this->Flags() & GameObjectFlags_MissileClient;
		return Missile();
	}

	bool GameObject::IsInvulnerable()
	{
		return this->StatusFlags & GameObjectStatusFlags_Invulnerable;
	}

	bool GameObject::IsMagicImmune()
	{
		return this->StatusFlags & GameObjectStatusFlags_MagicImmune;
	}

	bool GameObject::IsPhysicalImmune()
	{
		return this->StatusFlags & GameObjectStatusFlags_PhysicalImmune;
	}

	bool GameObject::IsValidTarget(float range, bool onlyEnemyTeam, Vector3 rangeCheckFrom)
	{
		if (!this->Alive() || !this->IsVisible || !this->Targetable() || this->StatusFlags & GameObjectStatusFlags_Invulnerable)//|| this->IsUntargetableToAllies || this->StatusFlags & GameObjectStatusFlags_Invulnerable)
		{
			return false;
		}

		if (onlyEnemyTeam && this->Team == ObjectManager::Player->Team)
		{
			return false;
		}

		if (range != FLT_MAX)
		{
			//auto position = this->Flags() & GameObjectFlags_AIBaseClient ? this->Position : this->Position;
			auto position = this->Position;
			return rangeCheckFrom.IsValid() ? rangeCheckFrom.DistanceSquared(position) < range * range : ObjectManager::Player->Position.DistanceSquared(position) < range * range;
		}
		return true;
	}

	float GameObject::TotalHealth()
	{
		auto result = this->Health;
		//if (this->Flags() & GameObjectFlags_AIHeroClient && this->BaseCharacterData->SkinHash == Character::Kled)
		if (this->Hero() && this->BaseCharacterData->SkinHash == Character::Kled)
		{
			result += this->SecondaryResource;
		}
		return result;
	}

	float GameObject::TotalMaxHealth()
	{
		auto result = this->MaxHealth;
		//if (this->Flags() & GameObjectFlags_AIHeroClient && this->BaseCharacterData->SkinHash == Character::Kled)
		if (this->Hero() && this->BaseCharacterData->SkinHash == Character::Kled)
		{
			result += this->MaxSecondaryResource;
		}
		return result;
	}

	float GameObject::TotalShield()
	{
		auto result = this->AllShield;
		//if (this->Flags() & GameObjectFlags_AIHeroClient && this->BaseCharacterData->SkinHash == Character::Blitzcrank)
		if (this->Hero() && this->BaseCharacterData->SkinHash == Character::Blitzcrank)
		{
			for (auto buffManager : this->Buffs)
			{
				auto buff = buffManager.Buff;
				if (buff->IsActive())
				{
					auto hash = buff->Script->Hash;
					if (hash == 0x6FF6CCC4 // "BlitzcrankManaBarrierCD"
						|| hash == 0xDEE0E3A1) // "ManaBarrier"
					{
						return result;
					}
				}
			}
			result += this->Resource * 0.5f;
		}
		return result;
	}

	bool GameObject::CanAttack()
	{
		return this->ActionState & GameObjectActionState_CanAttack;
	}

	bool GameObject::CanCrit()
	{
		return this->ActionState & GameObjectActionState_CanCrit;
	}

	bool GameObject::CanCast()
	{
		return this->ActionState & GameObjectActionState_CanCast;
	}

	bool GameObject::CanMove()
	{
		return this->ActionState & GameObjectActionState_CanMove;
	}

	bool GameObject::IsImmovable()
	{
		return this->ActionState & GameObjectActionState_Immovable;
	}

	bool GameObject::IsStealthed()
	{
		return this->ActionState & GameObjectActionState_Stealthed;
	}

	bool GameObject::IsTaunted()
	{
		return this->ActionState & GameObjectActionState_Taunted;
	}

	bool GameObject::IsFeared()
	{
		return this->ActionState & GameObjectActionState_Feared;
	}

	bool GameObject::IsFleeing()
	{
		return this->ActionState & GameObjectActionState_Fleeing;
	}

	bool GameObject::IsSupressed()
	{
		return this->ActionState & GameObjectActionState_Supressed;
	}

	bool GameObject::IsAsleep()
	{
		return this->ActionState & GameObjectActionState_Sleep;
	}

	bool GameObject::IsGhosted()
	{
		return this->ActionState & GameObjectActionState_Ghosted;
	}

	bool GameObject::IsCharmed()
	{
		return this->ActionState & GameObjectActionState_Charmed;
	}

	bool GameObject::IsSlowed()
	{
		return this->ActionState & GameObjectActionState_Slowed;
	}

	bool GameObject::IsDisarmed()
	{
		return !(this->ActionState & (GameObjectActionState_CanAttack | GameObjectActionState_Charmed | GameObjectActionState_Fleeing | GameObjectActionState_Taunted | GameObjectActionState_Sleep | GameObjectActionState_Supressed)) && !(this->StatusFlags & GameObjectStatusFlags_Invulnerable);
	}

	bool GameObject::IsRooted()
	{
		return !(this->ActionState & (GameObjectActionState_CanMove | GameObjectActionState_Charmed | GameObjectActionState_Fleeing | GameObjectActionState_Taunted | GameObjectActionState_Sleep | GameObjectActionState_Supressed)) && !(this->StatusFlags & GameObjectStatusFlags_Invulnerable);
	}

	bool GameObject::IsSilenced()
	{
		return !(this->ActionState & (GameObjectActionState_CanCast | GameObjectActionState_Charmed | GameObjectActionState_Fleeing | GameObjectActionState_Taunted | GameObjectActionState_Sleep | GameObjectActionState_Supressed)) && !(this->StatusFlags & GameObjectStatusFlags_Invulnerable);
	}

	bool GameObject::IsStunned()
	{
		return !(this->ActionState & (GameObjectActionState_CanAttack | GameObjectActionState_CanMove | GameObjectActionState_CanCast | GameObjectActionState_Charmed | GameObjectActionState_Fleeing | GameObjectActionState_Taunted | GameObjectActionState_Sleep | GameObjectActionState_Supressed)) && !(this->StatusFlags & GameObjectStatusFlags_Invulnerable);
	}

	float GameObject::TotalBaseAttackDamage()
	{
		return (this->BaseAttackDamage + this->FlatBaseAttackDamageMod) * (1.0f + this->PercentBaseAttackDamageMod);
	}

	float GameObject::TotalBonusAttackDamage()
	{
		return this->FlatAttackDamageMod * (1.0f + this->PercentBonusAttackDamageMod);
	}

	float GameObject::TotalAttackDamage()
	{
		return (this->TotalBaseAttackDamage() + this->TotalBonusAttackDamage()) * (1.0f + this->PercentAttackDamageMod);
	}

	float GameObject::TotalAbilityPower()
	{
		return this->BaseAbilityPower + this->FlatAbilityPowerMod * (1.0f + this->PercentAbilityPowerMod);
	}

	float GameObject::GetAutoAttackRange(GameObject* target)
	{
		if (this->Hero())
		{
			auto result = this->AttackRange + this->GetBoundingRadius();
			if (target)
			{
				switch (this->BaseCharacterData->SkinHash)
				{
				case Character::Caitlyn:
					//if (target->Flags() & GameObjectFlags_AIBaseClient && target->FindBuff(0x1E8B8CA0)) // caitlynyordletrapinternal
					if (target && target->FindBuffHash(0x1E8B8CA0)) // caitlynyordletrapinternal
					{
						result += 650.0f;
					}
					break;
				}
			}
			return result;
		}
		//else if (sourceFlags & GameObjectFlags_AIMinionClient)
		else if (this->Minion())
		{
			switch (this->BaseCharacterData->SkinHash)
			{
			case Character::AzirSoldier:
				return this->BaseCharacterData->Properties->AttackRange - this->GetBoundingRadius();
			}
		}

		return 0.0f;
	}

	bool GameObject::IsInAutoAttackRange(GameObject* target) {

		if (this == ObjectManager::Player && this->BaseCharacterData->SkinHash == Character::Azir) {
			for (auto soldier : SDK::Orbwalker::AzirSoldiers) {
				if (soldier->Alive() && ObjectManager::Player->Position.IsInRange(soldier->Position, 790.0f)) {
					auto pathController = soldier->GetPathController();
					if ((!pathController->HasNavigationPath || !pathController->GetNavigationPath()->IsDashing) && soldier->IsInAutoAttackRange(target)) {
						return true;
					}
				}
			}
		}

		return this->Position.IsInRange(target->Position, this->GetAutoAttackRange(target));
	}

	bool GameObject::IsInTurretAutoAttackRange(GameObject* target) {

		return this->Position.IsInRange(target->Position, this->GetAutoAttackRange(target));
	}


	bool GameObject::IsMelee()
	{
		return this->CombatType == GameObjectCombatType::Melee;
	}

	bool GameObject::IsRanged()
	{
		return !this->IsMelee();
	}

	BuffManager* GameObject::GetBuffManager() {
		return (BuffManager*)((DWORD)this + (DWORD)Offsets::GameObject::Buffs);
	}

	unsigned int GameObject::BuffCount()
	{
		return this->Buffs.size();
	}

	BuffInstance* GameObject::GetBuff(unsigned int index)
	{
		return this->Buffs[index].Buff;
	}

	BuffInstance* GameObject::FindBuffHash(unsigned int hash)
	{
		for (auto buffManager : this->Buffs)
		{
			auto buff = buffManager.Buff;
			if (buff->IsActive() && buff->Script->Hash == hash)
			{
				//GameClient::PrintChat("buff hash found!", IM_COL32(144, 238, 144, 69));
				return buff;
			}
		}
		return nullptr;
	}


	BuffInstance* GameObject::FindBuffName(std::string name)
	{
		for (auto buffManager : this->Buffs)
		{
			auto buff = buffManager.Buff;
			if (buff->IsActive() && buff->Script->Name == name)
			{
				//GameClient::PrintChat("buff name found!", IM_COL32(144, 238, 144, 69));
				return buff;
			}
		}
		return nullptr;
	}

	BuffInstance* GameObject::FindBuffType(BuffType type)
	{
		for (auto buffManager : this->Buffs)
		{
			auto buff = buffManager.Buff;
			if (buff->IsActive() && buff->Type == type)
			{
				//GameClient::PrintChat("buff type found!", IM_COL32(144, 238, 144, 69));
				return buff;
			}
		}
		return nullptr;
	}

	int GameObject::GetBuffCount(std::string name)
	{
		for (auto buffManager : this->Buffs)
		{
			auto buff = buffManager.Buff;
			if (buff->IsActive() && buff->Script->Name == name)
			{
				return buff->GetCount();
			}
		}
		return 0;
	}

	SpellCastInfo* GameObject::ActiveSpell()
	{
		return this->Spellbook.ActiveSpellInstance;
	}

	Character GameObject::Hash()
	{
		return this->BaseCharacterData->SkinHash;
	}

	bool GameObject::IsLaneMinion()
	{
		switch (this->BaseCharacterData->SkinHash)
		{
		case Character::SRU_ChaosMinionMelee:
		case Character::SRU_OrderMinionMelee:
		case Character::SRU_ChaosMinionRanged:
		case Character::SRU_OrderMinionRanged:
		case Character::SRU_ChaosMinionSiege:
		case Character::SRU_OrderMinionSiege:
		case Character::SRU_ChaosMinionSuper:
		case Character::SRU_OrderMinionSuper:
		case Character::HA_ChaosMinionMelee:
		case Character::HA_OrderMinionMelee:
		case Character::HA_ChaosMinionRanged:
		case Character::HA_OrderMinionRanged:
		case Character::HA_ChaosMinionSiege:
		case Character::HA_OrderMinionSiege:
		case Character::HA_ChaosMinionSuper:
		case Character::HA_OrderMinionSuper:
			return true;
		default:
			return false;
		}
	}

	bool GameObject::IsSiegeMinion() {
		switch (this->BaseCharacterData->SkinHash) {
		case Character::SRU_ChaosMinionSiege:
		case Character::SRU_OrderMinionSiege:
		case Character::HA_ChaosMinionSiege:
		case Character::HA_OrderMinionSiege:
			return true;
		default:
			return false;
		}
	}

	bool GameObject::IsPet()
	{
		switch (this->BaseCharacterData->SkinHash)
		{
		case Character::AnnieTibbers:
		case Character::EliseSpiderling:
		case Character::GangplankBarrel:
		case Character::HeimerTBlue:
		case Character::HeimerTYellow:
		case Character::IvernMinion:
		case Character::MalzaharVoidling:
		case Character::ShacoBox:
		case Character::VoidSpawn:
		case Character::VoidSpawnTracer:
		case Character::YorickGhoulMelee:
		case Character::YorickBigGhoul:
		case Character::ZyraThornPlant:
		case Character::ZyraGraspingPlant:
			return true;
		default:
			return false;
		}
	}

	bool GameObject::IsWard()
	{
		switch (this->BaseCharacterData->SkinHash)
		{
		case Character::JammerDevice:
		case Character::SightWard:
		case Character::BlueTrinket:
		case Character::YellowTrinket:
			return true;
		default:
			return false;
		}
	}

	bool GameObject::IsPlant()
	{
		switch (this->BaseCharacterData->SkinHash)
		{
		case Character::SRU_Plant_Health:
		case Character::SRU_Plant_Satchel:
		case Character::SRU_Plant_Vision:
			return true;
		default:
			return false;
		}
	}

	bool GameObject::IsLargeMonster()
	{
		switch (this->BaseCharacterData->SkinHash)
		{
		case Character::SRU_Razorbeak:
		case Character::SRU_Murkwolf:
		case Character::SRU_Gromp:
		case Character::SRU_Krug:
		case Character::SRU_Red:
		case Character::SRU_Blue:
		case Character::Sru_Crab:
			return true;
		default:
			return false;
		}
	}

	bool GameObject::IsEpicMonster()
	{
		switch (this->BaseCharacterData->SkinHash)
		{
		case Character::SRU_Dragon_Air:
		case Character::SRU_Dragon_Earth:
		case Character::SRU_Dragon_Fire:
		case Character::SRU_Dragon_Water:
		case Character::SRU_Dragon_Elder:
		case Character::SRU_Baron:
		case Character::SRU_RiftHerald:
			return true;
		default:
			return false;
		}
	}

	SpellCastInfo* GameObject::GetBasicAttack()
	{
		return this->AttackData;
		//return Functions::GetBasicAttack(this, NULL);
	}

	unsigned int GameObject::ItemSlotsCount()
	{
		return 7;
	}

	InventorySlot* GameObject::GetItem(unsigned int index)
	{
		return this->HeroInventory.Slots[index];
	}

	InventorySlot* GameObject::FindItem(Item itemId)
	{
		auto inventorySlots = this->HeroInventory.Slots;
		for (auto i = 0; i < 7; i++)
		{
			auto item = inventorySlots[i];
			auto itemInfo = item->ItemInfo;
			if (itemInfo && itemInfo->ItemData->ItemId == itemId)
			{
				return item;
			}
		}
		return nullptr;
	}

	kSpellSlot GameObject::FindItemSpellSLot(Item itemId)
	{
		if (this->HeroInventory.Slots6->ItemInfo && this->HeroInventory.Slots6->ItemInfo->ItemData->ItemId == itemId) return kSpellSlot::SpellSlot_Item1;
		if (this->HeroInventory.Slots7->ItemInfo && this->HeroInventory.Slots7->ItemInfo->ItemData->ItemId == itemId) return kSpellSlot::SpellSlot_Item2;
		if (this->HeroInventory.Slots8->ItemInfo && this->HeroInventory.Slots8->ItemInfo->ItemData->ItemId == itemId) return kSpellSlot::SpellSlot_Item3;
		if (this->HeroInventory.Slots9->ItemInfo && this->HeroInventory.Slots9->ItemInfo->ItemData->ItemId == itemId) return kSpellSlot::SpellSlot_Item4;
		if (this->HeroInventory.Slots10->ItemInfo && this->HeroInventory.Slots10->ItemInfo->ItemData->ItemId == itemId) return kSpellSlot::SpellSlot_Item5;
		if (this->HeroInventory.Slots11->ItemInfo && this->HeroInventory.Slots11->ItemInfo->ItemData->ItemId == itemId) return kSpellSlot::SpellSlot_Item6;
		if (this->HeroInventory.Slots12->ItemInfo && this->HeroInventory.Slots12->ItemInfo->ItemData->ItemId == itemId) return kSpellSlot::SpellSlot_Trinket;
		/*if (this->HeroInventory.GetItemSpell(SpellSlot_Item1)->ItemInfo && this->HeroInventory.GetItemSpell(SpellSlot_Item1)->ItemInfo->ItemData->ItemId == itemId )
			return kSpellSlot::SpellSlot_Item1;
		if (this->HeroInventory.GetItemSpell(SpellSlot_Item2)->ItemInfo && this->HeroInventory.GetItemSpell(SpellSlot_Item2)->ItemInfo->ItemData->ItemId == itemId)
			return kSpellSlot::SpellSlot_Item2;
		if (this->HeroInventory.GetItemSpell(SpellSlot_Item3)->ItemInfo && this->HeroInventory.GetItemSpell(SpellSlot_Item3)->ItemInfo->ItemData->ItemId == itemId)
			return kSpellSlot::SpellSlot_Item3;
		if (this->HeroInventory.GetItemSpell(SpellSlot_Item4)->ItemInfo && this->HeroInventory.GetItemSpell(SpellSlot_Item4)->ItemInfo->ItemData->ItemId == itemId)
			return kSpellSlot::SpellSlot_Item4;
		if (this->HeroInventory.GetItemSpell(SpellSlot_Item5)->ItemInfo && this->HeroInventory.GetItemSpell(SpellSlot_Item5)->ItemInfo->ItemData->ItemId == itemId)
			return kSpellSlot::SpellSlot_Item5;
		if (this->HeroInventory.GetItemSpell(SpellSlot_Item6)->ItemInfo && this->HeroInventory.GetItemSpell(SpellSlot_Item6)->ItemInfo->ItemData->ItemId == itemId)
			return kSpellSlot::SpellSlot_Item6;
		if (this->HeroInventory.GetItemSpell(SpellSlot_Trinket)->ItemInfo && this->HeroInventory.GetItemSpell(SpellSlot_Trinket)->ItemInfo->ItemData->ItemId == itemId)
			return kSpellSlot::SpellSlot_Trinket;*/
		return kSpellSlot::SpellSlot_Unknown;
	}

	float GameObject::GetAutoAttackDamage(GameObject* target)
	{
		return SDK::Damage::CalculateAutoAttackDamage(this, target);
	}

	bool GameObject::Alive() {
		return Functions::IsAlive(this) && this->Health > 0.0f;
	}

	bool GameObject::Minion() {
		return Functions::IsMinion(this);
	}

	bool GameObject::Turret() {
		return Functions::IsTurret(this);
	}

	bool GameObject::Hero() {
		return Functions::IsHero(this);
	}

	bool GameObject::Missile() {
		return Functions::IsMissile(this);
	}

	bool GameObject::Targetable() {
		return Functions::IsTargetable(this);
	}

	bool GameObject::Nexus() {
		return Functions::IsNexus(this);
	}

	bool GameObject::Inhibitor() {
		return Functions::IsInhibitor(this);
	}

	bool GameObject::TroyEnt() {
		return Functions::IsTroyEnt(this);
	}

}