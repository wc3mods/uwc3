#include <amxmodx>
#include <cstrike>
#include <uwc3>

/* Model names for T and CT skins */
new CTSkins[4][] =
{
	"sas",
	"gsg9",
	"urban",
	"gign"
}

new TSkins[4][] =
{
	"arctic",
	"leet",
	"guerilla",
	"terror"
}

public plugin_precache()
{
	// Precache std CS player models for use with decoy ability
	if(file_exists("models/player/zombie/zombie.mdl"))
		precache_model("models/player/zombie/zombie.mdl")

/* // Default models do not need to be precache Tested this in a mini plugin when using cs_set_user_model
	precache_model("models/player/leet/leet.mdl")
	precache_model("models/player/arctic/arctic.mdl")
	precache_model("models/player/guerilla/guerilla.mdl")
	precache_model("models/player/terror/terror.mdl")

	precache_model("models/player/gign/gign.mdl")
	precache_model("models/player/sas/sas.mdl")
	precache_model("models/player/gsg9/gsg9.mdl")
	precache_model("models/player/urban/urban.mdl")

	precache_model("models/player/vip/vip.mdl")*/
}

public plugin_init()
{
	register_plugin("UWC3 FunctiONs", VER, "UWC3 TEAM");

	new map[32];

	pMax = get_maxplayers();
	get_mapname(map, 31)
	checkmap(map)

	/* TEST COMMANDS */
	register_clcmd("loc", "admin_loc", ADMIN_MENU, "-Displays your coordinate locatiON")
	/* END TEST COMMANDS */
}

/*
 * BEGIN changeskin subroutine Routine to change a players skin as needed by item or skill
 */

public native_changeskin(id, num)
{
	new index = get_param(1)

	if(index < 1 || index > pMax || !is_connected[index])
		return;

	new type = get_param(2)

	switch(type)
	{
		case 1:
		{
			cs_reset_user_model(index)
			set_changeskin(index, OFF)
		}
		case 2:
		{
			cs_set_user_model(index, "zombie")
			set_changeskin(index, ON)
		}
		default:
		{
			new num = random_num(0, 3)

			switch(get_user_team(index))
			{
				case TEAM_T:
					cs_set_user_model(index, CTSkins[num])
				case TEAM_CT:
					cs_set_user_model(index, TSkins[num])
			}
			set_changeskin(index, ON)
		}
	}
}

/*
 * END changeskin subroutine
 */

/*
 * BEGIN count_team subroutine
 * Count the members of the T/CT team - rewrite for buggy amxx functiON
 * teame06 rewrite to ONe single functiON redundant to have two.
 */
public native_count_team(id, num)
{

	new team = get_param(1)

	new count = 0
	new players[32]
	new numberofplayers
	new i = 0

	get_players(players, numberofplayers)

	for(i = 0; i < numberofplayers; ++i)
	{
		if(is_connected[i])
			if(get_user_team(players[i]) == team)
				count += 1
	}
	return count
}

// **************************************************************************
// END count_team T & CT subroutine
// **************************************************************************

// **************************************************************************
// BEGIN admin_loc subroutine
// Display your locatiON in coordinates
// **************************************************************************
public admin_loc(id)
{
	new origin[3]

	get_user_origin(id, origin)
	client_print(id, print_chat, "[%s] Your locatiON is: <%d %d %d>", MOD, origin[0], origin[1], origin[2])

	return PLUGIN_HANDLED
}
// **************************************************************************
// END admin_loc subroutine
// **************************************************************************
/*
 * Prevents weapONs from being given in knife ONly maps
 */

checkmap(mapname[])
{
	new gaben = get_cvar_num("FT_no_gloves_ON_ka")

	if(equali(mapname, "ka_", 3) || equali(mapname, "he_", 3) || containi(mapname,"scoutzknivez") != -1)
	{
		no_weap(give_pistol, ON)
		no_weap(give_nade, gaben ? OFF : ON)
	}
	else
	{
		no_weap(give_nade, ON)
		no_weap(give_pistol, ON)
	}

	if(get_cvar_num("FT_no_orcnades_ON_he"))
	{
		if(equali(mapname,"ka_", 3) || equali(mapname,"jail_riot", 3) || equali(mapname,"fy_", 3))
			no_weap(no_hegren, ON)
		else
			no_weap(no_hegren, OFF)
	}

	/* This is ONly ran ONce so it ok.*/
	new gaben_map[][] =
	{
		"cs_tibet",
		"awp_city",
		"cs_deagle5",
		"cs_speedball",
		"cs_dualies",
		"knife_zone"
	}

	for(new i = 0; i < 6; i++)
	{
		if(equali(mapname, gaben_map[i]))
		{
			no_weap(give_pistol, ON)
			no_weap(give_nade, gaben ? OFF : ON)
			break
		}
	}
}

/*
 * END checkmap subroutine
 */

public client_putinserver(id)
{
	is_connected[id] = true
}

public client_discONnect(id)
{
	is_connected[id] = false
}

public plugin_natives()
{
	register_native("changeskin", "native_changeskin")
	register_native("count_team", "native_count_team")
	register_library("uwc3")
}