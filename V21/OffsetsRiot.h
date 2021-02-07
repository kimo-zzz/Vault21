#pragma once

//9.8

namespace HACKUZAN {

	namespace Offsets {


		// MISSING
		enum class ClockFacade {   //update
			Instance = 0x1C5D7FC,									// ?? --not used anyways, we can ignore
			GameTime = 0x02f4759c,									// 11.3
		};

		//MISSING
		enum class DeviceHandler { //update
			Instance = 0x22fee94,									// 11.3
			DevicePtr = 0x18,
			Direct3DDevice = 0x208									// 11.3
		};

		enum class ManagerTemplate {

			AIBases = 0x22FEE78,									// 11.3  
			Turrets = 0x2F46554,									// 11.3
			Inhibitor = 0x2F4F888,									// 11.3
			Heroes = 0x16B09E4,										// 11.3
			Minions = 0x22FEE74,									// 11.3
			Missiles = 0x2F4D664,									// 11.3

			oIsAlive = 0x186A10,									// 11.3
			oIsInhib = 0x1951F0,									// 11.3
			oIsNexus = 0x1952F0,									// 11.3
			oIsMinion = 0x1953B0,									// 11.3
			oIsTurret = 0x1955E0,									// 11.3
			oIsTargetable = 0x1BD200,								// 11.3
			oIsDragon = 0x15A850,									// 11.3
			oIsBaron = 0x15C520,									// 11.3
			oIsHero = 0x195370,										// 11.3
			oIsMissile = 0x1953D0,									// 11.3
			oIsTroy = 0xA40600  									// 11.3
		};

		enum class GameClient {
			Instance = 0x22FEE94,//0x34FB6F4, A1 ? ? ? ? 68 ? ? ? ? 8B 70 08 // dword_[offset]

			GameState = 0x8, //almost never changes

			ChatClientPtr = 0x22FF400, //8B 0D ? ? ? ? 6A 01 FF 74 24 08 // dword_[offset] // 8B ?? ?? ?? ?? ?? 6A ?? 50 E8 ?? ?? ?? ?? 33 C0 5F C2 // dword_[offset]
			PrintChat = 0x546F30, //E8 ? ? ? ? 6A 00 68 ? ? ? ? E8 ? ? ? ? 83 C4 04 // sub_[offset]
			SendChat = 0x58A6D0 //A1 ? ? ? ? 56 6A FF // fn
		};
		// MISSING
		enum class HashFunctions { // no update
			TranslateString = 0xa4b7c0,
		};

		//MISSING
		enum class HudManager {
			Instance = 0x16b2AAC,  // update 

			CursorTargetLogic = 0x14, // no update
			SelectLogic = 0x30,		  // no update
		};

		enum class NetClient {
			Instance = 0x2f566f0,  // update 
			GetPing = 0x308c10,
		};

		enum class ObjectManager {
			Instance = 0x016b2a74,  // update 
			GetFirstObject = 0x26CA90, //E8 ? ? ? ? 8B F8 85 FF 0F 84 ? ? ? ? 53 8B 5C // sub_[offset]
			GetNextObject = 0x26E8D0, //E8 ? ? ? ? 8B F0 85 F6 75 E4 // sub_[offset]

			Player = 0x2F4F764, //MISSING replaceable?

			ObjectsArray = 0x14,
		};

		enum class RenderLayer {
			Instance = 0x2F73E78,//0x35179E4, //MISSING ; what is this
			WorldToScreen = 0x91BA50, // 83 EC 10 56 E8 ? ? ? ? 8B 08 // fn 

			ClientWidth = 0x10, //0x14,
			ClientHeight = 0x14,//0x18,
			ViewMatrix = 0x60,
			ProjectionMatrix = 0xA0
		};

		enum class RiotWindow {
			Instance = 0x2f4f3cc, // 11.3
		};

		///////////////////////////////////FUNCTIONS//////////////////////////////////////

		enum class Functions {

			IssueOrder = 0x15C5D0,									// 11.3
			IssueOrderTrueReturn = 0x15C864,
			
			CastSpell = 0x4D55E0,
			TrueCastSpellReturn = 0x58A994,

			OnProcessSpellW = 0x4D5190,								// 11.3  
			OnFinishCast = 0x4CD9C0,								// 11.3
			OnStopCast = 0x4D7C30,									// 11.3

			OnCreateObject = 0x2794E0,								// 11.3
			OnDeleteObject = 0x26A4F0,								// 11.3

			OnNewPath = 0x2882d0,									// 11.3

			GetAttackCastDelay = 0x2846C0,							// 11.3
			GetAttackDelay = 0x2847C0,								// 11.3
			GetBasicAttack = 0x14FD80,								// 11.3

			IsNotWall = 0x899380,									// 11.3
			IsWall = 0x8e4710,										// ??

			RetAddress = 0x6E502
		};

		// MISSING ; We might replace this, mainly used to check for IsWall. We can use !IsNotwall func instead
		enum class NavGrid {
			Instance = 0x2F6C888,									// 11.3
			GetCell = 0x8941B0,										// 11.3
			GetHeightForPosition = 0x8E2010
		};

		//MISSING ; this stuff is needed sadly
		enum class AIManagerClient
		{
			CreatePath = 0x1A7D70,									// 11.3  //xref CalculatePath   83 EC 10 56 8B F1 57 8B BE
			CalculatePath = 0x891330,								// 11.3  //inside CreatePath	83 EC 50 8B 44 24 58
			SmoothPath = 0x1A6320,									// 11.3  inside IssueOrder		55 8B EC 83 E4 F0 83 EC 68 6B
		};

		///////////////////////////////////STRUCTS///////////////////////////////////////

		//This probably didn't change. Simply our getMouseWorldPos Func from Engine.h 
		enum class HudCursorTargetLogic {
			CursorPosition = 0x1C,
			HoveredUnitId = 0x3C,
		};

		enum class GameObject {
			Virtual_GetBoundingRadius = 36,							// should still be correct
			Virtual_GetPathController = 148,

			ClassData = 0x4,
			Id = 0x20,
			Team = 0x4C,
			Flags = 0x40,//0x48, // idk about that
			Name = 0x6C,
			NetworkId = 0xCC,
			IsDead = 0x1D4, // MISSING
			Position = 0x1D8,
			IsVisible = 0x270, // E8 ? ? ? ? C7 85 ? ? ? ? ? ? ? ? 33 C9 C7 // sub_xxxx + 5b7
			SourceID = 0x290, // 89 81 ? ? ? ? 8B 44 24 04 89 81 ? ? ? ?
			Resource = 0x298, // idk
			MaxResource = Resource + 0x10, // idk
			ResourceType = MaxResource + 0x40,
			SecondaryResource = ResourceType + 0x20,
			MaxSecondaryResource = SecondaryResource + 0x10,
			StatusFlags = 0x3D0,
			IsTargetable = 0xD00, // 88 83 ? ? ? ? C6 83 ? ? ? ? ? C7 // fn + EBE
			IsUntargetableToAllies = IsTargetable + 0x10,
			IsUntargetableToEnemies = IsUntargetableToAllies + 0x10,
			Health = 0xD98, // // 8D 8B ? ? ? ? C6 83 ? ? ? ? ? 50 8D 83 ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 50
			MaxHealth = Health + 0x10,
			AllShield = 0xDD8,
			PhysicalShield = AllShield + 0x10,
			MagicalShield = PhysicalShield + 0x10,
			ActionState = 0x1054,
			CharacterIntermediate = 0x1670, // xref "mPhysicalDamagePercentageModifier"
			CombatType = 0x2058,
			Buffs = 0x2160 + 0x10,
			Spellbook = 0x2708,
			CharacterDataStack = 0x2F60,
			BaseCharacterData = 0x2EDC,
			CharacterData = BaseCharacterData + 0x4,
			SkinName = 0x3134,
			AttackData = 0x2ED0,
			Experience = 0x36BC - 0x10, // mLevelRef
			Level = Experience + 10,
			HeroInventory = 0x36F0,
		};

		enum class MissileClient {
			MissileSpellCastInfo = 0x230,
			SourceId = 0x28C,
			NetworkId = 0x29C,
			TargetId = 0x2E8,
			StartPos = 0x2A8,
			EndPos = 0x2B4,
			CastDelay = 0x6E8,
			Delay = 0x6F8,
			IsBasicAttack = 0x708,//0x768,//0x708,
			IsSpecialAttack = IsBasicAttack + 0x4,
			Slot = 0x710,
			StartTime = 0x75C,
		};

		enum class ClassData {
			Type = 0x0,
			Name = 0x68//0x18
		};

		enum class SpellCastInfo {
			SpellData = 0x0,
			SpellInfo = 0x8,
			Level = 0x54,
			StartPosition = 0x80,
			EndPosition = 0x8C,
			SoureNetworkId = 0x74,
			HasTarget = 0xBC,
			SourceId = 0x6C, //0x68
			TargetId = 0xC0, //0x74
			CastDelay = 0x4C0,
			Delay = 0x4D0,
			Cooldown = 0x4D4,
			IsWindingUp = 0x4DC,
			IsBasicAttack = 0x4E0,
			IsSpecialAttack = IsBasicAttack + 0x4,
			Slot = 0x4E8,
			EndTime = 0x52C,
			CastEndTime = 0x530,
			StartTime = 0x544,
			SpellWasCast = 0x538,
			IsStopped = 0x53C,
			IsInstantCast = 0x540,
		};

		enum class SpellInfo {
			Level = 0x54,
			SoureNetworkId = 0x6C,
			StartPosition = 0x78,
			EndPosition = 0x84,
			HasTarget = 0xB4,
			SourceId = 0x64,
			TargetId = 0xB8,
			CastDelay = 0x4B8,
			Delay = 0x4C8,
			Cooldown = 0x4D4,
			IsWindingUp = 0x4D4,
			IsBasicAttack = 0x4D8,
			IsSpecialAttack = IsBasicAttack + 0x4,
			Slot = 0x4E0,
			EndTime = 0x524,
			CastEndTime = 0x528,
			StartTime = 0x53C,
			SpellWasCast = 0x538,
			IsStopped = 0x534,
			IsInstantCast = 0x538,
		};

		enum class SpellData {
			Script = 0xC,
			Name = 0x18,
			Resource = 0x44,
		};

		enum class SpellDataScript {
			Name = 0x8,
			Hash = 0x88
		};

		enum class SpellDataResource {
			MissileName = 0x64,
			SpellName = 0x88,
			DisplayNameLocalizationKey = 0x1F0,
			eEffect1 = 0xA0,//0xD0,
			eEffect2 = eEffect1 + 0x1C,
			eEffect3 = eEffect2 + 0x1C,
			eEffect4 = eEffect3 + 0x1C,
			eEffect5 = eEffect4 + 0x1C,
			eEffect6 = eEffect5 + 0x1C,
			eEffect7 = eEffect6 + 0x1C,
			eEffect8 = eEffect7 + 0x1C,
			eEffect9 = eEffect8 + 0x1C,
			eEffect10 = eEffect9 + 0x1C,
			eEffect11 = eEffect10 + 0x1C,
			aEffect1 = 0x200,
			aEffect2 = 0x204,
			CastDelay = 0x250,
			CastTime = 0x270,
			CantCancelWhileWindingUp = 0x35C,
			CantCancelWhileChanneling = 0x36C,
			ChannelIsInterruptedByAttacking = 0x370,
			CanMoveWhileChanneling = 0x378,
			MissileSpeed = 0x434,//0x450,
			SpellDataSpellWidth = 0x44C,//0x458,
			SpellDataSpellRadius = 0x3F8,
			CastRange = 0x3C0,
			CastRangeDisplayOverride = 0x3B8,
			SpellDataEffectSpellRange = 0x10C, // starts from F4
			SpellDataEffectSpellRangeAfterEffect = 0xB8 // starts from A0
		};

		//MISSING
		enum class CharacterIntermediate {
			PassiveCooldownEndTime = 0x30,
			PassiveCooldownTotalTime = 0x40,
			PercentDamageToBarracksMinionMod = 0x16A0,
			FlatDamageReductionFromBarracksMinionMod = 0x16B0,
			FlatAttackDamageMod = 0x80,
			PercentAttackDamageMod = 0x90,
			PercentBonusAttackDamageMod = 0xA0,
			FlatAbilityPowerMod = 0xC0,
			PercentAbilityPowerMod = 0xD0,
			FlatMagicReduction = 0xE0,
			PercentMagicReduction = 0xF0,
			AttackSpeedMod = 0x110,
			BaseAttackDamage = 0x140,
			FlatBaseAttackDamageMod = 0x160,
			PercentBaseAttackDamageMod = 0x170,
			BaseAbilityPower = 0x180,
			Armor = 0x1D0,
			BonusArmor = 0x1E0,
			MagicResist = 0x1F0,
			BonusMagicResist = 0x200,
			MoveSpeed = 0x230,
			AttackRange = 0x250,
			PhysicalLethality = 0x290,
			PercentArmorPenetration = 0x2A0,
			PercentBonusArmorPenetration = 0x2B0,
			FlatMagicPenetration = 0x2E0,
			PercentMagicPenetration = 0x300,
			PercentBonusMagicPenetration = 0x310,
		};

		// MISSING , but changes rarely
		enum class BuffInstance {
			Type = 0x4,
			Script = 0x8,
			StartTime = 0xC,
			ExpireTime = 0x10,
			ScriptInfo = 0x20,
			IsPermanent = 0x70,
			Counter = 0x74,
			fCounter = 0x2C
		};

		// MISSING , but changes rarely
		enum class BuffScript {
			Virtual_GetDisplayName = 14,

			Name = 0x8,
			Type = 0x4,
			BaseStacksAlt = 0x20,
			StacksAlt = 0x24,
			Hash = 0x88
		};
		// MISSING , but changes rarely
		enum class BuffScriptInstance {
			CasterId = 0x4
		};

		enum class SpellbookClient {
			Virtual_GetSpellState = 1,
			Virtual_GetSpell = 19,

			GetSpellState = 0x4C5E70, // E8 ? ? ? ? 8B F8 8B CB 89 // sub_[offset]

			ActiveSpellInstance = 0x20
		};

		enum class SpellInstanceClient {
			CastInfo = 0x8
		};

		// MISSING
		enum class SpellDataInst {
			Level = 0x20,
			IsLearned = 0x24,
			CastTime = 0x28,
			Ammo = 0x31C,
			AmmoRechargeTime = 0x354,
			AmmoCd = 0x68,
			ToggleState = 0x70,
			SpellCD = 0x78,
			fEffect1 = 0x88,
			fEffect2 = fEffect1 + 0x4,
			fEffect3 = fEffect2 + 0x4,
			fEffect4 = fEffect3 + 0x4,
			fEffect5 = fEffect4 + 0x4,
			fEffect6 = fEffect5 + 0x4,
			fEffect7 = fEffect6 + 0x4,
			fEffect8 = fEffect7 + 0x4,
			fEffect9 = fEffect8 + 0x4,
			fEffect10 = fEffect9 + 0x4,
			fEffect11 = fEffect10 + 0x4,
			SpellData = 0x13C, //"SpellDataInstClient::SetSpellData: %s n"...
		};

		enum class CharacterDataStack {
			SkinName = 0xC,
			SkinId = 0x18//0x0EBC // E8 ? ? ? ? 83 C4 0C 80 BF // scroll down, instruction: cmp byte ptr [xxx+[offset]h], 0
		};

		enum class CharacterData {
			SkinName = 0x4,
			SkinHash = 0x10,
			Properties = 0x1C
		};

		enum class CharacterProperties {
			AttackRange = 0x1CC,
		};

		enum class Experience {
			Level = 0x10,
		};

		enum class HeroInventoryCommon {
			Slots = 0x18,
			Slots6 = 0x18,
			Slots7 = 0x1C,
			Slots8 = 0x20,
			Slots9 = 0x24,
			Slots10 = 0x28,
			Slots11 = 0x2C,
			Slots12 = 0x30,
		};

		enum class InventorySlot {
			Stacks = 0x0,
			ItemInfo = 0xC
		};

		enum class ItemInfo {
			ItemData = 0x20,
			Ammo = 0x24,
		};

		enum class ItemData {
			ItemId = 0x68, // "ItemId" 0x13BDDC7B
			MaxStacks = 0xA8,//0x6C, // "MaxStack" 0x8CCDDBB3
			Price = 0x4AC,
			DisplayNameLocalizationKey = 0x4B0
		};

		enum class PathControllerCommon {
			NavMesh = 0x100,//0x100, //CreatePath
			HasNavigationPath = 0x198,
			NavigationPath = 0x19C,
			ServerPosition = 0x2BC,
		};

		enum class NavigationPath {
			Index = 0x0,
			StartPosition = 0x8,
			EndPosition = StartPosition + 0xC,
			Path = EndPosition + 0xC,
			DashSpeed = 0x1D0,//0x34,
			IsDashing = 0x1EC//0x1E8//0x4C
		};

		enum class AvatarClient
		{
			HasRunes = 0x1E4,
			Runes = 0x300,
		};

		enum class RuneInstance {
			Id = 0x4,
		};
	}
}