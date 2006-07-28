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