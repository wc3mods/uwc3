// Write a player's xp / info to the vault
public write_xp_id(id)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp") || xpreadytoload[id])
	{
		// Do not write XP if it mp_savexp not set or xp hasn't been loaded yet
		//log_amx("In write_xp_id() id=(%d) mp_savexp=(%d) xpready=(%d)",
		//   id, get_cvar_num("mp_savexp"), xpreadytoload[id])
		return PLUGIN_CONTINUE
	}

	new playerid[32], playername[32], timet[32], ip[32]
	new pkey[128]
	new pdata[512]

	get_time("%m %d %H %M", timet, 31)
	get_user_authid(id, playerid, 31)
	get_user_name(id, playername, 31)
	get_user_ip(id, ip, 31)

	if(playerxp[id] <= 150)
	{
		// Do not save XP if <= 150
		//log_amx("In write_xp_id() id=(%d) playerxp=(%d) Not enough XP to save", id, playerxp[id])
		return PLUGIN_CONTINUE
	}

	format(pdata, 255, "%s %d %s %s", playerid, playerxp[id], ip, timet)

	format(pkey, 127, "%s", playerid)
	set_vaultdata(pkey, pdata)

	return PLUGIN_CONTINUE
}

// Write a player's enhancements(attributes/resists) info to the vault
public write_enh_id(id)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp") || xpreadytoload[id])
	{
		// Do not write XP if it mp_savexp not set or xp hasn't been loaded yet
		//log_amx("In write_xp_id() id=(%d) mp_savexp=(%d) xpready=(%d)", id, get_cvar_num("mp_savexp"), xpreadytoload[id])
		return PLUGIN_CONTINUE
	}

	new playerid[32]
	new pkey[128]
	new pdata[128]

	get_user_authid(id, playerid, 31)

	format(pkey, 127, "%s_enh", playerid)
	format(pdata, 127, "%d %d %d %d %d %d %d %d %d %d", p_attribs[id][ATTRIBIDX_STR], p_attribs[id][ATTRIBIDX_INT], p_attribs[id][ATTRIBIDX_DEX], p_attribs[id][ATTRIBIDX_AGI], p_attribs[id][ATTRIBIDX_CON], p_resists[id][RESISTIDX_POISON], p_resists[id][RESISTIDX_DISEASE], p_resists[id][RESISTIDX_ELECTRIC], p_resists[id][RESISTIDX_FIRE], p_resists[id][RESISTIDX_MAGIC])

	set_vaultdata(pkey, pdata)

	return PLUGIN_CONTINUE
}

// Write a player's skills settings to the vault
public write_skills_id(id, set)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp") || xpreadytoload[id])
	{
		// Do not write XP if it mp_savexp not set or xp hasn't been loaded yet
		//log_amx("In write_xp_id() id=(%d) mp_savexp=(%d) xpready=(%d)", id, get_cvar_num("mp_savexp"), xpreadytoload[id])
		return PLUGIN_CONTINUE
	}

	if(set < 1 || set > 4) // set will allow multiple skillsets to be saved and retrieved
	{
		set = 1
	}

	new pkey[128]
	new playerid[32]
	new temp1[256]
	new temp2[256]
	new temp3[256]
	new temp4[256]
	new pdata[512]

	get_user_authid(id, playerid, 31)

	format(pkey, 127, "%s_%d", playerid, set)
	format(temp1, 255, "%d %d %d %d %d %d %d %d %d %d", p_skills[id][1], p_skills[id][2], p_skills[id][3], p_skills[id][4], p_skills[id][5], p_skills[id][6], p_skills[id][7], p_skills[id][8], p_skills[id][9], p_skills[id][10])
	format(temp2, 255, "%d %d %d %d %d %d %d %d %d %d", p_skills[id][11], p_skills[id][12], p_skills[id][13], p_skills[id][14], p_skills[id][15], p_skills[id][16], p_skills[id][17], p_skills[id][18], p_skills[id][19], p_skills[id][20])
	format(temp3, 255, "%d %d %d %d %d %d %d %d %d %d", p_skills[id][21], p_skills[id][22], p_skills[id][23], p_skills[id][24], p_skills[id][25], p_skills[id][26], p_skills[id][27], p_skills[id][28], p_skills[id][29], p_skills[id][30])
	format(temp4, 255, "%d %d %d %d %d %d %d %d %d %d", p_skills[id][31], p_skills[id][32], p_skills[id][33], p_skills[id][34], p_skills[id][35], p_skills[id][36], p_skills[id][37], p_skills[id][38], p_skills[id][39], p_skills[id][40])
	format(pdata, 255, "%s %s %s %s", temp1, temp2, temp3, temp4)

	set_vaultdata(pkey, pdata)

	return PLUGIN_CONTINUE
}

// Retrieve a player's xp from the vault
public get_xp_id(id)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp"))
		return PLUGIN_CONTINUE

	new playerid[32]
	new playername[32]
	new ip[32]
	new xp[32]
	new tmth[32], tday[32], thr[32], tmin[32]


	get_user_name(id, playername, 31)
	get_user_authid(id, playerid, 31)
	get_user_ip(id, ip, 31)

	new pkey[128]
	new pdata[512]
	new temp1[512]
	new temp2[512]

	format(pkey, 127, "%s", playerid)

	new attempt = get_vaultdata(pkey, pdata, 511)

	if(attempt)
	{
		//server_print("From Vault for(%s)- [%s]", pkey, pdata)

		str_break(pdata, playerid, temp1, 32, 511)
		str_break(temp1, xp, temp2, 32, 511)
		str_break(temp2, ip, temp1, 32, 511)
		str_break(temp1, tmth, temp2, 32, 511)
		str_break(temp2, tday, temp1, 32, 511)
		str_break(temp1, thr, temp2, 32, 511)
		str_break(temp2, tmin, temp1, 32, 511)

		playerxp[id] = str_to_num(xp)

		//server_print("Loaded %d XP for %s", playerxp[id], playername)

		client_print(id, print_chat, "Your XP has been loaded from the vault")
		displaylevel(id, 0)
	}
	else
	{
		client_print(id, print_chat, "No XP found for SteamID %s", playerid)
	}
	return PLUGIN_CONTINUE
}

// Retrieve a player's enh info(attribs/resists) from the vault
public get_enh_id(id)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp"))
		return PLUGIN_CONTINUE

	new playerid[32]
	new pkey[128]
	new pdata[512]
	new temp1[512]
	new temp2[512]
	new tnum[32]

	get_user_authid(id, playerid, 31)
	format(pkey, 127, "%s_enh", playerid)

	new attempt = get_vaultdata(pkey, pdata, 511)

	if(attempt)
	{
		server_print("Attempt Get From Vault for OK(%s)- [%s]", pkey, pdata)
		str_break(pdata, tnum, temp1, 32, 511)
		p_attribs[id][ATTRIBIDX_STR] = str_to_num(tnum)
		str_break(temp1, tnum, temp2, 32, 511)
		p_attribs[id][ATTRIBIDX_INT] = str_to_num(tnum)
		str_break(temp2, tnum, temp1, 32, 511)
		p_attribs[id][ATTRIBIDX_DEX] = str_to_num(tnum)
		str_break(temp1, tnum, temp2, 32, 511)
		p_attribs[id][ATTRIBIDX_AGI] = str_to_num(tnum)
		str_break(temp2, tnum, temp1, 32, 511)
		p_attribs[id][ATTRIBIDX_CON] = str_to_num(tnum)
		str_break(temp1, tnum, temp2, 32, 511)
		p_resists[id][RESISTIDX_POISON] = str_to_num(tnum)
		str_break(temp2, tnum, temp1, 32, 511)
		p_resists[id][RESISTIDX_DISEASE] = str_to_num(tnum)
		str_break(temp1, tnum, temp2, 32, 511)
		p_resists[id][RESISTIDX_ELECTRIC] = str_to_num(tnum)
		str_break(temp2, tnum, temp1, 32, 511)
		p_resists[id][RESISTIDX_FIRE] = str_to_num(tnum)
		str_break(temp1, tnum, temp2, 32, 511)
		p_resists[id][RESISTIDX_MAGIC] = str_to_num(tnum)
		server_print("Loaded From Vault for(%s) Attribs=(%d %d %d %d %d) Resists=(%d %d %d %d %d)", playerid, p_attribs[id][ATTRIBIDX_STR], p_attribs[id][ATTRIBIDX_INT], p_attribs[id][ATTRIBIDX_DEX], p_attribs[id][ATTRIBIDX_AGI], p_attribs[id][ATTRIBIDX_CON], p_resists[id][RESISTIDX_POISON], p_resists[id][RESISTIDX_DISEASE], p_resists[id][RESISTIDX_ELECTRIC], p_resists[id][RESISTIDX_FIRE], p_resists[id][RESISTIDX_MAGIC])

		// [09-07-04] Now check to make sure total attrib/resist pts meet server limits - K2
		new acount = get_attribcount(id)
		new rcount = get_resistcount(id)

		if(acount > ATTRIB_MAX_PTS)
		{
			resetattrib[id] = true
			client_print(id, print_chat, "Accrued Attrib Pts [%d] Exceeds Server Limit [%d], Invoking Reset", acount, ATTRIB_MAX_PTS)
			server_print("Accrued Attrib Pts [%d] Exceeds Server Limit [%d], Invoking Reset", acount, ATTRIB_MAX_PTS)
		}

		if(rcount > RESIST_MAX_PTS)
		{
			resetresist[id] = true
			client_print(id, print_chat, "Accrued Resist Pts [%d] Exceeds Server Limit [%d], Invoking Reset", rcount, RESIST_MAX_PTS)
			server_print("Accrued Resist Pts [%d] Exceeds Server Limit [%d], Invoking Reset", rcount, RESIST_MAX_PTS)
		}
	}
	else
	{
		client_print(id, print_chat, "No ENH data found for SteamID %s", playerid)
	}
	return PLUGIN_CONTINUE
}

// Retrieve a player's skills from the vault
public get_skills_id(id, set)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp"))
		return PLUGIN_CONTINUE

	if(set < 1 || set > 4) // set will allow multiple skillsets to be saved and retrieved
	{
		set = 1
	}

	new Nskills[MAX_SKILLS]

	for(new i = 1; i< MAX_SKILLS; i++)
		Nskills[i] = 0

	new playername[32]
	new playerid[32]
	new pkey[128]
	new pdata[512]

	get_user_name(id, playername, 31)
	get_user_authid(id, playerid, 31)

	format(pkey, 127, "%s_%d", playerid, set)

	new attempt = get_vaultdata(pkey, pdata, 511)

	if(attempt)
	{
		//server_print("From Vault for(%s)- [%s]", pkey, pdata)
		//server_print("Ready to Parse pdata=[%s]", pdata)

		//log_amx("Loading vault data for key(%s)", pkey)
		new i
		new j = 1
		new k
		new skills_loaded
		new s = strlen(pdata)

		for(i = 0; i < s; i++)
		{
			//check for space or quote
			if(i ==(s-1))
			{
				++skills_loaded

				if(k == 0)
					Nskills[j] = str_to_num(pdata[i])
				else
					Nskills[j] += power(10, k) * str_to_num(pdata[i])

				/*if(j <= 8)
					copy(sname, 31, skillset1[j])
				else if(j <= 16)
					copy(sname, 31, skillset2[j-8])
				else if(j <= 24)
					copy(sname, 31, skillset3[j-16])
				else if(j <= 32)
					copy(sname, 31, skillset4[j-24])
				else if(j <= 40)
					copy(sname, 31, skillset5[j-32])

				server_print("Parsing skills: i=(%d) j=(%d) skill=(%s) val=<%d>", i, j, sname, Nskills[j]) */
			}
			else if(pdata[i] == 32 || pdata[i] == 34)
			{

				/*if(j <= 8)
				copy(sname, 31, skillset1[j])
				else if(j <= 16)
				copy(sname, 31, skillset2[j-8])
				else if(j <= 24)
				copy(sname, 31, skillset3[j-16])
				else if(j <= 32)
				copy(sname, 31, skillset4[j-24])
				else if(j <= 40)
				copy(sname, 31, skillset5[j-32])

				server_print("Parsing skills: i=(%d) j=(%d) skill=(%s) val=<%d>",
				i, j, sname, Nskills[j]) */

				++j
				k = 0
				++skills_loaded
			}
			else
			{
				// Reading in a non-space or EOL character
				Nskills[j] += power(10, k++) * str_to_num(pdata[i])

				//server_print("NEW - Parsing skills: i=(%d) j=(%d) val=(%d)", i, j, Nskills[j])
			}
		}

		//for(i = 1; i <= skills_loaded; i++)
		for(i = 1; i < MAX_SKILLS; i++)
		{
			new tnum = Nskills[i]
			if(tnum >= 0 && tnum <= 10)
			{
				p_skills[id][i] = tnum

				// Check for an Ultimate ability in saved skills
				new skill_idx = i

				/*if(i <= 8)
					copy(sname, 31, skillset1[i])
				else if(i <= 16)
					copy(sname, 31, skillset2[i-8])
				else if(i <= 24)
					copy(sname, 31, skillset3[i-16])
				else if(i <= 32)
					copy(sname, 31, skillset4[i-24])
				else if(i <= 40)
					copy(sname, 31, skillset5[i-32])

				server_print("Loading %d for skill %s", tnum, sname)*/

				if(tnum >= 1 && skill_ultimates[skill_idx][0])
				{
					// This is an Ultimate ability being retrieved from saved skills
					client_print(id, print_console, "[%s] Retrieved saved ultimate(%s)", MOD, skill_ultimates[skill_idx][0])
					ultlearned[id]++
				}
			}
			else
			{
				p_skills[id][i] = 0
			}
		}

		//server_print("Loaded %d skills [skillset%d] for %s ", skills_loaded, set, playername)

		client_print(id, print_chat, "Your skillset %d has been loaded from the vault", set)
		displaylevel(id, 0)
	}
	else
	{
		client_print(id, print_chat, "No Skills found for skillset %d , SteamID %s", set, playerid)
	}
	return PLUGIN_CONTINUE
}
