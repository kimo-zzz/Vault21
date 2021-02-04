#pragma once
#define TARGET_GAMEVERSION "Version 11.3.357.5376 [PUBLIC]"  // 

#define oRetAddr 0x6E502 // 				// find addr : ret // opcode : C3
#define oDrawCircleRetAddr 0x6E4FF // 		// find addr : add esp, 28 ret // opcode : 83 C4 1C C3
#define oNetClient 0x2F566F0 // 			8B 0D ? ? ? ? 85 C9 74 07 8B 01 6A 01 FF 50 08 8B // dword_[offset] // this addy us for IsTargetable as per chinese tool but nah~
#define oPingInstance 0x2F566F0 //			8B 0D ? ? ? ? 85 C9 74 07 8B 01 6A 01 FF 50 08 8B // dword_[offset] // why do we have that twice lol
#define oChatClientPtr 0x22FF400 //			8B 0D ? ? ? ? 6A 01 FF 74 24 08 // dword_[offset] // 8B ?? ?? ?? ?? ?? 6A ?? 50 E8 ?? ?? ?? ?? 33 C0 5F C2 // dword_[offset]
#define oObjManager 0x16B2A74 //			8B 0D ? ? ? ? 89 74 24 14 // dword_[offset]
#define oZoomClass 0x2F46D80 //				A3 ? ? ? ? 83 FA 10 72 32 // dword_[offset]
#define oGameInfo 0x22FEE94 // 				A1 ? ? ? ? 83 78 08 02 0F 94 C0 // dword_[offset]
#define oHudInstance 0x16B2AAC //			8B 0D ? ? ? ? 6A 00 8B 49 34 E8 ? ? ? ? B0 // dword_[offset]
#define oRenderer 0x2F73E78 // 				8B 15 ? ? ? ? 83 EC 08 F3 // dword_[offset]
#define oUnderMouseObject 0x16840A0 // 		8B 0D ? ? ? ? 89 0D // dword_[offset]
#define oRenderer 0x2F73E78 // 				8B 15 ? ? ? ? 83 EC 08 F3 // dword_[offset]
#define oLocalPlayer 0x2F4F764 //			A1 ?? ?? ?? ?? 85 C0 74 07 05 ?? ?? ?? ?? EB 02 33 C0 56 // dword_[offset]
#define oGameTime 0x2F4759C // 				F3 0F 11 05 ? ? ? ? 8B 49 // dword_[offset]
#define oMenuGUI 0x2F4F3C8 // 				8B 0D ? ? ? ? 6A 00 E8 ? ? ? ? C7 // dword_[offset]

// FUNCTIONS
#define oGetNextObject 0x26E8D0 //			E8 ? ? ? ? 8B F0 85 F6 75 E4 // sub_[offset]
#define oGameVersion 0x5376A0 //			XREF: "Version" string
#define oGetFirstObject 0x26CA90 //			E8 ? ? ? ? 8B F8 85 FF 0F 84 ? ? ? ? 53 8B 5C // sub_[offset]
#define oWorldToScreen 0x91BA50 // 			83 EC 10 56 E8 ? ? ? ? 8B 08 // fn 
#define oCastSpell 0x4D5930 // 				xref: ERROR: Client Tried to cast a spell from an invalid slot: %d.\n
#define oDrawCircle 0x4CBEB0 //			 	E8 ? ? ? ? 83 C4 1C 8B 7C 24 28 // sub_[offset]
#define oGetBasicAttack 0x14FD80 // 	 	53 8B D9 B8 ? ? ? ? 8B 93 // fn
#define oGetAttackCastDelay 0x2846C0 // 	83 EC 0C 53 8B 5C 24 14 8B CB 56 57 8B 03 FF 90 // fn // E8 ?? ?? ?? ?? D9 ?? ?? ?? ?? ?? 56 E8 // sub_[offset]
#define oGetAttackDelay 0x2847C0 // 		E8 ? ? ? ? 8B 44 24 1C 83 C4 0C 8B CE // sub_[offset] // this addy is for oGetPing as per chinese tool but nah~
#define oGetPing 0x308BF0 // 				E8 ? ? ? ? 8B 4F 2C 85 C9 0F // sub_[offset]
#define oGetSpellState 0x4C5E50 // 			E8 ? ? ? ? 8B F8 8B CB 89 // sub_[offset] // this addy us for IsTargetable as per chinese tool but nah~
#define oIsTargetable 0x1BD200 //			56 8B F1 E8 ? ? ? ? 84 C0 74 2E // fn
#define oIsAlive 0x186A10  // 				56 8B F1 8B 06 8B 80 ? ? ? ? FF D0 84 C0 74 19 // fn
#define oIsBaron 0x15C520 //				E8 ? ? ? ? 84 C0 74 0A BB // sub_[offset]
#define oIsTurret 0x1955E0 // 				E8 ? ? ? ? 83 C4 04 84 C0 74 09 5F // sub_[offset]
#define oIsInhib 0x1951F0 // 				E8 ? ? ? ? 83 C4 04 84 C0 74 38 // sub_[offset]
#define oIsHero 0x195370 // 				E8 ? ? ? ? 83 C4 04 84 C0 74 2B 57 // sub_[offset]
#define oIsMinion 0x1953B0 // 				E8 ? ? ? ? 83 C4 04 84 C0 74 16 8B 0F // sub_[offset]
#define oIsDragon 0x15A850 //  				E8 ? ? ? ? 33 DB 84 C0 0F 95 C3 8D 1C 5D ? ? ? ? E9 // sub_[offset]
#define oIsMissile 0x1953D0 //		   		E8 ?? ?? ?? ?? 83 C4 04 84 C0 74 60 // sub_[offset]
#define oIsNexus 0x1952F0 //				E8 ? ? ? ? 83 C4 04 84 C0 0F 95 C0 C3 CC 8A // sub_[offset]
#define oIsNotWall 0x899380 // 				E8 ? ? ? ? 33 C9 83 C4 0C 84 // sub_[offset]
#define oIssueOrder 0x15C5D0 // 			81 EC ? ? ? ? 56 57 8B F9 C7 // fn // E8 ?? ?? ?? ?? 89 ?? ?? ?? ?? ?? 8B 84 ?? ?? ?? ?? ?? 8B CF F3 0F 7E 00 // sub_[offset]
#define oPrintChat 0x546F30 //				E8 ? ? ? ? 6A 00 68 ? ? ? ? E8 ? ? ? ? 83 C4 04 // sub_[offset]
#define oSendChat 0x58A6D0 //				A1 ? ? ? ? 56 6A FF // fn
#define oGetTimerExpiry 0x156270 // 		E8 ? ? ? ? 51 D9 1C 24 E8 ? ? ? ? 8B // sub_[offset]
#define oHealthBarPosition 0x5762A0 //		E8 ?? ?? ?? ?? 8B 4E ?? 8D 54 ?? ?? 52 8B 01 FF ?? ?? 5E 83 ?? ?? C3 // sub_[offset]
#define oBaseDrawPosition 0x1520D0 //		E8 ?? ?? ?? ?? EB ?? 8B 01 8B 40 // sub_[offset]
#define oIssueOrderCheck 0x1B8EB0 // 		83 EC 14 53 55 8B 6C 24 20 56 57 89 // fn

//returns
#define oCastSpellTrueReturn 0x4EBA7C // 	above it should be: call sub_xxxx (CastSpell)
#define oIssueOrderTrueReturn 0x15C864 // 	89 35 ? ? ? ? 8B 84 // above it should be: call sub_xxxx (IssueOrder)

//functions - skinChanger
#define translateString_UNSAFE_DONOTUSE 0xA4B7C0 //		E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 04 8B F0 6A 0B // sub_[offset] // 83 EC 0C 56 8B 74 24 14 56 E8 ? ? ? ? 83 C4 04 89 74 24 04 89 44 24 08 A8 01 // fn
#define CharacterDataStack__Push 0x11B150 //		83 EC 4C 53 55 57 8B F9 8B // fn
#define CharacterDataStack__Update 0x10E530 //		83 EC 18 53 56 57 8D 44 24 20 // fn
#define GetOwnerObject 0x151B50 //		E8 ? ? ? ? 39 44 24 1C 5F // sub_[offset]
#define CharacterData__GetCharacterPackage 0x21F920 //		81 EC ? ? ? ? A1 ? ? ? ? 33 C4 89 84 24 ? ? ? ? 56 8B B4 24 ? ? ? ? 8B C6 // fn // E8 ? ? ? ? 8B 54 24 30 83 C4 08 89 44 24 10 // sub_[offset]
#define GetAiManager 0x1546A0 // E8 ?? ?? ?? ?? 50 8B CE E8 ?? ?? ?? ?? 80 BB ?? ?? ?? ?? ?? // sub_[offset]
#define SetBaseCharacterData 0x181600 // E8 ?? ?? ?? ?? 8B 54 ?? ?? 83 ?? ?? 72 ?? 8B 4C ?? ?? 42 8B C1 81 ?? ?? ?? ?? ?? 72 ?? 8B 49 ?? 83 ?? ?? 2B C1 83 ?? ?? 83 ?? ?? 0F ?? ?? ?? ?? ?? 52 51 E8 ?? ?? ?? ?? 83 ?? ?? 8B 06 8B CE FF ?? ?? ?? ?? ?? 8B CE F3 0F // sub_[offset]
#define IsLaneMinion 0x37A8 //		8A 87 ? ? ? ? 88 4C 24 0B // edi+[offset]h - 1

// ai_base  - skinChanger
#define CharacterDataStack 0x2F60 //		83 C0 C0 8D // go to fn xref to operand, go to 1 result, lea ecx, [esi+[offset]h]
#define SkinId 0x0EBC //		E8 ? ? ? ? 83 C4 0C 80 BF // scroll down, instruction: cmp byte ptr [xxx+[offset]h], 0

//global - skinChanger
#define ChampionManager 0x2F48610 //		89 1D ? ? ? ? 56 8D 4B 04 // 8B 0D ? ? ? ? 83 C1 0C 89 14 24 // 8B 0D ? ? ? ? 57 FF 74 24 08 E8 ? ? ? ? 8B F8 // dword_[offset]
#define ManagerTemplate_AIMinionClient_ 0x22FEE74 //		A1 ? ? ? ? 53 55 8B 6C 24 1C // A1 ? ? ? ? F3 0F 10 40 ? F3 0F 11 44 24 // dword_[offset]
#define ManagerTemplate_AIHero_ 0x16B09E4 // 8B 0D ? ? ? ? 50 8D 44 24 18 // 8B 35 ? ? ? ? C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? E8 // dword_[offset]
#define ManagerTemplate_AITurret_ 0x2F46554 // 8B 3D ? ? ? ? 83 C4 0C F3 0F 10 44 24 ? D9 5C 24 10 // dword_[offset]
#define ManagerTemplate_AIMissile_ 0x2F4D664 // 89 0D ? ? ? ? C7 01 ? ? ? ? 66 // dword_[offset]

//events
#define oOnprocessSpell 0x4D7940 // 		E8 ?? ?? ?? ?? 8B CE E8 ?? ?? ?? ?? 80 BE ?? ?? ?? ?? ?? D8 // sub_[offset]
#define OnCreateObject 0x2794E0 // 		E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 32 C9 EB // E8 ? ? ? ? 0F B6 47 35 // sub_[offset]
#define oOnDeleteObject 0x26A4F0 // 		E8 ? ? ? ? 8B 4E 20 85 C9 74 0F // E8 ? ? ? ? 83 C7 04 3B FB 75 E2 // sub_[offset]
#define oOnNewPath 0x2882D0 // 		83 EC 18 56 8B 74 24 20 8B CE 57 // fn // E8 ? ? ? ? 8B 74 24 4C 83 C4 30 // sub_[offset]

//lol
#define Riot__g_window 0x2F4F3CC // 3B 05 ? ? ? ? 75 72 // dword_[offset]
#define GfxWinMsgProc 0x2F6BA50 // A1 ? ? ? ? 55 57 53 // off_[offset]
#define GameClient 0x22FEE94 // A1 ? ? ? ? 68 ? ? ? ? 8B 70 08 // dword_[offset]
#define D3DDevice 0x208 //8B 86 ? ? ? ? 89 4C 24 08 // mov eax, [esi+[offset]h]
#define SwapChain 0x10C // 8B 8E ? ? ? ? 52 57 // mov ecx, [esi+[offset]h]
#define Riot__Renderer__MaterialRegistry__GetSingletonPtr 0x969C30 // A1 ? ? ? ? 85 C0 75 0B 8B 0D ? ? ? ? 8B 01 FF 60 14 // fn
#define GetGoldRedirectTarget 0x151B50 // E8 ? ? ? ? 39 44 24 1C 5F // E8 ? ? ? ? 8B F8 3B F7 0F 84 // sub_[offset]
#define SummonerEmoteUserComponent__GetSummonerEmoteData 0x23F650 // 81 EC ? ? ? ? A1 ? ? ? ? 33 C4 89 84 24 ? ? ? ? 56 FF B4 24 ? ? ? ? 8D 44 24 14 // fn // E8 ? ? ? ? 83 C4 04 85 C0 74 65 57 // sub_[offset]
#define SummonerEmoteUserComponent__SetEmoteIdForSlot 0x245ED0 // 83 EC 08 56 57 FF 74 24 14 // fn

//CObject
#define oObjIndex 0x20 // 8D 52 20 0F 57 D0 66 0F 38 DC D1 49
#define oObjTeam 0x4C // 8D 70 4C 89 75 DC 89 75 EC C7 45 ? ? ? ? ? FF 75 0C 50
#define oObjName 0x6C // 8D 4E 6C E8 ? ? ? ? 8D 4E 60 E8 ? ? ? ? 8D 4E 54 E8 ? ? ? ? 8D 4E 48 E8 ? ? ? ?
#define oObjNetworkID 0xCC // 8D 83 ? ? ? ? 50 8D 44 24 34 50 E8 ? ? ? ? 8B 00 89 83 ? ? ? ? 8B 8B ? ? ? ?
#define oObjPos 0x1D8 // 8D 8E ? ? ? ? E8 ? ? ? ? 8D 8E ? ? ? ? E8 ? ? ? ? 8D 4E 10
#define oObjVisibility 0x270 // 8D 8E ? ? ? ? E8 ? ? ? ? 8B CE E8 ? ? ? ? F6 44 24 ? ? 5F
#define oObjHealth 0xD98 // 8D 8B ? ? ? ? C6 83 ? ? ? ? ? 50 8D 83 ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 50
#define oObjMana 0x298 // 8D B3 ? ? ? ? F3 0F 11 83 ? ? ? ? BF ? ? ? ? C6 83 ? ? ? ? ?
#define oObjMaxMana (oObjMana + 0x10);
#define oObjArmor 0x1298
#define oObjMagicRes 0x12A0
#define oObjBonusMagicRes 0x0
#define oObjBaseAtk 0x1270 // 8B 81 ? ? ? ? 81 C1 ? ? ? ? 89 47 04 E8 ? ? ? ? 8B 4E 04
#define oObjBonusAtk 0x11F0 // 8D 90 ? ? ? ? 89 8B ? ? ? ? 89 83 ? ? ? ? 89 83 ? ? ? ? C7 83 ? ? ? ? ? ? ? ?
#define oObjMoveSpeed 0x1290 // 8B 82 ? ? ? ? 89 41 08 8B 82 ? ? ? ? 89 41 0C C2 04 00
#define oObjAtkRange 0x12B8 // 8B 9E ? ? ? ? 66 0F D6 4D ? 3B CE 77 0C 3B 76 10 77 07 BF ? ? ? ? EB 02
#define oObjAtkSpeedMod 0x126C // 8B 83 ? ? ? ? 89 46 5C
#define oObjBuffMgr 0x2160 // 8D 8D ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? E8 ? ? ? ?
#define oObjSpellBook 0x2708 // (0x2B80 - 0x478) // 81 C7 ? ? ? ? C6 05 ? ? ? ? ? 6A 01 8B CF E8 ? ? ? ? 8B 83 ? ? ? ?
#define oObjChampionName 0x3134 // 8D 87 ? ? ? ? 89 B7 ? ? ? ? 83 78 14 10 72 02 8B 00
#define oObjLevel 0x36BC // 8D 9E ? ? ? ? 83 C4 14 B9 ? ? ? ? 6A 00 68 ? ? ? ?
#define oObjIsOnScreen 0x1a8 // 89 81 ? ? ? ? C2 04 00 8B 44 24 04 F3 0F 7E 00 66 0F D6 81 ? ? ? ? 8B 40 08 C6 81 ? ? ? ? ? 89 81 ? ? ? ? C2 04 00
#define oObjSourceIndex 0x290 // 89 81 ? ? ? ? 8B 44 24 04 89 81 ? ? ? ?
#define oObjMovementSpeed 0x9C // 89 B3 ? ? ? ? 33 C0
#define oInhiRemainingRespawnTime 0x1014 // C7 83 ? ? ? ? ? ? ? ? E8 ? ? ? ? 85
#define oObjTargetable 0xD00 // 88 83 ? ? ? ? C6 83 ? ? ? ? ? C7 // fn + EBE
#define oObjAlive 0x218 // 51 56 8B F1 C7 06 ? ? ? ? 83 7E // fn + 490
#define oObjVisible 0x270 // E8 ? ? ? ? C7 85 ? ? ? ? ? ? ? ? 33 C9 C7 // sub_xxxx + 5b7

#define oUnitComponentInfo 0x2F3C
#define oUCIProperties 0x1C
#define oBaseAtkSpeed 0x1D0



//ObjectManager
#define objectArray 0x14 // not using

//SpellBook
#define oSpellBookOwner 0x1c // 11.1

//MissileInfo
#define oMissileSpellInfo 0x230 // 11.1
#define oMissileSourceIndex 0x290 // 11.1
#define oMissileTargetIndex 0x2E8 // 11.1 serge
#define oMissileStartPos 0x2A8 // 11.1
#define oMissileEndPos 0x2B4 // 11.1


//Buff
#define O_BUFFMGR_BUFFNAME 0x08 // 11.1
#define O_BUFFMGR_STARTTIME 0xC // 11.1
#define O_BUFFMGR_ENDTIME 0x10 // 11.1
#define O_BUFFMGR_flBUFFCOUNT 0x130 // 11.1
#define O_BUFFMGR_iBUFFCOUNT 0x74 // 11.1
#define oBuffType 0x4 // 11.1
#define oBuffCountAlt 0x24 // 11.1
#define oBuffCountAlt2 0x20 // 11.1

//Zoom
#define oMaxZoom 0x28 // 11.1

//ChatClient
#define oChatClientStruct 0x2F4F818 // xref: game_announcement_OnPauseEnabled // if ( *(_BYTE *)(dword_[offset] + xxxx) )

//ChatClientStruct
#define oIsChatOpen 0x62C //

//oSpellSlot
#define oSpellSlotRemainingCharge 0x58 //11.1
#define oSpellSlotLevel 0x20 // not using
#define oSpellSlotRemainingRechargeCD 0x64 //11.1
#define oSpellSlotTime 0x28 //11.1
#define oSpellSlotCD 0x18 // not using
#define oSpellSlotDamage 0x94 //11.1
#define oSpellSlotSpellInfo 0x13C //11.1
#define oSpellSlotRemainingCD 0x28 //11.1

//oSpellInfoSpellData
#define oSpellInfoSpellData 0x44 //11.1


//ActiveSpellEntry
#define oActive_Spell_Entry 0x2728
#define oActive_Spell_Instance 0x8
#define oActive_Spell_SpellSlot 0xC
#define oActive_Spell_MissleIndex 0x14
#define oActive_Spell_CasterName 0x20
#define oActive_Spell_SenderIndex 0x6c
#define oActive_Spell_SpellStartPosition 0x80
#define oActive_Spell_SpellEndPosition 0x8C
#define oActive_Spell_CastPos 0x98
#define oActive_Spell_TargetIndex 0xC0
#define oActive_Spell_WindupTime 0x4C0
#define oActive_Spell_Cooldown 0x4D4
#define oActive_Spell_Spell 0x4DC
#define oActive_Spell_IsBasicAttack 0x4E0
#define oActive_Spell_IsSpecialAttack 0x4E1
#define oActive_Spell_ManaCost 0x4F0
#define oActive_Spell_CastStartTime 0x524
#define oActive_Spell_CastEndTime 0x528
#define oActiveSpellEntry 0x2728
#define oSpellInfoStartPos 0x78
#define oSpellInfoCastPos 0x98
#define oSpellInfoEndPos 0x84

//SpellData
#define oSpellDataMissileName 0x0058 // not using
#define oSpellDataSpellName 0x0064 //11.1
#define oSpellDataDescription 0x0088 // not using
#define oSpellDataEffectAmount 0xD0 // not using
#define oSpellDataIncreaseDamage 0xEC // not using
#define oSpellDataSpellDuration 0x108 // not using
#define oSpellDataRootDuration 0x15C // not using
#define oSpellDataIncreaseDamageBonus 0x178 // not using
#define oSpellDataCoefficient 0x200 // not using
#define oSpellDataCoefficient2 0x204 // not using
#define oSpellDataMaxHighlightTargets 0x208 // not using
#define oSpellDataCooldownTime 0x280 // not using
#define oSpellDataDelayCastOffsetPercent 0x29C // not using
#define oSpellDataDelayTotalTimePercent 0x2A0 // not using
#define oSpellDataMaxAmmo 0x31C // not using
#define oSpellDataAmmoUsed 0x338 // not using
#define oSpellDataAmmoRechargetime 0x354 // not using
#define oSpellDataMissileSpeed 0x440  // 11.1 jingz // not using
#define oSpellDataSpellWidth 0x474 // 11.1 quakehero123
#define oSpellDataSpellRange 0x3B4 // 11.1 quakehero123
#define oSpellDataEffectSpellRange 0x10C // 11.1
#define oSpellDataEffectSpellRangeAfterEffect 0xB8 // starts from A0 // not using

//champion_manager
#define oChampionManagerChampionList 0xC //11.1
#define oChampionCount 154 //11.1
#define oGetChampionSkins 0x58 //11.1
#define oGetChampionName 0x4 //11.1
