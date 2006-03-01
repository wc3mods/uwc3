static OBJECTIVES[] = "FT_objectives"

public got_defuse(id)
{
   if(warcraft3)
      hasdefuse[id]=true
}

public bartime(id)
{
	//id = player index
	new time = read_data(1)

	if(time != WITHKIT || time != WITHOUTKIT)
		return;

	if(get_cvar_num("FT_attempt_defuse_bonus"))
	{
		new team = get_user_team(id), temp = 0, sName[32]
		get_user_name(id, sName, 31)

		new plevel = p_level[id]
		if(time == WITHKIT) // Is trying to defuse with defuse kit
		{
			if(++bombDefuserIndex[id] && (team == TEAM_CT))
			{
				if (get_cvar_num(OBJECTIVES))
				{
					temp = (1/6 * DEFUSEXP) + xpgiven_lev[plevel]
					playerxp[id] += temp

					client_print(id, print_chat,
						"[%s] %s recieved %d XP for cutting the red wire on the bomb.",	MOD, sName, temp)
				}
				displaylevel(id, 1)
			}
		}
		else if(time == WITHOUTKIT) // is trying to defuse without kit.
		{
			if((++bombDefuserIndex[id] == 1) && (team == TEAM_CT))
			{
				if(get_cvar_num(OBJECTIVES))
				{
					temp = xpgiven_lev[plevel]
					playerxp[bombdefuser]+=  temp

					client_print(id, print_chat,
						"[%s] %s recieved %d XP for attempting to defuse bare handed.^n", MOD, id, temp)
				}
				displaylevel(id, 1)
			}
		}
	}
}

/* Player Trigger Action Bomb's */

public bomb_pickup(id)
{
	bombCarrier = id

	if(get_cvar_num("FT_bomb_event_bonus"))
	{
		if (get_cvar_num(OBJECTIVES))
		{
			new sName[32]
			get_user_name(id, sName, 31)
			new temp = xpgiven_lev[p_level[id]]
			playerxp[id] += temp
			client_print(id, print_chat,
				"[%s] %s gained %d XP for picking up the bomb.", MOD, sName, temp)
		}
		displaylevel(id, 1)
	}
}

public bomb_drop(id)
{
	if(warcraft3)
	{
		new objective = get_cvar_num(OBJECTIVES)
		if(objective)
		{
			new planted = read_data(4)

			if(!planted) // Bomb Droped
			{
				new sName[32], temp
				new plevel = p_level[id]
				get_user_name(id, sName, 31)
				bombCarrier = 0

				if(get_cvar_num("FT_bomb_event_bonus"))
				{
					if(objective)
					{
						temp = xpgiven_lev[plevel]
						playerxp[id] -=  temp
						client_print(id, print_chat,
							"[%s] %s lost the %d XP bomb carrying bonus.", MOD, sName, temp)
					}
					displaylevel(id, 1)
				}
			}
		}
	}
}

public bomb_event(id)
{
	if(!warcraft3)
		return;

	new objective = get_cvar_num(OBJECTIVES)
	if(!id)
		return;

	new param[13]
	read_data(2, param, 12)

	if(param[11] == 'P' || param[11] == 'D')
	{
		new origin[3], player[32], numplayers, targetorigin[3]
		new targetid
		new  sName[32], radius
		new temp = 0
		get_players(player, numplayers, "a")
		get_user_name(id, sName, 31)
		get_user_origin(id, origin)


		if(param[11] == 'P')// Planted
		{
			bombCarrier = 0
			bombstate = BOMB_PLANTED
			new Float:c4time = get_cvar_float("mp_c4timer")
			set_task((c4time - 1.0), "check_for_living", 645+id)
			set_task((c4time + 1.0), "check_for_living2", 645+id)
			if(objective)
			{
				temp = 3 * xpgiven_lev[p_level[id]]
				playerxp[id] += temp

				client_print(id, print_chat,
					"[%s] %s recieved %d extra XP for personally planting the bomb.", MOD,sName,temp)
			}

			for(new i = 0; i < numplayers; ++i)
			{
				targetid=player[i]

				if(get_user_team(targetid) != TEAM_T)
					continue

				get_user_origin(targetid, targetorigin)

				//[09-27-04] Modified playerlist so now have to check for living status - K2mia
				if(get_distance(origin, targetorigin) <= radius)
				{
					if(objective)
					{
						temp = BOMBPLANTXP + xpgiven_lev[p_level[targetid]]
						playerxp[targetid]+=  temp

						get_user_name(targetid,sName,31)
						client_print(targetid, print_chat,
							"[%s] %s recieved %d for supporting the bomb planting effort.", MOD, sName, temp)
					}
				}
			}
			displaylevel(targetid, 1)
		}
		else if(param[11] == 'D')// Defused
		{
			// Bomb defused
			bombstate = BOMB_DEFUSED  // Set BOMB_DEFUSED state, bomb was defused

			if(objective)
			{
				temp = 3 *  xpgiven_lev[p_level[id]]
				playerxp[id] += temp
				client_print(id, print_chat,
					"[%s] %s recieved %d extra XP for defusing the bomb.", MOD, sName, temp)
			}

			for(new i = 0; i < numplayers; ++i)
			{
				targetid=player[i]
				if(get_user_team(targetid) != TEAM_CT)
					continue

				get_user_origin(targetid, targetorigin)

				// [09-27-04] Modified playerlist so now have to check for living status - K2mia
				if(get_distance(origin, targetorigin) <= get_cvar_num("FT_xp_radius"))
				{

					if(objective)
					{
						temp = DEFUSEXP +  xpgiven_lev[p_level[targetid]]
						playerxp[targetid]+=  temp

						get_user_name(targetid,sName,31)
						client_print(targetid, print_chat,
							"[%s] %s recieved %d for supporting the bomb defusing effort.",	MOD, sName, temp)
					}
				}
				displaylevel(targetid, 1)
			}
		}
	}
	return;
}

/* Hostage Kill Event */
public event_hostkill(id)
{
	if(!warcraft3)
		return 0;

	new temp, sName[32]
	get_user_name(id, sName, 31)
	if(get_cvar_num("FT_hostage_kill_xp"))
	{
		if(get_cvar_num(OBJECTIVES))
		{
			temp = xpgiven_lev[p_level[id]]
			playerxp[id] -= temp
			client_print(id,print_chat, "[%s] %s lost %d XP for killing a hostage.", MOD, sName, temp)
		}
		displaylevel(id, 1)
	}
	return 0;
}