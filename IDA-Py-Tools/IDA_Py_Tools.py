from idc import BADADDR, INF_BASEADDR, SEARCH_DOWN, FUNCATTR_START, FUNCATTR_END
import idc
import idaapi
import datetime

def DecToHex(Addr):
	return "0%0.2X" % Addr

def MakeEnum(enumName, offsetArray):
    print ("enum class %s\r\n{" % enumName)
    for offset in offsetArray:
        if len(offset[0]) == 0:
            print ("")
            continue
        if type(offset[1]) is str:
            print ("   %s = %s," % ( offset[0], offset[1]))
            continue

        fncValue = offset[1] if offset[1] != -1 else 0x0

        locByName = idc.get_name_ea_simple(offset[0])
        isMismatch = locByName == 0x0

        if locByName == BADADDR:
            locByName = fncValue
        
        if locByName > idaapi.get_imagebase():
            set_name(locByName, offset[0])
            locByName = locByName - idaapi.get_imagebase()

        print ("   %s = %s,%s" % (offset[0], hex(locByName), '// Possible mismatch' if isMismatch else ''))

    print ("};\r\n")
    
def FindFunctionAddr(name, offset, operandValue):
    address = idc.find_binary(0, SEARCH_DOWN, "\"" + name + "\"")
    dword = -1
    
    if address == BADADDR:
        return BADADDR
    
    xrefs = XrefsTo(address)
    for xref in xrefs:
        dword = xref.frm + offset
    
    if dword == BADADDR:
        return BADADDR
    
    return idc.get_operand_value(dword, operandValue)

def FindOffsetPattern(Pattern, Operand):
	addr = idc.find_binary(0, SEARCH_DOWN, Pattern)
	if addr == BADADDR: return 0
	
	return idc.get_operand_value(addr, Operand)

def FindFunctionFirstXRef(name):
    address = idc.find_binary(0, SEARCH_DOWN, "\"" + name + "\"")
    dword = BADADDR
    
    if address == BADADDR:
        return BADADDR
    
    xrefs = XrefsTo(address)
    for xref in xrefs:
        dword = xref.frm
    
    try:
        return idaapi.get_func(dword).start_ea
    except Exception:
        return -1

def FindFunctionByPatternStartEA(pattern):
    address = idc.find_binary(0, SEARCH_DOWN, pattern)
    if address == BADADDR:
        return BADADDR
    

    try:
        return idaapi.get_func(address).start_ea
    except Exception:
        return -1
        
def FindFuncCall(Pattern): # Find's Func. by Pattern to a Call
    addr = idc.find_binary(0, SEARCH_DOWN, Pattern)
    if addr == BADADDR: return 0
    return idc.get_operand_value(addr, 0)

def main():
    print ("[*] League of Legends Client Update Tool")
    print ("[*] By Dencelle for unknowncheats.me")
    print ("[*] Started at: %s" % datetime.datetime.now())
    print ("----------------------------")
    # Functions that need to be sorted
    MakeEnum("Functions", [
        ["GetNextObject", FindFunctionByPatternStartEA("8B 44 24 04 56 8B 71 18")],
        ["GameVersion", FindFuncCall("E8 ? ? ? ? 50 68 ? ? ? ? 6A 00 6A 01 6A 02 E8 ? ? ? ? E8 ? ? ? ?")],
        ["GetFirstObject", FindFuncCall("E8 ? ? ? ? 8B F0 85 F6 74 21 0F 1F 44 00 ?")],
        ["WorldToScreen", FindFunctionByPatternStartEA("83 EC 10 56 E8 ? ? ? ? 8B 08")],
        ["CastSpell", FindFunctionFirstXRef("ERROR: Client Tried to cast a spell from")],
        ["DrawCircle", FindFunctionByPatternStartEA("33 C4 89 84 24 ? ? ? ? F3 0F 10 84 24 ? ? ? ? 8D 0C")],
        ["GetBasicAttack", FindFunctionByPatternStartEA("53 8B D9 B8 ? ? ? ? 8B 93")],
        ["GetAttackCastDelay", FindFunctionByPatternStartEA("83 EC 0C 53 8B 5C 24 14 8B CB 56")],
        ["GetAttackDelay", FindFunctionByPatternStartEA("8B 44 24 04 51 F3")],
        ["GetPing", FindFunctionByPatternStartEA("55 8B EC 83 EC 08 0F B6")],
        ["GetSpellState", FindFuncCall("E8 ? ? ? ? 8B F8 8B CB 89")],
        ["IsTargetable", FindFunctionByPatternStartEA("56 8B F1 E8 ? ? ? ? 84 C0 74 2E 8D")],
        ["IsAlive", FindFunctionByPatternStartEA("56 8B F1 8B 06 8B 80 ? ? ? ? FF D0 84 C0 74 19")],
        ["IsBaron", FindFunctionByPatternStartEA("56 81 C1 ? ? ? ? E8 ? ? ? ? 68")],
        ["IsTurret", FindFuncCall("E8 ? ? ? ? 83 C4 04 84 C0 75 ?")],
        ["IsInhib", FindFuncCall("E8 ? ? ? ? 55 88 44 24 1F")],
        ["IsHero", FindFuncCall("E8 ? ? ? ? 83 C4 04 84 C0 74 52")],
        ["IsMinion", FindFuncCall("E8 ? ? ? ? 83 C4 04 80 7F 26 06")],
        ["IsDragon", FindFunctionByPatternStartEA("83 EC 10 A1 ? ? ? ? 33 C4 89 44 24 0C 56 81")],
        ["IsMissile", FindFuncCall("E8 ? ? ? ? 83 C4 04 84 C0 74 3F")],
        ["IsNexus", FindFuncCall("E8 ? ? ? ? 55 88 44 24 20")],
        ["IsNotWall", FindFunctionByPatternStartEA("85 FF 0F 48 C3 0F AF C8 8B 86 ? ? ? ? 5F 5E 5B 03 CA 8D")],
        ["IsTroy", FindFuncCall("E8 ? ? ? ? 33 C9 83 C4 04 84 C0 0F 45 4C 24 ? 8B C1 C3 CC CC CC CC CC CC CC CC FF 74 24 04 E8 ? ? ? ? 33 C9 83 C4 04 84 C0 0F 45 4C 24 ? 8B C1 C3 CC CC CC CC CC CC CC CC 56")],
        ["IssueOrder", FindFuncCall("E8 ?? ?? ?? ?? 89 ?? ?? ?? ?? ?? 8B 84 ?? ?? ?? ?? ?? 8B CF F3 0F 7E 00")],
        ["PrintChat", FindFuncCall("E8 ? ? ? ? 8B 77 40 8A 47 48")],
        ["SendChat", FindFunctionByPatternStartEA("A1 ? ? ? ? 56 6A FF")],
        ["GetTimerExpiry", FindFuncCall("E8 ? ? ? ? 51 D9 1C 24 E8 ? ? ? ? 8B")],
        ["HealthBarPosition", FindFuncCall("E8 ?? ?? ?? ?? 8B 4E ?? 8D 54 ?? ?? 52 8B 01 FF ?? ?? 5E 83 ?? ?? C3")],
        ["BaseDrawPosition", FindFunctionByPatternStartEA("30 44 14 10 42 3B D1 72 F0 8B 74 24 14 83 7C 24 ? ? 74")],
        ["Hud_OnDisconnect", FindFunctionFirstXRef("game_messagebox_caption_disconnect")],
        ["Hud_OnAfk", FindFunctionFirstXRef("game_messagebox_text_afkwarningcaption")],
        ["OnCreateObject", FindFuncCall("E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 32 C9 EB")],
        ["oOnDeleteObject", FindFuncCall("E8 ? ? ? ? 8B 4E 20 85 C9 74 0F")],
        ["translateString_UNSAFE_DONOTUSE", FindFuncCall("E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 04 8B F0 6A 0B")],
        ["CharacterDataStack__Push", FindFunctionByPatternStartEA("83 EC 4C 53 55 57 8B F9 8B 47 04")],
        ["CharacterDataStack__Update", FindFunctionByPatternStartEA("83 EC 18 53 56 57 8D 44 24 20")],
        ["GetOwnerObject", FindFuncCall("E8 ? ? ? ? 39 44 24 1C 5F")],
        ["CharacterData__GetCharacterPackage", FindFunctionByPatternStartEA("81 EC ? ? ? ? A1 ? ? ? ? 33 C4 89 84 24 ? ? ? ? 56 8B B4 24 ? ? ? ? 8B C6")],
        ["GetAiManager", FindFuncCall("E8 ?? ?? ?? ?? 50 8B CE E8 ?? ?? ?? ?? 80 BB ?? ?? ?? ?? ??")],
        ["SetBaseCharacterData", FindFuncCall("E8 ?? ?? ?? ?? 8B 54 ?? ?? 83 ?? ?? 72 ?? 8B 4C ?? ?? 42 8B C1 81 ?? ?? ?? ?? ?? 72 ?? 8B 49 ?? 83 ?? ?? 2B C1 83 ?? ?? 83 ?? ?? 0F ?? ?? ?? ?? ?? 52 51 E8 ?? ?? ?? ?? 83 ?? ?? 8B 06 8B CE FF ?? ?? ?? ?? ?? 8B CE F3 0F")],
        ["OnprocessSpell", FindFuncCall("E8 ?? ?? ?? ?? 8B CE E8 ?? ?? ?? ?? 80 BE ?? ?? ?? ?? ?? D8")],
        ["OnNewPath", FindFunctionByPatternStartEA("83 EC 18 56 8B 74 24 20 8B CE 57")],
        ["Riot__Renderer__MaterialRegistry__GetSingletonPtr", FindFunctionByPatternStartEA("A1 ? ? ? ? 85 C0 75 0B 8B 0D ? ? ? ? 8B 01 FF 60 14")],
        ["GetGoldRedirectTarget", FindFuncCall("E8 ? ? ? ? 39 44 24 1C 5F")],
        ["SummonerEmoteUserComponent__GetSummonerEmoteData", FindFunctionByPatternStartEA("81 EC ? ? ? ? A1 ? ? ? ? 33 C4 89 84 24 ? ? ? ? 56 FF B4 24 ? ? ? ? 8D 44 24 14")],
        ["SummonerEmoteUserComponent__SetEmoteIdForSlot", FindFunctionByPatternStartEA("83 EC 08 56 57 FF 74 24 14")],
    ])
    # Spoof Returns
    MakeEnum("Offsets", [
        ["RetAddr", idc.find_binary(0, SEARCH_DOWN, "E8 ? ? ? ? 83 C4 1C C3 E8 ? ? ? ?")+0x8],
        ["DrawCircleRetAddr", idc.find_binary(0, SEARCH_DOWN, "83 C4 1C C3")],
	])
    # Offsets that need to be sorted
    MakeEnum("Offsets", [
        ["NetClient", FindOffsetPattern("8B 0D ? ? ? ? 85 C9 74 07 8B 01 6A 01 FF 50 08 8B", 1)],
        ["PingInstance", FindOffsetPattern("8B 0D ? ? ? ? 85 C9 74 07 8B 01 6A 01 FF 50 08 8B", 1)],
        ["ChatClientPtr", FindOffsetPattern("8B 35 ? ? ? ? 8D 44 24 14 53 8B 1D ? ? ? ? 8B CF", 1)],
        ["ObjManager", FindOffsetPattern("8B 0D ? ? ? ? 89 74 24 14", 1)],
        ["ZoomClass", FindOffsetPattern("A3 ? ? ? ? 83 FA 10 72 32", 0)],
        ["GameInfo", FindOffsetPattern("A1 ? ? ? ? 83 78 08 02 0F 94", 1)],
        ["HudInstance", FindOffsetPattern("8B 0D ? ? ? ? 6A 00 8B 49 34 E8 ? ? ? ? B0 01 C2", 1)],
        ["Renderer", FindOffsetPattern("8B 15 ? ? ? ? 83 EC 08", 1)],
        ["UnderMouseObject", FindOffsetPattern("8B 0D ? ? ? ? 89 0D", 1)],
        ["D3DRenderer", FindOffsetPattern("A1 ? ? ? ? 68 ? ? ? ? 8B 70 08 E8", 1)],
        ["GameTime", FindOffsetPattern("F3 0F 11 05 ? ? ? ? 8B 49", 0)],
        ["MenuGUI", FindOffsetPattern("8B 0D ? ? ? ? 6A 00 E8 ? ? ? ? C7", 1)],
        ["Riot__g_window", FindOffsetPattern("3B 05 ? ? ? ? 75 72", 1)],
        ["GfxWinMsgProc", FindOffsetPattern("A1 ? ? ? ? 55 57 53", 1)],
        ["GameClient", FindOffsetPattern("A1 ? ? ? ? 68 ? ? ? ? 8B 70 08", 1)],
        ["D3DDevice", FindOffsetPattern("8B 86 ? ? ? ? 89 4C 24 08", 1)],
        ["SwapChain", FindOffsetPattern("8B 8E ? ? ? ? 52 57", 1)],
        ["MissionInfo", FindOffsetPattern("8B 0D ? ? ? ? 83 C4 0C 8B 35 ? ? ? ? E8 ? ? ? ? 50", 1)],
        ["ChampionManager", FindOffsetPattern("89 1D ?? ?? ?? ?? 56 8D 4B 04", 0)],
        ["LocalPlayer", FindOffsetPattern("A1 ?? ?? ?? ?? 85 C0 74 07 05 ?? ?? ?? ?? EB 02 33 C0 56", 1)],
        ["ManagerTemplate_AIMinionClient_", FindOffsetPattern("A1 ?? ?? ?? ?? 53 55 8B 6C 24 1C", 1)],
        ["ManagerTemplate_AIHero_", FindOffsetPattern("8B 0D ?? ?? ?? ?? 50 8D 44 24 18", 1)],
        ["ManagerTemplate_AITurret_", FindOffsetPattern("A1 ? ? ? ? 53 56 8B 70 04", 1)],
        ["ManagerTemplate_AIMissile_", FindOffsetPattern("8B 1D ? ? ? ? 85 DB 0F 84 ? ? ? ? 0F B6 43 08 33 C9", 1)],
	])
    #cobject offsets
    MakeEnum("CObject", [
        ["SkinId", FindOffsetPattern("80 BF ? ? ? ? ? 75 50 0F 31 33 C9 66 C7 87 ? ? ? ? ? ?", 0)],
        ["oObjIndex", FindOffsetPattern("8D 52 20 0F 57 D0 66 0F 38 DC D1 49", 1)],
        ["oObjTeam", FindOffsetPattern("8D 70 4C 89 75 DC 89 75 EC C7 45 ? ? ? ? ? FF 75 0C 50", 1)],
        ["oObjName", FindOffsetPattern("8D 4E 6C E8 ? ? ? ? 8D 4E 60 E8 ? ? ? ? 8D 4E 54 E8 ? ? ? ? 8D 4E 48 E8 ? ? ? ?", 1)],
        ["oObjNetworkID", FindOffsetPattern("8D 83 ? ? ? ? 50 8D 44 24 34 50 E8 ? ? ? ? 8B 00 89 83 ? ? ? ? 8B 8B ? ? ? ?", 1)],
        ["oObjPos", FindOffsetPattern("8D 8E ? ? ? ? E8 ? ? ? ? 8D 8E ? ? ? ? E8 ? ? ? ? 8D 4E 10", 1)],
        ["oObjVisibility", FindOffsetPattern("8D 8E ? ? ? ? E8 ? ? ? ? 8B CE E8 ? ? ? ? F6 44 24 ? ? 5F", 1)],
        ["oObjHealth", FindOffsetPattern("8D 8B ? ? ? ? C6 83 ? ? ? ? ? 50 8D 83 ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 50", 1)],
        ["oObjMaxHealth", FindOffsetPattern("8D 8B ? ? ? ? C6 83 ? ? ? ? ? 50 8D 83 ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 50", 1)+0x10],
        ["oObjMana", FindOffsetPattern("8D B3 ? ? ? ? F3 0F 11 83 ? ? ? ? BF ? ? ? ? C6 83 ? ? ? ? ?", 1)],
        ["oObjMaxMana", FindOffsetPattern("8D B3 ? ? ? ? F3 0F 11 83 ? ? ? ? BF ? ? ? ? C6 83 ? ? ? ? ?", 1)+0x10],
        ["oObjArmor", FindOffsetPattern("C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 89 BB ? ? ? ? C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 89 BB ? ? ? ?", 0)],
        ["oObjMagicRes", FindOffsetPattern("C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 89 BB ? ? ? ? C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ?", 0)],
        ["oObjBonusMagicRes", FindOffsetPattern("C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? C7 83 ? ? ? ? ? ? ? ? 33 C9 89 B3 ? ? ? ? 33 C0", 0)],
        ["oObjBaseAtk", FindOffsetPattern("8B 81 ? ? ? ? 81 C1 ? ? ? ? 89 47 04 E8 ? ? ? ? 8B 4E 04", 1)],
        ["oObjBonusAtk", FindOffsetPattern("8D 90 ? ? ? ? 89 8B ? ? ? ? 89 83 ? ? ? ? 89 83 ? ? ? ? C7 83 ? ? ? ? ? ? ? ?", 1)],
        ["oObjMoveSpeed", FindOffsetPattern("8B 82 ? ? ? ? 89 41 08 8B 82 ? ? ? ? 89 41 0C C2 04 00", 1)],
        ["oObjAtkRange", FindOffsetPattern("8B 9E ? ? ? ? 66 0F D6 4D ? 3B CE 77 0C 3B 76 10 77 07 BF ? ? ? ? EB 02", 1)],
        ["oObjBuffMgr", FindOffsetPattern("8D 8D ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? C7 85 ? ? ? ? ? ? ? ? E8 ? ? ? ?", 1)],
        ["oObjSpellBook", FindOffsetPattern("81 C7 ? ? ? ? C6 05 ? ? ? ? ? 6A 01 8B CF E8 ? ? ? ? 8B 83 ? ? ? ?", 1)],
        ["oObjChampionName", FindOffsetPattern("8D 87 ? ? ? ? 89 B7 ? ? ? ? 83 78 14 10 72 02 8B 00", 1)],
        ["oObjLevel", FindOffsetPattern("8D 9E ? ? ? ? 83 C4 14 B9 ? ? ? ? 6A 00 68 ? ? ? ?", 1)],
        ["oObjIsOnScreen", FindOffsetPattern("89 81 ? ? ? ? C2 04 00 8B 44 24 04 F3 0F 7E 00 66 0F D6 81 ? ? ? ? 8B 40 08 C6 81 ? ? ? ? ? 89 81 ? ? ? ? C2 04 00", 0)],
        ["oObjSourceIndex", FindOffsetPattern("89 81 ? ? ? ? 8B 44 24 04 89 81 ? ? ? ?", 0)],
        ["oObjMovementSpeed", FindOffsetPattern("89 B3 ? ? ? ? 33 C0", 0)],
        ["IsLaneMinion", FindOffsetPattern("8A 87 ? ? ? ? 88 4C 24 0B", 1)-1],
        ["CharacterDataStack", FindOffsetPattern("8B 8F ? ? ? ? 85 C9 74 05 E8 ? ? ? ? 89 B7 ? ? ? ? 5F 5E 83 C4 10 C2 0C 00", 1)],
        ["oObjPerk1", FindOffsetPattern("8D 8E ? ? ? ? E8 ? ? ? ? 8B CE E8 ? ? ? ? A1", 1)],
        ["ModelName", FindOffsetPattern("8D 8F ? ? ? ? 8D 44 24 40 3B C8 74 14 83 7C 24 ? ? FF 74 24 50 0F 43 44 24 ? 50", 1)],
        ["ExperiencePoints", FindOffsetPattern("8D 8B ? ? ? ? E8 ? ? ? ? 8D 83 ? ? ? ? 50 8D AB ? ? ? ? 55", 1)],
        ["SpellUpgradePoints", FindOffsetPattern("89 9F ? ? ? ? 8B 8F ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 8B 01", 1)],
        ["HeroInventory", FindOffsetPattern("8D 8B ? ? ? ? E8 ? ? ? ? 8D B3 ? ? ? ? C7 06 ? ? ? ? 89 5E 04 6A 18", 1)],
        ["CharacterDataStack", FindOffsetPattern("81 C1 ? ? ? ? E8 ? ? ? ? F3 0F 7E 05 ? ? ? ? 68 ? ? ? ?", 1)],
	])
    
main()