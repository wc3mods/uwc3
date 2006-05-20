// Make sure a revive is not wasted when more than one blood mage on a team
public check_for_reviving(victim_id)
{
	new players[32], numberofplayers, y
	get_players(players, numberofplayers)

	if(get_user_team(victim_id) == TEAM_T && pheonixexistsT > 0)
	{
		new id
		for(y = 0; y < numberofplayers; y++)
		{
			id = players[y]

			if(phoenix[id] && !to_be_revived[victim_id] && !endround && id != victim_id && !changingteam[victim_id] && is_user_alive(id) && get_user_team(id) == get_user_team(victim_id) && !is_user_alive(victim_id) && get_user_team(victim_id) != SPEC)
			{
				phoenix[id] = false

				#if !defined CSDM
				new parm[2]
				parm[0] = victim_id
				set_task(0.7, "func_spawn", 2, parm, 2) // Spawns player
				#endif


				new name[32]
				get_user_name(id, name, 31)

				set_hudmessage(200, 100, 0, -0.8, 0.1, 0, 1.0, 5.0, 0.1, 0.2, 2)
				show_hudmessage(victim_id, "%s has revived you", name)

				#if defined SKILL_PRINT_MESSAGE
				client_print(victim_id, print_chat, "[%s] %s has revived you", MOD, name)
				#endif

				get_user_name(victim_id, name, 31)

				// [07-27-04] - Now gives XP bonus for reviving a teammate
				new xpbonus = PHOENIXXP + xpgiven_lev[p_level[id]]

				if(get_cvar_num("mp_savexp"))
					xpbonus += (PHOENIXXP * p_level[id])

				playerxp[id] += xpbonus

				#if defined SKILL_PRINT_MESSAGE
				client_print(id, print_chat,"[%s] Your Phoenix has revived %s <%d XP>", MOD, name, xpbonus)
				#endif

				show_hudmessage(id, "You have revived %s, way to go !", name)

				#if !defined CSDM
				to_be_revived[victim_id] = true
				#endif

				pheonixexistsT--

				return PLUGIN_CONTINUE
			}
		}
	}
	else if(get_user_team(victim_id) == TEAM_CT && pheonixexistsCT > 0)
	{
		for(y = 0; y < numberofplayers; ++y)
		{
			new id = players[y]
			if(phoenix[id] && !to_be_revived[victim_id] && !endround && id!=victim_id && !changingteam[victim_id] && is_user_alive(id) && get_user_team(id)==get_user_team(victim_id) && !is_user_alive(victim_id) && get_user_team(victim_id)!=SPEC)
			{
				phoenix[id] = false
				#if !defined CSDM
				new parm[2]
				parm[0] = victim_id
				set_task(0.7, "func_spawn", 2, parm, 2) // Spawns player
				#endif


				new name[32]
				get_user_name(id, name, 31)
				set_hudmessage(200, 100, 0, -1.6, 0.1, 0, 1.0, 5.0, 0.1, 0.2, 2)
				show_hudmessage(victim_id, "%s has revived you", name)

				#if defined SKILL_PRINT_MESSAGE
				client_print(victim_id, print_chat, "[%s] %s has revived you", MOD, name)
				#endif

				get_user_name(victim_id, name, 31)


				// [07-27-04] - Now gives XP bonus for reviving a teammate
				new xpbonus = PHOENIXXP + xpgiven_lev[p_level[id]]

				if(get_cvar_num("mp_savexp"))
					xpbonus += (PHOENIXXP * p_level[id])

				playerxp[id] += xpbonus

				#if defined SKILL_PRINT_MESSAGE
				client_print(id, print_chat,"[%s] Your Phoenix has revived %s <%d XP>", MOD, name, xpbonus)
				#endif

				show_hudmessage(id, "You have revived %s, way to go !", name)

				#if !defined CSDM
				to_be_revived[victim_id] = true
				#endif

				pheonixexistsCT--

				return PLUGIN_CONTINUE
			}
		}
	}
	return PLUGIN_CONTINUE

}

#if !defined CSDM
// Respawn a dead player
public func_spawn(parm[2])
{
	new id = parm[0]

	if(endround)
		return PLUGIN_CONTINUE

	if(get_user_team(id) == SPEC)
		return PLUGIN_CONTINUE

	playeritem[id] = 0

	if(playeritem2[id] != MOLE)
		playeritem2[id] = 0

	if(is_user_alive(id))
		return PLUGIN_CONTINUE

	if(!is_user_connected(id))
		return PLUGIN_CONTINUE

	new team[32]
	get_user_team(id, team, 31)

	playerhasspawned[id] = true
	deathmessageshown[id] = false

	//icon_controller(id, ICON_SHOW)
	displaylevel(id, 4)

	user_spawn(id)

	slowed[id] = false
	stunned[id] = false

	isburning[id] = 0            // Reset burning from flamethrower status
	isnburning[id] = 0           // Reset burning from napalm status
	ispoisonedss[id] = 0         // Reset poisoned status
	isdiseasedcb[id] = 0         // Reset poisoned status

	// [08-19-04] Fix to prevent MOLE item from being used on Respawn
	hasrespawned[id] = true

	set_task(0.1, "spawn_player", 2, parm, 2)
	set_task(0.3, "spawn_player", 2, parm, 2)
	set_task(0.5, "player_giveitems", 2, parm, 2)

	give_item(id, "weapon_knife")

	new money = cs_get_user_money(id)

	if(money < get_cvar_num("mp_startmoney"))
		cs_set_user_money(id,get_cvar_num("mp_startmoney"),0)

	return PLUGIN_CONTINUE
}


// Respawn a dead player cont.
public spawn_player(parm[2])
{
	new id = parm[0]

	if(!is_user_connected(id))
		return PLUGIN_CONTINUE

	playerhasspawned[id] = true

	user_spawn(id)

	give_item(id, "item_suit")
	give_item(id, "weapon_knife")

	// [08-19-04] Give player 2 seconds Godmode after respawn
	set_user_godmode(id, 1)

	set_task(2.0, "reset_respawngod", 2, parm, 2)

	return PLUGIN_CONTINUE
}
#endif

// Resets a player's respawn godmode
public reset_respawngod(args[])
{
	new id = args[0]

	if(!is_user_connected(id))
		return PLUGIN_CONTINUE

	set_user_godmode(id, 0)

	client_print(id, print_chat, "[%s] Your Respawn grace period has ended, Beware!", MOD)

	return PLUGIN_CONTINUE
}


