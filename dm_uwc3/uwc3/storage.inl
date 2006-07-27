// Call routines to write all players' xp
/*
public write_all()
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	new players[32], numofplayers, id, i//, usersql[512], bigsql[2048], bigsqllen = 0 // let's hope this is enough
	get_players(players, numofplayers, "c") // no bots...

	//new usenewway = get_cvar_num("usenewway")
	for(i = 0; i < numofplayers; i++)
	{
		id = players[i]
		if(get_cvar_num("mp_sql") && !get_cvar_num("mp_sql_saveoncmdonly"))
		{
			/*if(usenewway)
			{
				sqlgetsql_xp_id(id, usersql, 511)
				// Merge
				bigsqllen += format(bigsql[bigsqllen], 2047 - bigsqllen, "%s", usersql)
			}
			else
			{*/
			/*sqlwrite_xp_id(id)
			purge(id)
			//}
		}
		else
			write_xp_id(id)
	}*/

	/*if(usenewway && bigsqllen > 0 && get_cvar_num("mp_sql") && !get_cvar_num("mp_sql_saveoncmdonly"))
	{
		// Connect, query, disconnect
		// Connect to the SQL server and create the tables if they do not exist

		server_print("UWC3 write_all:^n^n%s^n^n", bigsql)

		new Result:result = dbi_query(sql, bigsql)

		if(result > RESULT_NONE)
			dbi_free_result(result)

//		dbi_close(sql)
	}
	return PLUGIN_CONTINUE
}*/


// Frontend for saving a player's XP, Attribs, Resists, Skills, etc
public uwc3_savexp(id)
{
	if(get_cvar_num("mp_savexp"))
	{
		if(get_cvar_num("mp_sql"))
			sqlwrite_xp_id(id)
		else
			write_xp_id(id)

		client_print(id,print_chat, "[%s] Your UWC3 XP has been saved.",MOD)
	}
	else
	{
		client_print(id,print_chat,"[%s] Long term XP mode is NOT enabled now.", MOD)
	}

	return PLUGIN_HANDLED
}

public uwc3_saveskills(id)
{
	if(!get_cvar_num("mp_savexp"))
	{
		client_print(id, print_chat, "[%s] Long term XP mode is NOT enabled now.", MOD)
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("mp_sql"))
		sqlwrite_skills_id(id, -1, 1)
	else
		write_skills_id(id, 1)

	client_print(id, print_chat, "[%s] You have saved your UWC3 skills", MOD)

	return PLUGIN_HANDLED
}

public uwc3_saveattribs(id)
{
	if(!get_cvar_num("mp_savexp"))
	{
		client_print(id,print_chat,"[%s] Long term XP mode is NOT enabled now.", MOD)
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("mp_sql"))
		sqlwrite_enh_id(id, 1)
	else
		write_enh_id(id)

	client_print(id,print_chat, "[%s] Your UWC3 Attributes have been saved.",MOD)

	return PLUGIN_HANDLED
}

public uwc3_saveresists(id)
{
	if(!get_cvar_num("mp_savexp"))
	{
		client_print(id,print_chat,"[%s] Long term XP mode is NOT enabled now.", MOD)
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("mp_sql"))
		sqlwrite_enh_id(id, 1)
	else
		write_enh_id(id)

	client_print(id,print_chat, "[%s] Your UWC3 Resistances have been saved.",MOD)

	return PLUGIN_HANDLED
}

public uwc3_saveall(id)
{
	if(!get_cvar_num("mp_savexp"))
	{
		client_print(id,print_chat,"[%s] Long term XP mode is NOT enabled now.", MOD)
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("mp_sql"))
	{
		client_print(id,print_chat, "[%s] Saving all your UWC3 Data now.(SQL)",MOD)
		sqlwrite_xp_id(id)
		sqlwrite_skills_id(id, -1, 1)
		sqlwrite_enh_id(id, 1)
	}
	else
	{
		client_print(id,print_chat, "[%s] Saving all your UWC3 Data now.(Vault)",MOD)
		write_xp_id(id)
		write_skills_id(id, 1)
		write_enh_id(id)
	}
	return PLUGIN_HANDLED
}