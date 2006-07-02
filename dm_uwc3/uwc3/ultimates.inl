// Undead suicide ultimate
public ultimate_wcsuicide(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_SUICIDE])
	{
		client_print(id, print_chat, "You do not know the Suicide Bomber ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"Ultimates are disabled^nfor the first %d seconds",get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("UW_DEBUG") == 6)
	{
		new name [32]
		get_user_name(id, name, 31)
		log_amx("DEBUG: In ultimate_wcsuicide() <%s> invoked Suicide Bomber", name)
	}

	if(is_user_alive(id))
	{
		if(get_cvar_num("FT_warn_suicide"))
		{
			if(suicideAttempt[id])
			{
				user_kill(id, 1)
			}
			else
			{
				new parm[1]
				parm[0]=id

				//icon_controller(id, ICON_FLASH)
				displaylevel(id, 4)
				suicideAttempt[id] = 1

				set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
				show_hudmessage(id,"Suicide Bomb Armed^nPress Again To Detonate")
			}
		}
		else
		{
			user_kill(id,1)
		}
	}
	return PLUGIN_HANDLED
}

public implosion(parm[])
{
	new id = parm[0]

	if(get_user_team(id) == SPEC)
		return PLUGIN_CONTINUE

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(TE_IMPLOSION)
	write_coord(parm[2])
	write_coord(parm[3])
	write_coord(parm[4])
	write_byte(100)
	write_byte(20)
	write_byte(5)
	message_end()

	return PLUGIN_CONTINUE
}

public apacheexplode(parm[]) // Suicide Bomber
{
	new id = parm[0]
	if(get_user_team(id) == SPEC)
		return PLUGIN_CONTINUE

	new origin[3]
	get_user_origin(id, origin)

	// random explosions
	message_begin(MSG_PVS, SVC_TEMPENTITY, origin)
	write_byte(TE_EXPLOSION) // This just makes a dynamic light now
	write_coord(origin[0] + random_num(-100, 100))
	write_coord(origin[1] + random_num(-100, 100))
	write_coord(origin[2] + random_num(-50, 50))
	write_short(g_sModelIndexFireball)
	write_byte(random_num(0, 20) + 20) // scale * 10
	write_byte(12) // framerate
	write_byte(TE_EXPLFLAG_NONE)
	message_end()

	// lots of smoke
	message_begin(MSG_PVS, SVC_TEMPENTITY, origin)
	write_byte(TE_SMOKE)
	write_coord(origin[0] + random_num(-100, 100))
	write_coord(origin[1] + random_num(-100, 100))
	write_coord(origin[2] + random_num(-50, 50))
	write_short(g_sModelIndexSmoke)
	write_byte(60) // scale * 10
	write_byte(10) // framerate
	message_end()

	new players[32]
	new numberofplayers
	new i
	new targetid
	new distancebetween
	new targetorigin[3]
	new damage
	new multiplier

	get_players(players, numberofplayers)

	for(i = 0; i < numberofplayers; ++i)
	{
		targetid = players[i]

		get_user_origin(targetid, targetorigin)
		distancebetween=get_distance(origin, targetorigin)

		if(distancebetween < get_cvar_num("FT_explosion_range") && get_user_team(id) != get_user_team(targetid) && get_user_team(id) != SPEC && playeritem[targetid] != IMMUNITY && !hasblink[targetid])
		{
			if((!temp_immunity[targetid] && is_user_alive(targetid)) && (p_resists[targetid][RESISTIDX_MAGIC] >= RESIST_MAX_VALUE || magic_saving_throw(targetid)))
			{
				//client_print(targetid, print_console, "DEBUG: [Immune to Bomber] magic_resist=(%d)", p_resists[targetid][RESISTIDX_MAGIC])

				temp_immunity[targetid] = true

				#if defined SKILL_PRINT_MESSAGE
				client_print(targetid, print_chat, "[%s] Your magic resistance makes you temporarily immune to a Suicide Bomber's blast waves", MOD)
				#endif

				new iparm[2]

				iparm[0] = targetid // Player with temp immunity
				iparm[1] = 2        // Immunity type 2 - Suicide Bomber

				set_task(1.5, "reset_immunity", 201 + id, iparm, 2)
			}
			else
			{
				//client_print(targetid, print_console, "DEBUG: [Bomber Save Failed] magic_resist=(%d)", p_resists[targetid][RESISTIDX_MAGIC])
			}

			if(!temp_immunity[targetid] && p_resists[targetid][RESISTIDX_MAGIC] < RESIST_MAX_VALUE)
			{
				multiplier = get_cvar_num("FT_explosion_max_damage") * get_cvar_num("FT_explosion_max_damage") / get_cvar_num("FT_explosion_range")
				damage= (get_cvar_num("FT_explosion_range") - distancebetween) * multiplier
				damage = sqrt(damage)

				if(is_user_alive(targetid))
					do_damage(targetid, id, damage, 12, 3, 0, 0, 0)
			}

		}

		if(distancebetween < get_cvar_num("FT_explosion_range"))
		{
			message_begin(MSG_ONE, gmsgShake, {0, 0, 0}, targetid)
			write_short(1<<14)// amplitude
			write_short(1<<13)// duration
			write_short(1<<14)// frequency
			message_end()
		}
	}

	--parm[1]

	if(parm[1] > 0)
		set_task(0.1, "apacheexplode", 33, parm, 2)

	return PLUGIN_CONTINUE
}


public blastcircles(parm[])
{
	new id = parm[0]

	if(get_user_team(id) == SPEC)
		return PLUGIN_CONTINUE

	new origin[3]
	get_user_origin(id,origin)

	// blast circles
	message_begin(MSG_PAS, SVC_TEMPENTITY, origin)
	write_byte(TE_BEAMCYLINDER)
	write_coord(origin[0])
	write_coord(origin[1])
	write_coord(origin[2] - 16)
	write_coord(origin[0])
	write_coord(origin[1])
	write_coord(origin[2] - 16 + get_cvar_num("FT_blast_radius"))
	write_short(m_iSpriteTexture)
	write_byte(0) // startframe
	write_byte(0) // framerate
	write_byte(6) // life
	write_byte(16)  // width
	write_byte(0) // noise
	write_byte(188)
	write_byte(220)
	write_byte(255)
	write_byte(255) //brightness
	write_byte(0) // speed
	message_end()

	message_begin(MSG_PAS, SVC_TEMPENTITY, origin)
	write_byte(TE_BEAMCYLINDER)
	write_coord(origin[0])
	write_coord(origin[1])
	write_coord(origin[2] - 16)
	write_coord(origin[0])
	write_coord(origin[1])
	write_coord(origin[2] - 16 + (get_cvar_num("FT_blast_radius") / 2))
	write_short(m_iSpriteTexture)
	write_byte(0) // startframe
	write_byte(0) // framerate
	write_byte(6) // life
	write_byte(16)  // width
	write_byte(0) // noise
	write_byte(188)
	write_byte(220)
	write_byte(255)
	write_byte(255) //brightness
	write_byte(0) // speed
	message_end()

	return PLUGIN_CONTINUE
}

// Blood Mage flame strike ultimate
public ultimate_wcflame(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_FSTRIKE])
	{
		client_print(id, print_chat, "You do not know the Flame Strike ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "Ultimates are disabled for the^nfirst %d seconds of the round", get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("UW_DEBUG") == 6)
	{
		new name [32]
		get_user_name(id, name, 31)
		log_amx("DEBUG: In ultimate_wcflame() <%s> invoked Flame Strike", name)
	}

	if(is_user_alive(id) && !ultimateused[id])
	{
		//Flame Strike
		fire_flamethrower(id)
		flame_count[id]++

		if(flame_count[id] > 5)
		{
			ultimateused[id] = true
			//icon_controller(id, ICON_HIDE)
			displaylevel(id, 4)
			flame_count[id] = 0
		}
	}
	return PLUGIN_HANDLED
}

// From Warcraft3FT  - Throws a shower of sprites or models
public te_spray(args[])
{
	//TE_SPRAY
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte(120) // Throws a shower of sprites or models
	write_coord(args[0]) // start pos
	write_coord(args[1])
	write_coord(args[2])
	write_coord(args[3]) // velocity
	write_coord(args[4])
	write_coord(args[5])
	write_short(fire) // spr
	write_byte(8) // count
	write_byte(70) // speed
	write_byte(100) //(noise)
	write_byte(5) //(rendermode)
	message_end()

	return PLUGIN_CONTINUE
}

// Flame thrower ultimate skill
fire_flamethrower(id)
{
	emit_sound(id, CHAN_WEAPON, "warcraft3/flamestriketargetwavenonloop1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

	new vec[3], aimvec[3], velocityvec[3], length, speed = 10

	get_user_origin(id, vec)
	get_user_origin(id, aimvec,2)
	new dist = get_distance(vec, aimvec)

	if(flame_count[id] == 1)
	{
		new parm[1]
		parm[0] = id

		set_task(get_cvar_float("FT_flamestrike_cooldown"), "cooldown", 500 + id, parm, 1)
	}
	new speed1 = 160
	new speed2 = 350
	new radius = 105

	if(dist < 50)
	{
		radius = 0
		speed = 5
	}
	else if(dist < 150)
	{
		speed1 = speed2 = 1
		speed = 5
		radius = 50
	}
	else if(dist < 200)
	{
		speed1 = speed2 = 1
		speed = 5
		radius = 90
	}
	else if(dist < 250)
	{
		speed1 = speed2 = 90
		speed = 6
		radius = 90
	}
	else if(dist < 300)
	{
		speed1 = speed2 = 140
		speed = 7
	}
	else if(dist < 350)
	{
		speed1 = speed2 = 190
		speed = 7
	}
	else if(dist < 400)
	{
		speed1 = 150
		speed2 = 240
		speed = 8
	}
	else if(dist < 450)
	{
		speed1 = 150
		speed2 = 290
		speed = 8
	}
	else if(dist < 500)
	{
		speed1 = 180
		speed2 = 340
		speed = 9
	}

	velocityvec[0] = aimvec[0] - vec[0]
	velocityvec[1] = aimvec[1] - vec[1]
	velocityvec[2] = aimvec[2] - vec[2]

	length = sqrt(velocityvec[0] * velocityvec[0] + velocityvec[1] * velocityvec[1] + velocityvec[2] * velocityvec[2])

	velocityvec[0] = velocityvec[0]*speed / length
	velocityvec[1] = velocityvec[1]*speed / length
	velocityvec[2] = velocityvec[2]*speed / length

	new args[8]

	args[0] = vec[0]
	args[1] = vec[1]
	args[2] = vec[2]
	args[3] = velocityvec[0]
	args[4] = velocityvec[1]
	args[5] = velocityvec[2]

	set_task(0.1, "te_spray", 0, args, 8, "a", 2)

	check_burnzone(id, vec, aimvec, speed1, speed2, radius)
}

// For use with skills that cause burning
burn_victim(id, killer, tk)
{
	if(isburning[id] == 1)
		return

	if(playeritem[id]==IMMUNITY || hasblink[id])
		return

	isburning[id] = 1

	new hp = get_user_health(id), args[4]

	if(hp > 250)
		hp = 250

	// [08-06-04] - Added Fire Resistance Checks - K2mia
	new Float:ftimer = 1.5
	new bool:do_burn = true

	if(USE_ENH && p_resists[id][RESISTIDX_FIRE])
	{
		new Float:chance_to_avoid = (p_resists[id][RESISTIDX_FIRE] >= RESIST_MAX_VALUE) ? 1.00 : (float(p_resists[id][RESISTIDX_FIRE]) / float(RESIST_MAX_VALUE))
		new Float:randomnumber = random_float(0.0, 1.0)

		//client_print(id, print_console, "DEBUG: fresist=(%d) chance_to_avoid=(%f) rnum=(%f)", p_resists[id][RESISTIDX_FIRE], chance_to_avoid, randomnumber)

		if(randomnumber < chance_to_avoid)
		{
			do_burn = false

			#if defined SKILL_PRINT_MESSAGE
			client_print(id, print_chat, "[%s] Your fire resistance protects you from a flamethrower attack", MOD)
			client_print(killer, print_chat, "[%s] Your flamethrower attack is deflected by your target's fire resistance", MOD)
			#endif
		}
		else if((randomnumber / 2.0) < chance_to_avoid)
		{
			ftimer = 3.5
			#if defined SKILL_PRINT_MESSAGE
			client_print(id, print_chat, "[%s] Your fire resistance fails to fully protect you from a flamethrower attack", MOD)
			#endif
		}
	}

	if(do_burn)
	{
		args[0] = id
		args[1] = killer
		args[2] = tk
		set_task(ftimer, "on_fire", ONFIRE + id, args, 4, "a",(hp / 10))
	}
	else
		isburning[id] = 0
}

public on_fire(args[])
{
	new rx, ry, rz, forigin[3]
	new id = args[0]
	new killer = args[1]

	if(!is_user_alive(id) || !isburning[id])
		return

	rx = random_num(-30, 30)
	ry = random_num(-30, 30)
	rz = random_num(-30, 30)

	get_user_origin(id, forigin)

	//TE_SPRITE - additive sprite, plays 1 cycle
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte(17)
	write_coord(forigin[0] + rx) // coord, coord, coord(position)
	write_coord(forigin[1] + ry)
	write_coord(forigin[2] + 10 + rz)
	write_short(burning) // short(sprite index)
	write_byte(30) // byte(scale in 0.1's)
	write_byte(200) // byte(brightness)
	message_end()

	//Smoke
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte(5)
	write_coord(forigin[0] + (rx * 2)) // coord, coord, coord(position)
	write_coord(forigin[1] + (ry * 2))
	write_coord(forigin[2] + 100 + (rz * 2))
	write_short(g_sModelIndexSmoke)// short(sprite index)
	write_byte(60) // byte(scale in 0.1's)
	write_byte(15) // byte(framerate)
	message_end()

	if(!is_user_alive(id))
		return

	do_damage(id, killer, 10, 13, 1, 0, 0, 0)
}

check_burnzone(id, vec[], aimvec[], speed1, speed2, radius)
{
	new tbody,tid
	get_user_aiming(id,tid,tbody,550)

	if((tid > 0) && (tid <= g_MAXPLAYERS))
	{
		if(cvar_exists("mp_friendlyfire"))
		{
			new ffcvar = get_cvar_num("mp_friendlyfire")
			if(ffcvar == 0 || ffcvar == 1)
			{
				if(get_user_team(tid) != get_user_team(id))
					burn_victim(tid, id, 0)
			}
			else
			{
				if(get_user_team(tid) == get_user_team(id))
					burn_victim(tid, id, 1)
				else
					burn_victim(tid, id, 0)
			}
		}
		else
		{
			burn_victim(tid,id,0)
		}
	}

	new burnvec1[3],burnvec2[3],length1

	burnvec1[0] = aimvec[0] - vec[0]
	burnvec1[1] = aimvec[1] - vec[1]
	burnvec1[2] = aimvec[2] - vec[2]

	length1 = sqrt(burnvec1[0] * burnvec1[0] + burnvec1[1] * burnvec1[1] + burnvec1[2] * burnvec1[2])

	burnvec2[0] = burnvec1[0] * speed2 / length1
	burnvec2[1] = burnvec1[1] * speed2 / length1
	burnvec2[2] = burnvec1[2] * speed2 / length1
	burnvec1[0] = burnvec1[0] * speed1 / length1
	burnvec1[1] = burnvec1[1] * speed1 / length1
	burnvec1[2] = burnvec1[2] * speed1 / length1

	burnvec1[0] += vec[0]
	burnvec1[1] += vec[1]
	burnvec1[2] += vec[2]
	burnvec2[0] += vec[0]
	burnvec2[1] += vec[1]
	burnvec2[2] += vec[2]

	new origin[3]
	for(new i=1; i <= g_MAXPLAYERS; i++)
	{
		if(cvar_exists("mp_friendlyfire"))
		{
			new ffcvar = get_cvar_num("mp_friendlyfire")

			if(ffcvar == 0 || ffcvar == 1)
			{
				if(get_user_team(i) != get_user_team(id))
				{
					if(is_user_alive(i) && i != id)
					{
						get_user_origin(i, origin)

						if(get_distance(origin, burnvec1) < radius)
							burn_victim(i, id, 0)
						else if(get_distance(origin, burnvec2) < radius)
							burn_victim(i, id, 0)
					}
				}
			}
			else
			{
				if(is_user_alive(i) && i != id)
				{
					get_user_origin(i, origin)
					if(get_user_team(i) == get_user_team(id))
					{
						if(get_distance(origin, burnvec1) < radius)
							burn_victim(i, id, 1)
						else if(get_distance(origin, burnvec2) < radius)
							burn_victim(i, id, 1)
					}
					else
					{
						if(get_distance(origin, burnvec1) < radius)
							burn_victim(i, id, 0)
						else if(get_distance(origin, burnvec2) < radius)
							burn_victim(i, id, 0)
					}
				}
			}
		}
		else
		{
			if(is_user_alive(i) && i != id)
			{
				get_user_origin(i, origin)

				if(get_distance(origin, burnvec1) < radius)
					burn_victim(i, id, 0)
				else if(get_distance(origin, burnvec2) < radius)
					burn_victim(i, id, 0)

			}
		}
	}
}

// Human teleport ultimate
public ultimate_wcteleport(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_TELEPORT])
	{
		client_print(id, print_chat, "You do not know the Teleport ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "Ultimates are disabled for the^nfirst %d seconds of the round", get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if( get_cvar_num("UW_DEBUG") == 6)
	{
		new name[32]
		get_user_name(id, name, 31)
		log_amx("DEBUG: In ultimate_wcteleport() <%s> invoked Teleport", name)
	}

	if(is_user_alive(id) && !ultimateused[id])
	{
		if(get_cvar_num("FT_blinkenabled") > 0)
		{
			new oldLocation[3], newLocation[3], parm[3]
			parm[0] = id

			if(get_cvar_num("FT_blink_diziness") == 2)
			{
				// This will cause Teleportation Blueness
				message_begin(MSG_ONE, gmsgFade, {0,0,0}, id) // use the magic #1 for "one client"
				write_short(1<<12) // fade lasts this long duration
				write_short(1<<8) // fade lasts this long hold time
				write_short(1<<0) // fade type IN
				write_byte(76) // fade red
				write_byte(163) // fade green
				write_byte(223) // fade blue
				write_byte(200) // fade alpha
				message_end()
			}

			get_user_origin(id, oldLocation)

			// Makes sure nearby enemy doesn't have immunity
			new teamname[32], players[32], numplayers, targetorigin[3], targetid
			get_user_team(id, teamname, 31)

			get_players(players, numplayers)

			new idname[32]
			get_user_name(id, idname, 31)

			for(new i = 0; i < numplayers; i++)
			{
				targetid = players[i]

				if(get_user_team(id) == get_user_team(targetid) || get_user_team(targetid) == SPEC || !is_user_alive(targetid))
					continue

				if(playeritem[targetid] == IMMUNITY || p_resists[targetid][RESISTIDX_MAGIC] >= RESIST_MAX_VALUE || magic_saving_throw(targetid))
				{
					get_user_origin(targetid, targetorigin)

					if(get_distance(oldLocation, targetorigin) <= get_cvar_num("FT_blink_radius"))
					{
						#if defined DEBUG
						if(get_cvar_num("UW_DEBUG") == 5)
						{
							new tname[32]
							get_user_name(targetid, tname, 31)

							log_amx("DEBUG: TELEPORT_FAILED_IMMUNITY_2 Player=(%s) Target=(%s) Is_Target_Alive=(%d)", idname, tname, is_user_alive(targetid))
							client_print(id, print_console, "DEBUG: TELEPORT_FAILED_IMMUNITY_2 Player=(%s) Target=(%s) Is_Target_Alive=(%d)", idname, tname, is_user_alive(targetid))
						}
						#endif

						set_hudmessage(255, 255, 10, -1.0, -0.4, 1, 0.5, get_cvar_float("FT_blink_cooldown"), 0.2, 0.2, 3)
						show_hudmessage(id, "Teleport Failed^nA Nearby Enemy Has Immunity")

						new coolparm[1]
						coolparm[0] = id

						set_task(get_cvar_float("FT_blink_cooldown"), "cooldown", 50 + id, coolparm, 1)

						if(file_exists("sound/warcraft3/blinkarrival.wav") == 1)
							emit_sound(id, CHAN_STATIC, "warcraft3/blinkarrival.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
						else
							emit_sound(id, CHAN_STATIC, "x/x_shoot1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

						return PLUGIN_HANDLED
					}
				}
			}

			// END Makes sure nearby enemy doesn't have immunity
			oldLocation[2] += 30 // Make sure it doesn't teleport you back into the ground.
			savedOldLoc[id] = oldLocation
			get_user_origin(id, newLocation, 3)

			if((newLocation[0] - oldLocation[0]) > 0)
				newLocation[0] -= 50
			else
				newLocation[0] += 50

			if((newLocation[1] - oldLocation[1]) > 0)
				newLocation[1] -= 50
			else
				newLocation[1] += 50

			newLocation[2] += 40

			savedNewLoc[id] = newLocation

			if(file_exists("sound/warcraft3/blinkarrival.wav"))
				emit_sound(id,CHAN_STATIC, "warcraft3/blinkarrival.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
			else
				emit_sound(id,CHAN_STATIC, "x/x_shoot1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

			// blast circles
			message_begin(MSG_PAS, SVC_TEMPENTITY, oldLocation)
			write_byte(TE_BEAMCYLINDER)
			write_coord(oldLocation[0])
			write_coord(oldLocation[1])
			write_coord(oldLocation[2] + 10)
			write_coord(oldLocation[0])
			write_coord(oldLocation[1])
			write_coord(oldLocation[2] + 10 + TELEPORT_RADIUS)
			write_short(m_iSpriteTexture)
			write_byte(0) // startframe
			write_byte(0) // framerate
			write_byte(3) // life
			write_byte(60)  // width
			write_byte(0) // noise
			write_byte(255)  // red
			write_byte(255)  // green
			write_byte(255)  // blue
			write_byte(255) //brightness
			write_byte(0) // speed
			message_end()

			// Stop bomb planting...
			client_cmd(id, "-use")

			if(get_cvar_num("FT_blink_protection"))
			{
				new mapname[32]
				get_mapname(mapname,32)

				if(equali(mapname, "as_oilrig"))
				{
					if(newLocation[0] > 1530 && get_user_team(id) == TEAM_CT)
					{
						user_kill(id)
						client_print(id, print_chat,"[%s] You have been slain for teleporting into a restricted area", MOD)
						return PLUGIN_HANDLED
					}
				}
			}

			// Test sending player, should work most of the time.
			set_user_origin(id, newLocation)

			new Float:PlayerOrigin[3]
			entity_get_vector(id, EV_VEC_origin, PlayerOrigin)

			if(get_cvar_num("FT_blink_protection"))
				set_task(1.5, "ceiling_check", 23+id, parm, 2)

			// Check if Blink laned you in a wall, if so, abort
			parm[1] = 1

			set_task(0.1, "blink_controller", 10098, parm, 2)

			ultimateused[id] = true
			//icon_controller(id, ICON_HIDE)
			displaylevel(id, 4)

			if(file_exists("sound/warcraft3/blinkarrival.wav"))
				emit_sound(id,CHAN_ITEM, "warcraft3/blinkarrival.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
			else
				emit_sound(id,CHAN_ITEM, "x/x_shoot1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)


			message_begin(MSG_PAS, SVC_TEMPENTITY, newLocation)
			write_byte(TE_BEAMCYLINDER)
			write_coord(newLocation[0])
			write_coord(newLocation[1])
			write_coord(newLocation[2] + 90)
			write_coord(newLocation[0])
			write_coord(newLocation[1])
			write_coord(newLocation[2] + 90 + TELEPORT_RADIUS)
			write_short(m_iSpriteTexture)
			write_byte(0) // startframe
			write_byte(0) // framerate
			write_byte(3) // life
			write_byte(60)  // width
			write_byte(0) // noise
			write_byte(255)  // red
			write_byte(255)  // green
			write_byte(255)  // blue
			write_byte(255) //brightness
			write_byte(0) // speed
			message_end()
		}
		else
		{
			new i
			new j
			new tmp
			new numberofplayers
			new targetid
			new targetid2
			new teamname[32]
			new distancebetween
			new distancebetween2
			new origin[3]
			new targetorigin[3]
			new targetorigin2[3]
			get_user_team(id, teamname, 31)
			get_players(teleportid[id], numberofplayers)
			get_user_origin(id, origin)

			for(i = 0; i < numberofplayers; i++) // Sort by closest to furthest
			{
				for(j = i + 1; j < numberofplayers; j++)
				{
					targetid = teleportid[id][i]

					if(get_user_team(id) != get_user_team(targetid))
						continue

					get_user_origin(targetid, targetorigin)
					distancebetween = get_distance(origin,targetorigin)
					targetid2 = teleportid[id][j]

					get_user_origin(targetid2, targetorigin2)
					distancebetween2 = get_distance(origin, targetorigin2)

					if(distancebetween2 < distancebetween && targetid2 != id) // Don't put self first
					{
						tmp = teleportid[id][i]
						teleportid[id][i] = teleportid[id][j]
						teleportid[id][j] = tmp
					}
					else if(targetid == id) // Put self last
					{
						tmp = teleportid[id][i]
						teleportid[id][i] = teleportid[id][j]
						teleportid[id][j] = tmp
					}
				}
			}

			if(numberofplayers > 9)
				numberofplayers = 9
			else
				--numberofplayers       // Remove self from list

			if(numberofplayers)
			{
				remove_task(666 + id)
				teleportmenu[id] = true

				new menuparm[2]

				menuparm[0] = id
				menuparm[1] = numberofplayers
				telemenu(menuparm)
			}
			else
			{
				set_hudmessage(178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 3)
				show_hudmessage(id, "No valid targets found!")
			}
		}
	}
	return PLUGIN_HANDLED
}

public ceiling_check(parm[2]) // Sooooooo Waste of cpu cycle
{
	new id = parm[0]
	new mapname[32]
	new slay
	new origin[3]
	get_user_origin(id, origin)
	new x = origin[0]
	new y = origin[1]
	new z = origin[2]
	get_mapname(mapname, 32)

	if(!is_user_connected(id))
		return

	new Float:porigin[3]
	entity_get_vector(id, EV_VEC_origin, porigin)

	if(PointContents(porigin) == -6)
		slay = 1

	if(equali(mapname, "de_dust"))
	{
		if(z > 220)
		{
		    slay = 1
		}
	}
	else if(equali(mapname, "awp_assault"))
	{
		if(z > 520)
		{
			if(y > 2400 && y < 2600)
				slay = 1
		}
	}
	else if(equali(mapname, "de_dust_cz"))
	{
		if(z > 220)
			slay = 1
	}
	else if(equali(mapname, "de_aztec_cz"))
	{
		if(z > 300)
			slay = 1
	}
	else if(equali(mapname, "cs_assault_upc"))
	{
		if(z > 650)
		slay=1
	}
	else if(equali(mapname, "de_aztec"))
	{
		if(z > 300)
			slay = 1
	}
	else if(equali(mapname, "de_cbble"))
	{
		if(z>315)
		{
			if((x >- 1320 && x <- 1150) && (y > 2600 && y < 2900))
				return
			else
				slay=1
		}
	}
	else if(equali(mapname, "de_cbble_cz"))
	{
		if(z > 315)
		{
			if((x>-1320 && x<-1150) &&(y>2600 && y<2900))
				return
			else
				slay = 1
		}
	}
	else if(equali(mapname, "cs_assault"))
	{
		if(z > 700)
		{
			slay = 1
		}
	}
	else if(equali(mapname, "cs_militia"))
	{
		if(z > 500)
			slay = 1
	}
	else if(equali(mapname, "cs_militia_cz"))
	{
		if(z > 500)
			slay = 1
	}
	else if(equali(mapname, "cs_italy"))
	{
		if(z >- 220)
		{
			if(y <- 2128)
			{
				slay = 1
			}
		}

		if(z > 250)
		{
			if((x <- 1000 && x >- 1648) && (y > 1900 && y <2050))
				slay = 1

			if((x<-1552 && x>-1648) &&(y>1520 && y<2050))
				slay = 1
		}
	}
	else if(equali(mapname, "cs_italy_cz"))
	{
		if(y > 2608)
			slay = 1
	}
	else if(equali(mapname, "de_dust2"))
	{
		if(z > 270)
			slay = 1
	}
	else if(equali(mapname, "de_dust2_cz"))
	{
		if(z > 270)
			slay = 1
	}
	else if(equali(mapname, "fy_dustworld"))
	{
		if(z > 82)
		slay = 1
	}
	else if(equali(mapname, "fy_pool_day"))
	{
		if(z > 190)
		slay = 1
	}
	else if(equali(mapname, "as_oilrig"))
	{
		if(x > 1530 && get_user_team(id) == TEAM_CT)
			slay = 1
		else if(x > 1700)
			slay = 1
	}
	else if(equali(mapname,"cs_mice_final"))
	{
		if((x >= 798 && x <= 860) && y <- 1430)
			slay = 1
		else
			slay = 0
	}

	if(slay)
	{
		new name[32]
		get_user_name(id, name, 31)
		client_print(id, print_chat, "[%s] You have been slayed for entering a restricted area", MOD)
		set_user_health_log(id, -1, true, "check_ceiling() condition=slay")
	}
}

public blink_controller(parm[2])
{
	new id = parm[0]
	new newLocation[3]
	new curLocation[3]
	new oldLocation[3]
	new origin[3]

	if(parm[1] == 1)
	{
		// Teleport failure check and unsticker
		new coolparm[1]
		coolparm[0] = id

		newLocation = savedNewLoc[id]
		get_user_origin(id, curLocation, 0)
		//client_print(id,print_chat,"blinked to: x:%i y:%i z%i", newLocation[0],newLocation[1],newLocation[2])
		//client_print(id,print_chat,"currently: x:%i y:%i z%i", curLocation[0],curLocation[1],curLocation[2])

		if(newLocation[2] == curLocation[2]) // Teleportation Failure
		{
			oldLocation = savedOldLoc[id]
			//client_print(id,print_chat,"sent back to: x:%i y:%i z%i", oldLocation[0],oldLocation[1],oldLocation[2])

			set_hudmessage(255, 255, 10, -1.0, -0.4, 1, 0.5, get_cvar_float("FT_blink_cooldown"), 0.2, 0.2, 3);
			show_hudmessage(id, "Teleport Failed^nBad Destination");
			set_user_origin(id, oldLocation)

			parm[1] = 0
			set_task(0.1, "blink_controller", 0, parm, 2)
			set_task(get_cvar_float("FT_blink_cooldown"),"cooldown",50 + id,coolparm,1)
		}
		else
		{
			// Teleportation Success if not near player with immunity
			new teamname[32]
			new players[32]
			new numplayers
			new targetorigin[3]
			new targetid
			new bool:teleportSuccess = true

			get_user_origin(id, origin)
			get_user_team(id, teamname, 31)

			get_players(players, numplayers)

			new idname[32]
			get_user_name(id, idname, 31)

			for(new i = 0; i < numplayers; i++)
			{
				targetid = players[i]

				if(get_user_team(id) == get_user_team(targetid) || get_user_team(targetid) == SPEC || !is_user_alive(targetid))
					continue


				if(playeritem[targetid] == IMMUNITY || p_resists[targetid][RESISTIDX_MAGIC] >= RESIST_MAX_VALUE || magic_saving_throw(targetid))
				{
					get_user_origin(targetid, targetorigin)
					if(get_distance(origin, targetorigin) <= get_cvar_num("FT_blink_radius"))
					{
						oldLocation = savedOldLoc[id]
						//client_print(id,print_chat,"sent back to: x:%i y:%i z%i", oldLocation[0],oldLocation[1],oldLocation[2])

						#if defined DEBUG
						if(get_cvar_num("UW_DEBUG") == 5)
						{
							new tname[32]
							get_user_name(targetid, tname, 31)

							log_amx("DEBUG: TELEPORT_FAILED_IMMUNITY_1 Player=(%s) Target=(%s) Is_Target_Alive=(%d)", idname, tname, is_user_alive(targetid))
							client_print( id, print_console, "DEBUG: TELEPORT_FAILED_IMMUNITY_1 Player=(%s) Target=(%s) Is_Target_Alive=(%d)", idname, tname, is_user_alive(targetid))
						}
						#endif

						set_hudmessage(255, 255, 10, -1.0, -0.4, 1, 0.5, get_cvar_float("FT_blink_cooldown"), 0.2, 0.2, 3);
						show_hudmessage(id, "Teleport Failed^nAn Enemy Near Your Destination Has Immunity");
						set_user_origin( id, oldLocation)

						parm[1] = 0
						set_task(0.1, "blink_controller", 0, parm, 2)
						set_task(get_cvar_float("FT_blink_cooldown"), "cooldown", 50 + id, coolparm, 1)
						teleportSuccess = false
					}
				}
			}

			if(teleportSuccess)
			{
				parm[1] = 2

				// [08-05-04] - Added faster cooldown for Master Intellect
				new Float:tele_cool = get_cvar_float("FT_teleport_cooldown")

				if(USE_ENH && p_attribs[id][ATTRIBIDX_INT] >= INT_TELEPORT_LEVEL)
				{
					tele_cool = INT_TELEPORT_COOL

					#if defined DEBUG
					if(get_cvar_num("UW_DEBUG") == 1)
					{
						client_print(id, print_console, "[%s DEBUG] Tele_Cool=(%f)", MOD, tele_cool)
					}
					#endif
				}

				set_task(tele_cool, "cooldown", 500 + id, coolparm, 1)

				// Call phase 2 of the blindness
				set_task(0.6, "blink_controller", 0, parm, 2)

				// Sprays white bubbles everywhere
				get_user_origin(id, origin)

				message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
				write_byte(TE_SPRITETRAIL)
				write_coord(origin[0])
				write_coord(origin[1])
				write_coord(origin[2] + 40)
				write_coord(origin[0])
				write_coord(origin[1])
				write_coord(origin[2])
				write_short(flaresprite)
				write_byte(30)   // count
				write_byte(10)   // life
				write_byte(1)    // scale
				write_byte(50)   // velocity
				write_byte(10)   // randomness in velocity
				message_end()

				if(get_cvar_num("FT_blink_diziness") == 1)
				{
					// This will cause Teleportation Dizziness(like a flash grenade)
					message_begin(MSG_ONE, gmsgFade, {0, 0, 0}, id)
					write_short(1<<15)
					write_short(1<<10)
					write_short(1<<12)
					write_byte(255)
					write_byte(255)
					write_byte(255)
					write_byte(255)
					message_end()
				}
			}
		}
	}
	else if(parm[1] == 2) // Make the Teleport Blues hold..
	{
		if(get_cvar_num("FT_blink_diziness") == 2)
		{
			message_begin(MSG_ONE, gmsgFade, {0,0,0}, id) // use the magic #1 for "one client"
			write_short(1<<0) // fade lasts this long duration
			write_short(1<<0) // fade lasts this long hold time
			write_short(1<<2) // fade type HOLD
			write_byte(76)    // fade red
			write_byte(163)   // fade green
			write_byte(223)   // fade blue
			write_byte(200)   // fade alpha
			message_end()
		}

		parm[1] = 0
		set_task(3.0, "blink_controller", 0, parm, 2)

	}
	else // Teleport Blueness goes away
	{
		if(get_cvar_num("FT_blink_diziness") == 2)
		{
			message_begin(MSG_ONE, gmsgFade, {0,0,0}, id) // use the magic #1 for "one client"
			write_short(1<<12)// fade lasts this long duration
			write_short(1<<8) // fade lasts this long hold time
			write_short(1<<1) // fade type OUT
			write_byte(76)    // fade red
			write_byte(163)   // fade green
			write_byte(223)   // fade blue
			write_byte(200)   // fade alpha
			message_end()
		}
	}
}

public telemenu(parm[2])
{
	new id = parm[0]
	new numberofplayers = parm[1]
	new targetid
	new name[32]
	new origin[3]
	new targetorigin[3]
	new distancebetween
	new temp[64]
	new i
	new keys = (1<<9)

	new menu_body[512]

	format(menu_body, 511, "\yTeleport To:\w^n")
	get_user_origin(id, origin)

	for(i = 0; i < numberofplayers; i++)
	{
		targetid = teleportid[id][i]
		get_user_name(targetid, name, 31)
		get_user_origin(targetid, targetorigin)
		distancebetween = get_distance(origin,targetorigin)

		if(is_user_alive(targetid))
		{
			format(temp, 63, "^n\w%d. %s(\y%dm\w)", i + 1, name, distancebetween / 40)
			keys |= (1<<i)
		}
		else
			format(temp, 63, "^n\d%d. %s", i + 1, name)

		add(menu_body, 255, temp)
	}

	format(temp, 63, "^n^n\w0. Cancel")
	add(menu_body, 255, temp)
	show_menu(id, keys, menu_body, -1)

	new menuparm[2]
	menuparm[0] = id
	menuparm[1] = numberofplayers

	if(teleportmenu[id])
		set_task(1.0, "telemenu", 666 + id, parm, 2)
}

// Orc chain lightning ultimate
public ultimate_wcchain(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_LIGHTNING])
	{
		client_print(id, print_chat, "You do not know the Chain Lightning ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "Ultimates are disabled for the^nfirst %d seconds of the round",get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("UW_DEBUG") == 6)
	{
		new name[32]
		get_user_name(id, name, 31)
		log_amx("DEBUG: In ultimate_wcchain() <%s> invoked Chain Lightning", name)
	}

	if(is_user_alive(id) && !issearching[id] && !ultimateused[id])
	{
		new parm[2]
		parm[0] = id
		parm[1] = ULTIMATESEARCHTIME
		lightsearchtarget(parm)
	}
	return PLUGIN_HANDLED
}

public lightsearchtarget(parm[2])
{
	new id = parm[0]
	new enemy, body
	get_user_aiming(id, enemy, body)

	if(0 < enemy <= g_MAXPLAYERS && get_user_team(id) != get_user_team(enemy) && playeritem[enemy] != IMMUNITY && !hasblink[enemy] && is_user_alive(id) && is_user_alive(enemy))
	{
		ultimateused[id] = true
		//icon_controller(id, ICON_HIDE)
		displaylevel(id, 4)
		new linewidth = 80
		new damage = 50
		issearching[id] = false

		// [08-05-04] - Added Electricity resistance check - K2mia
		new actual_damage = damage
		if(USE_ENH && p_resists[enemy][RESISTIDX_ELECTRIC])
		{
			actual_damage = actual_damage - floatround((float(p_resists[enemy][RESISTIDX_ELECTRIC]) / float(RESIST_MAX_VALUE)) * float(actual_damage))

			#if defined DEBUG
			client_print(enemy, print_console, "DEBUG: orig_ltning_dmg=(%d) modified_dmg=(%d)", damage, actual_damage)
			#endif

			#if defined SKILL_PRINT_MESSAGE
			if(p_resists[enemy][RESISTIDX_ELECTRIC] == RESIST_MAX_VALUE)
				client_print(enemy, print_chat, "[%s] Your resistance to electricity absorbs all of the Chain Lightning damage", MOD)
			else
				client_print(enemy, print_chat, "[%s] Your resistance to electricity absorbs some of the Chain Lightning damage", MOD)
			#endif
		}

		lightningeffect(id, enemy, linewidth, actual_damage, id)

		new lightparm[4]

		lightparm[0] = enemy
		lightparm[1] = damage
		lightparm[2] = linewidth
		lightparm[3] = id

		set_task(0.2, "lightningnext", 24, lightparm, 4)

		new cooldownparm[1]
		cooldownparm[0] = id
		set_task(get_cvar_float("FT_chainlightning_cooldown"), "cooldown", 50 + id, cooldownparm, 1)
	}
	else
	{
		issearching[id]=true
		//icon_controller(id,ICON_FLASH)
		displaylevel(id, 4)
		new counter = parm[1]
		while(counter >= 0)
		{
			counter -= 10

			if(counter == 0)
				emit_sound(id,CHAN_ITEM, "turret/tu_ping.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
		}

		--parm[1]

		if(parm[1] > 0 && get_user_health(id) >= 0)
		{
			set_task(0.1,"lightsearchtarget",200+id,parm,2)
		}
		else
		{
			issearching[id] = false
			//icon_controller(id, ICON_SHOW)
			displaylevel(id, 4)
		}
	}
}

public lightningnext(parm[4]) // Chain Lightning
{
	new id = parm[0]
	new caster = parm[3]
	new origin[3]
	get_user_origin(id, origin)
	new players[32]
	new teamname[32]
	get_user_team(id, teamname, 31)
	new numberofplayers
	get_players(players, numberofplayers)
	new i
	new targetid
	new distancebetween
	new targetorigin[3]
	new damage = parm[1] * 2 / 3
	new linewidth = parm[2] * 2 / 3
	new closestdistance
	new closestid

	for(i = 0; i < numberofplayers; i++)
	{
		targetid = players[i]

		if(get_user_team(id) != get_user_team(targetid))
			continue

		if(get_user_team(id) == get_user_team(targetid) && is_user_alive(targetid) && is_user_alive(id))
		{
			get_user_origin(targetid, targetorigin)
			distancebetween = get_distance(origin, targetorigin)

			if(distancebetween < LIGHTNING_RANGE && playeritem[targetid] != IMMUNITY && !lightninghit[targetid] && !hasblink[targetid] && p_resists[targetid][RESISTIDX_MAGIC] < RESIST_MAX_VALUE && !magic_saving_throw(targetid))
			{
				if(distancebetween < closestdistance || closestid == 0)
				{
					closestdistance = distancebetween
					closestid = targetid
					break
				}
			}
		}
	}

	if(closestid)
	{
		new actual_damage = damage
		// [08-05-04] - Added Electricity resistance check - K2mia
		if(USE_ENH && p_resists[closestid][RESISTIDX_ELECTRIC])
		{
			actual_damage -=(p_resists[closestid][RESISTIDX_ELECTRIC] / 100) * actual_damage

			#if defined SKILL_PRINT_MESSAGE
			if(p_resists[closestid][RESISTIDX_ELECTRIC] == RESIST_MAX_VALUE)
				client_print(closestid, print_chat, "[%s] Your resistance to electricity absorbs all of the Chain Lightning damage", MOD)
			else
				client_print(closestid, print_chat, "[%s] Your resistance to electricity absorbs some of the Chain Lightning damage", MOD)
			#endif
		}

		lightningeffect(id, closestid, linewidth, actual_damage, caster)

		parm[0] = closestid
		parm[1] = damage
		parm[2] = linewidth
		parm[3] = caster

		set_task(0.2, "lightningnext", 27, parm, 4)
	}
	else
	{
		for(i = 0; i < numberofplayers; i++)
		{
			lightninghit[players[i]]=false
		}
	}
}

public lightningeffect(id,targetid,linewidth,damage,caster)
{
	lightninghit[targetid] = true

	new parm[1]
	parm[0] = id
	//icon_controller(id, ICON_HIDE)
	displaylevel(id, 4)

	do_damage(targetid, caster, damage, 11, 3, 0, 0, 0)

	if(get_user_armor(targetid) - damage <= 0)
		set_user_armor_log(targetid, 0, true, "lightningeffect()-1")
	else
		set_user_armor_log(targetid, (get_user_armor(targetid)-damage), true, "lightningeffect()-2")

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(TE_BEAMENTS)
	write_short(id)         // start entity
	write_short(targetid)   // entity
	write_short(lightning) // model
	write_byte(0)         // starting frame
	write_byte(15)        // frame rate
	write_byte(10)        // life
	write_byte(linewidth) // line width
	write_byte(10)        // noise amplitude
	write_byte(255)       // r, g, b
	write_byte(255)       // r, g, b
	write_byte(255)       // r, g, b
	write_byte(255)       // brightness
	write_byte(0)         // scroll speed
	message_end()

	new origin[3]
	get_user_origin(targetid, origin)

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(TE_ELIGHT)
	write_short(targetid)   // entity
	write_coord(origin[0])  // initial position
	write_coord(origin[1])  // initial position
	write_coord(origin[2])  // initial position
	write_coord(100)        // radius
	write_byte(255)       // r, g, b
	write_byte(255)       // r, g, b
	write_byte(255)       // r, g, b
	write_byte(10)        // life
	write_coord(0)          // decay rate
	message_end()

	if(file_exists("sound/warcraft3/lightningbolt.wav"))
		emit_sound(id,CHAN_ITEM, "warcraft3/lightningbolt.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
	else
		emit_sound(id,CHAN_ITEM, "weapons/gauss2.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
}

// Night elf entangle ultimate
public ultimate_wcentangle(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_ENTANGLE])
	{
		client_print(id, print_chat, "You do not know the Entangle ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"Ultimates are disabled for the^nfirst %d seconds of the round",get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("UW_DEBUG") == 6)
	{
		new name[32]
		get_user_name(id, name, 31)
		log_amx("DEBUG: In ultimate_wcentangle() <%s> invoked Entangle Roots", name)
	}

	if(is_user_alive(id) && !issearching[id] && !ultimateused[id])
	{
		new parm[2]
		parm[0] = id
		parm[1] = ULTIMATESEARCHTIME
		searchtarget(parm)
	}

	return PLUGIN_HANDLED
}

public searchtarget(parm[2])
{
	new id = parm[0]
	new enemy, body
	get_user_aiming(id, enemy, body)

	if((0 < enemy <= g_MAXPLAYERS) && !temp_immunity[enemy])
	{
		if(p_resists[enemy][RESISTIDX_MAGIC] >= RESIST_MAX_VALUE || magic_saving_throw(enemy))
		{
			temp_immunity[enemy] = true

			#if defined SKILL_PRINT_MESSAGE
			client_print(enemy, print_chat, "[%s] Your magic resistance makes you temporarily immune to Entangling Roots", MOD)
			#endif

			new iparm[2]

			iparm[0] = enemy // Player with temp immunity
			iparm[1] = 1     // Immunity type 1 - Entangling roots

			set_task(5.0, "reset_immunity", 201+id, iparm, 2)
		}
	}

	if(0 < enemy <= g_MAXPLAYERS && !temp_immunity[enemy] && !stunned[enemy] && get_user_team(id) != get_user_team(enemy) && playeritem[enemy] != IMMUNITY && !hasblink[enemy] && is_user_alive(id) && is_user_alive(enemy))
	{
		issearching[id] = false
		ultimateused[id] = true
		//icon_controller(id, ICON_HIDE)
		displaylevel(id, 4)

		if(file_exists("sound/warcraft3/entanglingrootstarget1.wav"))
			emit_sound(id, CHAN_ITEM, "warcraft3/entanglingrootstarget1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
		else
			emit_sound(id, CHAN_ITEM, "weapons/cbar_hitbod3.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

		new waitparm[6]

		waitparm[0] = enemy
		waitparm[1] = 100
		waitparm[5] = floatround(get_user_maxspeed(enemy))

		set_user_maxspeed(enemy, 1.0)

		waitstop(waitparm)
		stunned[enemy] = true

		new cooldownparm[1]

		cooldownparm[0] = id
		set_task(get_cvar_float("FT_entanglingroots_cooldown"), "cooldown", 50 + id, cooldownparm, 1)
	}
	else
	{
		issearching[id] = true
		//icon_controller(id, ICON_FLASH)
		displaylevel(id, 4)
		new counter = parm[1]
		while(counter >= 0)
		{
			counter -= 10
			if(counter == 0)
			emit_sound(id, CHAN_ITEM, "turret/tu_ping.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
		}
		--parm[1]

		if(parm[1] > 0 && get_user_health(id) > 0)
		{
			set_task(0.1, "searchtarget", 200 + id, parm, 2)
		}
		else
		{
			issearching[id] = false
			//icon_controller(id, ICON_SHOW)
			displaylevel(id, 4)
		}
	}
	return PLUGIN_CONTINUE
}

public reset_immunity(parm[2])
{
	new id = parm[0]

	#if defined SKILL_PRINT_MESSAGE
	switch(parm[1])
	{
		case 1:
			client_print(id, print_chat, "[%s] Your magic resistance to Entangling Roots has ended", MOD)
		case 2:
			client_print(id, print_chat, "[%s] Your magic resistance to Suicide Bomber blast waves has ended", MOD)
	}
	#endif

	temp_immunity[id] = false
}

public waitstop(parm[6])
{
	new id = parm[0]
	new origin[3]
	get_user_origin(id, origin)

	if(origin[0]==parm[2] && origin[1]==parm[3] && origin[2]==parm[4])
	{
		new normalspeed = parm[5]
		new resetparm[2]

		resetparm[0] = id
		resetparm[1] = normalspeed

		set_task(float(parm[1] / 10), "reset_maxspeed", 1000 + id, resetparm, 2)

		new entangleparm[2]

		entangleparm[0] = parm[0]
		entangleparm[1] = parm[1]

		entangle(entangleparm)
	}
	else
	{
		parm[2] = origin[0]
		parm[3] = origin[1]
		parm[4] = origin[2]
		set_task(0.1, "waitstop", 29, parm, 6)
	}
}

public entangle(parm[2])
{
	// Entangle Roots(DOESN'T WORK ON BOTS)
	new id = parm[0]
	new origin[3]
	get_user_origin(id, origin)

	if(file_exists("sound/warcraft3/entanglingrootstarget1.wav"))
		emit_sound(id, CHAN_ITEM, "warcraft3/entanglingrootstarget1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
	else
		emit_sound(id, CHAN_ITEM, "weapons/electro5.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

	new parm2[1]
	parm2[0] = id
	set_task(20.0, "cooldown", 500 + id, parm2, 1)

	// ****************************************************************************
	// [09-01-04] - Entangling Roots gfx redone, root model by steve french - K2mia
	new vec[3]
	new Float:rsize[2][3] = { {-50.0, -50.0, -50.0}, {50.0, 50.0, 50.0} }
	get_user_origin(id, vec)

	new Float:fOrigin[3]
	fOrigin[0] = float(vec[0])
	fOrigin[1] = float(vec[1])
	fOrigin[2] = float(vec[2]) - 35.0

	new roots_ent = create_entity("info_target")

	entity_set_string(roots_ent, EV_SZ_classname, "roots")
	entity_set_model(roots_ent, "models/roots2.mdl")
	entity_set_int(roots_ent, EV_INT_body, 3)
	entity_set_int(roots_ent, EV_INT_sequence, 0)
	set_size(roots_ent, rsize[0], rsize[1])
	entity_set_origin(roots_ent, fOrigin)

	new args[1]
	args[0] = roots_ent
	set_task(10.0, "destroy_roots", 130 + id, args, 1)
	// [09-01-04] - Entangling Roots gfx redone, root model by steve french - K2mia
	// ****************************************************************************
}

public destroy_roots(args[])
{
	new ent = args[0]

	if(pev_valid(ent))
		remove_entity(ent)
}

// Shadow Hunter Voodoo ultimate
public ultimate_wcvoodoo(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_VOODOO])
	{
		client_print(id, print_chat, "You do not know the Big Bad Voodoo ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"Ultimates are disabled for the^nfirst %d seconds of the round",get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("UW_DEBUG") == 6)
	{
		new name [32]
		get_user_name( id, name, 31)
		log_amx( "DEBUG: In ultimate_wcvoodoo() <%s> invoked Voodoo", name )
	}

	if(!ultimateused[id])
	{
		new parm[2]
		parm[0] = id
		parm[1] = 3

		godshealth[id] = get_user_health(id)
		hasgodmode[id] = true

		set_user_health_log(id, get_user_health(id) + 2048, true, "ultimate_wcvoodoo() ultimateused")

		new duration = 2

		if(USE_ENH && p_attribs[id][ATTRIBIDX_INT] >= INT_MASTER_LEVEL)
		{
			set_task(4.0,"reset_godmode",4500 + id,parm,2)
			message_begin(MSG_ONE, 108, {0, 0, 0}, id)
			// Bar(thanks to bad-at-this)
			write_byte(4) // duration
			write_byte(0) // duration
			message_end()
			duration = 4
		}
		else
		{
	         set_task(2.0, "reset_godmode", 4500 + id, parm, 2)
	         message_begin(MSG_ONE, 108, {0, 0, 0}, id)
	         // Bar(thanks to bad-at-this)
	         write_byte(2) // duration
	         write_byte(0) // duration
	         message_end()
		}

		new pname[32]
		get_user_name(id, pname, 31)
		log_amx( "In ultimate_wcvoodoo() :: [%s] Voodoo Activated(for %d secs)", pname, duration)


		if(file_exists("sound/warcraft3/divineshield.wav"))
			emit_sound(id,CHAN_STATIC, "warcraft3/divineshield.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

		ultimateused[id] = true
		//icon_controller(id, ICON_FLASH)
		displaylevel(id, 4)
	}
	return PLUGIN_HANDLED
}

public reset_godmode(parm[])
{
	new id = parm[0]

	new name[32]
	get_user_name(id, name, 31)

	if(godshealth[id] < 1)
		set_user_health_log(id, 30, true, "reset_godmode() condition1")
	else
		set_user_health_log(id, godshealth[id], true, "reset_godmode() condition2")


	hasgodmode[id] = false
	set_task(get_cvar_float("FT_bigbadvoodoo_cooldown"), "cooldown", 500 + id, parm, 1)
	set_hudmessage(0, 100, 0, 0.05, 0.75, 2, 0.02, 10.0, 0.01, 0.1, 2)
	show_hudmessage(id, "Your Voodoo has expired")
	//icon_controller(id, ICON_HIDE)
	displaylevel(id, 4)
}
// **************************************************************************
// END ultimate_wcvoodoo and supporting routines
// **************************************************************************


// **************************************************************************
// BEGIN ultimate_wclocust and supporting routine
// Crypt Lord Locust Swarm ultimate
// **************************************************************************
public ultimate_wclocust(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_LOCUST])
	{
		client_print(id, print_chat, "You do not know the Locust Swarm ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "Ultimates are disabled for the^nfirst %d seconds of the round", get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("UW_DEBUG") == 6)
	{
		new name[32]
		get_user_name(id, name, 31)
		log_amx("DEBUG: In ultimate_wclocust() <%s> invoked Locust Swarm", name)
	}

	if(is_user_alive(id) && !ultimateused[id])
		calllocustfunction(id)

	return PLUGIN_HANDLED
}

public calllocustfunction(id)
{
	new parm[11]
	parm[7] = id
	new players[32], numberofplayers, i, player, possibility[33], count
	get_players(players, numberofplayers)

	for(i = 0; i < numberofplayers; i++)
	{
		player = players[i]

		if(get_user_team(player) != get_user_team(id) && is_user_alive(player) && !hasblink[player] && playeritem[player] != IMMUNITY)
		{
			if(p_resists[player][RESISTIDX_MAGIC] < RESIST_MAX_VALUE && !magic_saving_throw(player))
			{
				possibility[count] = player
				count++
			}
		}
	}

	if(count == 0)
	{
		set_hudmessage(178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 3)
		show_hudmessage(id, "No valid targets found!")
		return
	}

	parm[6] = 0 // Initialize target parameter

	//client_print(0, print_console, "DEBUG: before-count=(%d) parm6=(%d)", count, parm[6])

	// Prevents target from being the server
	while(parm[6] == 0)
		parm[6] = possibility[random_num(0, count)]

	//client_print(0, print_console, "DEBUG: after-count=(%d) parm6=(%d)", count, parm[6])

	if(parm[6] > numberofplayers)
	{
		set_hudmessage(178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 3)
		show_hudmessage(id, "No valid targets found!")
		return
	}

	new origin[3], origin2[3]
	get_user_origin(id, origin)
	get_user_origin(parm[6], origin2)

	parm[0] = origin[0]
	parm[1] = origin[1]
	parm[2] = origin[2]

	ultimateused[id] = true

	if(file_exists("sound/warcraft3/locustswarmloopwav.wav"))
		emit_sound(id, CHAN_ITEM, "warcraft3/locustswarmloopwav.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

	drawfunnels(parm)
}

public drawfunnels(parm[])
{
	new MULTIPLIER = 150 // the lower the number the faster it reaches the target
	new id = parm[6]
	new caster = parm[7]

	if(p_resists[id][RESISTIDX_MAGIC] >= RESIST_MAX_VALUE || magic_saving_throw(id))
	{
		#if defined SKILL_PRINT_MESSAGE
		client_print(id, print_chat, "[%s] Your magic resistance protects you from a Locust Swarm", MOD)
		#endif

		ultimateused[caster] = false
		//icon_controller(caster, ICON_SHOW)
		displaylevel(id, 4)
		return
	}

	if(playeritem[id] == IMMUNITY || hasblink[id] || !is_user_alive(id))
	{
		ultimateused[caster] = false
		//icon_controller(caster, ICON_SHOW)
		displaylevel(id, 4)
		return
	}

	new origin[3], funnel[3], name[32], name2[32]
	get_user_name(id, name, 31)
	get_user_name(caster, name2, 31)
	get_user_origin(id, origin)

	funnel[0] = parm[0] // Origin of the funnel
	funnel[1] = parm[1]
	funnel[2] = parm[2]

	//icon_controller(caster,ICON_FLASH)

	message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
	write_byte(TE_LARGEFUNNEL)
	write_coord(funnel[0])         // origin, x
	write_coord(funnel[1])         // origin, y
	write_coord(funnel[2])         // origin, z
	write_short(snow)              // sprite(0 for none)
	write_short(0)                 // 0 for collapsing, 1 for sending outward
	message_end()

	new xdist = diff(origin[0], funnel[0])
	new ydist = diff(origin[1], funnel[1])
	new zdist = diff(origin[2], funnel[2])

	if(diff(origin[0], (funnel[0] - MULTIPLIER)) < xdist)
		parm[0] = funnel[0] - MULTIPLIER
	else if(diff(origin[0], (funnel[0] + MULTIPLIER)) < xdist)
		parm[0] = funnel[0] + MULTIPLIER
	else
		parm[0] = origin[0]

	if(diff(origin[1], (funnel[1] - MULTIPLIER)) < ydist)
		parm[1] = funnel[1] - MULTIPLIER
	else if(diff(origin[1], (funnel[1] + MULTIPLIER)) < ydist)
		parm[1] = funnel[1] + MULTIPLIER
	else
		parm[1] = origin[1]

	if(diff(origin[2], (funnel[2] - MULTIPLIER)) < zdist)
		parm[2] = funnel[2] - MULTIPLIER
	else if(diff(origin[2], (funnel[2]+MULTIPLIER)) < zdist)
		parm[2] = funnel[2] + MULTIPLIER
	else
		parm[2] = origin[2]

	if(!endround)
	{
		if(!(xdist < 50 && ydist < 50 && zdist < 50))
			set_task(0.1, "drawfunnels", id + 800, parm, 11)
		else
		{
			new actual_damage = 35
			// [08-05-04] - Added Disease resistance check - K2mia
			if(USE_ENH && p_resists[id][RESISTIDX_DISEASE])
			{
				actual_damage = actual_damage - floatround((float(p_resists[id][RESISTIDX_DISEASE]) / float(RESIST_MAX_VALUE)) * float(actual_damage))

				//client_print(id, print_console, "DEBUG: orig_locust_dmg=(%d) mod_dmg=(%d)", 35, actual_damage)

				#if defined SKILL_PRINT_MESSAGE
				if(p_resists[id][RESISTIDX_DISEASE] == RESIST_MAX_VALUE)
					client_print(id, print_chat, "[%s] Your disease resistance absorbs all of the Locust Swarm damage", MOD)
				else
					client_print(id, print_chat, "[%s] Your disease resistance absorbs some of the Locust Swarm damage", MOD)
				#endif
			}

			do_damage(id, caster, actual_damage, 15, 3, 0, 0, 0)

			if(file_exists("sound/warcraft3/locustswarmloopwav.wav"))
				emit_sound(id, CHAN_ITEM, "warcraft3/locustswarmloopwav.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

			//icon_controller(caster, ICON_HIDE)
			displaylevel(id, 4)

			new cooldownparm[1]
			cooldownparm[0] = caster
			set_task(get_cvar_float("FT_locusts_cooldown"), "cooldown", 500 + id, cooldownparm, 1)
		}
	}
}

// Flash of Light Ultimate
public ultimate_wcflash(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_FLASH])
	{
		client_print(id, print_chat, "You do not know the Flash of Light ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "Ultimates are disabled for the^nfirst %d seconds of the round",get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("UW_DEBUG") == 6)
	{
		new name[32]
		get_user_name(id, name, 31)
		log_amx("DEBUG: In ultimate_wcflash() <%s> invoked Flash of Light", name)
	}

	if(is_user_alive(id) && !issearching[id] && !ultimateused[id])
	{
		new parm[2]
		parm[0] = id
		parm[1] = ULTIMATESEARCHTIME
		searchtargetflash(parm)
	}
	return PLUGIN_HANDLED
}

public searchtargetflash(parm[2])
{
	new id = parm[0]
	new enemy, body
	get_user_aiming(id, enemy, body)

	if(0 < enemy <= g_MAXPLAYERS && !stunned[enemy] && get_user_team(id) != get_user_team(enemy) && playeritem[enemy]!=IMMUNITY && !hasblink[enemy] && is_user_alive(id) && is_user_alive(enemy) && p_resists[enemy][RESISTIDX_MAGIC] < RESIST_MAX_VALUE && !magic_saving_throw(enemy))
	{
		issearching[id] = false
		ultimateused[id] = true
		//icon_controller(id, ICON_HIDE)
		displaylevel(id, 4)

		new name[32], ename[32]
		get_user_name(id , name, 31)
		get_user_name(enemy , ename, 31)

		#if defined SKILL_PRINT_MESSAGE
		client_print(id, print_chat, "[%s] Your Flash of Light blinds %s", MOD, ename)
		client_print(enemy, print_chat, "[%s] You are blinded by a Flash of Light from %s", MOD, name)
		#endif

		message_begin(MSG_ONE, gmsgFade,{0,0,0},enemy)
		write_short(1<<15)
		write_short(1<<10)
		write_short(1<<12)
		write_byte(255)
		write_byte(255)
		write_byte(255)
		write_byte(255)
		message_end()

		emit_sound(id, CHAN_BODY, "weapons/flashbang-2.wav", 1.0, ATTN_NORM, 0, PITCH_HIGH)

		new cooldownparm[1]
		cooldownparm[0] = id
		set_task(get_cvar_float("FT_entanglingroots_cooldown"), "cooldown", 50 + id, cooldownparm, 1)
	}
	else
	{
		issearching[id] = true
		//icon_controller(id, ICON_FLASH)
		displaylevel(id, 4)
		new counter = parm[1]
		while(counter >= 0)
		{
			counter -= 10
			if(counter == 0)
				emit_sound(id,CHAN_ITEM, "turret/tu_ping.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
		}

		--parm[1]

		if(parm[1] > 0 && get_user_health(id) > 0)
		{
			set_task(0.1, "searchtargetflash", 200 + id, parm, 2)
		}
		else
		{
			issearching[id] = false
			//icon_controller(id, ICON_SHOW)
			displaylevel(id, 4)
		}
	}
}

// Ultimate Ability: Decoy Spawn - Spawns 2 decoys to fool your enemies
public ultimate_wcdecoy(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_DECOY])
	{
		client_print(id, print_chat, "You do not know the Decoy ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id, "Ultimates are disabled for the^nfirst %d seconds of the round",get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("UW_DEBUG") == 6)
	{
		new name [32]
		get_user_name(id, name, 31)
		log_amx("DEBUG: In ultimate_wcdecoy() <%s> invoked Decoy", name)
	}

	new arg[64]
	new Float:arg2[3]
	new vec[3]
	new aimvec[3]
	new Float:b_orig[3]

	if(decoy[id])
	{
		client_print(id, print_chat, "[%s] You cannot have more than one decoy at a time", MOD)
		return PLUGIN_HANDLED
	}

	if(decoyused[id])
	{
		client_print(id, print_chat, "[%s] You may only create 1 decoy per round", MOD)
		return PLUGIN_HANDLED
	}

	if(!is_user_alive(id))
	{
		client_print(id, print_chat, "[%s] Shhhh, you are dead, act like it.", MOD)
		return PLUGIN_HANDLED
	}

	if(skinchanged[id])
	{
		client_print(id, print_chat, "[%s] You are currently a Goombah and unable to create a decoy.", MOD)
		return PLUGIN_HANDLED
	}


	get_user_origin(id, vec)
	get_user_origin(id, aimvec, 2)

	// [07-20-04] - Fix for people spawning decoys on other players to trap them
	new sClass[32], target, body

	get_user_aiming(id, target, body)

	if(0 < target <= MAX_PLAYERS)
	{
		entity_get_string(target, EV_SZ_classname, sClass, 32)

		#if defined DEBUG
		client_print(id, print_console, "[%s DEBUG] Decoy triggered on class %s", MOD, sClass)
		#endif

		if(equal(sClass, "player"))
		{
			client_print(id, print_chat, "[%s] You cannot create a Decoy on a player.", MOD)
			return PLUGIN_HANDLED
		}
	}

	b_orig[0] = float(aimvec[0]);
	b_orig[1] = float(aimvec[1]);
	b_orig[2] = float(aimvec[2] + 35);

	entity_get_string(id, EV_SZ_classname, arg, 63)
	//client_print(id, print_chat, "Your class: %s", arg)
	new decoyEnt = create_entity("info_target")
	entity_set_string(decoyEnt, EV_SZ_classname, "decoy_entity")

	entity_set_origin(decoyEnt, b_orig)

	entity_set_byte(decoyEnt, EV_BYTE_controller1, 127)

	entity_get_vector(id,EV_VEC_absmin,arg2)
	entity_set_vector(decoyEnt, EV_VEC_absmin, arg2)
	entity_get_vector(id,EV_VEC_absmax,arg2)
	entity_set_vector(decoyEnt, EV_VEC_absmax, arg2)
	entity_get_vector(id,EV_VEC_mins,arg2)
	entity_set_vector(decoyEnt, EV_VEC_mins, arg2)
	entity_get_vector(id,EV_VEC_maxs,arg2)
	entity_set_vector(decoyEnt, EV_VEC_maxs, arg2)
	entity_get_vector(id, EV_VEC_size,arg2)
	entity_set_vector(decoyEnt, EV_VEC_size, arg2)

	new model[32]
	new pmodel[128]

	// [10-08-04] Check for Goombah before setting decoy skin - K2mia
	if(skinchanged[id])
	{
		format(pmodel, 127, "models/player/zombie/zombie.mdl")
	}
	else
	{
		get_user_info(id, "model", model, 31)
		format(pmodel, 127, "models/player/%s/%s.mdl", model, model)
	}

	entity_set_model(decoyEnt, pmodel)

	entity_set_float(decoyEnt, EV_FL_health, 99999200.0)
	entity_set_float(decoyEnt, EV_FL_takedamage, 1.0)
	entity_set_float(decoyEnt, EV_FL_dmg_take, 1.0)

	entity_set_int(decoyEnt, EV_INT_movetype, 13)
	entity_set_int(decoyEnt, EV_INT_solid, 2)
	entity_set_int(decoyEnt, EV_INT_spawnflags, 0)
	entity_set_int(decoyEnt, EV_INT_sequence, 1)

	decoy[id] = decoyEnt
	decoy_frame[id] = 0.0
	decoy_seq[id] = 0
	decoyused[id] = true

	new cargs[2]

	cargs[0] = id
	cargs[1] = decoyEnt

	set_task(15.0, "cleanup_decoy", 866, cargs, 2)

	return PLUGIN_HANDLED
}

public cleanup_decoy(args[])
{
	new id = args[0]
	new ent = args[1]

	new Float:orig[3]

	entity_get_vector(ent, EV_VEC_origin, orig)
	message_begin(MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte(11)
	write_coord(floatround(orig[0]))
	write_coord(floatround(orig[1]))
	write_coord(floatround(orig[2]))
	message_end()

	decoy[id] = 0

	if(pev_valid(ent))
		remove_entity(ent)

	client_print(id, print_chat, "Your decoy has expired")
}

// Ultimate Ability: Gate - Opens a getway to teleport player back home
public ultimate_wcgate(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_skills[id][SKILLIDX_GATE])
	{
		client_print(id, print_chat, "You do not know the Gate ultimate yet")
		return PLUGIN_HANDLED
	}

	if(freezetime == 1)
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"You may not use your ultimate during freezetime")
		return PLUGIN_HANDLED
	}

	if(u_delay[id])
	{
		set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2, 3)
		show_hudmessage(id,"Ultimates are disabled for the^nfirst %d seconds of the round",get_cvar_num("FT_ultimatedelay"))
		return PLUGIN_HANDLED
	}

	if(get_cvar_num("UW_DEBUG") == 6)
	{
		new name[32]
		get_user_name(id, name, 31)
		log_amx("DEBUG: In ultimate_wcgate() <%s> invoked Gate", name)
	}

	if(!is_user_alive(id))
	{
		client_print(id, print_chat, "[%s] Shhhh, you are dead, act like it.", MOD)
		return PLUGIN_HANDLED
	}
	else if(numgates[id] == 0)
	{
		client_print(id, print_chat, "[%s] You have used up your available Gateways this round.", MOD)
		return PLUGIN_HANDLED
	}
	else if(ultimateused[id])
	{
		client_print(id, print_chat, "[%s] Ultimate delay now in effect, please wait.", MOD)
		return PLUGIN_HANDLED
	}

	// Call the front end to the gate routine
	do_gate(id)

	return PLUGIN_HANDLED
}


do_gate(id)
{
	new numplayers = 0
	new Float:vicinity = 96.0
	new entsFound[1]
	new Float:origin[3]
	new porigin[3]
	new szEnt[32]
	new bool:spotOpen = false
	new team = get_user_team(id)

	if(team == TEAM_T)
		copy(szEnt, 31, "info_player_deathmatch")
	else if(team == TEAM_CT)
		copy(szEnt, 31, "info_player_start")
	else
		return

	new curEnt = find_ent_by_class(-1, szEnt);

	while(curEnt > 0)
	{
		numplayers = 0

		entity_get_vector( curEnt, EV_VEC_origin, origin)
		numplayers = find_sphere_class(0, "player", vicinity, entsFound, 1, origin)

		if(numplayers == 0)
		{
			spotOpen = true
			break
		}

		curEnt = find_ent_by_class(curEnt, szEnt);
	}

	if(spotOpen)
	{
		porigin[0] = floatround(origin[0])
		porigin[1] = floatround(origin[1])
		porigin[2] = floatround(origin[2])

		new parm2[6]
		parm2[0] = porigin[0]
		parm2[1] = porigin[1]
		parm2[2] = porigin[2]
		parm2[3] = 0
		parm2[4] = team
		parm2[5] = id
		set_task(0.1, "transport_gate", 2, parm2, 6)

		// [09-05-04] - Display gate sprite(tele) 1 cycle
		new idorigin[3]
		get_user_origin(id, idorigin)

		message_begin(MSG_ALL, SVC_TEMPENTITY)
		write_byte(TE_SPRITE)
		write_coord( idorigin[0] )
		write_coord( idorigin[1] )
		write_coord( idorigin[2] )
		write_short( gatespr )
		write_byte( 10 )
		write_byte( 255 )
		message_end()
	}
	#if defined SKILL_PRINT_MESSAGE
	else
	{
		client_print(id, print_chat, "[%s] Your gateway failed to open", MOD)
	}
	#endif
}

// Opens a Gateway to Teleport the player back home
public transport_gate(parm[6])
{
	new id = parm[5]
	new origin[3]
	new oldLocation[3]
	new Float:forigin[3]
	new Float:vicinity = 96.0
	new entsFound[1]

	origin[0] = parm[0]
	origin[1] = parm[1]
	origin[2] = parm[2]

	forigin[0] = float(parm[0])
	forigin[1] = float(parm[1])
	forigin[2] = float(parm[2])

	// Perform a final check to make sure this MOLE spot wasn't taken
	new numplayers = find_sphere_class(0, "player", vicinity, entsFound, 1, forigin)

	if(numplayers > 0)
	{
		#if defined SKILL_PRINT_MESSAGE
		client_print(id, print_chat, "[%s] An unknown force causes your gateway to collapse", MOD)
		#endif
		return
	}

	--numgates[id]

	#if defined SKILL_PRINT_MESSAGE
	client_print(id, print_chat, "[%s] Your gateway opens and is ready to take you home", MOD)
	#endif

	ultimateused[id] = true
	new cooldownparm[1]
	cooldownparm[0] = id

	set_task(get_cvar_float("FT_teleport_cooldown"), "cooldown", 500 + id, cooldownparm, 1)

	get_user_origin(id, oldLocation)
	oldLocation[2] += 30

	if(file_exists("sound/warcraft3/massteleporttarget.wav"))
		emit_sound(id, CHAN_ITEM, "warcraft3/massteleporttarget.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
	else
		emit_sound(id, CHAN_ITEM, "x/x_shoot1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

	// [09-05-04] - Display gate sprite(tele) 1 cycle
	message_begin(MSG_ALL, SVC_TEMPENTITY)
	write_byte(TE_SPRITE)
	write_coord(origin[0])
	write_coord(origin[1])
	write_coord(origin[2])
	write_short(gatespr)
	write_byte(10)
	write_byte(255)
	message_end()

	// blast circles
	message_begin(MSG_PAS, SVC_TEMPENTITY, oldLocation)
	write_byte(TE_BEAMCYLINDER)
	write_coord(oldLocation[0])
	write_coord(oldLocation[1])
	write_coord(oldLocation[2] + 10)
	write_coord(oldLocation[0])
	write_coord(oldLocation[1])
	write_coord(oldLocation[2] + 10 + TELEPORT_RADIUS)
	write_short(m_iSpriteTexture)
	write_byte(0) // startframe
	write_byte(0) // framerate
	write_byte(3) // life
	write_byte(60)  // width
	write_byte(0) // noise
	write_byte(255)  // red
	write_byte(255)  // green
	write_byte(255)  // blue
	write_byte(255) //brightness
	write_byte(0) // speed
	message_end()

	set_user_origin(id,origin)

	if(file_exists("sound/warcraft3/blinkarrival.wav"))
		emit_sound(id,CHAN_STATIC, "warcraft3/blinkarrival.wav", 1.0,ATTN_NORM, 0, PITCH_NORM)
	else
		emit_sound(id,CHAN_STATIC, "x/x_shoot1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)

	message_begin(MSG_ONE, gmsgShake, {0, 0, 0}, id)
	write_short(255<<14) //ammount
	write_short(10<<14) //lasts this long
	write_short(255<<14) //frequency
	message_end()
}

// Ultimate abilities cooldown routines
public cooldown(parm[1])
{
	new id = parm[0]

	if(get_cvar_num("sv_restrictultimate") == 0)
	{
		ultimateused[id] = false
		//icon_controller(id, ICON_SHOW)
		displaylevel(id, 4)
	}
}

public cooldown2(parm[1])
{
	new id = parm[0]

	if(p_skills[id][SKILLIDX_TELEPORT])
	{
		blinkdelayed[id] = false
		//icon_controller(id, ICON_SHOW)
		displaylevel(id, 4)
	}
}

public udelay_cooldown(DELAY)
{
	new id = DELAY - UTLIMATE_DELAY
	u_delay[id] = false

	displaylevel(id, 4)
	//icon_controller(id, ICON_SHOW)
}