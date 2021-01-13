#pragma once
#include <string>

#include "Vector.h"

extern std::list<CObject*> heroList;
extern std::list<CObject*> minionList;

static int lastObjCount = 0;
static Vector me_lastPos = Vector(0, 0, 0);

static int CastSpellCtr = 0;

static bool g_draw_lp_range = false;
static bool g_draw_ally_range = false;
static bool g_draw_enemy_range = false;
static bool g_draw_cd_tracker = false;
static bool g_draw_line = false;
static bool g_draw_wards = false;
static bool g_champ_name = false;
static bool g_campTimer = false;
static bool g_enemy_turret = false;

static bool g_debug_address = false;
static bool g_debug_name = false;
static bool g_debug_location = false;

static bool g_inhi_respawn = false;
static bool OnStartMessage = false;

static bool g_autoSmite = false;
static bool g_drawEnemyMissles = false;
static bool g_display_spell_texture = false;
static int j_key_flag = 0;
static int x_key_flag = 0;
static int u_key_flag = 0;
static bool is_j_key_ready = false;
static bool is_x_key_ready = false;
static bool is_u_key_ready = false;

static int zoomValue = 1001;
static int zoomValueLast = 1001;
//static ImGuiSliderFlags flags = ImGuiSliderFlags_None;

static bool isOneTime = false;

static int g_spellTimer_message = 0;
static int opt_autoCleanse_c = 0;

static std::string castSpellCtr_debug = "0";
static std::string d_spellName_debug = "";
static std::string d_spellCD_debug = "";
static std::string d_spellIsDone_debug = "";
static std::string d_spellChrg_debug = "";
static std::string d_spellDmg_debug = "";

static std::string d_spellCDAbs_debug = "";
static std::string d_spellIsDoneAbs_debug = "";
static std::string f_spellName_debug = "";
static std::string f_spellCD_debug = "";
static std::string f_spellIsDone_debug = "";
static std::string f_spellChrg_debug = "";
static std::string f_spellDmg_debug = "";
static std::string f_spellCDAbs_debug = "";
static std::string f_spellIsDoneAbs_debug = "";
static std::string me_isImmobile_debug = "False";
static std::string me_healthPercentage_debug = "100%";

static std::string IsChatOpen_debug = "false";

static std::string base_debug = "0x0";
static std::string hiddenBase_debug = "0x0";

static DWORD Base = 0x0;
static DWORD HiddenBase = 0x0;

static bool IsChatBoxOpen = false;


static bool g_onprocessspell = false;
static bool g_oncreateobject = false;
static bool g_ondeleteobject = false;
static bool g_onnewpath = false;

static bool g_onprocessspell_last = false;
static bool g_oncreateobject_last = false;
static bool g_ondeleteobject_last = false;
static bool g_onnewpath_last = false;

//Orb settings
static float g_orbwalker_windup = 90.f;

//Callback debugging
static bool g_debug_cacheOnCreate = false;
static bool g_debug_cacheOnDelete = false;
static bool g_debug_cacheOnProcessSpell = false;
static bool g_debug_cacheOnNewPath = false;