// Check if mp_sqp is set and load sql config file if so
public sql_init()
{
	new type = get_cvar_num("mp_sql")
	if(type)
	{
		new sql_cfgfile[64]
		get_configsdir(sql_cfgfile, 63)
		format(sql_cfgfile, 63, "%s/UWC3/UWC3_sql.cfg", sql_cfgfile)

		if(file_exists(sql_cfgfile))
		{
			server_cmd("exec %s", sql_cfgfile)
			server_exec();

			#if defined SQLITEX
			if(type == 2)
			{
				if(LibraryExists("sqlite", LibType_Library))
				{
					new get_type[12], set_type[12];
					set_type = "sqlite";

					SQL_GetAffinity(get_type, 12);

					if(!equali(get_type, set_type))
					{
						if (!SQL_SetAffinity(set_type))
						{
							log_amx("Failed to set affinity from %s to %s.", get_type, set_type)
						}
					}
				}
				else
					// Has to think of an better line.
					log_amx("[%s] You have it currently set to sqlite but sqlite module isn't loaded", MOD)
			}
			#endif

			Tuple = _sql_init_conn();
			_sql_create_table();

			if(Tuple)
				set_task(165.0, "saveall", 93847, _, _, "b");
		}
		else
		{
			set_cvar_num("mp_savexp", 0)
			set_cvar_num("mp_sql", 0)
			log_amx("UWC3 SQL :: Configuration File NOT FOUND, Switching to Fast XP Mode")
		}
	}
}

public saveall() // Auto save does not have support for vault.
{
	if(!warcraft3)
		return;

	if(!get_cvar_num("mp_sql") || get_cvar_num("mp_sql_saveoncmdonly"))
		return;

	static players[32], num, index;
	get_players(players, num, "ch");

	for(new i = 0; i < num; i++)
	{
		index = players[i];

		if(playerxp[index] > 150)
		{
			sqlwrite_xp(index);
			sqlwrite_skills_id(index, -1, 1)
			sqlwrite_enh_id(index, 1)
		}
	}
}

Handle:_sql_init_conn()
{
	get_cvar_string("UW_sql_host", sqlhost, 63);
	get_cvar_string("UW_sql_user", sqluser, 31);
	get_cvar_string("UW_sql_pass", sqlpass, 31);
	get_cvar_string("UW_sql_db", sqldb, 1023);

	return SQL_MakeDbTuple(sqlhost, sqluser, sqlpass, sqldb);
}

Handle:_sql_create_conn() // To make another connection if lost connection during query.
{
	new Handle:Connection = SQL_Connect(Tuple, ErrorCode, Error, 255);

	if(!Connection)
	{
		log_amx("[SQLX] Database Connection Failed: [%d] %s", ErrorCode, Error)
		return Empty_Handle;
	}
	return Connection;
}

public _sql_create_table()
{
	Conn = _sql_create_conn();

	if(!Conn)
		return;

	#if defined SQLITEX
	if(!sqlite_TableExists(Conn, sqltable_xp))
		execute_query(Conn, "CREATE TABLE %s (name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, xp INT, ip VARCHAR(32), mtime VARCHAR(32), PRIMARY KEY(steamid))", sqltable_xp)

	if(!sqlite_TableExists(Conn, sqltable_skills1))
		execute_query(Conn, "CREATE TABLE %s (name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill1 SMALLINT, skill2 SMALLINT, skill3 SMALLINT, skill4 SMALLINT, skill5 SMALLINT, skill6 SMALLINT, skill7 SMALLINT, skill8 SMALLINT, skill9 SMALLINT, skill10 SMALLINT, PRIMARY KEY(steamid, nset))", sqltable_skills1)

	if(!sqlite_TableExists(Conn, sqltable_skills2))
		execute_query(Conn, "CREATE TABLE %s (name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill11 SMALLINT, skill12 SMALLINT, skill13 SMALLINT, skill14 SMALLINT, skill15 SMALLINT, skill16 SMALLINT, skill17 SMALLINT, skill18 SMALLINT, skill19 SMALLINT, skill20 SMALLINT, PRIMARY KEY(steamid, nset))", sqltable_skills2)

	if(!sqlite_TableExists(Conn, sqltable_skills3))
		execute_query(Conn, "CREATE TABLE %s (name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill21 SMALLINT, skill22 SMALLINT, skill23 SMALLINT, skill24 SMALLINT, skill25 SMALLINT, skill26 SMALLINT, skill27 SMALLINT, skill28 SMALLINT, skill29 SMALLINT, skill30 SMALLINT, PRIMARY KEY(steamid, nset))", sqltable_skills3)

	if(!sqlite_TableExists(Conn, sqltable_skills4))
		execute_query(Conn, "CREATE TABLE %s (name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill31 SMALLINT, skill32 SMALLINT, skill33 SMALLINT, skill34 SMALLINT, skill35 SMALLINT, skill36 SMALLINT, skill37 SMALLINT, skill38 SMALLINT, skill39 SMALLINT, skill40 SMALLINT, PRIMARY KEY(steamid, nset))", sqltable_skills4)

	if(!sqlite_TableExists(Conn, sqltable_enh))
		execute_query(Conn, "CREATE TABLE %s (name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, att1 SMALLINT, att2 SMALLINT, att3 SMALLINT, att4 SMALLINT, att5 SMALLINT, res1 SMALLINT, res2 SMALLINT, res3 SMALLINT, res4 SMALLINT, res5 SMALLINT, PRIMARY KEY(steamid, nset)) ", sqltable_enh)
	#else
	new SAVEBY[8]
	format(SAVEBY, 7, "%s", (get_cvar_num("mp_sql_saveby") == SQL_SAVEBY_NAME) ? "name" : "steamid")

	// Create the uwc3_xp table if necessary
	execute_query(Conn, "CREATE TABLE IF NOT EXISTS %s(name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, xp INT, ip VARCHAR(32), mtime VARCHAR(32), PRIMARY KEY(%s))", sqltable_xp, SAVEBY)
	execute_query(Conn, "CREATE TABLE IF NOT EXISTS %s(name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill1 SMALLINT, skill2 SMALLINT, skill3 SMALLINT, skill4 SMALLINT, skill5 SMALLINT, skill6 SMALLINT, skill7 SMALLINT, skill8 SMALLINT, skill9 SMALLINT, skill10 SMALLINT, PRIMARY KEY(%s, nset))", sqltable_skills1, SAVEBY)
	execute_query(Conn, "CREATE TABLE IF NOT EXISTS %s(name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill11 SMALLINT, skill12 SMALLINT, skill13 SMALLINT, skill14 SMALLINT, skill15 SMALLINT, skill16 SMALLINT, skill17 SMALLINT, skill18 SMALLINT, skill19 SMALLINT, skill20 SMALLINT, PRIMARY KEY(%s, nset))", sqltable_skills2, SAVEBY)
	execute_query(Conn, "CREATE TABLE IF NOT EXISTS %s(name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill21 SMALLINT, skill22 SMALLINT, skill23 SMALLINT, skill24 SMALLINT, skill25 SMALLINT, skill26 SMALLINT, skill27 SMALLINT, skill28 SMALLINT, skill29 SMALLINT, skill30 SMALLINT, PRIMARY KEY(%s, nset))", sqltable_skills3, SAVEBY)
	execute_query(Conn, "CREATE TABLE IF NOT EXISTS %s(name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill31 SMALLINT, skill32 SMALLINT, skill33 SMALLINT, skill34 SMALLINT, skill35 SMALLINT, skill36 SMALLINT, skill37 SMALLINT, skill38 SMALLINT, skill39 SMALLINT, skill40 SMALLINT, PRIMARY KEY(%s, nset))", sqltable_skills4, SAVEBY)
	execute_query(Conn, "CREATE TABLE IF NOT EXISTS %s(name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, att1 SMALLINT, att2 SMALLINT, att3 SMALLINT, att4 SMALLINT, att5 SMALLINT, res1 SMALLINT, res2 SMALLINT, res3 SMALLINT, res4 SMALLINT, res5 SMALLINT, PRIMARY KEY(%s, nset)) ", sqltable_enh, SAVEBY)

	// Set Key
	execute_query(Conn, "ALTER TABLE %s DROP PRIMARY KEY, ADD PRIMARY KEY(%s)", sqltable_xp, SAVEBY);
	execute_query(Conn, "ALTER TABLE %s DROP PRIMARY KEY, ADD PRIMARY KEY(%s, nset)", sqltable_skills1, SAVEBY) // NON SQLITE COMPLIANT
	execute_query(Conn, "ALTER TABLE %s DROP PRIMARY KEY, ADD PRIMARY KEY(%s, nset)", sqltable_skills2, SAVEBY) // NON SQLITE COMPLIANT
	execute_query(Conn, "ALTER TABLE %s DROP PRIMARY KEY, ADD PRIMARY KEY(%s, nset)", sqltable_skills3, SAVEBY) // NON SQLITE COMPLIANT
	execute_query(Conn, "ALTER TABLE %s DROP PRIMARY KEY, ADD PRIMARY KEY(%s, nset)", sqltable_skills4, SAVEBY) // NON SQLITE COMPLIANT
	execute_query(Conn, "ALTER TABLE %s DROP PRIMARY KEY, ADD PRIMARY KEY(%s, nset)", sqltable_enh, SAVEBY) // NON SQLITE COMPLIANT

	#endif

	//SQL_FreeHandle(Conn); // Free Connection.
	// Keep connection open for saving at when plugin shutdown.
}

execute_query(Handle:Connection, message[], {Float,Sql,Result,_}:...)
{
	static query[1024];
	vformat(query, sizeof(query)-1, message, 3);

	new Handle:_query

	_query = SQL_PrepareQuery(Connection, query)

	if(!SQL_Execute(_query))
	{
		ErrorCode = SQL_QueryError(_query, Error, 254);
		log_amx("Query failure: [%d] %s", ErrorCode, Error);
		SQL_FreeHandle(_query);

		return;
	}

	SQL_FreeHandle(_query);
}

// Write a player's xp / info to the SQL table
public sqlwrite_xp_id(id)
{
	sqlwrite_xp(id); // To deal with people deleting exp.
}

sqlwrite_xp(id, delete = 0)
{
	if(!warcraft3)
		return;

	if(is_user_bot(id))
		return;

	if(!get_cvar_num("mp_savexp") || !get_cvar_num("mp_sql") || xpreadytoload[id])
	{
		// Do not write XP if it mp_savexp not set or xp hasn't been loaded yet or if mp_sql mode not set
		//log_amx("In write_xp_id() id=(%d) mp_savexp=(%d) xpready=(%d)", id, get_cvar_num("mp_savexp"), xpreadytoload[id])
		return;
	}

	static playerid[32], playername[32], timet[32], ip[32]

	get_time("%m %d %H %M", timet, 31)
	get_user_authid(id, playerid, 31)
	get_user_name(id, playername, 31)
	get_user_ip(id, ip, 31, 1) // This is changed so port is not included. Not sure why anyone would not a port when it can be changed.

	replaceall(playername, 31, "'", "")

	if(!delete && playerxp[id] <= 150) // This will not work if a playerson delete the exp
		return;

	static query[1024]
	format(query, 1023, "REPLACE INTO %s(name, steamid, xp, ip, mtime) VALUES('%s', '%s', '%d', '%s', '%s');", sqltable_xp, playername, playerid, playerxp[id], ip, timet) // SQLITE COMPLIANT

	static data[1]
	data[0] = id

	if(PluginEnding)
		execute_query(Conn, query);
	else
		SQL_ThreadQuery(Tuple, "write_to_sql", query, data, 1);


}

public write_to_sql(failstate, Handle:query, error[], errnum, data[], size)
{
	if (failstate)
	{
		if (failstate == TQUERY_CONNECT_FAILED)
		{
			server_print("Connection failed!")
		} else if (failstate == TQUERY_QUERY_FAILED) {
			server_print("Query failed!")
		}
		log_amx("Error code: %d (Message: ^"%s^")", errnum, error)
		return;
	}
}

// Write a player's enh info(attribs / resists) to the SQL table
public sqlwrite_enh_id(id, nset)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp") || !get_cvar_num("mp_sql"))
	{
		// Do not write XP if it mp_savexp not set or mp_sql not set
		//log_amx("In write_xp_id() id=(%d) mp_savexp=(%d) xpready=(%d)", id, get_cvar_num("mp_savexp"), xpreadytoload[id])
		return PLUGIN_CONTINUE
	}

	if((nset < 1) ||(nset > 4))
		nset = 1

	new playerid[32], playername[32]

	get_user_authid(id, playerid, 31)
	get_user_name(id, playername, 31)

	replaceall(playername, 31, "'", "")

	static query[1024]
	format(query, 1023, "REPLACE INTO %s(name, steamid, nset, att1, att2, att3, att4, att5, res1, res2, res3, res4, res5) VALUES('%s', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d');", sqltable_enh, playername, playerid, nset, p_attribs[id][1], p_attribs[id][2], p_attribs[id][3], p_attribs[id][4], p_attribs[id][5], p_resists[id][1], p_resists[id][2], p_resists[id][3], p_resists[id][4], p_resists[id][5]) // SQLITE COMPLIANT

	static data[1]
	data[0] = id

	if(PluginEnding)
		execute_query(Conn, query);
	else
		SQL_ThreadQuery(Tuple, "write_to_sql", query, data, 1);

	return PLUGIN_CONTINUE
}

// Write a player's enh info(attribs / resists) to the SQL table
public sqlwrite_skills_id(id, sidx, nset)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp") || !get_cvar_num("mp_sql"))
	{
		// Do not write XP if it mp_savexp not set or mp_sql not set
		//log_amx("In write_xp_id() id=(%d) mp_savexp=(%d) xpready=(%d)", id, get_cvar_num("mp_savexp"), xpreadytoload[id])
		return PLUGIN_CONTINUE
	}

	new playerid[32], playername[32]

	get_user_authid(id, playerid, 31)
	get_user_name(id, playername, 31)

	replaceall(playername, 31, "'", "")

	static query[1024]
	static data[1]
	data[0] = id

	format(query, 1023, "REPLACE INTO %s (name, steamid, nset, skill1, skill2, skill3, skill4, skill5, skill6, skill7, skill8, skill9, skill10) VALUES('%s', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d');", sqltable_skills1, playername, playerid, nset, p_skills[id][1],  p_skills[id][2],  p_skills[id][3],  p_skills[id][4],  p_skills[id][5],  p_skills[id][6],  p_skills[id][7],  p_skills[id][8],  p_skills[id][9],  p_skills[id][10]);

	if(PluginEnding)
		execute_query(Conn, query);
	else
		SQL_ThreadQuery(Tuple, "write_to_sql", query, data, 1);

	format(query, 1023, "REPLACE INTO %s (name, steamid, nset, skill11, skill12, skill13, skill14, skill15, skill16, skill17, skill18, skill19, skill20) VALUES('%s', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d');", sqltable_skills2, playername, playerid, nset, p_skills[id][11],  p_skills[id][12],  p_skills[id][13],  p_skills[id][14],  p_skills[id][15],  p_skills[id][16],  p_skills[id][17],  p_skills[id][18],  p_skills[id][19],  p_skills[id][20]);

	if(PluginEnding)
		execute_query(Conn, query);
	else
		SQL_ThreadQuery(Tuple, "write_to_sql", query, data, 1);

	format(query, 1023, "REPLACE INTO %s (name, steamid, nset, skill21, skill22, skill23, skill24, skill25, skill26, skill27, skill28, skill29, skill30) VALUES('%s', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d');", sqltable_skills3, playername, playerid, nset, p_skills[id][21],  p_skills[id][22],  p_skills[id][23],  p_skills[id][24],  p_skills[id][25],  p_skills[id][26],  p_skills[id][27],  p_skills[id][28],  p_skills[id][29],  p_skills[id][30]);

	if(PluginEnding)
		execute_query(Conn, query);
	else
		SQL_ThreadQuery(Tuple, "write_to_sql", query, data, 1);

	format(query, 1023, "REPLACE INTO %s (name, steamid, nset, skill31, skill32, skill33, skill34, skill35, skill36, skill37, skill38, skill39, skill40) VALUES('%s', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d');", sqltable_skills4, playername, playerid, nset, p_skills[id][31],  p_skills[id][32],  p_skills[id][33],  p_skills[id][34],  p_skills[id][35],  p_skills[id][36],  p_skills[id][37],  p_skills[id][38],  p_skills[id][39],  p_skills[id][40]);

	if(PluginEnding)
		execute_query(Conn, query);
	else
		SQL_ThreadQuery(Tuple, "write_to_sql", query, data, 1);

	return PLUGIN_CONTINUE
}

// Retrieve a player's xp from the sql server
public sqlget_xp_id(id)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp") || !get_cvar_num("mp_sql"))
		return PLUGIN_CONTINUE

	static playerid[32], playername[64], query[1024]
	static data[1]
	data[0] = id

	get_user_authid(id, playerid, 31)
	get_user_name(id, playername, 31)

	replaceall(playername, 31, "'", "")

	new SELECTBY = (get_cvar_num("mp_sql_saveby") == SQL_SAVEBY_NAME) ? 1 : 0

	format(query, 1023, "SELECT name, steamid, xp, ip, mtime FROM %s WHERE %s = '%s'", sqltable_xp, (SELECTBY) ? "name" : "steamid", (SELECTBY) ? playername : playerid)

	SQL_ThreadQuery(Tuple, "read_from_sql_xp", query, data, 1);
	return PLUGIN_CONTINUE
}

public read_from_sql_xp(failstate, Handle:query, error[], errnum, data[], size)
{
	new id = data[0]
	if (failstate)
	{
		if (failstate == TQUERY_CONNECT_FAILED)
		{
			server_print("Connection failed!")
		} else if (failstate == TQUERY_QUERY_FAILED) {
			server_print("Query failed!")
		}
		log_amx("Error code: %d (Message: ^"%s^")", errnum, error)
		return;
	}

	if(SQL_NumResults(query) > 0)
	{
		playerxp[id] = SQL_ReadResult(query, 2)

		displaylevel(id, 0)
	}
}

// Retrieve a player's enh info(attribs/resists) from the sql server
public sqlget_enh_id(id)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp"))
		return PLUGIN_CONTINUE

	static playerid[32], playername[32], query[1024]
	static data[1]
	data[0] = id

	get_user_authid(id, playerid, 31)
	get_user_name(id, playername, 31)

	replaceall(playername, 31, "'", "")

	new SELECTBY = (get_cvar_num("mp_sql_saveby") == SQL_SAVEBY_NAME) ? 1 : 0

	format(query, 1023, "SELECT name, steamid, att1, att2, att3, att4, att5, res1, res2, res3, res4, res5 FROM %s WHERE %s = '%s'", sqltable_enh, (SELECTBY) ? "name" : "steamid", (SELECTBY) ? playername : playerid)
	SQL_ThreadQuery(Tuple, "read_from_sql_enh", query, data, 1);

	return PLUGIN_CONTINUE
}

public read_from_sql_enh(failstate, Handle:query, error[], errnum, data[], size)
{
	new index = data[0];
	if (failstate)
	{
		if (failstate == TQUERY_CONNECT_FAILED)
		{
			server_print("Connection failed!")
		} else if (failstate == TQUERY_QUERY_FAILED) {
			server_print("Query failed!")
		}
		client_print(index, print_chat, "[%s] Error Retrieving Your SQL Data: [ %s ]", MOD, error)
		log_amx("Error code: %d (Message: ^"%s^")", errnum, error)

		return;
	}

	new att[6], res[6]

	if(SQL_NumResults(query) > 0)
	{
		att[1] = SQL_ReadResult(query, 2)
		att[2] = SQL_ReadResult(query, 3)
		att[3] = SQL_ReadResult(query, 4)
		att[4] = SQL_ReadResult(query, 5)
		att[5] = SQL_ReadResult(query, 6)
		res[1] = SQL_ReadResult(query, 7)
		res[2] = SQL_ReadResult(query, 8)
		res[3] = SQL_ReadResult(query, 9)
		res[4] = SQL_ReadResult(query, 10)
		res[5] = SQL_ReadResult(query, 11)

		p_attribs[index] = att
		p_resists[index] = res

		//log_amx("UWC3 SQL :: Attrib/Resist Table Loaded: name=(%s) steamid=(%s) Attribs=(%d %d %d %d %d) Resists=(%d %d %d %d %d)", name, pid, att[1], att[2], att[3], att[4], att[5], res[1], res[2], res[3], res[4], res[5])
	}
	else
	{
		new playername[32];
		get_user_name(index, playername, 31);
		client_print(index, print_chat, "[%s] No Saved Attribute/Resistance SQL Data Found for Player %s", MOD, playername)
	}
}

// Retrieve a player's skills info from the sql server
public sqlget_skills_id(id)
{
	if(!warcraft3)
		return PLUGIN_HANDLED

	if(!get_cvar_num("mp_savexp"))
		return PLUGIN_CONTINUE

	static playerid[32], playername[32], query[1024]

	get_user_authid(id, playerid, 31)
	get_user_name(id, playername, 31)

	replaceall(playername, 31, "'", "")

	static data[2]
	data[0] = id

	new SELECTBY = (get_cvar_num("mp_sql_saveby") == SQL_SAVEBY_NAME) ? 1 : 0

	data[1] = 0
	format(query, 1023, "SELECT name, steamid, skill1, skill2, skill3, skill4, skill5, skill6, skill7, skill8, skill9, skill10 FROM %s WHERE %s = '%s'", sqltable_skills1, (SELECTBY) ? "name" : "steamid", (SELECTBY) ? playername : playerid)
	SQL_ThreadQuery(Tuple, "read_from_sql_skill", query, data, 2);

	data[1] = 10
	format(query, 1023, "SELECT name, steamid, skill11, skill12, skill13, skill14, skill15, skill16, skill17, skill18, skill19, skill20 FROM %s WHERE %s = '%s'", sqltable_skills2, (SELECTBY) ? "name" : "steamid", (SELECTBY) ? playername : playerid)
	SQL_ThreadQuery(Tuple, "read_from_sql_skill", query, data, 2);

	data[1] = 20
	format(query, 1023, "SELECT name, steamid, skill21, skill22, skill23, skill24, skill25, skill26, skill27, skill28, skill29, skill30 FROM %s WHERE %s = '%s'", sqltable_skills3, (SELECTBY) ? "name" : "steamid", (SELECTBY) ? playername : playerid)
	SQL_ThreadQuery(Tuple, "read_from_sql_skill", query, data, 2);

	data[1] = 30
	format(query, 1023, "SELECT name, steamid, skill31, skill32, skill33, skill34, skill35, skill36, skill37, skill38, skill39, skill40 FROM %s WHERE %s = '%s'", sqltable_skills4, (SELECTBY) ? "name" : "steamid", (SELECTBY) ? playername : playerid)
	SQL_ThreadQuery(Tuple, "read_from_sql_skill", query, data, 2);

	return PLUGIN_CONTINUE
}

public read_from_sql_skill(failstate, Handle:query, error[], errnum, data[], size)
{
	new index = data[0];
	new _index = data[1];
	if (failstate)
	{
		if (failstate == TQUERY_CONNECT_FAILED)
		{
			server_print("Connection failed!")
		} else if (failstate == TQUERY_QUERY_FAILED) {
			server_print("Query failed!")
		}
		client_print(index, print_chat, "[%s] Error Retrieving Your Skills SQL Data: [ %s ]", MOD, error)

		log_amx("Error code: %d (Message: ^"%s^")", errnum, error)
		return;
	}

	if(SQL_NumResults(query) > 0)
	{
		p_skills[index][_index + 1] = SQL_ReadResult(query, 2)
		p_skills[index][_index + 2] = SQL_ReadResult(query, 3)
		p_skills[index][_index + 3] = SQL_ReadResult(query, 4)
		p_skills[index][_index + 4] = SQL_ReadResult(query, 5)
		p_skills[index][_index + 5] = SQL_ReadResult(query, 6)
		p_skills[index][_index + 6] = SQL_ReadResult(query, 7)
		p_skills[index][_index + 7] = SQL_ReadResult(query, 8)
		p_skills[index][_index + 8] = SQL_ReadResult(query, 9)
		p_skills[index][_index + 9] = SQL_ReadResult(query, 10)
		p_skills[index][_index + 10] = SQL_ReadResult(query, 11)
	}
	else
	{
		new playername[32]
		get_user_name(index, playername, 31);
		client_print(index, print_chat, "[%s] No Saved Skills SQL Data Found for Player %s", MOD, playername);
		return;
	}

	for(new i = 1; i < MAX_SKILLS; i++)
	{
		if(p_skills[index][i] >= 1 && skill_ultimates[i][0])
		{
			// This is an Ultimate ability being retrieved from saved skills
			client_print(index, print_console, "[%s] Retrieved saved ultimate(%s)^n", MOD, skill_ultimates[i][0])
			ultlearned[index]++
		}
	}

	//new tstr[512]
	//new len

	//len += format(tstr[len], 512-len, "UWC3 SQL :: Skills Table Loaded: name=(%s) steamid=(%s) Skills=(%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ", name, pid, skills[1], skills[2], skills[3], skills[4], skills[5], skills[6], skills[7], skills[8], skills[9], skills[10], skills[11], skills[12], skills[13], skills[14], skills[15], skills[16], skills[17], skills[18], skills[19], skills[20])
	//len += format(tstr[len], 512-len, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d)", skills[21], skills[22], skills[23], skills[24], skills[25], skills[26], skills[27], skills[28], skills[29], skills[30], skills[31], skills[32], skills[33], skills[34], skills[35], skills[36], skills[37], skills[38], skills[39], skills[40])

	//log_amx("%s", tstr)

//	dbi_close(sql)
}