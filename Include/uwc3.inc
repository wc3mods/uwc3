/*
 * (C)2005-2006 UWC3 Team
 * Ultimate Warcraft 3 Info
 */

#if defined _uwc3_included
  #endinput
#endif

#include <uwc3_const>

enum
{
	give_pistol = 0,
	give_nade,
	no_hegren,
}

enum
{
	gate = 0,
	mole_,
	fan_of_knives
}

/***********
 * NATIVES *
 ***********/

native no_weap(key, setting);

native uwc3_commands(index);

native uwc3_tips(index);

native uwc3_tips2(index);

native war3_info(index);

native uwc3_attributes(index);

native uwc3_resistances(index);

native show_news(index);

native uwc3_credits(index);

native items_info(index);

native items_info2(index);

native uwc3_ultimates(index);

native change_race(index);

/* reset = 1 Reset model, reset = 2 zombie, reset 3 = Mole/chameleon */
native changeskin(id, reset);

native set_changeskin(id, setting);

/* return the count of team. team = 1 T's team = 2 CT's*/
native count_team(team);

stock MaxBPAmmo[] =
{
	0,  //Nothing
	52,	//CSW_P228
	90,	//CSW_SCOUT
	1,	//CSW_HEGRENADE
	32,	//CSW_XM1014
	0,	//CSW_C4
	100,//CSW_MAC10
	90,	//CSW_AUG
	1,	//CSW_SMOKEGRENADE
	120,//CSW_ELITE
	100,//CSW_FIVESEVEN
	100,//CSW_UMP45
	90,	//CSW_SG550
	90,	//CSW_GALIL
	90,	//CSW_FAMAS
	100,//CSW_USP
	120,//CSW_GLOCK18
	30,	//CSW_AWP
	120,//CSW_MP5NAVY
	200,//CSW_M249
	21,	//CSW_M3
	90,	//CSW_M4A1
	120,//CSW_TMP
	90,	//CSW_G3SG1
	2,	//CSW_FLASHBANG
	35,	//CSW_DEAGLE
	90,	//CSW_SG552
	90,	//CSW_AK47
	0,	//CSW_KNIFE
	100 //CSW_P90
}

stock const weapon_names[][] =
{
	"",
	"weapon_p228",
	"weapon_scout",
	"weapon_hegrenade",
	"weapon_xm1014",
	"",
	"weapon_mac10",
	"weapon_aug",
	"weapon_smokegrenade",
	"weapon_elite",
	"weapon_fiveseven",
	"weapon_ump45",
	"weapon_sg550",
	"weapon_galil",
	"weapon_famas",
	"weapon_usp",
	"weapon_glock18",
	"weapon_awp",
	"weapon_mp5navy",
	"weapon_m249",
	"weapon_m3",
	"weapon_m4a1",
	"weapon_tmp",
	"weapon_g3sg1",
	"weapon_flashbang",
	"weapon_deagle",
	"weapon_sg552",
	"weapon_ak47",
	"",
	"weapon_p90",
	"item_kevlar",
	"item_assaultsuit"
}

stock team_classname[][] =
{
	"info_player_deathmatch",
	"info_player_start"
}

stock str_break(const String[], Left[], Right[], lMax, rMax)
{
	strbreak(String, Left, lMax, Right, rMax);
}

stock replaceall(text[], len, what[], with[])
{
	replace_all(text, len, what, with);
}

stock const MOD[] = "UWC3";

stock bool:is_connected[33];
stock pMax = 0;

stock check_player(index)
{
	if(index < 1 || index > pMax)
	{
		log_error(AMX_ERR_NATIVE, "Invalid player %d", index);
		return 1;
	}
	else if(!is_connected[index])
	{
		log_error(AMX_ERR_NATIVE, "Player %d is not in game", index);
		return 1;
	}
	return 0;
}

stock Create_Fade(id, duration, holdtime, type, r, g, b, alpha)
{
	message_begin(MSG_ONE, gmsgFade, {0, 0, 0}, id)
	write_short(duration) // fade lasts this long duration
	write_short(holdtime) // fade lasts this long hold time
	write_short(type) // fade type (in / out)
	write_byte(r) // fade red
	write_byte(g) // fade green
	write_byte(b) // fade blue
	write_byte(alpha) // fade alpha
	message_end()
}