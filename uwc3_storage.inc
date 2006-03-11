
/** *************************************************************************
*** AMXX Plugin:   Ultimate Warcraft3 (UWC3)
*** Plugin Author: SGR
*** UWC3 Module:   SQL Storage
*** Date:          May 25, 2004
*** Last Update:   September 24, 2004
*
*  Module:  SQL Storage
*  Subroutines to handle saving XP / Resists / Attribs
*----------------------------------------------------------------------------
*
*  UWC3 is written exclusively for AMX Mod X
*
*  Ultimate Warcraft3 Dev. Team
*  ------------------------------
*   Small scripting:  SGR ( Alex Forbes ) < SGR@dumbclan.com >
*   Small scripting:  K2mia ( Andrew Cowan ) < admin@mudconnect.com >
*   Graphics:         steve french < garbageweed@hotmail.com >
*   Website Design:   Jim Rhoades < jim@radzone.org >
*
****************************************************************************/

// **************************************************************************
// BEGIN check_sql and grab_sql_vars subroutines
// Check if mp_sqp is set and load sql config file if so
// **************************************************************************

public check_sql(){

   new basedir[64]
   new sql_cfgfile[64]
   get_customdir(basedir, 63)

   format(sql_cfgfile, 63, "%s/UWC3/UWC3_sql.cfg", basedir)
   if (file_exists(sql_cfgfile)){
       log_amx("UWC3 SQL :: Loading Configuration File...")
       server_cmd("exec %s", sql_cfgfile)
       log_amx("UWC3 SQL :: Configuration File Loaded [OK]")
       set_task(0.6, "grab_sql_vars", 824)
   }
   else {
         log_amx("UWC3 SQL :: No Configuration File -- NOT SAVING XP...")
         set_cvar_num("mp_savexp", 0);
   }
   return PLUGIN_CONTINUE
}

public grab_sql_vars(){

   get_cvar_string("UW_sql_host", ucw3_sqlhost, 63)
   get_cvar_string("UW_sql_user", ucw3_sqluser, 31)
   get_cvar_string("UW_sql_pass", ucw3_sqlpass, 31)
   get_cvar_string("UW_sql_db",   ucw3_sqldb, 31)

   log_amx("UWC3 SQL :: Config Report: host=(%s) user=(%s) pass=(*) db=(%s)", ucw3_sqlhost, ucw3_sqluser, ucw3_sqldb)

   sqlconn = dbi_connect(ucw3_sqlhost, ucw3_sqluser, ucw3_sqlpass, ucw3_sqldb, ucw3_sqlerror, 127)

   if (sqlconn <= SQL_FAILED) {
       log_amx("UWC3 SQL :: Connect Error [ %s ]", ucw3_sqlerror)
       return PLUGIN_HANDLED
   }
   else{
          log_amx("UWC3 SQL :: Connected [OK]" )
   }
   new squery[4096]

   format(squery, 4096, "CREATE TABLE IF NOT EXISTS `%s` (`steamid` VARCHAR(32) NOT NULL, `xp` INT, `ip` ", ucw3_sqltable)
   format(squery, 4096, "%s%s", squery, "VARCHAR(32), `mtime`  VARCHAR(32), `skill1` SMALLINT DEFAULT '0', `skill2` SMALLINT ")
   format(squery, 4096, "%s%s", squery, "DEFAULT '0', `skill3` SMALLINT DEFAULT '0', `skill4` SMALLINT DEFAULT '0', `skill5` ")
   format(squery, 4096, "%s%s", squery, "SMALLINT DEFAULT '0', `skill6` SMALLINT DEFAULT '0', `skill7` SMALLINT DEFAULT '0', ")
   format(squery, 4096, "%s%s", squery, "`skill8` SMALLINT DEFAULT '0', `skill9` SMALLINT DEFAULT '0', `skill10` SMALLINT ")
   format(squery, 4096, "%s%s", squery, "DEFAULT '0', `skill11` SMALLINT DEFAULT '0', `skill12` SMALLINT DEFAULT '0', `skill13` ")
   format(squery, 4096, "%s%s", squery, "SMALLINT DEFAULT '0', `skill14` SMALLINT DEFAULT '0', `skill15` SMALLINT DEFAULT '0', ") 
   format(squery, 4096, "%s%s", squery, "`skill16` SMALLINT DEFAULT '0', `skill17` SMALLINT DEFAULT '0', `skill18` SMALLINT ")
   format(squery, 4096, "%s%s", squery, "DEFAULT '0', `skill19` SMALLINT DEFAULT '0', `skill20` SMALLINT DEFAULT '0', `skill21` ")
   format(squery, 4096, "%s%s", squery, "SMALLINT DEFAULT '0', `skill22` SMALLINT DEFAULT '0', `skill23` SMALLINT DEFAULT '0', ")
   format(squery, 4096, "%s%s", squery, "`skill24` SMALLINT DEFAULT '0', `skill25` SMALLINT DEFAULT '0', `skill26` SMALLINT ")    
   format(squery, 4096, "%s%s", squery, "DEFAULT '0' , `skill27` SMALLINT DEFAULT '0', `skill28` SMALLINT DEFAULT '0', ")
   format(squery, 4096, "%s%s", squery, "`skill29` SMALLINT DEFAULT '0', `skill30` SMALLINT DEFAULT '0', `skill31` SMALLINT ")
   format(squery, 4096, "%s%s", squery, "DEFAULT '0', `skill32` SMALLINT DEFAULT '0', `skill33` SMALLINT  DEFAULT '0', ")
   format(squery, 4096, "%s%s", squery, "`skill34` SMALLINT DEFAULT '0', `skill35` SMALLINT DEFAULT '0', `skill36` SMALLINT ")
   format(squery, 4096, "%s%s", squery, "DEFAULT '0', `skill37` SMALLINT DEFAULT '0', `skill38` SMALLINT DEFAULT '0', ") 
   format(squery, 4096, "%s%s", squery, "`skill39` SMALLINT DEFAULT '0', `skill40` SMALLINT DEFAULT '0', `att1` SMALLINT ")
   format(squery, 4096, "%s%s", squery, "DEFAULT '0', `att2` SMALLINT DEFAULT '0', `att3` SMALLINT DEFAULT '0', `att4` SMALLINT ")
   format(squery, 4096, "%s%s", squery, "DEFAULT '0', `att5` SMALLINT DEFAULT '0', `res1` SMALLINT DEFAULT '0', `res2` SMALLINT ")
   format(squery, 4096, "%s%s", squery, "DEFAULT '0', `res3` SMALLINT DEFAULT '0', `res4` SMALLINT DEFAULT '0', `res5` SMALLINT ")
   format(squery, 4096, "%s%s", squery, "DEFAULT '0', PRIMARY KEY (`steamid`)) TYPE=MyISAM;")

   new Result:Res
   Res = dbi_query(sqlconn, "%s", squery)

   if (Res == RESULT_FAILED) {
      new error[128]
      dbi_error(sqlconn,error,127)
      log_amx("UWC3 SQL :: Error creating table.")
   }
   dbi_free_result(Res)
   return PLUGIN_CONTINUE
}
// **************************************************************************
// END check_sql and grab_sql_vars subroutines
// **************************************************************************

// **************************************************************************
// END sqlwrite_xp_id subroutine
// **************************************************************************

public sqlwrite_info_id(id) {

   if (!warcraft3) {
      return PLUGIN_HANDLED
   }
   if (!get_cvar_num("mp_savexp")){
      return PLUGIN_CONTINUE
   }
   if (sqlconn <= SQL_FAILED) { 
       return PLUGIN_HANDLED 
   }
   if (playerxp[id] <= 150) {
       return PLUGIN_CONTINUE
   }

   new playerid[32]
   new mtimet[32]
   new error[128]
   get_user_authid(id, playerid, 31)
   get_time("%d/%m/%Y %H:%M:%S", mtimet, 31)
   new squery[4096]

   format(squery, 4096, "REPLACE INTO `%s` SET steamid='%s', xp='%d',", ucw3_sqltable, playerid, playerxp[id])
   format(squery, 4096, "%s mtime='%s'  , skill1='%d' , skill2='%d' ,", squery, mtimet, p_skills[id][1], p_skills[id][2]) 
   format(squery, 4096, "%s skill3='%d' , skill4='%d' , skill5='%d' ,", squery,p_skills[id][3], p_skills[id][4], p_skills[id][5])
   format(squery, 4096, "%s skill6='%d' , skill7='%d' , skill8='%d' ,", squery,p_skills[id][6], p_skills[id][7], p_skills[id][8])
   format(squery, 4096, "%s skill9='%d' , skill10='%d', skill11='%d',", squery,p_skills[id][9], p_skills[id][10], p_skills[id][11])
   format(squery, 4096, "%s skill12='%d', skill13='%d', skill14='%d',",squery,p_skills[id][12], p_skills[id][13], p_skills[id][14])
   format(squery, 4096, "%s skill15='%d', skill16='%d', skill17='%d',",squery,p_skills[id][15], p_skills[id][16], p_skills[id][17]) 
   format(squery, 4096, "%s skill18='%d', skill19='%d', skill20='%d',",squery,p_skills[id][18], p_skills[id][19], p_skills[id][20])
   format(squery, 4096, "%s skill21='%d', skill22='%d', skill23='%d',",squery,p_skills[id][21], p_skills[id][22], p_skills[id][23])
   format(squery, 4096, "%s skill24='%d', skill25='%d', skill26='%d',",squery,p_skills[id][24], p_skills[id][25], p_skills[id][26])
   format(squery, 4096, "%s skill27='%d', skill28='%d', skill29='%d',",squery,p_skills[id][27], p_skills[id][28], p_skills[id][29])
   format(squery, 4096, "%s skill30='%d', skill31='%d', skill32='%d',",squery,p_skills[id][30], p_skills[id][31], p_skills[id][32])
   format(squery, 4096, "%s skill33='%d', skill34='%d', skill35='%d',",squery,p_skills[id][33], p_skills[id][34], p_skills[id][35])
   format(squery, 4096, "%s skill36='%d', skill37='%d', skill38='%d',",squery,p_skills[id][36], p_skills[id][37], p_skills[id][38])
   format(squery, 4096, "%s skill39='%d', skill40='%d', att1='%d'   , ",squery,p_skills[id][39], p_skills[id][40], p_attribs[id][1])
   format(squery, 4096, "%s att2='%d'   , att3='%d'   , att4='%d'   ,",squery,p_attribs[id][2], p_attribs[id][3], p_attribs[id][4])
   format(squery, 4096, "%s att5='%d'   , res1='%d'   , res2='%d'   ,",squery,p_attribs[id][5], p_resists[id][1], p_resists[id][2])
   format(squery, 4096, "%s res3='%d'   , res4='%d'   , res5='%d'   ;",squery, p_resists[id][3], p_resists[id][4], p_resists[id][5])
          
   new Result:Res
   Res = dbi_query(sqlconn, "%s", squery)
   if (Res == RESULT_FAILED) {
      dbi_error(sqlconn,error,127)
      client_print(id, print_chat, "[%s] Error Saving Your XP+Skills Data. Sorry :'(", MOD)
      log_amx("UWC3 SQL :: Error Saving Skills SQL Data for %s: %s", playerid, error)
   }
   dbi_free_result(Res)
   return PLUGIN_CONTINUE
}
// **************************************************************************
// END sqlwrite_skills_id subroutine
// **************************************************************************


// **************************************************************************
// BEGIN sqlget_skills_id subroutine
// Retrieve a player's skills info from the sql server
// **************************************************************************
public sqlget_info_id(id)
{

   if (!warcraft3) {
       return PLUGIN_HANDLED
   }
   if (!get_cvar_num("mp_savexp")) {
       return PLUGIN_CONTINUE
   }
   if (sqlconn <= SQL_FAILED) {
       return PLUGIN_HANDLED
   }
   new playerid[32]
   new error[128]
   get_user_authid(id, playerid, 31)

   new Result:Res
   new myselect[4096];
   
   format(myselect, 4096, "SELECT steamid, xp, ip, mtime, skill1, skill2, skill3,")
   format(myselect, 4096, "%s skill4, skill5, skill6, skill7, skill8, skill9, skill10, ", myselect)
   format(myselect, 4096, "%s skill11, skill12, skill13, skill14, skill15, skill16, skill17,", myselect)
   format(myselect, 4096, "%s skill18, skill19, skill20, skill21, skill22, skill23, skill24,", myselect)
   format(myselect, 4096, "%s skill25, skill26, skill27, skill28, skill29, skill30, skill31,", myselect)
   format(myselect, 4096, "%s skill32, skill33, skill34, skill35, skill36, skill37, skill38,", myselect)
   format(myselect, 4096, "%s skill39, skill40, att1, att2, att3, att4, att5, res1, res2,", myselect)
   format(myselect, 4096, "%s res3, res4, res5 FROM %s WHERE steamid = '%s';", myselect, ucw3_sqltable, playerid)
   
   Res = dbi_query(sqlconn,"%s",myselect)

   if (Res == RESULT_FAILED) {
       dbi_error(sqlconn,error,127)
       client_print(id, print_chat, "[%s] Error Retrieving Your XP+Skills Data. Sorry.", MOD)
       log_amx("UWC3 SQL :: Error Retrieving Skills SQL Data for %s: %s", playerid, error)
       dbi_free_result(Res)
       return PLUGIN_CONTINUE
   }
   else if (Res == RESULT_NONE) {
           client_print(id, print_chat, "[%s] No Saved Skills SQL Data Found for you (%s -> %s)", MOD, playerid, playerid )
           dbi_free_result(Res)
           return PLUGIN_CONTINUE
   }
   new ip[32]
   new mtime[32]
   new xp
   new pid[32]
   new skills[41] = 0
   new att[6]
   new res[6]

   if (dbi_nextrow(Res) > 0) {
       dbi_result(Res, "steamid", pid, 31)
       skills[1]  = dbi_result( Res, "skill1" )
       skills[2]  = dbi_result( Res, "skill2" )
       skills[3]  = dbi_result( Res, "skill3" )
       skills[4]  = dbi_result( Res, "skill4" )
       skills[5]  = dbi_result( Res, "skill5" )
       skills[6]  = dbi_result( Res, "skill6" )
       skills[7]  = dbi_result( Res, "skill7" )
       skills[8]  = dbi_result( Res, "skill8" )
       skills[9]  = dbi_result( Res, "skill9" )
       skills[10] = dbi_result( Res, "skill10" )
       skills[11] = dbi_result( Res, "skill11" )
       skills[12] = dbi_result( Res, "skill12" )
       skills[13] = dbi_result( Res, "skill13" )
       skills[14] = dbi_result( Res, "skill14" )
       skills[15] = dbi_result( Res, "skill15" )
       skills[16] = dbi_result( Res, "skill16" )
       skills[17] = dbi_result( Res, "skill17" )
       skills[18] = dbi_result( Res, "skill18" )
       skills[19] = dbi_result( Res, "skill19" )
       skills[20] = dbi_result( Res, "skill20" )
       skills[21] = dbi_result( Res, "skill21" )
       skills[22] = dbi_result( Res, "skill22" )
       skills[23] = dbi_result( Res, "skill23" )
       skills[24] = dbi_result( Res, "skill24" )
       skills[25] = dbi_result( Res, "skill25" )
       skills[26] = dbi_result( Res, "skill26" )
       skills[27] = dbi_result( Res, "skill27" )
       skills[28] = dbi_result( Res, "skill28" )
       skills[29] = dbi_result( Res, "skill29" )
       skills[30] = dbi_result( Res, "skill30" )
       skills[31] = dbi_result( Res, "skill31" )
       skills[32] = dbi_result( Res, "skill32" )
       skills[33] = dbi_result( Res, "skill33" )
       skills[34] = dbi_result( Res, "skill34" )
       skills[35] = dbi_result( Res, "skill35" )
       skills[36] = dbi_result( Res, "skill36" )
       skills[37] = dbi_result( Res, "skill37" )
       skills[38] = dbi_result( Res, "skill38" )
       skills[39] = dbi_result( Res, "skill39" )
       skills[40] = dbi_result( Res, "skill40" )
       att[1] = dbi_result( Res, "att1" )
       att[2] = dbi_result( Res, "att2" )
       att[3] = dbi_result( Res, "att3" )
       att[4] = dbi_result( Res, "att4" )
       att[5] = dbi_result( Res, "att5" )
       res[1] = dbi_result( Res, "res1" )
       res[2] = dbi_result( Res, "res2" )
       res[3] = dbi_result( Res, "res3" )
       res[4] = dbi_result( Res, "res4" )
       res[5] = dbi_result( Res, "res5" )
       xp = dbi_result(Res, "xp" )
       dbi_result(Res, "ip", ip, 31)
       dbi_result(Res, "mtime", mtime, 31)
       playerxp[id] = xp
       displaylevel(id, 0)
       p_attribs[id] = att
       p_resists[id] = res
   }
   p_skills[id] = skills
   for (new i = 1; i < MAX_SKILLS; i++) {
      if ((p_skills[id][i] >= 1) && (skill_ultimates[i][0])) {
           client_print(id, print_console, "[%s] Retrieved saved ultimate (%s)^n", MOD, skill_ultimates[i][0])
           ultlearned[id]++
      }
   }
   dbi_free_result(Res)
   return PLUGIN_CONTINUE
}
// **************************************************************************
// END sqlget_skills_id subroutine
// **************************************************************************

// **************************************************************************
// BEGIN uwc3_save*** routines
// Frontend for saving a player's XP, Attribs, Resists, Skills, etc
// **************************************************************************

public uwc3_saveall(id){

   if (!get_cvar_num("mp_savexp")) {
      client_print(id,print_chat,"[%s] Long term XP mode is NOT enabled now.", MOD)
      return PLUGIN_HANDLED
   }
   sqlwrite_info_id(id)
   client_print(id,print_chat, "[%s] Saving all your UWC3 Data now.", MOD)
   return PLUGIN_HANDLED
}
// **************************************************************************
// END uwc3_save*** subroutines
// **************************************************************************

public uwc3_willsaveon(id)
{
   set_hudmessage(200, 100, 0, -1.0, 0.35, 0, 1.0, 7.0, 0.1, 0.2, 2)
   show_hudmessage(id, "Your XP, Skills, Attributes and Resistances will be saved when you disconnect, and on a map change.")
   client_print(id,print_chat, "[%s] Your XP, Skills, Attributes and Resistances will be saved when you disconnect, and on a map change.",MOD)
   return PLUGIN_CONTINUE
}

public write_all()
{
   if (!warcraft3) {
       return PLUGIN_HANDLED
   }
   if (!get_cvar_num("mp_savexp")){ 
      return PLUGIN_HANDLED
   }
   new players[32]
   new numofplayers
   new id
   new i

   get_players(players, numofplayers)

   for (i=0; i < numofplayers; i++){
        id = players[i]
        sqlwrite_info_id( id )
   }
   return PLUGIN_CONTINUE
}

// **************************************************************************
// END SQL Storage Subroutines
// **************************************************************************

