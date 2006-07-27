// Front end to choose the appropriate routine for setting up XP variables
public set_xpfrontend()
{
	if(get_cvar_num("mp_savexp"))
	{
		set_longtermxp()
	}
	else
	{
		set_shorttermxp()
	}
}

// Modify some variables based on short term XP status
public set_shorttermxp()
{
	if(get_cvar_num("mp_savexp") == 0)
	{
		#if !defined CSDM_NO_OBJECTIVES
		BOMBPLANTXP /= 3
		DEFUSEXP /= 3
		HOSTAGEXP /= 3
		KILLRESCUEMANXP /= 3
		KILL_BOMB_CARRIER /= 3
		DEFUSER_KILL_BONUS /= 3
		VIP_ESCAPE_BONUS /= 3
		#endif

		HEADSHOT_BONUS /= 3
		XPBONUS /= 3
		ROUNDXP /= 3
		REPAIRXP /= 1
		MENDXP /= 1
		PHOENIXXP /= 1

		for(new j = 0; j < MAX_LEVEL; j++)
		{
			// Sets the amount of xp needed for next level
			if(j <= 10)
				xplevel_lev[j] = xplevel_base[j]
			else
				xplevel_lev[j] = XP_PER_LEVEL_BASE + ((j - 10) * XP_PER_LEVEL)

			// Sets the amount of xp given for each level
			xpgiven_lev[j] = XP_BASE + (j * XP_ADD_LEVEL)

			if(get_cvar_float("mp_xpmultiplier"))
			{
				new Float:xpmultiplier = get_cvar_float("mp_xpmultiplier")
				xplevel_lev[j] = floatround(xplevel_lev[j] * xpmultiplier)

				// Use additional multipliers for higher levels
				if(j >= 30)
					xplevel_lev[j] = floatround(xplevel_lev[j] * 3.0)
				else if(j >= 20)
					xplevel_lev[j] = floatround(xplevel_lev[j] * 2.0)
				else if(j >= 10)
					xplevel_lev[j] = floatround(xplevel_lev[j] * 1.5)
			}
		}

		// Set the attribute and resistance pt costs(based on xplevel table)
		attrib_pt_cost = floatround((xplevel_lev[10] / 10) * get_cvar_float("UW_attrib_xpmodifier"))
		resist_pt_cost = floatround((xplevel_lev[5] / 10) * get_cvar_float("UW_resist_xpmodifier"))
	}
	return PLUGIN_CONTINUE
}

// Modify some variables based on long term XP status
public set_longtermxp()
{
	if(get_cvar_num("mp_savexp"))
	{
		#if !defined CSDM_NO_OBJECTIVES
		BOMBPLANTXP /= 10
		DEFUSEXP /= 10
		HOSTAGEXP /= 10
		KILLRESCUEMANXP /= 10
		KILL_BOMB_CARRIER /= 10
		DEFUSER_KILL_BONUS /= 10
		VIP_ESCAPE_BONUS /= 10
		#endif

		HEADSHOT_BONUS /= 10
		XPBONUS /= 10
		ROUNDXP /= 10
		REPAIRXP /= 10
		MENDXP /= 10
		PHOENIXXP /= 10


		for(new j = 0; j < MAX_LEVEL; j++)
		{
			// Sets the amount of xp needed for next level
			if(j <= 10)
				xplevel_lev[j] = xplevel_base_LT[j]
			else
				xplevel_lev[j] = XP_PER_LEVEL_BASE_LT + ((j-10) * XP_PER_LEVEL_LT) + floatround(float(j / 20) * XP_PER_LEVEL_LT)

			// Sets the amount of xp given for each level
			xpgiven_lev[j] = (XP_BASE + (j * XP_ADD_LEVEL)) / 4

			if(get_cvar_float("mp_xpmultiplier"))
			{
				new Float:xpmultiplier = get_cvar_float("mp_xpmultiplier")
				xplevel_lev[j] = floatround(xplevel_lev[j] * xpmultiplier)

				// [07-11-2004] K2mia - Use additional multipliers for higher levels
				if(j >= 30)
					xplevel_lev[j] += floatround(xplevel_lev[j-1] * 1.25)
				else if(j >= 20)
					xplevel_lev[j] += floatround(xplevel_lev[j-1] * 1.00)
				else if(j >= 10)
					xplevel_lev[j] += floatround(xplevel_lev[j-1] * 0.75)
				else
					if(j >= 1)
					xplevel_lev[j] += floatround(xplevel_lev[j-1] * 0.50)

				// Use additional multipliers for higher levels - replaced 7-11-04
				/*if(j >= 30)
					xplevel_lev[j] = floatround(xplevel_lev[j] * 3.0)
				else if(j >= 20)
					xplevel_lev[j] = floatround(xplevel_lev[j] * 2.0)
				else if(j >= 10)
					xplevel_lev[j] = floatround(xplevel_lev[j] * 1.5) */
			}
		}

		// Set the attribute and resistance pt costs(based on xplevel table)
		attrib_pt_cost = floatround((xplevel_lev[10] / 10) * get_cvar_float("UW_attrib_xpmodifier"))
		resist_pt_cost = floatround((xplevel_lev[5] / 10) * get_cvar_float("UW_resist_xpmodifier"))
	}
	return PLUGIN_CONTINUE
}

// Setup various multipliers for WC3 experience
public set_xpmultiplier()
{
	if(!get_cvar_num("mp_weaponxpmodifier"))
		return PLUGIN_CONTINUE

	weaponxpmultiplier[CSW_WORLD] = 0.0
	weaponxpmultiplier[CSW_USP] = 1.0
	weaponxpmultiplier[CSW_DEAGLE] = 1.0
	weaponxpmultiplier[CSW_GLOCK18] = 2.0
	weaponxpmultiplier[CSW_ELITE] = 1.0
	weaponxpmultiplier[CSW_P228] = 1.0
	weaponxpmultiplier[CSW_FIVESEVEN] = 1.5

	weaponxpmultiplier[CSW_XM1014] = 1.25
	weaponxpmultiplier[CSW_M3] = 1.5

	weaponxpmultiplier[CSW_MP5NAVY] = 1.0
	weaponxpmultiplier[CSW_UMP45] = 1.25
	weaponxpmultiplier[CSW_P90] = 1.25
	weaponxpmultiplier[CSW_TMP] = 1.5
	weaponxpmultiplier[CSW_MAC10] = 1.5

	weaponxpmultiplier[CSW_AWP] = 1.0
	weaponxpmultiplier[CSW_M4A1] = 1.0
	weaponxpmultiplier[CSW_AK47] = 1.0
	weaponxpmultiplier[CSW_AUG] = 1.0
	weaponxpmultiplier[CSW_SG552] = 1.0
	weaponxpmultiplier[CSW_G3SG1] = 1.5
	weaponxpmultiplier[CSW_SG550] = 1.5
	weaponxpmultiplier[CSW_M249] = 1.25
	weaponxpmultiplier[CSW_SCOUT] = 2.0

	weaponxpmultiplier[CSW_HEGRENADE] = 1.0
	weaponxpmultiplier[CSW_KNIFE] = 2.0

	weaponxpmultiplier[CSW_C4] = 1.0
	weaponxpmultiplier[CSW_SMOKEGRENADE] = 1.0
	weaponxpmultiplier[CSW_FLASHBANG] = 1.0

	return PLUGIN_CONTINUE
}

// Displays the XP table
public xp_table(id)
{
	client_print(id, print_console, "UWC3 XP Table for this Server: ^n")
	client_print(id, print_console, "Attrib Pt Cost: %d E-XP , Resist Pt Cost: %d E-XP ^n", attrib_pt_cost, resist_pt_cost)

	for(new j = 1; j < MAX_LEVEL; j++)
	{
		client_print(id, print_console, "[level %d]  XP: %d  Total XP: %d ^n", j, (xplevel_lev[j] - xplevel_lev[j - 1]), xplevel_lev[j])
	}

	return PLUGIN_HANDLED
}

// Admin Cmd - Show a Player's XP
public admin_showxp(id, level)
{
	if(!(get_user_flags(id) & get_cvar_num("UW_admin_flag")))
	{
		if(id != 0)
		{
			client_print(id,print_console,"[%s] You have no access to that command", MOD)
			return PLUGIN_HANDLED
		}
	}

	new arg[32]
	new name[32]
	read_argv(1,arg,31)

	new player = cmd_target2(id, arg)

	if(!player)
		return PLUGIN_HANDLED

	get_user_name(player, name, 31)

	client_print(id, print_chat, "[%s] %s has %d XP", MOD, name, playerxp[player])

	return PLUGIN_HANDLED
}

// Resets all skills and sets your XP to 0
public amx_deletexp(id)
{
	p_level[id] = 0
	playerxp[id] = 0

	for(new j = 0; j<MAX_SKILLS; j++)
		p_skills[id][j] = 0 // Set all skills to 0

	// Set all attributes for this player slot to the base amount
	for(new j = 0; j < MAX_ATTRIBS; j++)
		p_attribs[id][j] = ATTRIB_BASE

	// Set all resistances for this player slot to 0
	for(new j = 0; j < MAX_RESISTS; j++)
		p_resists[id][j] = 0

	resetskill[id] = true
	resetattrib[id] = true
	resetresist[id] = true

	displaylevel(id, 1)

	if(get_cvar_num("mp_sql"))
	{
		sqlwrite_xp(id, 1);
		sqlwrite_skills_id(id, -1, 1)
		sqlwrite_enh_id(id, 1)
	}
	else
	{
		write_xp_id(id)
		write_skills_id(id, 1)
		write_enh_id(id)
	}

	client_print(id, print_chat, "[%s] Your skills/attributes/resistances and XP have been reset to 0.", MOD)
}

// Server command for issuing xp to a team or person
public amx_givexp(id, level)
{
	if(!(get_user_flags(id) & get_cvar_num("UW_admin_flag")))
	{
		if(id != 0)
		{
			client_print(id,print_console,"[%s] You have no access to that command", MOD)
			return PLUGIN_HANDLED
		}
	}

	if(get_cvar_num("UW_disable_givexp"))
	{
		client_print(id, print_console, "[%s] This command has been disabled.", MOD)
		return PLUGIN_HANDLED
	}

	new arg[32], xp[10]
	read_argv(1, arg, 31)
	read_argv(2, xp, 9)


	// [9-30-04] Added logging of amx_givexp() command - K2mia
	new name[32]
	get_user_name(id, name, 31)
	log_amx("[%s] %s issued amx_givexp() %s %s", MOD, name, arg, xp)

	if(equali(arg, "@ALL"))
	{
		new players[32], inum, i
		get_players(players, inum)

		for(new a = 0; a < inum; a++)
		{
			i = players[a]
			client_print(i, print_chat, "[%s] The admin gave you %d experience",MOD, str_to_num(xp))
			playerxp[i] += str_to_num(xp)
			displaylevel(i, 1)
		}
		return PLUGIN_HANDLED
	}

	if(arg[0] == '@')
	{
		new players[32], inum
		get_players(players, inum, "e", arg[1])

		if(inum == 0)
		{
			console_print(id, "No clients in such team")
			return PLUGIN_HANDLED
		}

		new i
		for(new a = 0; a < inum; a++)
		{
			i = players[a]
			client_print(i, print_chat, "[%s] The admin gave you %d experience",MOD, str_to_num(xp))
			playerxp[i] += str_to_num(xp)
			displaylevel(i, 1)
		}
	}
	else
	{
		new player = cmd_target2(id, arg)

		if(!player)
			return PLUGIN_HANDLED

		client_print(player,print_chat, "[%s] The admin gave you %d experience", MOD, str_to_num(xp))
		playerxp[player] += str_to_num(xp)
		displaylevel(player, 1)
	}
	return PLUGIN_HANDLED
}