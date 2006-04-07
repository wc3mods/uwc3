
/** *************************************************************************
*** AMXX Plugin:   Ultimate Warcraft3 (UWC3)
*** Plugin Author: K2mia
*** UWC3 Module:   Item Handlers
*** Date:          May 25, 2004
*** Last Update:   September 27, 2004
*
*  Module: Item Handlers
*  Subroutines for item-related activity (shopitems, weapons, equip, etc)
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
// BEGIN item_menu subroutine
// Display the Item menu
// **************************************************************************
item_menu(id){

   new pos = 0, i, menu_body[512], menu_items[4][32]
   new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<8)|(1<<9)

   menu_items[0]= "Shopmenu 1"
   menu_items[1]= "Shopmenu 2"
   menu_items[2]= "Show Shopmenu Information"
   menu_items[3]= "Show Shopmenu 2 Information"

   pos += format(menu_body[pos], 511-pos, "\yItem Options^n^n")
   for (i = 0; i<4; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n^n\w9. Back")
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")
   show_menu(id,keys,menu_body,-1)

   return PLUGIN_CONTINUE
}
// **************************************************************************
// END item_menu subroutine
// **************************************************************************


// **************************************************************************
// BEGIN do_itemmenu subroutine
// Handler for the items main menu
// **************************************************************************
public do_itemmenu(id,key){
   switch (key){
      case 0: shopmenu(id)
      case 1: shopmenu2(id)
      case 2: items_info(id)
      case 3: items_info2(id)
      case 8: main_menu(id)
      default: return PLUGIN_HANDLED
   }
   return PLUGIN_HANDLED
}
// **************************************************************************
// END do_itemmenu subroutine
// **************************************************************************

// **************************************************************************
// BEGIN shopmenu subroutine
// Display the shopmenu
// **************************************************************************
public shopmenu(id){

   if (warcraft3==false)
      return PLUGIN_CONTINUE

   if(get_cvar_num("FT_competitive")){
      if (!is_user_alive(id)){
         client_print(id,print_center,"You may not buy items when you are dead.")
         return PLUGIN_HANDLED
      }else if (!buytime){
         new Float:thetime = get_cvar_float("mp_buytime")*60.0
         client_print(id,print_center,"%0.0f seconds have passed. You can't buy anthing now.",thetime)
         return PLUGIN_HANDLED
      }else if(!isBuyzone[id]){
         client_print(id,print_center,"You must be in the buyzone to purchase an item")
         return PLUGIN_HANDLED
      }
   }

   new pos = 0
   new keys = (1<<0|1<<1|1<<2|1<<3|1<<4|1<<5|1<<6|1<<7|1<<8|1<<9)
   new menu_body[512]

   pos += format(menu_body[pos], 511-pos, "\yBuy Item\R$   Cost^n^n")
   for (new i = 0; i<9; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s\y\R%d^n",i+1,itemname[i],itemcost[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")


   show_menu(id, keys, menu_body, -1)

   return PLUGIN_HANDLED

}
// **************************************************************************
// END shopmenu subroutine
// **************************************************************************


// **************************************************************************
// BEGIN shopmenu2 subroutine
// Display the shopmenu
// **************************************************************************
public shopmenu2(id){

   if (warcraft3==false)
      return PLUGIN_CONTINUE

   if(get_cvar_num("FT_competitive")){
      if (!is_user_alive(id)){
         client_print(id,print_center,"You may not buy items when you are dead.")
         return PLUGIN_HANDLED
      }else if (!buytime){
         new Float:thetime = get_cvar_float("mp_buytime")*60.0
         client_print(id,print_center,"%0.0f seconds have passed. You can't buy anthing now.",thetime)
         return PLUGIN_HANDLED
      }else if(!isBuyzone[id]){
         client_print(id,print_center,"You must be in the buyzone to purchase an item")
         return PLUGIN_HANDLED
      }
   }

   new pos = 0
   new keys = (1<<0|1<<1|1<<2|1<<3|1<<4|1<<5|1<<6|1<<7|1<<8|1<<9)
   new menu_body[512]

   pos += format(menu_body[pos], 511-pos, "\yBuy An Item\R$   Cost^n^n")
   for (new i = 0; i<9; i++){
      pos += format(menu_body[pos], 511-pos, "\w%d. %s\y\R%d^n",i+1,itemname2[i],itemcost2[i])
   }
   pos += format(menu_body[pos], 511-pos, "^n\w0. Exit")


   show_menu(id, keys, menu_body, -1)

   return PLUGIN_HANDLED

}
// **************************************************************************
// END shopmenu2 subroutine
// **************************************************************************


// **************************************************************************
// BEGIN buy_item subroutine
// Handler for the shopmenu
// **************************************************************************
public buy_item(id,key){
   if (warcraft3==false)
      return PLUGIN_CONTINUE

   if(get_cvar_num("FT_competitive")){
      if (!is_user_alive(id)){
         client_print(id,print_center,"You may not buy items when you are dead.")
         return PLUGIN_HANDLED
      }else if(!buytime){
         new Float:thetime = get_cvar_float("mp_buytime")*60.0
         client_print(id, print_center,
            "%0.0f seconds have passed. You can't buy anthing now.",thetime)
         return PLUGIN_HANDLED
      }else if(!isBuyzone[id]){
         client_print(id,print_center,"You must be in the buyzone to purchase an item")
         return PLUGIN_HANDLED
      }
   }

   if ( !is_user_alive(id) &&
        (key==7 || key==6 || key==5 || key==4 || key==3 || key==2 || key==1))
   {
      client_print(id,print_center,"You may not purchase that item when dead")
      return PLUGIN_CONTINUE
   }

   new usermoney = cs_get_user_money(id)

   xpgiven = xpgiven_lev[p_level[id]]

   if (key==9)
      return PLUGIN_HANDLED
   else if(key==playeritem[id]-1 && key!=TOME-1){
      client_print(id,print_center,"You already own that item")
      return PLUGIN_HANDLED
   }else if (usermoney<itemcost[key]){
      client_print(id,print_center,"You have insufficient funds!")
      return PLUGIN_HANDLED
   }else if (key==TOME-1){
      cs_set_user_money(id,usermoney-itemcost[key],1)
      // Give XP bonus for buying Tome of Experience
      playerxp[id]+=(XPBONUS+xpgiven)

      if (file_exists("sound/warcraft3/tomes.wav")==1)
         emit_sound(id,CHAN_ITEM, "warcraft3/tomes.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

      displaylevel(id,3)
      return PLUGIN_HANDLED
   }else{
      cs_set_user_money(id,usermoney-itemcost[key],1)

      if (playeritem[id]==HEALTH)
         // Remove health bonus after buying new item
         set_user_health_log(id, get_user_health(id)-HEALTHBONUS, true, "buy_item() HEALTH1")

      playeritem[id]=key+1

      if (playeritem[id]==HEALTH){
         // Give health bonus for buying periapt of health
         set_user_health_log(id, get_user_health(id)+HEALTHBONUS, true, "buy_item() HEALTH2")
         #if defined DEBUG
         if ( get_cvar_num("UW_DEBUG") == 5 ){
            new name[32]
            get_user_name(id, name, 31)
            log_amx( "In buy_item() :: [%s] Bought Health Has=(%d) Max=(%d)",
                     name, get_user_health(id), maxhealth[id] )
         }
         #endif
      }
   }

   if (file_exists("sound/warcraft3/pickupitem.wav")==1)
      emit_sound(id,CHAN_ITEM, "warcraft3/pickupitem.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
   displaylevel(id,3)

   return PLUGIN_HANDLED
}
// **************************************************************************
// END buy_item subroutine
// **************************************************************************


// **************************************************************************
// BEGIN buy_item2 subroutine
// Handler for the shopmenu
// **************************************************************************
public buy_item2(id,key){

	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!is_user_alive(id))
	{
		client_print(id,print_center,"You may not buy items when you are dead.")
		return PLUGIN_CONTINUE
	}

	if(get_cvar_num("FT_competitive"))
	{
		if(!buytime)
		{
			new Float:thetime = get_cvar_float("mp_buytime")*60.0
			client_print(id,print_center,"%0.0f seconds have passed. You can't buy anthing now.",thetime)
			return PLUGIN_CONTINUE
		}
		else if(!isBuyzone[id])
		{
			client_print(id,print_center,"You must be in the buyzone to purchase an item")
			return PLUGIN_CONTINUE
		}
	}

	if(key == playeritem2[id] - 1 && key != REGEN - 1)
	{
		client_print(id,print_center,"You already own that item")
		return PLUGIN_HANDLED
	}

	if(key == RESPAWN - 1 && endround)
	{
		client_print(id,print_center,"You may not purchase this after the round has ended")
		return PLUGIN_HANDLED
	}

   	if(key == 9)
     	return PLUGIN_HANDLED
	else if (cs_get_user_money(id)<itemcost2[key])
	{
		client_print(id,print_center,"You have insufficient funds!")
		return PLUGIN_HANDLED
	}
	else
	{
		if(playeritem2[id] == CAT)
		{
			log_amx("%i", id)
			set_user_footsteps(id, 0)
		}
		if(playeritem2[id] == FEATHER)
			set_user_gravity(id, 1.0)
		if(playeritem2[id]==HELM)
			bImmuneHeadshots[id] = false;
		if(playeritem2[id] == CHAMELEON)
			changeskin(id,1)
		if(playeritem2[id] != REGEN)
		{
			if(task_exists(9000 + id))
				remove_task(9000 + id)
			rings[id] = 0
		}
		if(playeritem2[id] != INFERNO)
		{
			if(task_exists(9400 + id))
				remove_task(9400+id)
		}

		playeritem2[id] = key + 1

		if(playeritem2[id] == CHAMELEON && is_user_alive(id))
			changeskin(id,0)

		if (playeritem2[id] == HELM)
			bImmuneHeadshots[id] = true;

		if(playeritem2[id]==CAT)
		{
			log_amx("%i", id)
			set_user_footsteps(id)
		}

		if(playeritem2[id] == FEATHER)
			set_user_gravity(id, 0.35)

		if (playeritem2[id] == RESPAWN && !is_user_alive(id))
		{
			if(get_user_team(id) == TEAM_T || get_user_team(id) == TEAM_CT)
			{
				new parm[2]
				parm[0] = id
				parm[1] = 6
				set_task(0.2, "func_spawn", 2, parm, 2)
				spawnedfromitem[id] = true
				playeritem2[id] = 0
				playeritem[id] = 0
			}
		}

		if(playeritem2[id]==INFERNO)
		{
			if (get_cvar_num("FT_no_gloves_on_ka"))
			{
				if(!givenade)
				{
					client_print(id, print_center,
						"Flaming Gloves of Warmth are restricted on this map")

					playeritem2[id] = 0 // Fix for Gloves bug which still allowed it to be bought

					return PLUGIN_HANDLED
				}
				else
				{
					new parm[2]
					parm[0] = id

					giveheifnothas(parm)
					nadetime[id] = get_cvar_num("FT_glove_timer")
				}
			}
			else
			{
				new parm[2]
				parm[0] = id

				giveheifnothas(parm)
				nadetime[id] = get_cvar_num("FT_glove_timer")
			}
		}
		if(playeritem2[id] == REGEN)
		{
			new parm[2]
			parm[0] = id

			if(rings[id] < 5)
			{
				++rings[id]

				if(!task_exists(9000 + id))
					regenerate(parm)
			}
			else
			{
				client_print(id,print_center,"You may not purchase more than 5 rings")
				return PLUGIN_HANDLED
			}
		}

		if(playeritem2[id] == MOLE)
		{
			hasmole[id] = true
		}
		cs_set_user_money(id, cs_get_user_money(id) - itemcost2[key], 1)
	}

	if(file_exists("sound/warcraft3/pickupitem.wav"))
		emit_sound(id,CHAN_ITEM, "warcraft3/pickupitem.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

	displaylevel(id, 3)

	return PLUGIN_HANDLED

}
// **************************************************************************
// END buy_item2 subroutine
// **************************************************************************


// **************************************************************************
// BEGIN player_giveitems subroutine
// Restore player equipment when dead
// **************************************************************************
public player_giveitems(parm[2])
{
	new id = parm[0]

	if(!is_connected[id])
		return

	if(p_skills[id][SKILLIDX_VENGEANCE] && !spawnedfromitem[id]) // If spawned from vengeance
	{
		vengeance_state[id] = VENG_DONE	// Set vengeance state to completed respawn

		new health = (maxhealth[id] / 2)

		if(health < 50)
			health = 50

		set_user_health_log(id, health, true, "player_giveitems() VENGEANCE1")
	}
	else if (p_skills[id][SKILLIDX_DEVOTION]) // Devotion Aura
		set_user_health_log(id, p_devotion[p_skills[id][SKILLIDX_DEVOTION]-1], true, "player_giveitems() DEVOTION1")
	else // All other cases
		set_user_health_log(id,100, true, "player_giveitems() OTHER1")

	check_blink(id)
	give_items[id] = true

	set_task( 0.1, "weapon_controller", 10091, parm, 2 ) // Give weapons back

	give_item(id, "item_suit")
	give_item(id, "weapon_knife")

	// [08-29-04] Makes sure defusers get reincarnated - K2mia
	if(hasdefuse[id])
		cs_set_user_defuse(id, 1)
		//give_item(id, "item_thighpack")

	// [10-14-04] Makes sure Nightvision get reincarnated - K2mia
	if (hasnvision[id])
		cs_set_user_nvg(id, 1)

	// The following code decides if pistols should be given
	if(!givepistol)
		return

/*	new wpnList[32], number, foundGlock, foundUSP
	get_user_weapons(id, wpnList, number)

	for(new i = 0; i < number; i++)
	{
		switch(wpnList[i])
		{
			case CSW_GLOCK18:
				foundGlock = true
			case CSW_USP:
				foundUSP = true
		}
	}*/

	switch(get_user_team(id))
	{
		case TEAM_T:
		{
			if(!user_has_weapon(id, CSW_GLOCK18))
			{
				give_item(id,"weapon_glock18")
				give_item(id,"ammo_9mm")
				give_item(id,"ammo_9mm")
				give_item(id,"ammo_9mm") // change this to cs_set_user_bpammo
			}
		}
		case TEAM_CT:
		{
			if(!user_has_weapon(id, CSW_USP))
			{
				give_item(id,"weapon_usp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp") // change this to cs_set_user_bpammo
			}
		}
	}
}
// **************************************************************************
// END player_giveitems subroutine
// **************************************************************************


// **************************************************************************
// BEGIN weapon_controller subroutine
// From Warcraft3 FT
// **************************************************************************
public weapon_controller( parm[2] )
{
	if(!warcraft3)
		return

	new id = parm[0]
	new origin[3]
	new weaponid
	new bool:reincarnate
	new iweapons[32], wpname[32], inum

	get_user_weapons(id, iweapons, inum)

	//reincarnate = false /// Why????  This one was probably the mistake

	// Save weapons of people who survived: otherwise the stripper will nab them...
	if(!diedlastround[id])  // LIVED LAST ROUND
		reincarnate = false /// WHYYYYYYYYYYY YYY // This one is not even need because of when the bool is made it default false

	// Equipement & Eligibility Check for Re-Incarnation
	if(diedlastround[id])        // DIED LAST ROUND
	{
		new Float:randomnumber = random_float(0.0, 1.0)

		if(p_skills[id][SKILLIDX_EQREINC] && randomnumber <= p_ankh[p_skills[id][SKILLIDX_EQREINC]-1])
		{
			reincarnate = true
			//client_print(id,print_chat, "skill detected")
		}
		else if(give_items[id])
		{
			reincarnate = true
			give_items[id] = false
		}
		else if(playeritem[id] == ANKH)
			reincarnate = true
	}

	// Equipement Re-Incarnation: Includes pistols. Also, living ppl
	// need to be re-incarnated.
	if (reincarnate)
	{
		if(file_exists("sound/warcraft3/soundpack/reincarnation.wav")) // Make this into a array table for checking file_exists
			emit_sound(id, CHAN_ITEM, "warcraft3/soundpack/reincarnation.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

		// Disarm and Isolate Player Before Re-Incarnation
		get_user_origin(id, origin)
		origin[2] -= 2000
		set_user_origin(id, origin)

		new weapid
		for(new a = 0; a < inum; ++a) // Wonder if it better to strip the user and just give an knife
		{
			weapid = iweapons[a]
			if(iweapons[a] != CSW_C4)
			{
				get_weaponname(weapid, wpname,31)
				engclient_cmd(id, "drop", wpname)
			}
		}

		engclient_cmd(id, "weapon_knife")
		origin[2] += 2005

		if(armorondeath[id])
		{
			if(helmet[id])
				give_item(id, "item_assaultsuit")
			else
				give_item(id, "item_kevlar")

			set_user_armor_log(id, armorondeath[id], true, "armorondeath")
			//set_user_armor(id,armorondeath[id])
		}

		if(playerhasspawned[id] && !reincarnate)
			return

		if(hasdefuse[id])
			give_item(id,"item_thighpack")

		// [10-14-04] Makes sure Nightvision get reincarnated - K2mia
		if(hasnvision[id])
			cs_set_user_nvg(id, 1)


		/*new weapon
		for(new j=0; (j < savednumber[id]) && (j < 32); ++j)
		{
			weapon=savedweapons[id][j]

			if (weapon==CSW_M3||weapon==CSW_XM1014||weapon==CSW_MP5NAVY||
			weapon==CSW_TMP ||weapon==CSW_P90||weapon==CSW_MAC10||weapon==CSW_UMP45||
			weapon==CSW_AK47 ||weapon==CSW_SG552||weapon==CSW_M4A1||weapon==CSW_AUG||
			weapon==CSW_SCOUT ||weapon==CSW_AWP||weapon==CSW_G3SG1||
			weapon==CSW_SG550||weapon==CSW_M249)


		}*/

		if(cs_get_user_shield(id))
				gotShield[id]=false

		//if(gotShield[id])
		//   give_item(id,"weapon_shield")

		for (new j=0; j < savednumber[id]; ++j)
		{
			if(weaponid > 0 && weaponid < MAXWEAPON && weaponid !=5)
			{
				weaponid = savedweapons[id][j]

				give_item(id, weapon_names[weaponid])

				if(MaxBPAmmo[weaponid] != 0)
					cs_set_user_bpammo(id, weaponid, MaxBPAmmo[j]) // Check This later
			}
		}

		if (iglow[id][1] < 1)
		{
			parm[0] = id
			set_task(0.1, "glow_change", 4, parm, 2)
		}

		iglow[id][0] = 0
		iglow[id][1] += 100
		iglow[id][2] = 0
		iglow[id][3] = 0

		if(iglow[id][1] > MAXGLOW)
			iglow[id][1] = MAXGLOW

		Create_Fade(id, 1<<10, 1<<10, 1<<12, 0, 255, 0, iglow[id][1])

		// Restore Re-Incarnated Player to the map
		set_user_origin(id, origin)
	}

	if(is_connected[id])
	{
		if(!cs_get_user_nvg(id))
			hasnvision[id] = false

		if(!cs_get_user_defuse(id)) // This can be changed.
			hasdefuse[id] = false
	}

	item_functions(parm)
}
// **************************************************************************
// END weapon_controller subroutine
// **************************************************************************


// **************************************************************************
// BEGIN item_functions subroutines
// From Warcraft3FT
// **************************************************************************
public item_functions(parm[])
{
	new id = parm[0]

	if(!is_connected[id])
		return

	if(!hasrespawned[id] && hasmole[id]) // [08-19-04] Fixed to prevent respawn Moling from item
		set_task(0.1, "check_spot", id + 12, parm, 2)

	if(diedlastround[id])
	{
		playeritem[id] = 0
		playeritem2[id] = 0
		displaylevel(id, 3)
	}

	diedlastround[id] = false

	if(get_cvar_num("sv_gravity") > 650)
	{
		if(p_skills[id][SKILLIDX_LEVITATION] && playeritem2[id] != FEATHER) // Levitation
		{
			if(get_user_gravity(id) != p_levitation[p_skills[id][SKILLIDX_LEVITATION] - 1])
				set_user_gravity(id, p_levitation[p_skills[id][SKILLIDX_LEVITATION] - 1])
		}
		else if (playeritem2[id] == FEATHER)
			set_user_gravity(id, 0.35)
		else
			set_user_gravity(id, 1.0)
	}
	else
		set_user_gravity(id, 1.0)

	if(playeritem2[id] == CAT)
		set_user_footsteps(id)
	else
		set_user_footsteps(id, 0)

	if(playeritem2[id] != CHAMELEON && skinchanged[id])
		changeskin(id,1)

	if(task_exists(9000 + id) && playeritem2[id] != REGEN)
		remove_task(9000 + id)

	if(playeritem[id] == HEALTH)
		set_user_health_log(id, get_user_health(id) + HEALTHBONUS, true, "item_functions() HEALTH1")

	if(playeritem2[id] == REGEN && !task_exists(9000 + id))
		regenerate(parm)

	bImmuneHeadshots[id] = (playeritem2[id] == HELM) ? true : false

	if(playeritem2[id] == CHAMELEON && is_user_alive(id))
		changeskin(id, 0)
}
// **************************************************************************
// END item_functions subroutines
// **************************************************************************


// **************************************************************************
// BEGIN items subroutine
// Displays the player's items in the bottom center of the screen
// **************************************************************************
public items(parm2[2])
{
	new id = parm2[0]
	new temp2[128]
	new temp3[32]

   /*if ( get_cvar_num("UW_DEBUG") == 5 ){
      new name[32]
      get_user_name( id, name, 31)
      log_amx( "In items() Name: %s ultlearned: %d ultused: %d: ftime: %d",
               name, ultlearned[id], ultimateused[id], freezetime )
   }*/

	if(ultlearned[id] && !ultimateused[id] && !freezetime && !u_delay[id])
		format(temp3, 31, "[ Ultimates Ready ]  ")
	else if (ultlearned[id])
		format(temp3, 31, "[ Ultimates NOT Ready ]  ")


	if(!playeritem[id] && !playeritem2[id])
	{
		if(playeritem2[id] == REGEN && rings[id] > 1)
			format(temp2, 127, "%sMagic Items: %s and %ss x%d", temp3, playeritem[id] ? shortitemname[playeritem[id] -1 ] : "",
				playeritem2[id] ? shortitemname2[playeritem2[id] - 1] : "", rings[id])
		else
			format(temp2, 127, "%sMagic Items: %s and %s", temp3,
				playeritem[id] ? shortitemname[playeritem[id] - 1] : "",
				playeritem2[id] ? shortitemname2[playeritem2[id] - 1] : "") // Need to verify this
	}
	else if(playeritem[id] == 0 && !playeritem2[id])
	{
		if(playeritem2[id] == REGEN && rings[id] > 1)
			format(temp2, 127, "%sMagic Item: %ss x%d", temp3,
				playeritem2[id] ? shortitemname2[playeritem2[id] - 1] : "", rings[id])
		else
			format(temp2, 127, "%sMagic Item: %s", temp3,
				playeritem2[id] ? shortitemname2[playeritem2[id] - 1] : "")
	}
	else if(!playeritem[id] && playeritem2[id] == 0)
		format(temp2, 127, "%sMagic Item: %s",
			temp3, playeritem[id] ? shortitemname[playeritem[id] - 1] : "")
	else
		format(temp2, 127, "%sNo Magic Items (say /shopmenu)", temp3)

	itemsrunning[id] = true

	// Hud message set based on lowres setting
	if(lowres[id])
		set_hudmessage(255, 255, 255, 0.05, 0.75, 2, 0.02, 10.0, 0.01, 0.1, 2)
	else
		set_hudmessage(255, 255, 255, 0.025, 0.80, 2, 0.02, 10.0, 0.01, 0.1, 2)

	//set_hudmessage(255,255,255,-1.0,0.945,2, 1.5, 40.0, 0.02, 5.0, 1)
	//set_hudmessage(255, 255, 255, 0.05, -1.0, 2, 0.02, 10.0, 0.01, 0.1, 2)

	if(is_user_alive(id) && !endround)
		show_hudmessage(id, "%s", temp2)

	set_task(1.5, "items", 4511 + id, parm2, 2)
}
// **************************************************************************
// END items subroutine
// **************************************************************************


// **************************************************************************
// BEGIN giveheifnothas subroutine
// Gives a nade if the player doesn't have one
// **************************************************************************
public giveheifnothas(parm[2])
{
	new id = parm[0]
	//new wpnList[32]
	//new number
	//new bool:foundNade // No reason to have this.

	/*get_user_weapons(id,wpnList,number)
	for (new i = 0;i < number && !foundNade;i++) {
	if (wpnList[i] == CSW_HEGRENADE)
	foundNade = true
	}*/

	if(is_user_alive(id) && !user_has_weapon(id, CSW_HEGRENADE))
	{
		set_hudmessage(0, 100, 0, 0.05, 0.65, 2, 0.02, 10.0, 0.01, 0.1, 2)
		show_hudmessage(id, "Your Flaming Gloves provide you with a Nade!")
		give_item(id,"weapon_hegrenade")
	}

	justgivennade[id] = false
}
// **************************************************************************
// END givenadeifhasnot subroutine
// **************************************************************************


// **************************************************************************
// BEGIN flashbuy and hebuy subroutines
// Routine for handling buying of flash and he grens
// **************************************************************************
public flashbuy(id)
{
   if(!isBuyzone[id])
      return PLUGIN_HANDLED

   return PLUGIN_CONTINUE
}

public hebuy(id)
{
	if(!get_cvar_num("mp_grenadeprotection"))
		return PLUGIN_CONTINUE

	if(!isBuyzone[id])
		return PLUGIN_HANDLED

	if(he[id] > 0)
	{
		client_print(id, print_center, "Only one HE grenade per round")
		return PLUGIN_HANDLED
	}

	he[id]++

	return PLUGIN_CONTINUE
}
// **************************************************************************
// END flashbuy and hebuy subroutines
// **************************************************************************

// **************************************************************************
// BEGIN armor_type subroutine
// From Warcraft3FT
// **************************************************************************
public armor_type(id)
{
	if (!warcraft3)
		return;

	if(read_data(1))
	{
		helmet[id] = true
		hassuit[id] = true
	}
	else
	{
		helmet[id] = false
		hassuit[id] = true
	}

	check_skinbonus(id)	// Check if helmet helps w/ skin bonus
}
// **************************************************************************
// END armor_type subroutine
// **************************************************************************


// **************************************************************************
// BEGIN nadetimer subroutine
// Handler for flaming gloves nade giving
// **************************************************************************
public nadetimer(parm[2])
{
	new id = parm[0]

	set_hudmessage(0, 100, 0, 0.05, 0.65, 2, 0.02, 10.0, 0.01, 0.1, 2)

	if(playeritem2[id] != INFERNO)
	{
		show_hudmessage(id, "")
		return
	}

	if(nadetime[id] != 0)
		show_hudmessage(id, "%d second(s) until your next grenade", nadetime[id])

	nadetime[id] -= 1

	if(!is_user_alive(id))
		show_hudmessage(id, "If you could manage to stay alive I could provide you with more nades!")

	if (nadetime[id] != -1 &&  is_user_alive(id))
		set_task(1.0, "nadetimer", 9400 + id, parm, 2)

	if(nadetime[id] == -1)
	{
		giveheifnothas(parm)
		nadetime[id] = get_cvar_num("FT_glove_timer") // Change to to pcvar
		//return PLUGIN_HANDLED
	}
}
// **************************************************************************
// END nadetimer subroutine
// **************************************************************************


// **************************************************************************
// BEGIN check_weap subroutine
// From Warcraft3FT
// **************************************************************************
public check_weap(id)
{
	new weapon = (read_data(1))

	if(weapon == CSW_FLASHBANG)
		flashg[id]++

	if(cs_get_user_shield(id))
		gotShield[id] = false
}
// **************************************************************************
// END check_weap subroutine
// **************************************************************************



// **************************************************************************
// BEGIN change_weapon subroutine
// Handler for weapon change event
// **************************************************************************
public change_weapon(id)
{
	if(!warcraft3)
		return

	if(!is_connected[id])
		return

	new parm[2]

	parm[0] = id

	// Find out when they last fired
	fLastShotFired[id] = halflife_time();

	if(playeritem2[id] == INFERNO)
	{
		// Gives person a grenade if they weren't just given one
		/*new wpnList[32] = 0
		new number  = 0
		new foundNade = false
		get_user_weapons(id,wpnList,number)
		for(new i = 0; i < number && !foundNade; i++)
		{
			if (wpnList[i] == CSW_HEGRENADE)
			foundNade = true
		}*/

		if(!justgivennade[id] && !user_has_weapon(id, CSW_HEGRENADE))
		{
			justgivennade[id] = true
			nadetimer(parm)
		}
		// End nade giving segment
	}

	new clipamount
	new ammoamount
	new weaponnum
	weaponnum = get_user_weapon(id, clipamount, ammoamount)

	if(p_skills[id][SKILLIDX_INVIS]) // Invisibility
	{
		if(weaponnum == CSW_KNIFE)
		{
			set_user_rendering(id, kRenderFxNone, 0, 0, 0, kRenderTransTexture,p_invisibility[p_skills[id][SKILLIDX_INVIS] - 1] / 2)
			knifeselected[id] = true
		}
		else
		{
			set_user_rendering(id, kRenderFxNone, 0, 0, 0, kRenderTransTexture,p_invisibility[p_skills[id][SKILLIDX_INVIS] - 1])
			knifeselected[id] = false
		}
	}
	else if(playeritem[id] == CLOAK)
	{
		if(weaponnum == CSW_KNIFE)
		{
			set_user_rendering(id, kRenderFxNone, 0, 0, 0, kRenderTransTexture, CLOAKINVISIBILITY / 2)
			knifeselected[id] = true
		}
		else
		{
			set_user_rendering(id, kRenderFxNone, 0, 0, 0, kRenderTransTexture, CLOAKINVISIBILITY)
			knifeselected[id] = false
		}
	}
	else
		set_user_rendering(id)

	unholyspeed(parm)
}
// **************************************************************************
// END change_weapon subroutine
// **************************************************************************


// **************************************************************************
// BEGIN saveweapons subroutine
// Handler for Equip reincarnation skill
// **************************************************************************
public saveweapons(id)
{
	armorondeath[id] = get_user_armor(id)

	new i
	for(i = 0; i < 32; i++) // Equipment Reincarnation
		savedweapons[id][i] = 0 // Change this to new native faster to clear array

	savednumber[id] = 0
	get_user_weapons(id, savedweapons[id], savednumber[id])
}
// **************************************************************************
// END saveweapons subroutine
// **************************************************************************


// **************************************************************************
// BEGIN rings5 subroutine
// Allows a player to buy 5 rings of regeneration
// **************************************************************************
public rings5(id)
{
	if(!is_user_alive(id))
		return PLUGIN_HANDLED

	new usermoney
	new parm[2]
	parm[0] = id

	if(playeritem2[id] == CAT)
	set_user_footsteps(id, 0)

	if(playeritem2[id] == FEATHER)
		set_user_gravity(id, 1.0)

	if(playeritem2[id] == HELM)
		bImmuneHeadshots[id] = false;

	if(playeritem2[id] == CHAMELEON)
		changeskin(id, 1)

	while(rings[id] < 5)
	{
		usermoney = cs_get_user_money(id)

		if(usermoney < itemcost2[REGEN - 1])
			break

		cs_set_user_money(id, usermoney - itemcost2[REGEN - 1], 1)

		rings[id]++
		playeritem2[id] = REGEN

		if(!task_exists(9000 + id))
			regenerate(parm)
	}

	displaylevel(id, 3)

	return PLUGIN_HANDLED
}
// **************************************************************************
// END rings5 subroutine
// **************************************************************************

// Used for immunity to headshots!
#define HITGROUP_HEAD (1 << 1)

public traceline(Float:v1[3], Float:v2[3], noMonsters, pentToSkip)
{
	new iAttacker = pentToSkip;
	new iVictim = get_tr(TR_pHit);
	new iHitZone = (1 << get_tr(TR_iHitgroup));

	if ( iVictim >= 1 && iVictim <= g_MAXPLAYERS && bImmuneHeadshots[iVictim] && iAttacker >= 1 && iAttacker <= g_MAXPLAYERS)
	{
		// If so, then we want to block the shot
		if ( iHitZone & HITGROUP_HEAD )
		{
			set_tr(TR_flFraction, 1.0);

			// Do the check to see if we should flash the screen orange?
			new Float:time = halflife_time();

			if(time - fLastShotFired[iAttacker] < 0.1)
				Create_Fade(iVictim, 1<<10, 1<<10, 1<<12, 250, 164, 20, 150)

			return FMRES_SUPERCEDE;
		}
	}

	return FMRES_IGNORED;
}