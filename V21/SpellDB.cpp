#include "SpellDb.h"

namespace HACKUZAN {
	namespace SpellDb
	{
		std::vector<SkillshotData*> spells;
		std::vector<ISpecialSpell*> special_spells;

		void Initialize()
		{
			spells.push_back
			(
				InitializeSpell("AllChampions")->_spell_name("SummonerSnowball")->_spell_slot(kSpellSlot::SpellSlot_Summoner1)->_raw_range(1600)->_delay(0)->_raw_radius(50)->_missile_speed(1200)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("AllChampions")->_spell_name("SummonerPoroThrow")->_spell_slot(kSpellSlot::SpellSlot_Summoner1)->_raw_range(2500)->_delay(0)->_raw_radius(50)->_missile_speed(1200)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Aatrox")->_spell_name("AatroxQWrapperCast")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(650)->_raw_radius(130)->_delay(600)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Aatrox")->_spell_name("AatroxQ")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(650)->_raw_radius(130)->_delay(600)->_skillshot_type(kLine)
			);

			spells.push_back
			(
				InitializeSpell("Aatrox")->_spell_name("AatroxQ2")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(500)->_raw_radius(200)->_delay(600)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Aatrox")->_spell_name("AatroxQ3")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(825)->_raw_radius(80)->_delay(600)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Aatrox")->_spell_name("AatroxW")->_missile_name("AatroxW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(200)->_raw_radius(300)->_delay(250)->_missile_speed(1800)->_collision_flags(kWindwall)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Ahri")->_spell_name("AhriOrbofDeception")->_missile_name("AhriOrbMissile")->_danger_level(kMedium)->_raw_range(1000)->_raw_radius(100)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(400)->_missile_max_speed(2500)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ahri")->_spell_name("AhriOrbReturn")->_missile_name( "AhriOrbReturn" )->_danger_level(kHigh)->_raw_range(20000)->_delay(0)->_raw_radius(100)->_missile_speed(60)->_collision_flags(kYasuoWall)->_missile_follows_unit(TRUE)->_missile_accel(1900)->_missile_min_speed(60)->_missile_max_speed(2600)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ahri")->_spell_name("AhriSeduce")->_missile_name( "AhriSeduceMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1000)->_raw_radius(60)->_missile_speed(1550)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Akali")->_spell_name("AkaliE")->_missile_name( "AkaliEMis" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_radius(70)->_raw_range(900)->_missile_speed(1800)->_skillshot_type(kMissileLine)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Alistar")->_spell_name("Pulverize")->_danger_level(kHigh)->_raw_radius(365)->_skillshot_type(kCircle)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Amumu")->_spell_name("BandageToss")->_missile_name( "SadMummyBandageToss" )->_danger_level(kHigh)->_raw_range(1100)->_raw_radius(80)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Amumu")->_spell_name("Tantrum")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_radius(350)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Amumu")->_spell_name("CurseoftheSadMummy")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_radius(550)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Anivia")->_spell_name("FlashFrostSpell")->_missile_name( "FlashFrostSpell" )->_danger_level(kHigh)->_raw_range(1100)->_raw_radius(110)->_missile_speed(850)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Annie")->_spell_name("AnnieW")->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(600)->_raw_radius(300)->_delay(250)->_skillshot_type(kCone)->_danger_level(kLow)
			);

			spells.push_back
			(
				InitializeSpell("Annie")->_spell_name("AnnieR")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(600)->_raw_radius(250)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Ashe")->_spell_name("Volley")->_missile_name("VolleyAttack")->/*_missile_name("VolleyAttackWithSound" )->*/_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1150)->_raw_radius(20)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_infront_start(TRUE)->_skillshot_type(kMissileLine)->_multiple_angle(4.62f * M_PI / 180.0f)->_multiple_number(9)->_infront_start(85)
			);

			spells.push_back
			(
				InitializeSpell("Ashe")->_spell_name("VolleyRank2")->_missile_name("VolleyAttack")->_missile_name("VolleyAttackWithSound" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1150)->_raw_radius(20)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_infront_start(TRUE)->_skillshot_type(kMissileLine)->_multiple_angle(4.62f * M_PI / 180.0f)->_multiple_number(9)->_infront_start(85)
			);
			spells.push_back
			(
				InitializeSpell("Ashe")->_spell_name("VolleyRank3")->_missile_name("VolleyAttack")->_missile_name("VolleyAttackWithSound" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1150)->_raw_radius(20)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_infront_start(TRUE)->_skillshot_type(kMissileLine)->_multiple_angle(4.62f * M_PI / 180.0f)->_multiple_number(9)->_infront_start(85)
			);
			spells.push_back
			(
				InitializeSpell("Ashe")->_spell_name("VolleyRank4")->_missile_name("VolleyAttack")->_missile_name("VolleyAttackWithSound" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1150)->_raw_radius(20)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_infront_start(TRUE)->_skillshot_type(kMissileLine)->_multiple_angle(4.62f * M_PI / 180.0f)->_multiple_number(9)->_infront_start(85)
			);
			spells.push_back
			(
				InitializeSpell("Ashe")->_spell_name("VolleyRank5")->_missile_name("VolleyAttack")->_missile_name("VolleyAttackWithSound" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1150)->_raw_radius(20)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_infront_start(TRUE)->_skillshot_type(kMissileLine)->_multiple_angle(4.62f * M_PI / 180.0f)->_multiple_number(9)->_infront_start(85)
			);

			spells.push_back
			(
				InitializeSpell("Ashe")->_spell_name("EnchantedCrystalArrow")->_missile_name("EnchantedCrystalArrow" )->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(25000)->_raw_radius(130)->_missile_speed(1600)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("AurelionSol")->_spell_name("AurelionSolQ")->_missile_name( "AurelionSolQMissile" )->_danger_level(kMedium)->_raw_range(550)->_raw_radius(210)->_missile_speed(850)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(600)->_missile_max_speed(1000)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("AurelionSol")->_spell_name("AurelionSolR")->_missile_name( "AurelionSolRBeamMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1500)->_delay(350)->_raw_radius(120)->_missile_speed(4500)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Azir")->_spell_name("AzirQWrapper")->_danger_level(kMedium)->_raw_range(20000)->_delay(0)->_raw_radius(70)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Azir")->_spell_name("AzirR")->_danger_level(kExtreme)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(750)->_raw_radius(285)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_behind_start(300)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Bard")->_spell_name("BardQ")->_missile_name( "BardQMissile" )->_danger_level(kHigh)->_raw_range(950)->_raw_radius(60)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Bard")->_spell_name("BardQ2")->_missile_name( "BardQMissile2" )->_danger_level(kHigh)->_raw_range(525)->_raw_radius(80)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Bard")->_spell_name("BardR")->_missile_name( "BardRMissileFixedTravelTime")->_missile_name("BardRMissileRange1")->_missile_name("BardRMissileRange2")->_missile_name("BardRMissileRange3")->_missile_name("BardRMissileRange4")->_missile_name("BardRMissileRange5")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(3400)->_delay(500)->_raw_radius(350)->_missile_speed(2100)->_skillshot_type(kCircle)->_missile_min_speed(500)
			);

			spells.push_back
			(
				InitializeSpell("Blitzcrank")->_spell_name("RocketGrab")->_missile_name( "RocketGrabMissile" )->_danger_level(kHigh)->_raw_range(1050)->_raw_radius(70)->_missile_speed(1800)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Brand")->_spell_name("BrandQ")->_missile_name( "BrandQMissile" )->_danger_level(kHigh)->_raw_range(1100)->_raw_radius(60)->_missile_speed(1600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Brand")->_spell_name("BrandW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(900)->_delay(850)->_raw_radius(240)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Braum")->_spell_name("BraumQ")->_missile_name( "BraumQMissile" )->_danger_level(kHigh)->_raw_range(1050)->_raw_radius(60)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Braum")->_spell_name("BraumRWrapper")->_missile_name( "BraumRMissile" )->_danger_level(kExtreme)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1200)->_delay(550)->_raw_radius(115)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_radius_ex(330)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Caitlyn")->_spell_name("CaitlynPiltoverPeacemaker")->_missile_name( "CaitlynPiltoverPeacemaker" )->_danger_level(kMedium)->_raw_range(1300)->_delay(625)->_raw_radius(60)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Caitlyn")->_spell_name("CaitlynQBehind")->_missile_name( "CaitlynPiltoverPeacemaker2" )->_danger_level(kMedium)->_raw_range(1300)->_delay(0)->_raw_radius(90)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Caitlyn")->_spell_name("CaitlynYordleTrap")->_object_name("caitlyn_Base_yordleTrap_idle_red")->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(800)->_delay(160)->_raw_radius(70)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Caitlyn")->_spell_name("CaitlynEntrapment")->_missile_name( "CaitlynEntrapmentMissile" )->_object_name("caitlyn_Base_yordleTrap_idle_red")->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(800)->_delay(160)->_raw_radius(70)->_missile_speed(1600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Cassiopeia")->_spell_name("CassiopeiaQ")->_danger_level(kMedium)->_raw_range(850)->_delay(750)->_raw_radius(160)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Cassiopeia")->_spell_name("CassiopeiaR")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(790)->_delay(500)->_raw_radius(80)->_skillshot_type(kCone)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Chogath")->_spell_name("Rupture")->_danger_level(kHigh)->_raw_range(950)->_delay(1200)->_raw_radius(250)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Chogath")->_spell_name("FeralScream")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(650)->_delay(500)->_raw_radius(55)->_skillshot_type(kCone)
			);

			spells.push_back
			(
				InitializeSpell("Corki")->_spell_name("PhosphorusBomb")->_missile_name( "PhosphorusBombMissile")->_missile_name("PhosphorusBombMissileMin" )->_danger_level(kMedium)->_raw_range(825)->_delay(300)->_raw_radius(250)->_missile_speed(1000)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)
			);

			spells.push_back
			(
				InitializeSpell("Corki")->_spell_name("MissileBarrageMissile")->_missile_name( "MissileBarrageMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1300)->_delay(180)->_raw_radius(40)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Corki")->_spell_name("MissileBarrageMissile2")->_missile_name( "MissileBarrageMissile2" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1500)->_delay(180)->_raw_radius(40)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Darius")->_spell_name("DariusCleave")->_danger_level(kHigh)->_delay(750)->_raw_radius(325)->_skillshot_type(kRing)->_missile_follows_unit(TRUE)->_radius_ex(75)->_is_disabled_by_default(TRUE)->_dont_cross(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Darius")->_spell_name("DariusAxeGrabCone")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(510)->_raw_radius(50)->_skillshot_type(kCone)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Diana")->_spell_name("DianaArc")->_danger_level(kHigh)->_raw_range(850)->_raw_radius(195)->_missile_speed(1400)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Diana")->_spell_name("DianaQArc")->_danger_level(kHigh)->_raw_range(850)->_raw_radius(70)->_missile_speed(1400)->_skillshot_type(kArc)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)->_dont_cross(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("DrMundo")->_spell_name("InfectedCleaverMissile")->_missile_name( "InfectedCleaverMissile" )->_danger_level(kMedium)->_raw_range(1050)->_raw_radius(60)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Draven")->_spell_name("DravenDoubleShot")->_missile_name( "DravenDoubleShotMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1100)->_raw_radius(130)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Draven")->_spell_name("DravenRCast")->_extra_spell_names({ "DravenRDoublecast" })->_missile_name("DravenR")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(25000)->_delay(425)->_raw_radius(160)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_infront_start(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ekko")->_spell_name("EkkoQ")->_missile_name( "EkkoQMis" )->_danger_level(kMedium)->_raw_range(950)->_raw_radius(60)->_missile_speed(1650)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ekko")->_spell_name("EkkoQReturn")->_missile_name( "EkkoQReturn" )->_danger_level(kLow)->_raw_range(20000)->_delay(0)->_raw_radius(100)->_missile_speed(2300)->_collision_flags(kYasuoWall)->_missile_follows_unit(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ekko")->_spell_name("EkkoW")->_missile_name("EkkoWMis")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(20000)->_delay(2250)->_raw_radius(375)->_skillshot_type(kCircle)->_is_disabled_by_default(TRUE)->_extra_duration(1500)->_extra_delay({ 1050 })
			);

			spells.push_back
			(
				InitializeSpell("Ekko")->_spell_name("EkkoR")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(20000)->_raw_radius(375)->_missile_speed(1650)->_skillshot_type(kCircle)->_start_from_particle("Ekko_Base_R_TrailEnd")->_end_at_particle("Ekko_Base_R_TrailEnd")
			);

			spells.push_back
			(
				InitializeSpell("Elise")->_spell_name("EliseHumanE")->_missile_name( "EliseHumanE" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1100)->_raw_radius(55)->_missile_speed(1600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Evelynn")->_spell_name("EvelynnQ")->_missile_name( "EvelynnQ" )->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(800)->_raw_radius(70)->_missile_speed(2400)->_skillshot_type(kMissileLine)->_danger_level(kMedium)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Evelynn")->_spell_name("EvelynnR")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(0)->_raw_radius(450)->_skillshot_type(kCircle)->_danger_level(kHigh)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Ezreal")->_spell_name("EzrealQ")->_missile_name( "EzrealQ" )->_danger_level(kMedium)->_raw_range(1200)->_raw_radius(60)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ezreal")->_spell_name("EzrealW")->_missile_name( "EzrealW" )->_danger_level(kMedium)->_raw_range(1150)->_raw_radius(60)->_missile_speed(1650)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ezreal")->_spell_name("EzrealR")->_missile_name( "EzrealR" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(25000)->_delay(1000)->_raw_radius(160)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Fiora")->_spell_name("FioraW")->_missile_name( "FioraWMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(770)->_delay(500)->_raw_radius(70)->_missile_speed(3200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_behind_start(50)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Fizz")->_spell_name("FizzR")->_missile_name( "FizzRMissile" )->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1300)->_raw_radius(120)->_missile_speed(1350)->_extra_range(125)->_collision_flags(kHeroes | kYasuoWall)->_danger_level(kExtreme)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Galio")->_spell_name("GalioPassiveAttack")->_danger_level(kMedium)->_raw_radius(350)->_missile_speed(1300)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Galio")->_spell_name("GalioQ")->_missile_name( "GalioQMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(825)->_raw_radius(235)->_delay(250)->_missile_speed(1300)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_spell_slot(kSpellSlot::SpellSlot_Q)
			);

			spells.push_back
			(
				InitializeSpell("Galio")->_spell_name("GalioW")->_danger_level(kMedium)->_raw_radius(350)->_skillshot_type(kCircle)->_spell_slot(kSpellSlot::SpellSlot_Q)
			);

			spells.push_back
			(
				InitializeSpell("Galio")->_spell_name("GalioE")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(650)->_raw_radius(160)->_delay(400)->_missile_speed(2300)->_behind_start(300)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Galio")->_spell_name("GalioR")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_radius(650)->_raw_range(6000)->_delay(2250)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_is_disabled_by_default(TRUE)->_dont_cross(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Gnar")->_spell_name("GnarQMissile")->_missile_name( "GnarQMissile" )->_danger_level(kMedium)->_raw_range(1125)->_raw_radius(55)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(1400)->_missile_max_speed(2500)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Gnar")->_spell_name("GnarQMissileReturn")->_missile_name( "GnarQMissileReturn" )->_danger_level(kMedium)->_raw_range(3000)->_delay(0)->_raw_radius(75)->_missile_speed(60)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_accel(800)->_missile_min_speed(60)->_missile_max_speed(2600)->_invert(TRUE)->_is_disabled_by_default(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Gnar")->_spell_name("GnarBigQMissile")->_missile_name( "GnarBigQMissile" )->_danger_level(kMedium)->_raw_range(1150)->_delay(500)->_raw_radius(90)->_missile_speed(2100)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_radius_ex(210)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Gnar")->_spell_name("GnarBigW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(600)->_delay(600)->_raw_radius(110)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Gnar")->_spell_name("GnarE")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(475)->_delay(0)->_raw_radius(150)->_missile_speed(900)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Gnar")->_spell_name("GnarBigE")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(475)->_delay(0)->_raw_radius(350)->_missile_speed(800)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Gnar")->_spell_name("GnarR")->_spell_slot(kSpellSlot::SpellSlot_R)->_delay(275)->_raw_radius(500)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Gragas")->_spell_name("GragasQ")->_missile_name( "GragasQMissile" )->_extra_duration(0 /*4300*/)->_toggle_name("Gragas_.+_Q_(Enemy|Ally)")->_spellbook_spell_name("GragasQToggle")->_danger_level(kMedium)->_raw_range(850)->_raw_radius(280)->_missile_speed(1000)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)->_dont_cross(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Gragas")->_spell_name("GragasE")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(600)->_delay(0)->_raw_radius(200)->_missile_speed(900)->_collision_flags(kHeroes | kMinions)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Gragas")->_spell_name("GragasR")->_missile_name( "GragasRBoom" )->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1050)->_raw_radius(350)->_missile_speed(1800)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Graves")->_spell_name("GravesQLineSpell")->_missile_name( "GravesQLineMis" )->_danger_level(kMedium)->_raw_range(808)->_raw_radius(40)->_missile_speed(3000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Graves")->_spell_name("GravesQReturn")->_missile_name( "GravesQReturn" )->_danger_level(kMedium)->_raw_range(808)->_delay(0)->_raw_radius(100)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Graves")->_spell_name("GravesSmokeGrenade")->_missile_name( "GravesSmokeGrenadeBoom" )->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(950)->_raw_radius(225)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)->_extra_duration(4250)
			);

			spells.push_back
			(
				InitializeSpell("Graves")->_spell_name("GravesChargeShot")->_missile_name( "GravesChargeShotShot" )->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1000)->_raw_radius(100)->_missile_speed(2100)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kBox)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Hecarim")->_spell_name("HecarimUlt")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1000)->_delay(0)->_raw_radius(240)->_missile_speed(1100)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerQTurretBlast")->_missile_name( "HeimerdingerTurretEnergyBlast" )->_danger_level(kMedium)->_raw_range(1000)->_delay(0)->_raw_radius(50)->_missile_speed(1650)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(1200)->_missile_max_speed(1650)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerQTurretBigBlast")->_missile_name( "HeimerdingerTurretBigEnergyBlast" )->_danger_level(kMedium)->_raw_range(1000)->_delay(0)->_raw_radius(75)->_missile_speed(1650)->_collision_flags(kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(1200)->_missile_max_speed(1650)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerW")->_missile_name( "HeimerdingerWAttack2")->_missile_name("HeimerdingerWAttack2Ult" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1350)->_delay(0)->_raw_radius(40)->_missile_speed(750)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_missile_accel(4000)->_missile_min_speed(750)->_missile_max_speed(3000)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerE")->_missile_name( "HeimerdingerESpell" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(950)->_raw_radius(100)->_missile_speed(1200)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)
			);

			spells.push_back
			(
				InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerEUlt")->_missile_name( "HeimerdingerESpell_ult" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1000)->_raw_radius(150)->_missile_speed(1200)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)
			);

			spells.push_back
			(
				InitializeSpell("Heimerdinger")->_spell_name("HeimerdingerEUltBounce")->_missile_name( "HeimerdingerESpell_ult2")->_missile_name("HeimerdingerESpell_ult3" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1000)->_delay(175)->_raw_radius(150)->_missile_speed(1400)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_is_disabled_by_default(TRUE)->_missile_delayed(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Illaoi")->_spell_name("IllaoiQ")->_danger_level(kMedium)->_raw_range(850)->_delay(750)->_raw_radius(100)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_behind_start(50)
			);

			spells.push_back
			(
				InitializeSpell("Illaoi")->_spell_name("IllaoiE")->_missile_name( "IllaoiEMis" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(950)->_delay(265)->_raw_radius(50)->_missile_speed(1900)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Illaoi")->_spell_name("IllaoiR")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_delay(500)->_raw_radius(450)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Irelia")->_spell_name("IreliaW2")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(825)->_delay(250)->_raw_radius(120)->_missile_speed(INT_MAX)->_is_fixed_range(TRUE)->_skillshot_type(kLine)
			);

			spells.push_back
			(
				InitializeSpell("Irelia")->_spell_name("IreliaR")->_missile_name( "IreliaR" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(950)->_delay(400)->_raw_radius(160)->_missile_speed(2000)->_collision_flags(kYasuoWall | kHeroes)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ivern")->_spell_name("IvernQ")->_missile_name( "IvernQ" )->_danger_level(kHigh)->_raw_range(1100)->_raw_radius(65)->_missile_speed(1350)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Janna")->_spell_name("JannaQ")->_missile_name( "HowlingGaleSpell" )->_danger_level(kMedium)->_raw_range(20000)->_delay(0)->_raw_radius(120)->_missile_speed(900)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("JarvanIV")->_spell_name("JarvanIVDragonStrike")->_danger_level(kHigh)->_raw_range(770)->_delay(425)->_raw_radius(70)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("JarvanIV")->_spell_name("JarvanIVQE")->_danger_level(kHigh)->_raw_range(910)->_delay(450)->_raw_radius(120)->_missile_speed(2600)->_is_dangerous(TRUE)->_radius_ex(210)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("JarvanIV")->_spell_name("JarvanIVDemacianStandard")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(850)->_delay(450)->_raw_radius(150)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Jayce")->_spell_name("JayceShockBlast")->_missile_name( "JayceShockBlastMis" )->_danger_level(kMedium)->_delay(230)->_raw_range(1050)->_raw_radius(70)->_missile_speed(1450)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_radius_ex(275)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Jayce")->_spell_name("JayceQAccel")->_missile_name( "JayceShockBlastWallMis" )->_danger_level(kMedium)->_raw_range(2000)->_delay(0)->_raw_radius(70)->_missile_speed(2350)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_radius_ex(275)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Jhin")->_spell_name("JhinW")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(2550)->_delay(750)->_raw_radius(40)->_skillshot_type(kLine)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Jhin")->_spell_name("JhinRShot")->_missile_name( "JhinRShotMis")->_missile_name("JhinRShotMis4" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(3500)->_delay(190)->_raw_radius(80)->_missile_speed(5000)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Jinx")->_spell_name("JinxWMissile")->_missile_name( "JinxWMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1500)->_delay(600)->_raw_radius(60)->_missile_speed(3300)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Jinx")->_spell_name("JinxR")->_missile_name( "JinxR" )->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(25000)->_delay(600)->_raw_radius(140)->_missile_speed(1700)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Kaisa")->_spell_name("KaisaW")->_missile_name( "KaisaW" )->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(3000)->_delay(400)->_raw_radius(60)->_missile_speed(1750)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Kalista")->_spell_name("KalistaMysticShot")->_missile_name( "KalistaMysticShotMisTrue" )->_danger_level(kMedium)->_raw_range(1200)->_raw_radius(40)->_missile_speed(2400)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Karma")->_spell_name("KarmaQ")->_missile_name( "KarmaQMissile" )->_danger_level(kMedium)->_raw_range(1050)->_raw_radius(60)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Karma")->_spell_name("KarmaQMantra")->_missile_name( "KarmaQMissileMantra" )->_danger_level(kMedium)->_raw_range(950)->_raw_radius(80)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_extra_duration(1650)->_radius_ex(280)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Karthus")->_spell_name("KarthusLayWasteA1")->_extra_spell_names({ "KarthusLayWasteA2", "KarthusLayWasteA3", "KarthusLayWasteDeadA1", "KarthusLayWasteDeadA2", "KarthusLayWasteDeadA3" })->_danger_level(kMedium)->_raw_range(875)->_delay(950)->_raw_radius(160)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Kennen")->_spell_name("KennenShurikenHurlMissile1")->_missile_name( "KennenShurikenHurlMissile1" )->_danger_level(kMedium)->_raw_range(1050)->_delay(175)->_raw_radius(50)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Khazix")->_spell_name("KhazixW")->_extra_spell_names({ "KhazixWLong" })->_missile_name("KhazixWMissile")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1025)->_raw_radius(70)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)->_multiple_number(3)->_multiple_angle(22.0f * M_PI / 180.0f)
			);

			spells.push_back
			(
				InitializeSpell("Khazix")->_spell_name("KhazixE")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(700)->_delay(0)->_raw_radius(300)->_missile_speed(1250)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Khazix")->_spell_name("KhazixELong")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(900)->_delay(0)->_raw_radius(300)->_missile_speed(1200)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Kled")->_spell_name("KledQ")->_missile_name( "KledQMissile" )->_danger_level(kMedium)->_raw_range(800)->_raw_radius(45)->_missile_speed(1600)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Kled")->_spell_name("KledRiderQ")->_missile_name( "KledRiderQMissile" )->_danger_level(kMedium)->_raw_range(700)->_raw_radius(40)->_missile_speed(3000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)->_multiple_number(5)->_multiple_angle(5.0f * M_PI / 180.0f)
			);

			spells.push_back
			(
				InitializeSpell("Kled")->_spell_name("KledEDash")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(550)->_delay(0)->_raw_radius(100)->_missile_speed(970)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("KogMaw")->_spell_name("KogMawQ")->_missile_name( "KogMawQ" )->_danger_level(kMedium)->_raw_range(1200)->_raw_radius(70)->_missile_speed(1650)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("KogMaw")->_spell_name("KogMawVoidOozeMissile")->_missile_name( "KogMawVoidOozeMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1300)->_raw_radius(120)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("KogMaw")->_spell_name("KogMawLivingArtillery")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1200)->_delay(1150)->_raw_radius(240)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Kayle")->_spell_name("KayleQ")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(875)->_delay(600)->_raw_radius(100)->_missile_speed(1200)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Kayn")->_spell_name("KaynW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(700)->_delay(500)->_raw_radius(80)->_missile_speed(INT_MAX)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Kayn")->_spell_name("KaynAssW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(875)->_delay(600)->_raw_radius(80)->_missile_speed(INT_MAX)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Leblanc")->_spell_name("LeblancW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(600)->_delay(0)->_raw_radius(220)->_missile_speed(1600)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Leblanc")->_spell_name("LeblancRW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(600)->_delay(0)->_raw_radius(220)->_missile_speed(1600)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Leblanc")->_spell_name("LeblancE")->_missile_name( "LeblancEMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(950)->_raw_radius(55)->_missile_speed(1750)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Leblanc")->_spell_name("LeblancRE")->_missile_name( "LeblancREMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(950)->_raw_radius(55)->_missile_speed(1750)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("LeeSin")->_spell_name("BlindMonkQOne")->_missile_name( "BlindMonkQOne" )->_danger_level(kMedium)->_raw_range(1100)->_raw_radius(60)->_missile_speed(1800)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Leona")->_spell_name("LeonaZenithBlade")->_missile_name( "LeonaZenithBladeMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(900)->_raw_radius(70)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Leona")->_spell_name("LeonaSolarFlare")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1200)->_delay(930)->_raw_radius(300)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Lissandra")->_spell_name("LissandraQMissile")->_missile_name( "LissandraQMissile" )->_danger_level(kMedium)->_raw_range(700)->_raw_radius(75)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Lissandra")->_spell_name("LissandraQShards")->_missile_name( "LissandraQShards" )->_danger_level(kMedium)->_raw_range(1650)->_delay(0)->_raw_radius(90)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Lissandra")->_spell_name("LissandraEMissile")->_missile_name( "LissandraEMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1025)->_raw_radius(125)->_missile_speed(850)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Lucian")->_spell_name("LucianQ")->_danger_level(kMedium)->_raw_range(900)->_delay(350)->_raw_radius(65)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Lucian")->_spell_name("LucianW")->_missile_name( "LucianWMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(900)->_delay(325)->_raw_radius(55)->_missile_speed(1600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Lucian")->_spell_name("LucianR")->_missile_name( "LucianRMissileOffhand")->_missile_name( "LucianRMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1200)->_delay(0)->_raw_radius(110)->_missile_speed(2800)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_disabled_by_default(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Lulu")->_spell_name("LuluQ")->_missile_name( "LuluQMissile")->_missile_name("LuluQMissileTwo" )->_danger_level(kMedium)->_raw_range(950)->_raw_radius(60)->_missile_speed(1450)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Lux")->_spell_name("LuxLightBinding")->_missile_name( "LuxLightBindingMis" )->_danger_level(kMedium)->_raw_range(1300)->_raw_radius(70)->_missile_speed(1200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Lux")->_spell_name("LuxLightStrikeKugel")->_missile_name( "LuxLightStrikeKugel" )->_object_name("Lux_Base_E_tar_aoe_red")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1100)->_raw_radius(330)->_missile_speed(1300)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_dont_cross(TRUE)->_is_disabled_by_default(FALSE)
			);

			spells.push_back
			(
				InitializeSpell("Lux")->_spell_name("LuxMaliceCannon")->_missile_name("LuxRVfxMis")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(3300)->_delay(1000)->_raw_radius(150)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Malphite")->_spell_name("UFSlash")->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1000)->_delay(0)->_raw_radius(270)->_missile_speed(1600)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Malzahar")->_spell_name("MalzaharQ")->_missile_name( "MalzaharQMissile" )->_danger_level(kMedium)->_raw_range(900)->_delay(520)->_raw_radius(100)->_missile_speed(1600)->_is_fixed_range(TRUE)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Maokai")->_spell_name("MaokaiQ")->_missile_name( "MaokaiQMissile" )->_danger_level(kHigh)->_raw_range(650)->_delay(350)->_raw_radius(110)->_missile_speed(1600)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Mordekaiser")->_spell_name("MordekaiserQ")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(675)->_raw_radius(200)->_delay(400)->_missile_speed(INT_MAX)->_skillshot_type(kBox)->_dont_cross(TRUE)->_is_disabled_by_default(FALSE)
			);
			spells.push_back
			(
				InitializeSpell("Mordekaiser")->_spell_name("MordekaiserE")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(900)->_raw_radius(140)->_delay(400)->_missile_speed(INT_MAX)->_skillshot_type(kBox)->_collision_flags(kYasuoWall)->_dont_cross(TRUE)->_is_disabled_by_default(FALSE)
			);

			spells.push_back
			(
				InitializeSpell("Morgana")->_spell_name("MorganaQ")->_missile_name( "MorganaQ" )->_danger_level(kMedium)->_raw_range(1250)->_raw_radius(70)->_delay(250)->_missile_speed(1200)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Nami")->_spell_name("NamiQ")->_missile_name( "NamiQMissile" )->_danger_level(kMedium)->_raw_range(850)->_raw_radius(200)->_missile_speed(2500)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)->_extra_delay({ 700 })
			);

			spells.push_back
			(
				InitializeSpell("Nami")->_spell_name("NamiRMissile")->_missile_name( "NamiRMissile" )->_danger_level(kExtreme)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(2750)->_delay(500)->_raw_radius(250)->_missile_speed(850)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Nautilus")->_spell_name("NautilusAnchorDragMissile")->_missile_name( "NautilusAnchorDragMissile" )->_danger_level(kHigh)->_raw_range(1150)->_raw_radius(90)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Neeko")->_spell_name("NeekoQ")->_missile_name("NeekoQ")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(800)->_delay(250)->_raw_radius(200)->_missile_speed(1500)->_collision_flags( kYasuoWall )->_is_fixed_range(TRUE)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Neeko")->_spell_name("NeekoE")->_missile_name( "NeekoE" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1000)->_delay(250)->_raw_radius(70)->_missile_speed(1300)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Nidalee")->_spell_name("JavelinToss")->_missile_name( "JavelinToss" )->_danger_level(kHigh)->_raw_range(1500)->_raw_radius(40)->_missile_speed(1300)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Nocturne")->_spell_name("NocturneDuskbringer")->_missile_name( "NocturneDuskbringer" )->_danger_level(kMedium)->_raw_range(1200)->_raw_radius(60)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Olaf")->_spell_name("OlafAxeThrowCast")->_missile_name( "OlafAxeThrow" )->_danger_level(kMedium)->_raw_range(1000)->_minimum_range(400)->_raw_radius(90)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Ornn")->_spell_name("OrnnQ")->_missile_name( "OrnnQMissile" )->_danger_level(kMedium)->_raw_range(1000)->_raw_radius(65)->_missile_speed(1800)->_collision_flags(kYasuoWall | kHeroes | kMinions)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Ornn")->_spell_name("OrnnRWave")->_missile_name( "OrnnRWave2" )->_spell_slot(kSpellSlot::SpellSlot_R)->_danger_level(kExtreme)->_raw_range(3500)->_raw_radius(200)->_missile_speed(1650)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Orianna")->_spell_name("OrianaIzunaCommands")->_missile_name( "OrianaIzuna" )->_danger_level(kMedium)->_raw_range(2000)->_delay(0)->_raw_radius(80)->_missile_speed(1400)->_collision_flags(kYasuoWall)->_radius_ex(230)->_skillshot_type(kBox)//(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Orianna")->_spell_name("OrianaRedactCommand")->_spell_slot(kSpellSlot::SpellSlot_E)->_missile_name( "OrianaRedact" )->_danger_level(kMedium)->_raw_range(2000)->_delay(0)->_raw_radius(90)->_missile_speed(1850)->_collision_flags(kYasuoWall)->_missile_follows_unit(TRUE)->_radius_ex(230)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Orianna")->_spell_name("OrianaDetonateCommand")->_spell_slot(kSpellSlot::SpellSlot_R)->_delay(500)->_raw_radius(410)->_skillshot_type(kCircle)->_is_dangerous(TRUE)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Pantheon")->_spell_name("PantheonE")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(640)->_delay(400)->_raw_radius(70)->_skillshot_type(kCone)->_extra_duration(750)
			);

			spells.push_back
			(
				InitializeSpell("Poppy")->_spell_name("PoppyQ")->_danger_level(kMedium)->_raw_range(430)->_delay(500)->_raw_radius(100)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_extra_duration(900)
			);

			spells.push_back
			(
				InitializeSpell("Poppy")->_spell_name("PoppyRSpellInstant")->_danger_level(kExtreme)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(450)->_delay(350)->_raw_radius(100)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Poppy")->_spell_name("PoppyRSpell")->_missile_name( "PoppyRMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1200)->_delay(350)->_raw_radius(100)->_missile_speed(1600)->_collision_flags(kHeroes | kYasuoWall)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Pyke")->_spell_name("PykeQRange")->_missile_name( "PykeQRange" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(1100)->_delay(250)->_raw_radius(70)->_missile_speed(2000)->_collision_flags(kHeroes | kYasuoWall)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Pyke")->_spell_name("PykeE")->_missile_name( "PykeEMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1100)->_delay(1000)->_raw_radius(110)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_missile_follows_unit(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Quinn")->_spell_name("QuinnQ")->_missile_name( "QuinnQ" )->_danger_level(kMedium)->_raw_range(1050)->_raw_radius(60)->_missile_speed(1550)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Rakan")->_spell_name("RakanQ")->_missile_name( "RakanQMis" )->_danger_level(kMedium)->_delay(250)->_raw_range(850)->_raw_radius(65)->_missile_speed(1850)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Rakan")->_spell_name("RakanW")->_danger_level(kHigh)->_raw_range(650)->_raw_radius(275)->_missile_speed(2000)->_delay(250)->_extra_duration(280)->_is_dangerous(TRUE)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("RekSai")->_spell_name("RekSaiQBurrowed")->_missile_name( "RekSaiQBurrowedMis" )->_danger_level(kHigh)->_raw_range(1500)->_delay(125)->_raw_radius(65)->_missile_speed(1950)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Rengar")->_spell_name("RengarE")->_missile_name( "RengarEMis" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1000)->_raw_radius(70)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Rengar")->_spell_name("RengarEEmp")->_missile_name( "RengarEEmpMis" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1000)->_raw_radius(70)->_missile_speed(1500)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Riven")->_spell_name("RivenIzunaBlade")->_missile_name( "RivenWindslashMissileCenter")->_missile_name("RivenWindslashMissileLeft")->_missile_name("RivenWindslashMissileRight" )->_spell_slot(kSpellSlot::SpellSlot_R)->_danger_level(kExtreme)->_raw_range(1075)->_raw_radius(100)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_multiple_number(5)->_multiple_angle(6.69f * M_PI / 180.0f)
			);

			spells.push_back
			(
				InitializeSpell("Rumble")->_spell_name("RumbleGrenade")->_missile_name( "RumbleGrenadeMissile")->_missile_name( "RumbleGrenadeMissileDangerZone" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(950)->_raw_radius(60)->_missile_speed(2000)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Rumble")->_spell_name("RumbleR")->_missile_name( "RumbleCarpetBombMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(20000)->_delay(400)->_raw_radius(150)->_missile_speed(1600)->_can_be_deleted(FALSE)->_missile_delayed(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ryze")->_spell_name("RyzeQ")->_missile_name( "RyzeQ" )->_danger_level(kMedium)->_raw_range(1000)->_raw_radius(55)->_missile_speed(1700)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Senna")->_spell_name("SennaQCast")->_spell_slot(kSpellSlot::SpellSlot_Q)->_danger_level(kMedium)->_raw_range(1400)->_extra_range(150)->_delay(400)->_raw_radius(80)->_missile_speed(INT_MAX)->_is_dangerous(TRUE)->_skillshot_type(kLine)
			);

			spells.push_back
			(
				InitializeSpell("Senna")->_spell_name("SennaW")->_missile_name("SennaW")->_spell_slot(kSpellSlot::SpellSlot_W)->_danger_level(kMedium)->_raw_range(1300)->_extra_range(150)->_delay(250)->_raw_radius(60)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_missile_speed(1150)->_skillshot_type(kLine)
			);

			spells.push_back
			(
				InitializeSpell("Senna")->_spell_name("SennaR")->_missile_name( "SennaRWarningMis" )->_spell_slot(kSpellSlot::SpellSlot_R)->_danger_level(kHigh)->_raw_range(12500)->_extra_range(150)->_delay(1000)->_raw_radius(180)->_collision_flags(kYasuoWall)->_missile_speed(20000)->_skillshot_type(kLine)
			);

			spells.push_back
			(
				InitializeSpell("Sejuani")->_spell_name("SejuaniQ")->_spell_slot(kSpellSlot::SpellSlot_Q)->_danger_level(kHigh)->_raw_range(650)->_extra_range(150)->_delay(0)->_raw_radius(75)->_missile_speed(1000)->_collision_flags(kHeroes)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Sejuani")->_spell_name("SejuaniR")->_missile_name( "SejuaniRMissile" )->_danger_level(kExtreme)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1200)->_raw_radius(120)->_missile_speed(1600)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Sett")->_spell_name("SettW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(790)->_raw_radius(160)->_delay(750)->_missile_speed(INT_MAX)->_is_fixed_range(TRUE)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Sett")->_spell_name("SettE")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(490)->_raw_radius(200)->_delay(250)->_missile_speed(INT_MAX)->_is_fixed_range(TRUE)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Shen")->_spell_name("ShenE")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(600)->_delay(0)->_raw_radius(50)->_missile_speed(1300)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Shyvana")->_spell_name("ShyvanaFireball")->_missile_name( "ShyvanaFireballMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(950)->_raw_radius(60)->_missile_speed(1575)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Shyvana")->_spell_name("ShyvanaFireballDragon2")->_missile_name( "ShyvanaFireballDragonMissile")->_missile_name("ShyvanaFireballDragonMissileBig")->_missile_name("ShyvanaFireballDragonMissileMax" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(780)->_delay(350)->_raw_radius(60)->_missile_speed(1575)->_collision_flags(kHeroes | kYasuoWall)->_infront_start(TRUE)->_radius_ex(350)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Shyvana")->_spell_name("ShyvanaTransformCast")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(950)->_delay(300)->_raw_radius(150)->_missile_speed(1100)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Sion")->_spell_name("SionQ")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_Q)->_raw_range(600)->_raw_radius(250)->_delay(500)->_missile_speed(1800)->_is_fixed_range(TRUE)->_skillshot_type(kLine)
			);


			spells.push_back
			(
				InitializeSpell("Sion")->_spell_name("SionE")->_missile_name( "SionEMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(800)->_raw_radius(80)->_missile_speed(1800)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Sion")->_spell_name("SionEMinion")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1400)->_delay(0)->_raw_radius(60)->_missile_speed(2100)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Sivir")->_spell_name("SivirQ")->_missile_name( "SivirQMissile" )->_danger_level(kMedium)->_raw_range(1250)->_raw_radius(90)->_missile_speed(1350)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Sivir")->_spell_name("SivirQReturn")->_missile_name( "SivirQMissileReturn" )->_danger_level(kMedium)->_raw_range(20000)->_delay(0)->_raw_radius(100)->_missile_speed(1350)->_collision_flags(kYasuoWall)->_missile_follows_unit(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Skarner")->_spell_name("SkarnerFracture")->_missile_name( "SkarnerFractureMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1000)->_raw_radius(70)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Sona")->_spell_name("SonaR")->_missile_name( "SonaR" )->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1000)->_raw_radius(140)->_missile_speed(2400)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Soraka")->_spell_name("SorakaQ")->_missile_name( "SorakaQMissile" )->_danger_level(kMedium)->_raw_range(800)->_raw_radius(230)->_missile_speed(1100)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)
			);

			spells.push_back
			(
				InitializeSpell("Soraka")->_spell_name("SorakaE")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(920)->_delay(1770)->_raw_radius(250)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Swain")->_spell_name("SwainW")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(3500)->_delay(1500)->_raw_radius(240)->_missile_speed(INT_MAX)->_skillshot_type(kCircle)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Swain")->_spell_name("SwainE")->_danger_level(kHigh)->_missile_name( "SwainE")->_missile_name( "SwainEReturnMissile" )->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(850)->_delay(250)->_raw_radius(85)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Sylas")->_spell_name("SylasQ")->_missile_name( "SyndraQSpell" )->_spell_slot(kSpellSlot::SpellSlot_Q)->_danger_level(kMedium)->_missile_speed(INT_MAX)->_raw_range(775)->_delay(400)->_raw_radius(45)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Sylas")->_spell_name("SylasE2")->_missile_name( "SylasE2Mis" )->_spell_slot(kSpellSlot::SpellSlot_E)->_danger_level(kMedium)->_missile_speed(1600)->_raw_range(850)->_delay(250)->_raw_radius(60)->_skillshot_type(kLine)->_collision_flags(kYasuoWall)
			);

			spells.push_back
			(
				InitializeSpell("Syndra")->_spell_name("SyndraQ")->_missile_name( "SyndraQSpell" )->_danger_level(kMedium)->_raw_range(825)->_delay(650)->_raw_radius(180)->_skillshot_type(kCircle)->_missile_delayed(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Syndra")->_spell_name("SyndraWCast")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(950)->_raw_radius(210)->_missile_speed(1500)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Syndra")->_spell_name("SyndraE")->_danger_level(kMedium)->_is_dangerous(TRUE)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(650)->_raw_radius(40)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileCone)->_dont_cross(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Syndra")->_spell_name("SyndraE5")->_danger_level(kMedium)->_is_dangerous(TRUE)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(650)->_raw_radius(60)->_missile_speed(2500)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileCone)->_dont_cross(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Syndra")->_spell_name("SyndraEQ")->_danger_level(kMedium)->_is_dangerous(TRUE)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1300)->_delay(0)->_raw_radius(55)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("TahmKench")->_spell_name("TahmKenchQ")->_missile_name( "TahmKenchQMissile" )->_danger_level(kHigh)->_raw_range(850)->_raw_radius(70)->_missile_speed(2800)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Taliyah")->_spell_name("TaliyahQ")->_missile_name( "TaliyahQMis" )->_danger_level(kMedium)->_raw_range(1000)->_raw_radius(100)->_missile_speed(3600)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_missile_min_speed(1500)->_missile_max_speed(3600)->_is_disabled_by_default(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Taliyah")->_spell_name("TaliyahWVC")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(900)->_delay(800)->_raw_radius(200)->_skillshot_type(kCircle)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Taric")->_spell_name("TaricE")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(650)->_delay(1000)->_raw_radius(100)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_missile_connected_to_unit(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Teemo")->_spell_name("TeemoRCast")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(650)->_delay(500)->_raw_radius(100)->_skillshot_type(kCircle)->_is_fixed_range(FALSE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Thresh")->_spell_name("ThreshQ")->_missile_name( "ThreshQMissile" )->_danger_level(kHigh)->_raw_range(1100)->_delay(500)->_raw_radius(70)->_missile_speed(1900)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Thresh")->_spell_name("ThreshE")->_missile_name( "ThreshEMissile1" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1075)->_delay(0)->_raw_radius(110)->_behind_start(530)->_missile_speed(2000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Tristana")->_spell_name("TristanaW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(900)->_delay(300)->_raw_radius(270)->_missile_speed(1100)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("TwistedFate")->_spell_name("WildCards")->_missile_name( "SealFateMissile" )->_danger_level(kMedium)->_raw_range(1450)->_raw_radius(40)->_missile_speed(1000)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)->_multiple_number(3)->_multiple_angle(28.0f * M_PI / 180.0f)
			);

			spells.push_back
			(
				InitializeSpell("Twitch")->_spell_name("TwitchVenomCask")->_missile_name( "TwitchVenomCaskMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(950)->_raw_radius(285)->_missile_speed(1400)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_extra_duration(2850)->_is_disabled_by_default(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Urgot")->_spell_name("UrgotQ")->_missile_name( "UrgotQMissile" )->_danger_level(kMedium)->_raw_range(800)->_delay(525)->_raw_radius(150)->_missile_speed(5000)->_collision_flags(kYasuoWall)->_can_be_deleted(FALSE)->_missile_delayed(TRUE)->_is_fixed_range(FALSE)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Urgot")->_spell_name("UrgotE")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_delay(500)->_raw_range(500)->_raw_radius(100)->_missile_speed(1500)->_skillshot_type(kMissileLine)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Urgot")->_spell_name("UrgotR")->_missile_name( "UrgotR" )->_danger_level(kExtreme)->_spell_slot(kSpellSlot::SpellSlot_R)->_delay(400)->_raw_range(1650)->_raw_radius(80)->_missile_speed(3200)->_skillshot_type(kMissileLine)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Varus")->_spell_name("VarusQCast")->_missile_name( "VarusQMissile" )->_danger_level(kMedium)->_raw_range(20000)->_delay(0)->_raw_radius(70)->_missile_speed(1900)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Varus")->_spell_name("VarusQ")->_missile_name("VarusQMissile")->_danger_level(kMedium)->_raw_range(20000)->_delay(0)->_raw_radius(70)->_missile_speed(1900)->_collision_flags(kYasuoWall)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Varus")->_spell_name("VarusE")->_missile_name( "VarusEMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(925)->_raw_radius(260)->_missile_speed(1500)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_extra_delay({ 550 })
			);

			spells.push_back
			(
				InitializeSpell("Varus")->_spell_name("VarusR")->_missile_name( "VarusRMissile" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(1250)->_raw_radius(120)->_missile_speed(1950)->_collision_flags(kHeroes | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Veigar")->_spell_name("VeigarBalefulStrike")->_missile_name( "VeigarBalefulStrikeMis" )->_danger_level(kMedium)->_raw_range(950)->_raw_radius(70)->_missile_speed(2200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Veigar")->_spell_name(/*"VeigarDarkMatter"*/"VeigarDarkMatterCastLockout")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(900)->_delay(1250)->_raw_radius(225)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Veigar")->_spell_name("VeigarEventHorizon")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(700)->_delay(800)->_raw_radius(370)->_skillshot_type(kRing)->_radius_ex(70)->_extra_duration(3000)->_is_dangerous(TRUE)->_dont_cross(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Velkoz")->_spell_name("VelkozQ")->_missile_name( "VelkozQMissile" )->_danger_level(kMedium)->_raw_range(1100)->_raw_radius(50)->_missile_speed(1300)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Velkoz")->_spell_name("VelkozQSplit")->_missile_name( "VelkozQMissileSplit" )->_danger_level(kMedium)->_raw_range(1100)->_delay(0)->_raw_radius(45)->_missile_speed(2100)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Velkoz")->_spell_name("VelkozW")->_missile_name( "VelkozWMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1200)->_raw_radius(88)->_missile_speed(1700)->_collision_flags(kYasuoWall)->_behind_start(100)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Velkoz")->_spell_name("VelkozE")->_missile_name("VelkozEMissile")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(800)->_raw_radius(225)->_missile_speed(1500)->_skillshot_type(kCircle)->_extra_delay({ 550 })
			);

			spells.push_back
			(
				InitializeSpell("Vi")->_missile_name( "ViQMissile" )->_danger_level(kHigh)->_raw_range(800)->_extra_range(100)->_delay(0)->_raw_radius(90)->_missile_speed(1600)->_collision_flags(kHeroes)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Viktor")->_spell_name("ViktorGravitonField")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(700)->_delay(1500)->_raw_radius(300)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Viktor")->_spell_name("ViktorE")->_missile_name( "ViktorDeathRayMissile")->_missile_name( "ViktorEAugMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(710)->_delay(0)->_raw_radius(80)->_missile_speed(1050)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Viktor")->_spell_name("ViktorEExplosion")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(710)->_delay(1000)->_raw_radius(80)->_missile_speed(1500)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Warwick")->_spell_name("WarwickR")->_danger_level(kExtreme)->_is_dangerous(TRUE)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(2000)->_extra_range(100)->_delay(0)->_raw_radius(150)->_missile_speed(1500)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Xayah")->_spell_name("XayahQ")->_missile_name( "XayahQMissile1")->_missile_name( "XayahQMissile2" )->_spell_slot(kSpellSlot::SpellSlot_Q)->_danger_level(kMedium)->_raw_range(1100)->_delay(600)->_raw_radius(50)->_missile_speed(2400)->_missile_delayed(TRUE)->_skillshot_type(kMissileLine)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("XinZhao")->_spell_name("XinZhaoW")->_danger_level(kMedium)->_raw_range(900)->_delay(500)->_raw_radius(40)->_missile_speed(5000)->_skillshot_type(kLine)
			);

			spells.push_back
			(
				InitializeSpell("Xerath")->_spell_name("XerathArcanopulseChargeUp")->_danger_level(kMedium)->_raw_range(20000)->_delay(530)->_raw_radius(100)->_skillshot_type(kLine)
			);

			spells.push_back
			(
				InitializeSpell("Xerath")->_spell_name("XerathArcaneBarrage2")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1100)->_delay(780)->_raw_radius(260)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Xerath")->_spell_name("XerathMageSpear")->_missile_name( "XerathMageSpearMissile" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1100)->_raw_radius(60)->_missile_speed(1400)->_collision_flags(kHeroes | kMinions | kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Xerath")->_spell_name("XerathRMissileWrapper")->_missile_name( "XerathLocusPulse" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(20000)->_delay(650)->_raw_radius(200)->_skillshot_type(kCircle)->_missile_delayed(TRUE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Yasuo")->_spell_name("YasuoQ1Wrapper")->_extra_spell_names({ "YasuoQ" })->_danger_level(kHigh)->_missile_speed(INT_MAX)->_raw_range(520)->_delay(250)->_raw_radius(40)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_invert(TRUE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Yasuo")->_spell_name("YasuoQ2Wrapper")->_extra_spell_names({ "YasuoQ2" })->_danger_level(kHigh)->_missile_speed(INT_MAX)->_raw_range(520)->_delay(250)->_raw_radius(40)->_skillshot_type(kLine)->_is_fixed_range(TRUE)->_invert(TRUE)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Yasuo")->_spell_name("YasuoQ3Wrapper")->_missile_name( "YasuoQ3Mis" )->_danger_level(kHigh)->_raw_range(1100)->_delay(300)->_raw_radius(90)->_missile_speed(1200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_invert(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Yasuo")->_spell_name("YasuoQ3Mis")->_missile_name( "YasuoQ3Mis" )->_danger_level(kHigh)->_raw_range(1100)->_delay(300)->_raw_radius(90)->_missile_speed(1200)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_invert(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Yone")->_spell_name("YoneQ")->_spell_slot(kSpellSlot::SpellSlot_Q)->_danger_level(kMedium)->_missile_speed(INT_MAX)->_raw_range(450)->_delay(250)->_raw_radius(40)->_is_fixed_range(TRUE)->_invert(TRUE)->_skillshot_type(kLine)
			);

			spells.push_back
			(
				InitializeSpell("Yone")->_spell_name("YoneQ3Mis")->_missile_name("YoneQ3Mis")->_spell_slot(kSpellSlot::SpellSlot_Q)->_danger_level(kHigh)->_raw_range(1050)->_delay(250)->_raw_radius(80)->_missile_speed(1500)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_invert(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Yone")->_spell_name("YoneW")->_spell_slot(kSpellSlot::SpellSlot_W)->_danger_level(kMedium)->_missile_speed(INT_MAX)->_raw_range(600)->_delay(375)->_raw_radius(80)->_is_fixed_range(TRUE)->_invert(TRUE)->_skillshot_type(kCone)
			);

			spells.push_back
			(
				InitializeSpell("Yone")->_spell_name("YoneR")->_spell_slot(kSpellSlot::SpellSlot_R)->_danger_level(kHigh)->_missile_speed(INT_MAX)->_raw_range(1000)->_delay(750)->_raw_radius(113)->_is_fixed_range(TRUE)->_invert(TRUE)->_skillshot_type(kLine)
			);

			spells.push_back
			(
				InitializeSpell("Yorick")->_spell_name("YorickW")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(600)->_delay(750)->_raw_radius(250)->_skillshot_type(kCircle)->_is_disabled_by_default(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Yorick")->_spell_name("YorickE")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(550)->_delay(50)->_raw_radius(120)->_skillshot_type(kLine)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Zac")->_spell_name("ZacQ")->_danger_level(kMedium)->_raw_range(800)->_missile_speed(2800)->_delay(250)->_raw_radius(120)->_skillshot_type(kMissileLine)->_is_fixed_range(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Zac")->_spell_name("ZacE2")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1800)->_delay(0)->_raw_radius(250)->_missile_speed(1350)->_skillshot_type(kCircle)->_is_dangerous(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Zed")->_spell_name("ZedQ")->_missile_name( "ZedQMissile" )->_danger_level(kMedium)->_raw_range(925)->_raw_radius(50)->_missile_speed(1700)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Ziggs")->_spell_name("ZiggsQ")->_missile_name( "ZiggsQSpell" )->_danger_level(kMedium)->_raw_range(850)->_raw_radius(125)->_missile_speed(1700)->_collision_flags(kYasuoWall)->_skillshot_type(kBox)
			);

			spells.push_back
			(
				InitializeSpell("Ziggs")->_spell_name("ZiggsQBounce")->_missile_name( "ZiggsQSpell2")->_missile_name("ZiggsQSpell3" )->_danger_level(kMedium)->_raw_range(1100)->_delay(400)->_raw_radius(125)->_missile_speed(1600)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)->_missile_delayed(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Ziggs")->_spell_name("ZiggsW")->_missile_name( "ZiggsW" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_W)->_raw_range(1000)->_raw_radius(275)->_missile_speed(1750)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Ziggs")->_spell_name("ZiggsE")->_missile_name( "ZiggsE2" )->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(900)->_raw_radius(235)->_missile_speed(1550)->_collision_flags(kYasuoWall)->_skillshot_type(kCircle)
			);

			spells.push_back
			(
				InitializeSpell("Ziggs")->_spell_name("ZiggsR")->_missile_name("ZiggsRBoom")->_missile_name("ZiggsRBoomMedium")->_missile_name("ZiggsRBoomLong")->_danger_level(kMedium)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(5000)->_delay(400)->_raw_radius(500)->_missile_speed(1500)->_skillshot_type(kCircle)->_extra_delay({ 1100 })->_danger_level(kExtreme)
			);

			spells.push_back
			(
				InitializeSpell("Zilean")->_spell_name("ZileanQ")->_missile_name( "ZileanQMissile" )->_toggle_name("Zilean_.+_Q_Indicator_(Green|Red)")->_danger_level(kMedium)->_raw_range(900)->_raw_radius(150)->_missile_speed(2000)->_skillshot_type(kCircle)->_collision_flags(kYasuoWall)->_can_be_deleted(FALSE)->_dont_cross(TRUE)
			);

			spells.push_back
			(
				InitializeSpell("Zoe")->_spell_name("ZoeQ")->_missile_name( "ZoeQMissile" )->_spell_slot(kSpellSlot::SpellSlot_Q)->_danger_level(kMedium)->_is_fixed_range(TRUE)->_raw_range(875)->_raw_radius(50)->_missile_speed(1200)->_skillshot_type(kMissileLine)->_collision_flags(kYasuoWall | kHeroes | kMinions)->_is_dangerous(FALSE)
			);

			spells.push_back
			(
				InitializeSpell("Zoe")->_spell_name("ZoeQ2")->_missile_name( "ZoeQMis2" )->_spell_slot(kSpellSlot::SpellSlot_Q)->_danger_level(kMedium)->_raw_range(2500)->_raw_radius(70)->_missile_speed(2500)->_skillshot_type(kMissileLine)->_collision_flags(kYasuoWall | kHeroes | kMinions)->_is_dangerous(FALSE)
			);

			spells.push_back
			(
				InitializeSpell("Zoe")->_spell_name("ZoeE")->_missile_name( "ZoeEMis" )->_spell_slot(kSpellSlot::SpellSlot_E)->_danger_level(kMedium)->_is_fixed_range(TRUE)->_raw_range(800)->_raw_radius(50)->_missile_speed(1700)->_skillshot_type(kMissileLine)->_collision_flags(kYasuoWall | kHeroes | kMinions)->_is_dangerous(FALSE)
			);

			spells.push_back
			(
				InitializeSpell("Zyra")->_spell_name("ZyraQ")->_danger_level(kMedium)->_missile_speed(INT_MAX)->_raw_range(800)->_delay(825)->_raw_radius(200)->_skillshot_type(kBox)->_perpendicular(TRUE)->_radius_ex(450)
			);

			spells.push_back
			(
				InitializeSpell("Zyra")->_spell_name("ZyraE")->_missile_name( "ZyraE" )->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_E)->_raw_range(1100)->_raw_radius(70)->_missile_speed(1150)->_collision_flags(kYasuoWall)->_is_fixed_range(TRUE)->_is_dangerous(TRUE)->_skillshot_type(kMissileLine)
			);

			spells.push_back
			(
				InitializeSpell("Zyra")->_spell_name("ZyraR")->_danger_level(kHigh)->_spell_slot(kSpellSlot::SpellSlot_R)->_raw_range(700)->_delay(2200)->_raw_radius(520)->_skillshot_type(kCircle)
			);
		}
	}
}