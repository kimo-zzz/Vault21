#pragma once
#include <Windows.h>
#include "EnumMacro.h"

namespace V21 {
	enum SpellType
	{
		kLine,
		kMissileLine,
		kBox,
		kCircle,
		kCone,
		kMissileCone,
		kArc,
		kRing
	};

	enum DangerLevel
	{
		kLow,
		kMedium,
		kHigh,
		kExtreme
	};

	enum SpellCollisionFlags
	{
		kMinions = 1 << 0,
		kHeroes = 1 << 1,
		kYasuoWall = 1 << 2,
		kWall = 1 << 3
	};

	enum CrowdControllFlags
	{
		kHardCc = 1,
		kSlow = 2
	};

	enum SpellValidTargets
	{
		kAllyMinions = 1 << 0,
		kEnemyMinions = 1 << 1,

		kAllyWards = 1 << 2,
		kEnemyWards = 1 << 3,

		kAllyChampions = 1 << 4,
		kEnemyChampions = 1 << 5
	};

	enum EvadingSpellType
	{
		kWindwall,
		kDash,
		kBlink,
		kInvulnerability,
		kMovementSpeedBuff,
		kShield
	};

	enum eSkillshotType
	{
		kSkillshotLine,
		kSkillshotCircle,
		kSkillshotCone
	};

	enum class eOrbwalkingMode
	{
		kModeNone = 0,
		kModeCombo = 1 << 1,
		kModeMixed = 1 << 2,
		kModeFarm = 1 << 3,
		kModeLaneClear = 1 << 4,
		kModeHarass = 1 << 5,
		kModeFlee = 1 << 6
	};

	enum eCollisionFlags
	{
		kCollidesWithNothing = 0,
		kCollidesWithMinions = 1 << 1,
		kCollidesWithHeroes = 1 << 2,
		kCollidesWithYasuoWall = 1 << 3,
		kCollidesWithWalls = 1 << 4
	};

	enum class CombatType
	{
		Melee = 1,
		Ranged = 2
	};

	enum class CursorClickType
	{
		Normal,
		Red
	};

	enum text_flags
	{
		text_flags_centered_x = 1,
		text_flags_centered_y = 2,
		text_flags_outline = 4,
		text_flags_drop_shadow = 8
	};

	enum class DamageType
	{
		Physical = 0,
		Magical = 1,
		True = 2,
	};

	enum class HitChance : int
	{
		Immobile = 8,
		Dashing = 7,
		VeryHigh = 6,
		High = 5,
		Medium = 4,
		Low = 3,
		Impossible = 2,
		OutOfRange = 1,
		Collision = 0
	};

	enum _KeybindType
	{
		KeybindType_Hold,
		KeybindType_Toggle
	};
	typedef int KeybindType;

	enum eNavCollisionFlags
	{
		kNavFlagsNone,
		kNavFlagsGrass = 1 << 0,
		kNavFlagsWall = 1 << 1,
		kNavFlagsBuilding = 1 << 6,
		kNavFlagsProp = 1 << 7,
		kNavFlagsGlobalVision = 1 << 8
	};

	enum class GameType
	{
		Normal,
		Dominion,
		Tutorial,
		ARAM,
		FirstBlood,
		Ascension,
		KingPoro,
		Siege
	};

	enum class GameMapId
	{
		CrystalScar = 8,
		TwistedTreeline = 10,
		SummonersRift,
		HowlingAbyss
	};

	enum class EntityTypeFlags
	{
		BaseAI = 0xA,
		MinionAI = 0xB,
		HeroAI = 0xC,
		TurretAI = 0xD,
		MissileClient = 0xF,
		Building = 0x11
	};

	enum class EntityType
	{
		NeutralMinionCamp = 1,
		AIHeroClient = 2,
		AIMarker = 3,
		AIMinionClient = 4,
		LevelPropAIClient = 5,
		AITurretClient = 6,
		obj_GeneralParticleEmitter = 7,
		MissileClient = 8,
		DrawFX = 9,
		BarracksDampener = 10,
		Barracks = 11,
		obj_SpawnPoint = 12,
		GrassObject = 13,
		HQ = 14,
		obj_InfoPoint = 15,
		LevelPropGameObject = 16,
		LevelPropSpawnerPoint = 17,
		Shop = 18,
		obj_Turret = 19,
		AIBase = 20,
		Any = 21
	};

	enum class GameObjectTeam
	{
		Unknown,
		Order = 100,
		Chaos = 200,
		Neutral = 300
	};

	enum eInvulnerabilityFlags : int
	{
		kNotInvulnerable = 0,
		kInvulnerable,
		kDeadProtection
	};

	enum class IssueOrderType
	{
		HoldPosition = 1,
		MoveTo = 2,
		AttackUnit = 3,
		AutoAttackPet = 4,
		AutoAttack = 5,
		MovePet = 6,
		AttackTo = 7,
		Stop = 10
	};

	enum class SurrenderVoteType : int
	{
		Yes = 1,
		No = 2
	};

	enum class PingType
	{
		Default = 0,
		Danger = 2,
		EnemyMissing = 3,
		OnMyWay = 4,
		AssistMe = 6,
		VisionHere = 7
	};

	enum class EmoteType
	{
		Dance = 0,
		Taunt,
		Laugh,
		Joke,
		Toggle
	};

	enum class SpellTargeting
	{
		Self,
		Target,
		Area,
		Cone,
		SelfArea,
		Variable,
		Location,
		Direction,
		Vector,
		Global = 100
	};

	enum class EUnitType
	{
		Unknown = 0,
		Minion = 0x800,
		Troy = 0x801,
		Dragon = 0x802,
		Baron = 0x803,
		Hero = 0x1000,
		Turret = 0x2000,
		Missile = 0x8000,
		Inhibitor = 0x20000,
		Nexus = 0x20001,
	};

	enum class ChampionId
	{
		Unknown = 0,
		Aatrox = 1,
		Ahri = 2,
		Akali = 3,
		Alistar = 4,
		Amumu = 5,
		Anivia = 6,
		Annie = 7,
		Ashe = 8,
		AurelionSol = 9,
		Azir = 10,
		Bard = 11,
		Blitzcrank = 12,
		Brand = 13,
		Braum = 14,
		Caitlyn = 15,
		Camille = 16,
		Cassiopeia = 17,
		Chogath = 18,
		Corki = 19,
		Darius = 20,
		Diana = 21,
		Draven = 22,
		DrMundo = 23,
		Ekko = 24,
		Elise = 25,
		Evelynn = 26,
		Ezreal = 27,
		Fiddlesticks = 28,
		Fiora = 29,
		Fizz = 30,
		Galio = 31,
		Gangplank = 32,
		Garen = 33,
		Gnar = 34,
		Gragas = 35,
		Graves = 36,
		Hecarim = 37,
		Heimerdinger = 38,
		Illaoi = 39,
		Irelia = 40,
		Ivern = 41,
		Janna = 42,
		JarvanIV = 43,
		Jax = 44,
		Jayce = 45,
		Jhin = 46,
		Jinx = 47,
		Kalista = 48,
		Karma = 49,
		Karthus = 50,
		Kassadin = 51,
		Katarina = 52,
		Kayle = 53,
		Kayn = 54,
		Kennen = 55,
		Khazix = 56,
		Kindred = 57,
		Kled = 58,
		KogMaw = 59,
		Leblanc = 60,
		LeeSin = 61,
		Leona = 62,
		Lissandra = 63,
		Lucian = 64,
		Lulu = 65,
		Lux = 66,
		Malphite = 67,
		Malzahar = 68,
		Maokai = 69,
		MasterYi = 70,
		MissFortune = 71,
		MonkeyKing = 72,
		Mordekaiser = 73,
		Morgana = 74,
		Nami = 75,
		Nasus = 76,
		Nautilus = 77,
		Nidalee = 78,
		Nocturne = 79,
		Nunu = 80,
		Olaf = 81,
		Orianna = 82,
		Ornn = 83,
		Pantheon = 84,
		Poppy = 85,
		PracticeTool_TargetDummy = 86,
		Quinn = 87,
		Rakan = 88,
		Rammus = 89,
		RekSai = 90,
		Renekton = 91,
		Rengar = 92,
		Riven = 93,
		Rumble = 94,
		Ryze = 95,
		Sejuani = 96,
		Shaco = 97,
		Shen = 98,
		Shyvana = 99,
		Singed = 100,
		Sion = 101,
		Sivir = 102,
		Skarner = 103,
		Sona = 104,
		Soraka = 105,
		Swain = 106,
		Syndra = 107,
		TahmKench = 108,
		Taliyah = 109,
		Talon = 110,
		Taric = 111,
		Teemo = 112,
		Thresh = 113,
		Tristana = 114,
		Trundle = 115,
		Tryndamere = 116,
		TwistedFate = 117,
		Twitch = 118,
		Udyr = 119,
		Urgot = 120,
		Varus = 121,
		Vayne = 122,
		Veigar = 123,
		Velkoz = 124,
		Vi = 125,
		Viktor = 126,
		Vladimir = 127,
		Volibear = 128,
		Warwick = 129,
		Xayah = 130,
		Xerath = 131,
		XinZhao = 132,
		Yasuo = 133,
		Yorick = 134,
		Zac = 135,
		Zed = 136,
		Ziggs = 137,
		Zilean = 138,
		Zoe = 139,
		Zyra = 140,
		Kaisa = 141,
		Pyke = 142,
		Neeko = 143,
		Sylas = 144,
		Yuumi = 145,
		Qiyana = 146
	};


	enum class ItemId
	{
		Boots_of_Speed = 1001,
		Faerie_Charm = 1004,
		Rejuvenation_Bead = 1006,
		Giants_Belt = 1011,
		Cloak_of_Agility = 1018,
		Blasting_Wand = 1026,
		Sapphire_Crystal = 1027,
		Ruby_Crystal = 1028,
		Cloth_Armor = 1029,
		Chain_Vest = 1031,
		Null_Magic_Mantle = 1033,
		Long_Sword = 1036,
		Pickaxe = 1037,
		B_F_Sword = 1038,
		Hunters_Talisman = 1039,
		Hunters_Machete = 1041,
		Dagger = 1042,
		Recurve_Bow = 1043,
		Brawlers_Gloves = 1051,
		Amplifying_Tome = 1052,
		Vampiric_Scepter = 1053,
		Dorans_Shield = 1054,
		Dorans_Blade = 1055,
		Dorans_Ring = 1056,
		Negatron_Cloak = 1057,
		Needlessly_Large_Rod = 1058,
		Space_Vampiric_Scepter = 1059,
		Dark_Seal = 1082,
		Cull = 1083,
		Stalkers_Blade_Enchantment_Warrior = 1400,
		Stalkers_Blade_Enchantment_Cinderhulk = 1401,
		Stalkers_Blade_Enchantment_Runic_Echoes = 1402,
		Skirmishers_Sabre_Enchantment_Warrior = 1412,
		Skirmishers_Sabre_Enchantment_Cinderhulk = 1413,
		Skirmishers_Sabre_Enchantment_Runic_Echoes = 1414,
		Stalkers_Blade_Enchantment_Bloodrazor = 1416,
		Skirmishers_Sabre_Enchantment_Bloodrazor = 1419,
		Health_Potion = 2003,
		Mana_Potion = 2004,
		Total_Biscuit_of_Rejuvenation = 2009,
		Total_Biscuit_of_Everlasting_Will = 2010,
		Elixir_Of_Skill = 2011,
		Looted_Biscuit_of_Rejuvenation = 2012,
		Looted_Oracles_Extract = 2013,
		Kircheis_Shard = 2015,
		Refillable_Potion = 2031,
		Corrupting_Potion = 2033,
		Oracles_Extract = 2047,
		Explorers_Ward = 2050,
		Guardians_Horn = 2051,
		Poro_Snax = 2052,
		Raptor_Cloak = 2053,
		Diet_Poro_Snax = 2054,
		Control_Ward = 2055,
		Pilfered_Stealth_Ward = 2056,
		Peering_Farsight_Ward = 2057,
		Travel_Size_Elixir_of_Iron = 2058,
		Travel_Size_Elixir_of_Sorcery = 2059,
		Travel_Size_Elixir_of_Wrath = 2060,
		Pilfered_Health_Potion = 2061,
		Pilfered_Potion_of_Rouge = 2062,
		Shurelyas_Reverie = 2065,
		Elixir_of_Iron = 2138,
		Elixir_of_Sorcery = 2139,
		Elixir_of_Wrath = 2140,
		Sly_Sack_of_Gold = 2319,
		Minion_Dematerializer = 2403,
		Commencing_Stopwatch = 2419,
		Stopwatch = 2420,
		Broken_Stopwatch = 2421,
		Slightly_Magical_Boots = 2422,
		Stopwatch_1 = 2423,
		Broken_Stopwatch_1 = 2424,
		Abyssal_Mask = 3001,
		Archangels_Staff = 3003,
		Manamune = 3004,
		Atmas_Reckoning = 3005,
		Berserkers_Greaves = 3006,
		Archangels_Staff_Quick_Charge = 3007,
		Manamune_Quick_Charge = 3008,
		Boots_of_Swiftness = 3009,
		Catalyst_of_Aeons = 3010,
		Sorcerers_Shoes = 3020,
		Frozen_Mallet = 3022,
		Glacial_Shroud = 3024,
		Iceborn_Gauntlet = 3025,
		Guardian_Angel = 3026,
		Rod_of_Ages = 3027,
		Chalice_of_Harmony = 3028,
		Rod_of_Ages_Quick_Charge = 3029,
		Hextech_GLP_800 = 3030,
		Infinity_Edge = 3031,
		Mortal_Reminder = 3033,
		Last_Whisper = 3035,
		Lord_Dominiks_Regards = 3036,
		Seraphs_Embrace = 3040,
		Mejais_Soulstealer = 3041,
		Muramana = 3042,
		Muramana_1 = 3043,
		Phage = 3044,
		Phantom_Dancer = 3046,
		Ninja_Tabi = 3047,
		Seraphs_Embrace_1 = 3048,
		Zekes_Convergence = 3050,
		Jaurims_Fist = 3052,
		Steraks_Gage = 3053,
		Ohmwrecker = 3056,
		Sheen = 3057,
		Spirit_Visage = 3065,
		Kindlegem = 3067,
		Sunfire_Cape = 3068,
		Remnant_of_the_Ascended = 3069,
		Tear_of_the_Goddess = 3070,
		Black_Cleaver = 3071,
		Bloodthirster = 3072,
		Tear_of_the_Goddess_Quick_Charge = 3073,
		Ravenous_Hydra = 3074,
		Thornmail = 3075,
		Bramble_Vest = 3076,
		Tiamat = 3077,
		Trinity_Force = 3078,
		Space_Bloodthirster = 3080,
		Wardens_Mail = 3082,
		Warmogs_Armor = 3083,
		Overlords_Bloodmail = 3084,
		Runaans_Hurricane = 3085,
		Zeal = 3086,
		Statikk_Shiv = 3087,
		Rabadons_Deathcap = 3089,
		Wooglets_Witchcap = 3090,
		Wits_End = 3091,
		Remnant_of_the_Watchers = 3092,
		Rapid_Firecannon = 3094,
		Stormrazor = 3095,
		Nomads_Medallion = 3096,
		Targons_Brace = 3097,
		Frostfang = 3098,
		Lich_Bane = 3100,
		Stinger = 3101,
		Banshees_Veil = 3102,
		Lord_Van_Damms_Pillager = 3104,
		Aegis_of_the_Legion = 3105,
		Redemption = 3107,
		Fiendish_Codex = 3108,
		Knights_Vow = 3109,
		Frozen_Heart = 3110,
		Mercurys_Treads = 3111,
		Guardians_Orb = 3112,
		Aether_Wisp = 3113,
		Forbidden_Idol = 3114,
		Nashors_Tooth = 3115,
		Rylais_Crystal_Scepter = 3116,
		Boots_of_Mobility = 3117,
		Space_Knights_Vow = 3118,
		Wicked_Hatchet = 3122,
		Executioners_Calling = 3123,
		Guinsoos_Rageblade = 3124,
		Deathfire_Grasp = 3128,
		Sword_of_the_Divine = 3131,
		Caulfields_Warhammer = 3133,
		Serrated_Dirk = 3134,
		Void_Staff = 3135,
		Haunting_Guise = 3136,
		Dervish_Blade = 3137,
		Mercurial_Scimitar = 3139,
		Quicksilver_Sash = 3140,
		Youmuus_Ghostblade = 3142,
		Randuins_Omen = 3143,
		Bilgewater_Cutlass = 3144,
		Hextech_Revolver = 3145,
		Hextech_Gunblade = 3146,
		Duskblade_of_Draktharr = 3147,
		Space_Hextech_Gunblade = 3148,
		Space_Blade_of_the_Ruined_King = 3149,
		Liandrys_Torment = 3151,
		Hextech_Protobelt_01 = 3152,
		Blade_of_the_Ruined_King = 3153,
		Hexdrinker = 3155,
		Maw_of_Malmortius = 3156,
		Zhonyas_Hourglass = 3157,
		Ionian_Boots_of_Lucidity = 3158,
		Spear_of_Shojin = 3161,
		Space_Bilgewater_Cutlass = 3162,
		Space_Maw_of_Malmortius = 3163,
		Morellonomicon = 3165,
		Moonflair_Spellblade = 3170,
		Zephyr = 3172,
		Space_Boots_of_Lucidity = 3173,
		Athenes_Unholy_Grail = 3174,
		Head_of_KhaZix = 3175,
		Sanguine_Blade = 3181,
		Guardians_Hammer = 3184,
		Lightbringer = 3185,
		Arcane_Sweeper = 3187,
		Locket_of_the_Iron_Solari = 3190,
		Seekers_Armguard = 3191,
		Gargoyle_Stoneplate = 3193,
		Adaptive_Helm = 3194,
		Hex_Core_mk_1 = 3196,
		Hex_Core_mk_2 = 3197,
		Perfect_Hex_Core = 3198,
		Prototype_Hex_Core = 3200,
		Spectres_Cowl = 3211,
		Mikaels_Crucible = 3222,
		Space_Ravenous_Hydra = 3230,
		Space_Mercurial_Scimitar = 3231,
		Ludens_Echo = 3285,
		Ancient_Coin = 3301,
		Relic_Shield = 3302,
		Spellthiefs_Edge = 3303,
		Timeworn_Ancient_Coin = 3304,
		Timeworn_Nomads_Medallion = 3305,
		Timeworn_Talisman_of_Ascension = 3306,
		Timeworn_Relic_Shield = 3307,
		Timeworn_Targons_Brace = 3308,
		Timeworn_Face_of_the_Mountain = 3309,
		Timeworn_Spellthiefs_Edge = 3310,
		Timeworn_Frostfang = 3311,
		Timeworn_Frost_Queens_Claim = 3312,
		Warding_Totem_Trinket = 3340,
		Soul_Anchor_Trinket = 3345,
		Arcane_Sweeper_1 = 3348,
		Greater_Stealth_Totem_Trinket = 3361,
		Greater_Vision_Totem_Trinket = 3362,
		Farsight_Alteration = 3363,
		Oracle_Lens = 3364,
		Molten_Edge = 3371,
		Forgefire_Cape = 3373,
		Rabadons_Deathcrown = 3374,
		Infernal_Mask = 3379,
		Obsidian_Cleaver = 3380,
		Salvation = 3382,
		Circlet_of_the_Iron_Solari = 3383,
		Trinity_Fusion = 3384,
		Wooglets_Witchcrown = 3385,
		Zhonyas_Paradox = 3386,
		Frozen_Fist = 3387,
		Youmuus_Wraithblade = 3388,
		Might_of_the_Ruined_King = 3389,
		Ludens_Pulse = 3390,
		Your_Cut = 3400,
		Remnant_of_the_Aspect = 3401,
		Head_of_KhaZix_1 = 3410,
		Head_of_KhaZix_2 = 3416,
		Head_of_KhaZix_3 = 3422,
		Head_of_KhaZix_4 = 3455,
		Golden_Transcendence = 3460,
		Golden_Transcendence_Disabled = 3461,
		Seer_Stone_Trinket = 3462,
		Ardent_Censer = 3504,
		Essence_Reaver = 3508,
		ZzRot_Portal = 3512,
		Eye_of_the_Herald = 3513,
		Eye_of_the_Herald_1 = 3514,
		Ghost_Poro = 3520,
		Black_Spear = 3599,
		Black_Spear_2 = 3600,
		Siege_Teleport = 3630,
		Siege_Ballista = 3631,
		Siege_Teleport_1 = 3633,
		Tower_Beam_of_Ruination = 3634,
		Port_Pad = 3635,
		Tower_Storm_Bulwark = 3636,
		Nexus_Siege_Siege_Weapon_Slot = 3637,
		Flash_Zone = 3640,
		Vanguard_Banner = 3641,
		Siege_Refund = 3642,
		Entropy_Field = 3643,
		Seer_Stone_Trinket_1 = 3645,
		Shield_Totem = 3647,
		Siege_Teleport_Inactive = 3648,
		Siege_Sight_Warder = 3649,
		Pridestalkers_Blade_Enchantment_Warrior = 3671,
		Pridestalkers_Blade_Enchantment_Cinderhulk = 3672,
		Pridestalkers_Blade_Enchantment_Runic_Echoes = 3673,
		Pridestalkers_Blade_Enchantment_Bloodrazor = 3675,
		Frosted_Snax = 3680,
		Super_Spicy_Snax = 3681,
		Espresso_Snax = 3682,
		Rainbow_Snax_Party_Pack = 3683,
		Cosmic_Shackle = 3690,
		Singularity_Lantern = 3691,
		Dark_Matter_Scythe = 3692,
		Gravity_Boots = 3693,
		Cloak_of_Stars = 3694,
		Dark_Star_Sigil = 3695,
		Stalkers_Blade = 3706,
		Skirmishers_Sabre = 3715,
		Dead_Mans_Plate = 3742,
		Titanic_Hydra = 3748,
		Bamis_Cinder = 3751,
		Righteous_Glory = 3800,
		Crystalline_Bracer = 3801,
		Lost_Chapter = 3802,
		Deaths_Dance = 3812,
		Space_Deaths_Dance = 3813,
		Edge_of_Night = 3814,
		Fire_at_Will = 3901,
		Deaths_Daughter = 3902,
		Raise_Morale = 3903,
		Twin_Shadows = 3905,
		Spellbinder = 3907,
		Oblivion_Orb = 3916,
		Lifeline = 4003,
		Spectral_Cutlass = 4004,
		Pridestalkers_Blade = 4101,
		Enchantment_Warrior_3 = 4102, // rito pls
		Enchantment_Cinderhulk_3 = 4103,
		Enchantment_Runic_Echoes_3 = 4104,
		Enchantment_Bloodrazor_3 = 4105,
		Dorans_Lost_Shield = 4201,
		Dorans_Lost_Blade = 4202,
		Dorans_Lost_Ring = 4203,
		Dorans_Lost_Idol = 4204,
		Philosophers_Medallion = 4301,
		Heart_of_Targon = 4302,
		Force_of_Nature = 4401,
		Innervating_Locket = 4402,
		Stat_Stick_of_Stoicism = 4403
	};

	DECLARE_ENUM(ESummonerSpell,
		SummonerHeal,
		SummonerGhost,
		SummonerBarrier,
		SummonerExhaust,
		SummonerMark,
		SummonerClarity,
		SummonerFlash,
		SummonerTeleport,
		SummonerSmite,
		S5_SummonerSmiteDuel,
		S5_SummonerSmitePlayerGanker,
		SummonerCleanse,
		SummonerIgnite
		)
}