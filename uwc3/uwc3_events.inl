
/** *************************************************************************
*** AMXX Plugin:   Ultimate Warcraft3 (UWC3)
*** Plugin Author: K2mia
*** UWC3 Module:   Event Handlers
*** Date:          May 25, 2004
*** Last Update:   September 28, 2004
*
*  Module: Event Handlers
*  Subroutines for handling events not handled in more specific modules
*----------------------------------------------------------------------------
*
*  UWC3 is written exclusively for AMX Mod X
*
*  Ultimate Warcraft3 Dev. Team
*  ------------------------------
*   Small scripting:  K2mia ( Andrew Cowan ) < admin@mudconnect.com >
*   Graphics:         steve french < garbageweed@hotmail.com >
*   Website Design:   Jim Rhoades < jim@radzone.org >
*
*  str_break() routine provided by BAILOPAN from AMXX0.20 to replace parse()
*  ultimate_decoy() code derived from code for Beyonder superhero (Freecode/Asskicr)
*    some decoy concepts from AMXX forums posts by jjkiller
*
****************************************************************************/


// **************************************************************************
// BEGIN Event Handler Subroutine Declarations
// **************************************************************************


// **************************************************************************
// BEGIN game_commencing subroutine
// The game is beginning
// **************************************************************************
public game_commencing()
{
	new players[32], numplayers
	get_players(players, numplayers)

	for(new i = 0; i < numplayers; i++)
		game_commence[players[i]] = true
}
// **************************************************************************
// END game_commencing subroutine
// **************************************************************************


// **************************************************************************
// BEGIN restart_round subroutine
// Called when round restarts
// **************************************************************************
public restart_round()
{
	if(!warcraft3)
		return;

	new players[32], numplayers, id
	get_players(players, numplayers)

	//write_all()        // For saving xp info!
	//client_print(0, print_console, "UWC3-DEBUG: In restart_round!")

	for(new i = 0; i < numplayers; i++)
	{
		// Iterate over all players and initialize player data
		id = players[i]

		playeritem[id] = 0
		playeritem2[id] = 0
		he[id] = 0
		threwnapalm[id] = 0
		flashg[id] = 0
		diedlastround[id] = false

		if(!get_cvar_num("mp_savexp"))
		{
			p_level[id]=0          // Set player to level 0

			#if defined DEBUG
			if (get_cvar_num("UW_DEBUG_code"))
				playerxp[id] = DEBUG_STARTXP // Set player starting XP to DEBUG value
			else
				playerxp[id] = 0             // Set player starting XP to 0
			#else
			playerxp[id] = 0
			#endif

			for(new j = 0; j < MAX_SKILLS; j++)
				p_skills[id][j] = 0   // Set all skills to 0
		}
	}
}
// **************************************************************************
// END restart_round subroutine
// **************************************************************************


// **************************************************************************
// BEGIN new_round subroutine
// Called when a new round begins
// **************************************************************************
public new_round(id)
{
	if(!warcraft3 || id < 1 || id > g_MAXPLAYERS || !is_connected[id])
		return

	new origin[3]
	new name[32]

	bombstate = BOMB_NOTPLANTED

	if(game_commence[id])
	{
		playeritem[id] = 0
		playeritem2[id] = 0
		diedlastround[id] = false
		game_commence[id] = false

		// Reset spawn sites internally
		//client_print(0, print_console, "Resetting Spawn sites")
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			Tspawnpts[i] = {0, 0, 0}
			CTspawnpts[i] = {0, 0, 0}
		}
	}

	#if defined DEBUG
	if(get_cvar_num("UW_DEBUG") == 5)
	{
		// Added special debug info dump
		new mapname[32]
		get_mapname(mapname,31)
		new pname[32]
		get_user_name(id, pname, 31)

		new skillcount = get_skillcount(id)
		new acount = get_attribcount(id)
		new rcount = get_resistcount(id)

		new apts = get_availattribpts(id)
		new rpts = get_availresistpts(id)

		log_amx( "DEBUG #1: Mapname=(%s) Player=(%s) Level=(%d) #Skills=(%d) XP=(%d)",
			mapname, pname, p_level[id], skillcount, playerxp[id] )

		log_amx( "DEBUG #2: Server Max_Attribs=(%d) Apts_spent=(%d) Apts_left=(%d)",
			ATTRIB_MAX_PTS, acount, apts)

		log_amx( "DEBUG #3: Server Max_Resists=(%d) Rpts_spent=(%d) Rpts_left=(%d)",
			RESIST_MAX_PTS, rcount, rpts)
	}
	#endif

	// [09-13-04] Fix for Voodoo missing end_round task check - K2mia
	if(task_exists(4500+id))
	{
		remove_task(4500+id) // Resets godmode

		//client_print(0, print_console, "new_round: task exists 4500 + %d", id)

		set_user_health_log(id, maxhealth[id], true, "reset_godmode() from new_round()")

		hasgodmode[id] = false

		set_hudmessage(0, 100, 0, 0.05, 0.75, 2, 0.02, 10.0, 0.01, 0.1, 2)
		show_hudmessage(id, "Your Voodoo has been reset")
	}

	// [09-24-04] Added check for # skill points being in sync with player level - K2mia
	if ( get_cvar_num("UW_round_check") )
	{
		new pname[32]
		get_user_name(id, pname, 31)

		new skillcount = get_skillcount( id )

		if(skillcount > p_level[id])
		{
			client_print(id, print_chat,
				"[%s] You have spent too many skillpoints, skills reset forced.", MOD)

			log_amx("TOO MANY SKILLPOINTS: (%s) level=(%d) skillcount=(%d)",
				pname, p_level[id], skillcount)

			for(new j = 0; j < MAX_SKILLS; j++)
				p_skills[id][j] = 0 // Set all skills to 0

			ultlearned[id] = 0
			displaylevel(id, 3)

			if(get_cvar_num("mp_sql"))
				sqlwrite_skills_id( id, -1, 1)
			else
				write_skills_id( id, 1 )
		}
	}
	// [09-24-04] End of Added check for # skill points being in sync with player level

	// Free this mole spawn position for use this round
	MOLEspawnpts[id] = 0

	if(get_cvar_num("FT_competitive") && !buycalled)
	{
		set_task(get_cvar_float("mp_buytime") * 60.0, "set_buytime", 899)
		buycalled = true
	}

	icon_controller(id, ICON_HIDE)
	deathmessageshown[id] = false
	to_be_revived[id] = false
	vengeance_state[id] = VENG_READY

	if(get_cvar_num("UW_dmgxpbonus"))
		for (new i=0; i < g_MAXPLAYERS; i++)
			damageto[id][i] = -1

	if(get_cvar_num("mp_savexp") && xpreadytoload[id])
	{
		if(get_cvar_num("mp_sql"))
		{
			sqlget_xp_id(id)
			sqlget_enh_id(id)
			sqlget_skills_id(id)
		}
		else
		{
			get_xp_id(id)
			get_skills_id(id, 1)
			get_enh_id(id)
		}

		xpreadytoload[id] = 0
	}

	if(!get_cvar_num("mp_savexp"))
	{
		set_hudmessage(200, 100, 0, -1.0, 0.35, 0, 1.0, 15.0, 0.1, 0.2, 2)
		show_hudmessage(id, "Server Mode: Fast XP -- Long Term XP is Disabled")
	}

	#if defined DEBUG
	if(get_cvar_num("UW_DEBUG_code"))
		cs_set_user_money(id, DEBUG_CASH)
	#endif

	// Store spawn pt for use with mole
	get_user_name(id, name, 31)
	get_user_origin(id, origin)

	if(get_user_team(id) == TEAM_T)
	{
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			//if((Tspawnpts[i][0] + Tspawnpts[i][1] + Tspawnpts[i][2]) == 0){
			if(!(Tspawnpts[i][0] + Tspawnpts[i][1] + Tspawnpts[i][2]))
			{
				Tspawnpts[i] = origin
				break
			}
		}
	}
	else
	{
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			//if((CTspawnpts[i][0] + CTspawnpts[i][1] + CTspawnpts[i][2]) == 0){
			if(!(CTspawnpts[i][0] + CTspawnpts[i][1] + CTspawnpts[i][2]))
			{
				CTspawnpts[i] = origin
				break
			}
		}
	}

	//client_print(0, print_console, "%s has spawned a %s at < %d %d %d>", name, ( (get_user_team(id) == TEAM_T) ? "Terrorist" : "CT" ), origin[0], origin[1], origin[2])


	if(playerhasspawned[id])
	{
		if(p_skills[id][SKILLIDX_SERPWARD])
		{
			// Serpent Ward
			serpents[id] = p_serpent[p_skills[id][SKILLIDX_SERPWARD] - 1]
			if(USE_ENH && (p_attribs[id][ATTRIBIDX_INT] >= INT_SERPENT_LEVEL))
			{
				serpents[id] += INT_SERPENT_BONUS

				#if defined DEBUG
				if(get_cvar_num("UW_DEBUG") == 1)
				{
					client_print(id, print_console,
						"[%s DEBUG] Intellect-SerpWard #=(%d)", MOD, serpents[id])
				}
				#endif
			}
		}
		else
		{
			serpents[id] = 0
		}

		if(!freezetime && !ultimateused[id])
			icon_controller(id,ICON_SHOW)

		if(stunned[id] || slowed[id])
			set_user_maxspeed(id,240.0)

		if(playeritem2[id] != MOLE)
			playeritem2[id] = 0

		playeritem[id] = 0
		changeskin(id, 1)
		displaylevel(id, 3)
		return
	}

	if(hasmole[id] && playeritem2[id] != MOLE)
		playeritem2[id] = MOLE

	u_delay[id] = false

	if(get_cvar_num("FT_ultimatedelay"))
	{
		u_delay[id] = true
		if(!task_exists(UTLIMATE_DELAY + id))
			set_task(get_cvar_float("FT_ultimatedelay"), "udelay_cooldown", UTLIMATE_DELAY + id)
	}


	if (task_exists(357)) // Stops the medic sprite display
		remove_task(357)

	if (task_exists(358)) // Stops the monitor_players routine
		remove_task(358)

	if (task_exists(200+id)) // Stops any ultimates from continuing when the new round starts
		remove_task(200+id)

	if (task_exists(id+500)) // Stop any cooldowns in effect
		remove_task(500+id)

	if (task_exists(id+645)) // Remove bomb count down
		remove_task(645+id)


	// [09-03-04] - Display the medic alert sprites - K2mia
	if(get_cvar_num("UW_medicalerts") == 2)
		set_task(1.0, "draw_medicspr", 357)

	// [09-24-04] - Added check for insane amounts of player health
	set_task(1.0, "monitor_players", 358)

	buytime = true

	new parm[2]
	parm[0] = id

	if(get_cvar_num("FT_blinkstartdisabled"))
	{
		blinkdelayed[id] = true

		set_task(get_cvar_float("FT_blink_delay"), "cooldown2", 50 + id, parm, 1)
	}

	if(get_cvar_num("FT_warn_suicide"))
		suicideAttempt[id] = 0

	if(is_connected[id])
	{
		set_user_maxspeed(id, 0.0)

		if(playeritem2[id] == HELM)
			bImmuneHeadshots[id] = true;
	}

	set_task(0.1, "weapon_controller", 10091, parm, 2) // Weapon Reincarnation

	init_pdata(id, 1)		// Perform a Limited init of player data

	endround = false		// New round, reset endround state
	hostagesaver = 0		// Reset hostagesaver state

	// Checks skills
	check_skills(id, true)	// Check skill affects, set user health
	check_fan(id)		// Chec for Fan of Knives


	if(get_cvar_num("FT_reset_skills"))
	{
		if(resetskill[id])
		{
			for(new j = 0; j < MAX_SKILLS; j++)
				p_skills[id][j] = 0 // Set all skills to 0

			p_level[id] = 0
			ultlearned[id] = 0
			resetskill[id] = false
			displaylevel(id, 3)
      	}

		// [08-04-04] - Added check for resetting attribs/resists - K2mia
		if(resetattrib[id])
		{
			for(new j = 0; j < MAX_ATTRIBS; j++)
				p_attribs[id][j] = ATTRIB_BASE // Set all attributes to the default

			resetattrib[id] = false
		}
		if(resetresist[id])
		{
			for(new j = 0; j < MAX_RESISTS; j++)
				p_resists[id][j] = 0 // Set all resistances to 0

			resetresist[id] = false
		}
		if(reloadskill[id])
		{
			for(new j = 0; j < MAX_SKILLS; j++)
				p_skills[id][j]=0   // Set all skills to 0

			ultlearned[id] = 0
			reloadskill[id] = false

			get_skills_id(id, 1)

			displaylevel(id, 3)
		}

		// Reset dropskill for id to allow for selecting new skills - Marticus
		if(dropskill[id])
			dropskill[id] = false

	}

	vengeance_used[id] = 0

	new acount = get_attribcount(id)
	new apts = get_availattribpts(id)
	new rcount = get_resistcount(id)
	new rpts = get_availresistpts(id)

	if((apts > 0) && (acount < ((MAX_ATTRIBS-1) * (ATTRIB_MAX_VALUE - ATTRIB_BASE))) && (acount < ATTRIB_MAX_PTS))
		select_attrib(id)
	else if((rpts > 0) && (rcount < ((MAX_RESISTS-1) * RESIST_MAX_VALUE)) && (rcount < RESIST_MAX_PTS))
		select_resist(id)
	else
	{
		// Determine if player has skills that can be learned
		new skillptsavail = get_availskillpts( id )

		if(skillptsavail)
			select_skill(id, 1)
	}
}
// **************************************************************************
// END new_round subroutine
// **************************************************************************
public start_round()
{
   if(!freezecalled)
   {
      freezetime = 1
      freezecalled = 1
   }
}

/*
 * START endround/Twin/CTwin subroutine
 * Handler for the endround/Twin/CTwin team winning Rewritten by teame06 01-30=2006 to be more optimize
 */
public end_round()
{
	if(warcraft3)
	{
		new CTnum = count_team(TEAM_CT)
		new Tnum = count_team(TEAM_T)
		new a = 0, players[32], numberofplayers, id

		get_players(players, numberofplayers)

		//client_print(0, print_console, "DEBUG: MIH T_win() CTnum=(%d) Tnum=(%d)", CTnum, Tnum)
		//log_amx("DEBUG: MIH T_win() CTnum=(%d) Tnum=(%d)", CTnum, Tnum)
		if((Tnum+CTnum) > get_cvar_num("FT_min_b4_XP"))
		{
			if(Tnum > 0 && CTnum > 0)
			{
				new temp = 0, sName[32]
				new objectives = get_cvar_num("FT_objectives"), team = -1
				new param[13]

				read_data(2, param, 12)

				if(param[7] == 't')
					team = TEAM_T
				else if(param[7] == 'c')
					team = TEAM_CT

				for(a = 0; a < numberofplayers; a++)
				{
					id = players[a]

					if(!is_connected[id])
						return;

					if(get_user_team(id) == team)
					{
						temp = (ROUNDXP + xpgiven_lev[p_level[id]])
						if(objectives)
						{
							get_user_name(id, sName, 31)
							client_print(id, print_chat,
								"[%s] %s recieved %d XP for winning the round", MOD, sName, temp)
						}
						playerxp[id] += temp
						displaylevel(id, 3)
					}
				}
			}
		}

		endround = true
		#if defined COMPETITIVE
		buycalled = false
		#endif

		if(task_exists(899))
			remove_task(899)

		for(a = 0; a < MAX_PLAYERS; a++)
			spawnPointsused[a]=false

		new parm[2]
		for(a = 0; a < numberofplayers; a++)
		{
			id = players[a]

			if(task_exists(600 + id))     // Removes walker (undead changing speed)
				remove_task(600 + id)

			if(task_exists(id + 90))     // Removes any current healing processes
				remove_task(id + 90)

			if(task_exists(id + 1100))   // Removes any current con healing proocesses
				remove_task(id + 1100)

			if(task_exists(id + 1200))   // Removes any leather/steel skin proocesses
				remove_task(id + 1200)

			if(task_exists(9000 + id))   // Removes undead flashing process
				remove_task(9000 + id)

			if(task_exists(50 + id))     // Remove any current cooldowns
				remove_task(50 + id)

			if(task_exists(1000 + id)) // Remove any reset_maxspeeds occuring (a person could to move during freezetime
			{
				remove_task(1000 + id)
				parm[0] = id
				parm[1] = 0
				reset_maxspeed(parm)
			}

			if(task_exists(4500 + id)) // Resets godmode
			{
				remove_task(4500+id)
				parm[0] = id
				reset_godmode(parm)
			}

			if(is_user_alive(id))
				saveweapons(id)

			spawnedfromitem[id]=false
			playerhasspawned[id]=false

			if(playeritem2[id] != CHAMELEON)
				changeskin(id,1)

			he[id]=0
			threwnapalm[id]=0
			carrioncount[id]=2
			shadowcount[id]=2
			flashg[id]=0
			ismole[id]=false
			//itemsrunning[id]=true
		}

		freezecalled = 0
		pheonixexistsT=0
		pheonixexistsCT=0

		g_roundCounter++
		if(g_roundCounter % SAVEFREQUENCY == 0 && get_cvar_num("mp_savexp"))
		{
			set_task(2.5, "write_all")
			client_print(0, print_console, "write_all (%s)^n", get_cvar_num("usenewway") ? "new" : "old")
		}

	}
}
/*
 * END endround/Twin/CTwin subroutine
 */

// **************************************************************************
// BEGIN Target_Bombed subroutine
// Handler for the Target_Bombed event
// **************************************************************************
public Target_Bombed()
{
	new players[32], numberofplayers
	get_players(players, numberofplayers)
	new i

	for(i = 0; i < numberofplayers; i++)
		set_task(0.1, "check_alive", players[i])
}
// **************************************************************************
// END Target_Bombed subroutine
// **************************************************************************

// **************************************************************************
// BEGIN nade_thrown and hethrowevent subroutines
// Routines for handling nades in Warcraft
// **************************************************************************
public nade_thrown(id)
{
	if(!id)
		return;

   //new parm[2]
   //parm[0] = read_data(1)

/*
   //client_print(parm[0], print_console, "[DEBUG] you threw a nade")
   set_task(0.1, "hethrowevent",parm[0])

   return PLUGIN_HANDLED
}

public hethrowevent(id)
{
	*/
	new szModel[64], grenadeid = get_grenade(id)

	// [10-05-04] - Check for Valid grenade entity
	if(!is_valid_ent(grenadeid))
		return;

	entity_get_string(grenadeid, EV_SZ_model, szModel, 63)

	if(equal(szModel, "models/w_hegrenade.mdl"))
	{
		new rgb[3]

		if (p_skills[id][SKILLIDX_CRITGREN] && p_skills[id][SKILLIDX_NAPALM])
			rgb = {255, 32, 32}
		else if (p_skills[id][SKILLIDX_CRITGREN])
			rgb = {32, 255, 32}
		else if (p_skills[id][SKILLIDX_NAPALM])
			rgb = {32, 32, 255}

		if(p_skills[id][SKILLIDX_CRITGREN] || p_skills[id][SKILLIDX_NAPALM])
		{
			if((playeritem2[id] == INFERNO && get_cvar_num("FT_glove_orc_damage")) || (playeritem2[id] != INFERNO))
			{
				message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
				write_byte( TE_BEAMFOLLOW )
				write_short(grenadeid) // entity
				write_short(m_iTrail)  // model
				write_byte( 20 )       // life
				write_byte( 10 )       // width
				write_byte( rgb[0] )   // r, g, b
				write_byte( rgb[1] )   // r, g, b
				write_byte( rgb[2] )   // r, g, b
				write_byte( 210 )      // Brightness
				message_end() // move PHS/PVS data sending here (SEND_ALL,SEND_PVS,SEND_PHS)
			}
		}
	}
}
// **************************************************************************
// END nade_thrown and hethrowevent subroutines
// **************************************************************************


// **************************************************************************
// BEGIN event_player_action subroutine
// Handler for player events (bomb, hostages, etc)
// **************************************************************************
public event_player_action(){

   new sArg[MAX_VAR_LENGTH], sAction[MAX_VAR_LENGTH]
   new sName[MAX_NAME_LENGTH]
   new id, iUserId

   if (!warcraft3)
      return PLUGIN_CONTINUE

   new CTnum = count_team(TEAM_CT)
   new Tnum = count_team(TEAM_T)

   //client_print(0, print_console, "DEBUG: MIH event_player_action() CTnum=(%d) Tnum=(%d)", CTnum, Tnum)
   //log_amx("DEBUG: MIH event_player_action() CTnum=(%d) Tnum=(%d)", CTnum, Tnum)

   if((Tnum+CTnum) < get_cvar_num("FT_min_b4_XP"))
      return 0;

   if(Tnum < 1 && CTnum < 1)
      return 0;

   read_logargv(0,sArg,MAX_VAR_LENGTH)
   read_logargv(2,sAction,MAX_VAR_LENGTH)
   parse_loguser(sArg,sName,MAX_NAME_LENGTH,iUserId)

   id = find_player("k",iUserId)

   //client_print(0, print_console, "DEBUG: MIH event_player_action() id=(%d) iUid=(%d)  sAction=(%s)",id, iUserId, sAction )
   //log_amx("DEBUG: MIH event_player_action() id=(%d) iUid=(%d) sAction=(%s)",id, sAction, iUserId )


   if(!id) // Prevents the server from gaining XP
      return 0;

   new temp = 0 // Used for XP gain from objectives

   /*if (equal(sAction,"Spawned_With_The_Bomb")) {
      // Spawned with the bomb
      bombCarrier = id

      if(get_cvar_num("FT_bomb_event_bonus")){

         if (get_cvar_num("FT_objectives")){
            temp = xpgiven_lev[p_level[id]]
            playerxp[id] += temp
            client_print(id, print_chat,
               "[%s] %s recieved %d XP for spawning with the bomb.",MOD,sName,temp)
         }

         displaylevel(id, 1)
      }
   }else */
   if (equal(sAction,"Touched_A_Hostage")) {
      // Hostage touched
      if(get_cvar_num("FT_hostage_touch_bonus")){
         if (get_cvar_num("FT_objectives")){
            temp = xpgiven_lev[p_level[id]]
            playerxp[id]+=  temp
            client_print(id, print_chat,
               "[%s] %s recieved %d XP for beginning to escort the hostage to safety.",
               MOD,sName,temp)
         }

         displaylevel(id, 1)
         hostagesaver = id
      }
   }else if (equal(sAction,"Rescued_A_Hostage")) {
      // Hostage rescued
      new origin[3]
      new teamname[32]
      new player[32]
      new numplayers
      new targetorigin[3]
      new targetid

      get_user_origin(id, origin)
      get_user_team(id, teamname, 31)
      get_players(player, numplayers)


      if (get_cvar_num("FT_objectives")){
         temp = 1 * xpgiven_lev[p_level[id]]
         playerxp[id]+=  temp
         client_print(id, print_chat,
            "[%s] %s is given %d XP for escorting a hostage to safety.",MOD,sName,temp)
      }

      // Supporters and self
      for (new i=0; i<numplayers; ++i){
         targetid=player[i]

         if (get_user_team(targetid) != TEAM_CT) continue

         get_user_origin(targetid, targetorigin)

         // [09-27-04] Modified playerlist so now have to check for living status - K2mia
         if ( is_user_alive(targetid) &&
              (get_distance(origin, targetorigin)<=get_cvar_num("FT_xp_radius")) )
         {
            if (get_cvar_num("FT_objectives")){
               temp = HOSTAGEXP + xpgiven_lev[p_level[targetid]]
               playerxp[targetid]+=  temp
               get_user_name(targetid,sName,31)
               client_print(targetid, print_chat,
                  "[%s] %s recieved %d for supporting the hostage run.",MOD,sName,temp)
            }
         }
      }
      displaylevel(targetid, 1)
   }else if (equal(sAction,"Became_VIP")) {
      // VIP spawn
      vipID = id
      if(get_cvar_num("FT_VIP_spawn_bonus")){
         if (get_cvar_num("FT_objectives")){
            temp = xpgiven_lev[p_level[id]]
            playerxp[id]+=  temp
            client_print(id,print_chat, "[%s] %s recieved %d XP for being so important.",
               MOD,sName,temp)
         }

         displaylevel(id, 1)
      }
   }else if (equal(sAction,"Assassinated_The_VIP")) {
      // VIP assassinated
      new sNameVIP[MAX_NAME_LENGTH]
      get_user_name( vipID,sNameVIP,MAX_NAME_LENGTH)

      if(get_cvar_num("FT_VIP_kill_bonus")){
         if (get_cvar_num("FT_objectives")){
            temp = VIP_KILL_BONUS + xpgiven_lev[p_level[id]]
            playerxp[id]+=  temp
            client_print(id,print_chat, "[%s] %s gained %d XP for assinating %s the VIP.",
               MOD,sName,sNameVIP)
         }

         displaylevel(id, 1)
      }
   }else if (equal(sAction,"Escaped_As_VIP")) {
      // VIP escaped
      new origin[3]
      new teamname[32]
      new player[32]
      new numplayers
      new targetorigin[3]
      new targetid

      get_user_origin(id, origin)
      get_user_team(id, teamname, 31)
      get_players(player, numplayers)

      new nName[31]
      if (get_cvar_num("FT_objectives")){
         temp = xpgiven_lev[p_level[id]]
         playerxp[id]+=  temp
         client_print(id,print_chat, "[%s] %s gained %d XP for evading assasination.",
            MOD,sName,temp)
      }

      for (new i=0; i<numplayers; ++i){
         targetid=player[i]

         if (get_user_team(targetid) != TEAM_CT) continue

         get_user_origin(targetid, targetorigin)

         // [09-27-04] Added check for alive status of player - K2mia
         if ( is_user_alive(targetid) &&
              (get_distance(origin, targetorigin)<=get_cvar_num("FT_xp_radius")) &&
              (targetid!=id) )
         {
            if (get_cvar_num("FT_objectives")){
               temp = VIP_ESCAPE_BONUS + xpgiven_lev[p_level[targetid]]
               playerxp[targetid] += temp
               get_user_name(targetid,nName,31)
               client_print(targetid, print_chat,
                  "[%s] %s recieved %d for helping the %s escape.",MOD,nName,temp,sName)
            }
         }
         displaylevel(targetid, 1)
      }
   }
   return 0 // why ??
}
// **************************************************************************
// END event_player_action subroutine
// **************************************************************************



// **************************************************************************
// END Event Handler Subroutine Declarations
// **************************************************************************


