#include <amxmodx>
#include <uwc3>

new bool:warcraft3


new const theforward[] = "<li>"
new const backward[] = "</li>"
new const lastone[] = "</ul>"

new temp[3076]

public plugin_natives()
{
	register_native("uwc3_commands", "native_uwc3_commands")
	register_native("uwc3_tips", "native_uwc3_tips")
	register_native("uwc3_tips2", "native_uwc3_tips2")
	register_native("war3_info", "native_war3_info")
	register_native("uwc3_attributes", "native_uwc3_attributes")
	register_native("uwc3_resistances", "native_uwc3_resistances")
	register_native("show_news", "native_show_news")
	register_native("uwc3_credits", "native_uwc3_credits")
	register_native("uwc3_ultimates", "native_uwc3_ultimates")
	register_native("items_info", "native_items_info")
	register_native("items_info2", "native_items_info2")
	register_native("change_race", "native_change_race")
	register_library("uwc3")
}

public plugin_init()
{
	register_plugin("Ultimate Warcraft 3 Info", VER, "UWC3 TEAM")
	warcraft3 = get_cvar_num("sv_warcraft3") ? true : false
	pMax = get_maxplayers()


	// Client Command
	register_clcmd("news", "show_new_s", -1, "- Displays news about the Ultimate Warcraft3 plugin")
	register_clcmd("war3help", "war3_info2", -1, "- Displays help information for the UWC3 module")
	register_concmd("playerskills", "player_skills", -1, "-Displays the players rank and skills")
}

/*
 * BEGIN show_news subroutine
 * Display the server news
 */
public native_show_news(id, num)
{
	if(warcraft3)
	{
		new index = get_param(1)

		if(check_player(index))
			return;

		show_new_s(index)
	}
}

public show_new_s(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 3076-pos, "<body scroll='no' bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<table border=0><tr><td><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<center><b>Ultimate Warcraft3</b><br><a href='http://www.uwc3.com/'><b>http://www.uwc3.com/</b></a></center><p>" )
	pos += format(temp[pos], 3076-pos, "<b>Latest News About Ultimate Warcraft3</b><br>")
	pos += format(temp[pos], 3076-pos, "<b>http://forums.uwc3.com/ - See forums for complete ChangeLog</b><br><ul>")
	pos += format(temp[pos], 3076-pos, "<li>[10-30-04] Necklaces no longer protect against serpent wards.<p>")
	pos += format(temp[pos], 3076-pos, "<li>[10-24-04] Testing Teleport Immunity issues bugfix. Testing Mask of Death bugfix. Mirror damage skills/evasion bug fixed. Testing fix for skill/attrib/health known reset issue. Testing bugfix for Vengeance/Phoenix overlap. Fixed Ultimate-status HUD message. Playerskills has been replaced by /who, added /whois playername command to display player character sheets.<p>")
	pos += format(temp[pos], 3076-pos, "<li>[10-10-04] Many run-time errors fixed, thanks to Hellion and Kamikaze for sending their bug reports and error logs!<p>")
	pos += format(temp[pos], 3076-pos, "<li>[9-27-04] Fixed objectives bug which awarded XP to dead players (pffft). Skillcount bug should now be fixed, players with incorrect skillsets will be forced to reset.<p>")
	pos += format(temp[pos], 3076-pos, "<li>[9-23-04] Testing fixes for skillcount being out of sync (auto-reset), insane player health bug. Testing fixes for objective/winning XP award bugs. Version change to v1.0.0RC1 . Fixed /deletexp command. New commands: saveattribs, saveresists, saveskills, saveall<p>")
	pos += format(temp[pos], 3076-pos, "<li>[9-14-04] Serious Injury XP bonus bug fixed. Voodoo end of round bug fixed.<p>")
	pos += format(temp[pos], 3076-pos, "<li>[9-07-04] Big Changes! Attributes and Resistances now have server limits, Beta1 now limits players to 20 attribute pts and 250 resistance pts -- spend them wisely.<p>")
	pos += format(temp[pos], 3076-pos, "<li>[9-06-04] New animations for Napalm hits and new teleport animations for Gate ultimate. steve modified the Carrion Beetles bubble animation. Medic alert gfx, Armorsmith alert gfx added, live testing.<p>")
	pos += format(temp[pos], 3076-pos, "<li>[9-03-04] Crit strike/Trueshot backfire damage modified. Medic alerts now issues in sprite-mode as well as text-mode. UWC3 welcomes 'steve french' to the plugin dev team, and with him his replacement model for the Entangling Roots effect and replacement Carrion Beetles animated sprite, woot!<p>")
	pos += format(temp[pos], 3076-pos, "<li>[8-29-04] Equip Reincarnation fixed for failures due to Bomb Explosion, Defusers should always reincarnate now, grenades may still have some erratic respawn issues. Vengeance ultimate cooldown added, so ultimates will become available again after vengeance-based respawn.<p>")
	pos += format(temp[pos], 3076-pos, "<li>[8-22-04] Testing new bug fix for disease and poison resistance against Carrion Beetles and Shadow Strike<p>")
	pos += format(temp[pos], 3076-pos, "</ul></td></tr></table>")

	show_motd(id, temp, "UWC3 News")

	return PLUGIN_HANDLED
}
/*
 * END show_news subroutine
 */


/*
 * BEGIN uwc3_commands subroutine
 * Display the UWC3 commands list
 */
public native_uwc3_commands(id, num)
{
	if(!warcraft3)
		return;

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 2048-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<center><table border=1 cellpadding=3 cellspacing=3><tr><td><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<center><b>Ultimate Warcraft3</b><br><a href='http://www.uwc3.com/'><b>http://www.uwc3.com/</b></a></center><p>" )
	pos += format(temp[pos], 2048-pos, "<center><b>UWC3 Commands</b></center><p>" )
	pos += format(temp[pos], 2048-pos, "The following commands are available:<p>" )
	pos += format(temp[pos], 2048-pos, "say /war3menu -  Displays the UWC3 main menu <br>" )
	pos += format(temp[pos], 2048-pos, "say /help -  Displays the UWC3 help menu <br>" )
	pos += format(temp[pos], 2048-pos, "say /selectskill - Displays the skill selection menu <br>" )
	pos += format(temp[pos], 2048-pos, "say /selectattrib - Displays the attributes selection menu <br>" )
	pos += format(temp[pos], 2048-pos, "say /selectresist - Displays the resistances selection menu <br>" )
	pos += format(temp[pos], 2048-pos, "say /charactersheet - (also /charsheet) Displays your complete character summary (skills, attributes, resists, etc) <br>" )
	pos += format(temp[pos], 2048-pos, "say /who -  Displays a list of players on the server<br>" )
	pos += format(temp[pos], 2048-pos, "say /whois player_name - Displays the character sheet for the specified player<br>" )
	pos += format(temp[pos], 2048-pos, "say /skillsinfo - Displays information about each skill <br>" )
	pos += format(temp[pos], 2048-pos, "say /resetskills - Resets your skills so you can retrain them <br>" )
	pos += format(temp[pos], 2048-pos, "say /resetattribs - Resets your attributes so you can retrain them <br>" )
	pos += format(temp[pos], 2048-pos, "say /resetresists - Resets your resistances so you can retrain them <br>" )
	pos += format(temp[pos], 2048-pos, "say /deletexp - Deletes all of your XP and skill points <br>" )
	pos += format(temp[pos], 2048-pos, "say /itemsinfo - Displays the list of items in the shopmenu <br>" )
	pos += format(temp[pos], 2048-pos, "say /itemsinfo2 - Displays the list of items in the shopmenu2 <br>" )
	pos += format(temp[pos], 2048-pos, "say /level - Displays your rank and level<br>" )
	pos += format(temp[pos], 2048-pos, "say /toggle_lowres - Toggles the position of ultimate/shopitems HUD messages<br>" )
	pos += format(temp[pos], 2048-pos, "In console: xp_table - Displays the XP/Level table for this server<br>" )
	pos += format(temp[pos], 2048-pos, "</font></td></tr></table></center>")
	show_motd(index, temp, "UWC3 Commands")
}
/*
 * END uwc3_commands subroutine
 */


/*
 * BEGIN uwc3_credits subroutine
 * Display the credits for UWC3 plugin
 */
public native_uwc3_credits(id, num)
{
	if(!warcraft3)
		return;

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 3072-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 3072-pos, "<center><table border=1 cellpadding=3 cellspacing=3><tr><td><font color=#FFB000>")
	pos += format(temp[pos], 3072-pos, "<center><b>Ultimate Warcraft3</b><br><a href='http://www.uwc3.com/'><b>http://www.uwc3.com/</b></a></center><p>" )
	pos += format(temp[pos], 3072-pos, "Ultimate Warcraft3 (UWC3) [%s] Dev Team:<ul><li>K2mia (admin@mudconnect.com) - Small Coding<li>Jim Rhoade (jim@radzone.org) - UWC3 Website<li>steve french (garbageweed@hotmail.com) - Graphic effects</ul><p>", VER)
	pos += format(temp[pos], 3072-pos, "str_break() routine provided by BAILOPAN from AMXX0.20 to replace parse()<br>" )
	pos += format(temp[pos], 3072-pos, "ultimate_decoy() code derived from code for Beyonder superhero (Freecode/Asskicr)<br>some decoy concepts from AMXX forums posts by jjkiller<br>" )
	pos += format(temp[pos], 3072-pos, "Additional Credits: Kamikaze (beta server/bug reporting), Jatsu (beta server/bug reporting),<br>NeKo (Grenade trails code from War3FT)<br>")
	pos += format(temp[pos], 3072-pos, "UWC3 testing: Skat, Burning Angel, k-os (skill tests)<br>")
	pos += format(temp[pos], 3072-pos, "Exploding Bannana, [Cas]Williamz, Spitfire, Hades, Veritas (bug reports)<br>")
	pos += format(temp[pos], 3072-pos, "Thanks to: SpaceDude, Pimp-Daddy, BAILOPAN, PM, AssKicr<br>jjkiller and the members of amxmodx.org forums<br>")
	pos += format(temp[pos], 3072-pos, "bigboom for the 'fullupdate' bug report.<br>")
	pos += format(temp[pos], 3072-pos, "</font></td></tr><tr><td><font color=#FFB000>")
	pos += format(temp[pos], 3072-pos, "<center><b>Warcraft3 and Warcraft3FT Credits</b></center><p>" )
	pos += format(temp[pos], 3072-pos, "Warcraft 3: Frozen Throne by Pimp Daddy (OoTOAoO)<br>[ email: PimpDaddy@cinci.rr.com / MSN: PimpDaddy@cinci.rr.com ]<p>")
	pos += format(temp[pos], 3072-pos, "Spacedude (for War3 MOD)<br>Ludwig Van (for flamethrower)<br>OLO (for spectating rank info)<br>JGHG for the mole code<br>[AOL]Demandred, [AOL]LuckyJ for help coding it for steam<br>[AOL]Demandred for freezetime exploit fix<br>")
	pos += format(temp[pos], 3072-pos, "Denkkar for some of his code (ie. STEAM_POWERED)<br>Everyone at amxmod.net for help<br>joecool12321 for various health related fixes<br>Tri Moon for various improvements (No Race, war3menu, etc...)<br>xeroblood for spotting some bugs for me :)<br>bad-at-this for contributing the status bar code used for godmode (big bad voodoo)<br>")
	pos += format(temp[pos], 3072-pos, "kamikaze for help w/testing version before release<br>lui for the delayed ultimate code<br>The following people helped convert the text to German:<br>Walken / Altegarde.com , Fire , ^^plan.los^^<br>")
	pos += format(temp[pos], 3072-pos, "</font></td></tr></table></center>")
	show_motd(index, temp, "Ultimate Warcraft3 Credits")
}
/*
 * END uwc3_credits subroutine
 */

/*
 * BEGIN war3_info subroutine
 * Display the help info for the WC3FT-Unlimited plugin
 */
public native_war3_info(id, num)
{
	if(warcraft3)
	{
		new index = get_param(1)

		if(check_player(index))
			return;

		war3_info2(index)
	}
}

public war3_info2(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE;

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 2048-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<center><table border=1 cellpadding=3 cellspacing=3><tr><td><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<center><b>Ultimate Warcraft3</b><br><a href='http://www.uwc3.com/'><b>http://www.uwc3.com/</b></a></center><p>" )
	pos += format(temp[pos], 2048-pos, "Ultimate Warcraft3 %s created by K2mia (admin@mudconnect.com). Ultimate Warcraft3 (UWC3) is more or less a raceless, skill-based implementation of the Warcraft3 Frozen Throne plugin. Additional skills, items, etc are planned.<p>", VER )
	pos += format(temp[pos], 2048-pos, "This mod is a derived work based on Warcraft3 Frozen Thrones, developed by 4HM | Pimp Daddy, visit www.4hm.net (forums) for more details.<br><p>")
	pos += format(temp[pos], 2048-pos, "This is how you bind a key:<p>")
	pos += format(temp[pos], 2048-pos, "1) Type ` to get to the console (key to the left of number 1)<br>" )
	pos += format(temp[pos], 2048-pos, "2) Type bind - shopmenu  (displays shopmenu1)<br>" )
	pos += format(temp[pos], 2048-pos, "3) Type bind = shopmenu2 (displays shopmenu2)<br>" )
	pos += format(temp[pos], 2048-pos, "4) Type bind p wcward (to use the serpant ward ability)<p>" )
	pos += format(temp[pos], 2048-pos, "UWC3 allows you to have up to 3 ultimate skills as opposed to just one from the original Warcraft3. In the skills menu (/selectskill) ultimate skills will say (Ultimate: name), for example for the Teleport ultimate it says (Ultimate: wcteleport). Once you learn this skill you can use it by binding a key to the wcteleport command, done by typing: bind key wcteleport in the console.<p>" )
	pos += format(temp[pos], 2048-pos, "Say <b>/commands</b> to see the list of UWC3 spoken commands and say <b>/help</b> for the UWC3 help menu.<p>" )
	pos += format(temp[pos], 2048-pos, "</font></td></tr></table></center>")
	show_motd(id, temp, "Ultimate WC3FT Player Help")
	return PLUGIN_HANDLED;
}
/*
 * END war3_info subroutine
 */


/*
 * BEGIN uwc3_tips subroutine
 * Display the UWC3 tips page
 */
public native_uwc3_tips(id, num)
{
	if(!warcraft3)
		return;

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 2048-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<table border=0><tr><td><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<center><b>Ultimate Warcraft3</b><br><a href='http://www.uwc3.com/'><b>http://www.uwc3.com/</b></a></center><p>" )
	pos += format(temp[pos], 2048-pos, "<b>Tips for Playing UWC3</b><p>")
	pos += format(temp[pos], 2048-pos, "This is not the Warcraft3 plugin you are used to, the favorite race you want to play isn't here, but all your favorite skills and abiltiies are, plus many more. The following are some helpful tips for players new to WC3 and veterans of WC3 alike.<p>")
	pos += format(temp[pos], 2048-pos, "<li>There is no race menu, all skills are available via the skillsmenu by saying <b>/selectskill</b> <p>")
	pos += format(temp[pos], 2048-pos, "<li>Each page of skills has a corresponding page with info about each skills listed, page 1 skills info is accessed by saying <b>/skillsinfo1</b>, ... page 5 skills are accessed by saying <b>/skillsinfo5</b> <p>")
	pos += format(temp[pos], 2048-pos, "<li>In Ultimate Warcraft3 you gain levels and skills by earning experience. You earn experience by killing your enemies or seriously injuring them (assisting a teammate in killing), performing game objectives like planting the bomb or rescuring hostages, and other skill based methods such as healing your teamates or repairing their armor.<p>")
	pos += format(temp[pos], 2048-pos, "<li>Ultimate skills in UWC3 each have a unique command to bind to a key, the skillsmenu (/selectskill) displays these commands next to Ultimate skills. <p>")
	pos += format(temp[pos], 2048-pos, "</td></tr></table>")
	show_motd(index, temp, "UWC3 Tips")
}
/*
 * END uwc3_tips subroutine
 */


/*
 * BEGIN uwc3_tips2 subroutine
 * Display the UWC3 tips page 2
 */
public native_uwc3_tips2(id, num)
{
	if(!warcraft3)
		return;

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 2048-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<table border=0><tr><td><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<center><b>Ultimate Warcraft3</b><br><a href='http://www.uwc3.com/><b>http://www.uwc3.com/</b></a></center><p>" )
	pos += format(temp[pos], 2048-pos, "<b>Tips for Playing UWC3 -page 2</b><p>")
	pos += format(temp[pos], 2048-pos, "<li>Abilities are also bound like Ultimates in UWC3 and their commands are also displayed next to their skill name in the menu<p>")
	pos += format(temp[pos], 2048-pos, "<li>There are 2 shops in UWC3 where magical items can be bought, these range from items that increase your speed or attack abilities to some that will disguise you like the enemy or allow you to spawn in the enemy camp at the start of a round. More information is available by saying <b>/itemsinfo</b> and <b>/itemsinfo2</b>. <p>")
	pos += format(temp[pos], 2048-pos, "<li>One <b>VERY</b> important item in the shop that should always be bought when money allows is the <b>Necklace of Immunity</b>. The necklace will protect you from all Ultimate skill effects as well as some other nasty skills such as Napalm Grenades or Serpant Wards. If money is tight I will generally purchase a Necklace in place of Armor, but thats just me. :)<p>")
	pos += format(temp[pos], 2048-pos, "<li>One skill that is new to UWC3 is called Mend Wounds and beside being useful to your team by allowing you to heal damage, cure poison and ease burns, it also provides a reliable source of earning experience. <p>")
	pos += format(temp[pos], 2048-pos, "<li>You can reset your current skills by saying <b>/resetskills</b> - at the start of the next round your points will be returned and you may retrain a new set of skills. If by slim chance you ever want to delete your skills and XP you can do so with <b>/deletexp</b><p>")
	pos += format(temp[pos], 2048-pos, "<li>UWC3 now offers a complete line of player Attributes and Resistances you can train along with your skills. Complete information about Attributes, Resistances and Enhancement XP is available at our website -- <b>http://www.uwc3.com/enhancements.html</b><p>")
	pos += format(temp[pos], 2048-pos, "</td></tr></table>")
	show_motd(index, temp, "UWC3 Tips pg. 2")
}
/*
 * END uwc3_tips2 subroutine
 */


/*
 * BEGIN uwc3_ultimates subroutine
 * Display the info about ultimates in UWC3 plugin
 */
public native_uwc3_ultimates(id, num)
{
	if(!warcraft3)
		return;

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 3072-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 3072-pos, "<table border=0><tr><td><font color=#FFB000>")
	pos += format(temp[pos], 3072-pos, "<center><b>Ultimate Warcraft3</b><br><a href='http://www.uwc3.com/'><b>http://www.uwc3.com/</b></a></center><p>" )
	pos += format(temp[pos], 3072-pos, "<b>Important Information About UWC3 Ultimates</b><p>")
	pos += format(temp[pos], 3072-pos, "Unlike previous versions of the Warcraft3 and Warcraft Frozen Throne plugins,<br>Ultimate Warcraft3 allows players to train in up to 3 ultimate skills.<p>")
	pos += format(temp[pos], 3072-pos, "In order to make use of the ultimate skills in UWC3 you will need<br>to bind a key for each ultimate and each ultimate has a unique command to be bound. Special abilities are also handled the same way in UWC3. <p>")
	pos += format(temp[pos], 3072-pos, "<table border=1 cellpadding=4 cellspacing=4><tr><th><font color=#FFB000>Ultimate or Ability</th><th><font color=#FFB000>Bind Command</th></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Suicide Bomber</td><td><font color=#FFB000>bind key wcsuicide</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Teleport</td><td><font color=#FFB000>bind key wcteleport</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Chain Lightning</td><td><font color=#FFB000>bind key wclightning</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Entangle Roots</td><td><font color=#FFB000>bind key wcentangle</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Flame Strike</td><td><font color=#FFB000>bind key wcflame</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Big Bad Voodoo</td><td><font color=#FFB000>bind key wcvoodoo</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Vengeance</td><td><font color=#FFB000>Passive Ability</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Locust Swarm</td><td><font color=#FFB000>bind key wclocust</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Flash of Light</td><td><font color=#FFB000>bind key wcflash</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Decoy</td><td><font color=#FFB000>bind key wcdecoy</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Gate</td><td><font color=#FFB000>bind key wcgate</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Ability: Mend Wounds</td><td><font color=#FFB000>bind key wcmend</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Ability: Repair Armor</td><td><font color=#FFB000>bind key wcrepair</td></tr>")
	pos += format(temp[pos], 3072-pos, "<tr><td><font color=#FFB000>Ability: Serpant Ward</td><td><font color=#FFB000>bind key wcward</td></tr>")
	pos += format(temp[pos], 3072-pos, "</td></tr></table>")
	show_motd(index, temp, "UWC3 Ultimates")
}
/*
 * END uwc3_ultimates subroutine
 */


/*
 * BEGIN uwc3_attributes subroutine
 * Display the UWC3 Attributes Information page
 */
public native_uwc3_attributes(id, num)
{
	if(!warcraft3)
		return;

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 2048-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<b>Attributes Information</b><p>")
	pos += format(temp[pos], 2048-pos, "<p>UWC3 provides, in addition to the normal XP system, a new enhancement XP system (E-XP) which allows you to earn Attribute points to spend building your character. These attributes are as follows:</p><ol>")
	pos += format(temp[pos], 2048-pos, "<li><b>Strength</b> - Determines your starting base health, stacks with Devotion Aura <p>")
	pos += format(temp[pos], 2048-pos, "<li><b>Intellect</b> - Each level of intellect you enhance from 11 - 18 provides a different benefit, check the website for complete details (http://www.uwc3.com/enhancements.html) <p> ")
	pos += format(temp[pos], 2048-pos, "<li><b>Dexterity</b> - Helps to successfully Evade shots for characters trained in Evasion <p> ")
	pos += format(temp[pos], 2048-pos, "<li><b>Agility</b> - Determines your base runspeed, stacks with Unholy Aura <p> ")
	pos += format(temp[pos], 2048-pos, "<li><b>Constitution</b> - Provides health regeneration which stacks with Healing Wave ")
	show_motd(index, temp, "Attributes Information")
}
/*
 * END uwc3_attributes subroutine
 */

/*
 * BEGIN uwc3_resistances subroutine
 * Display the UWC3 Resistances Information page
 */
public native_uwc3_resistances(id, num)
{
	if(!warcraft3)
		return;

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 2048-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<b>Resistances Information</b><p>")
	pos += format(temp[pos], 2048-pos, "<p>UWC3 provides, in addition to the normal XP system, a new enhancement XP system (E-XP) which allows you to earn Resistance points to spend building a defense system for your character. Each resistance can be trained to 100 points at which time you should be totally covered from the specified damage type. These resistances are as follows:</p><ol>")
	pos += format(temp[pos], 2048-pos, "<li><b>Poison</b> -  protection against Shadow Strike poison-based attacks <p>")
	pos += format(temp[pos], 2048-pos, "<li><b>Disease</b> -  protection against Carrion Beetles and Locust Swarm disease-based attacks <p>")
	pos += format(temp[pos], 2048-pos, "<li><b>Electricity</b> -  protection against Chain Lightning attacks <p>")
	pos += format(temp[pos], 2048-pos, "<li><b>Fire</b> - protection against Flamethrower and Napalm Grenade fire-based attacks <p>")
	pos += format(temp[pos], 2048-pos, "<li><b>Magic</b> - Similar to a Necklace of Immunity with some exceptions made to attacks covered by other resistance types.<p>")
	show_motd(index, temp, "Resistances Information")
}
/*
 * END uwc3_resistances subroutine
 */

// **************************************************************************
// BEGIN items_info subroutine
// Display info about items in the shopmenu
// **************************************************************************
public native_items_info(id, num)
{
	if(!warcraft3)
		return;

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0

	new CLAWSOFATTACK = get_cvar_num("UW_CLAWSOFATTACK")
	pos += format(temp[pos], 1024-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 1024-pos, "The following items are available for purchase at the shop (say shopmenu):<p><ul>")
	pos += format(temp[pos], 1024-pos, "%sAnkh of Reincarnation: If you die you will retrieve your equipment the following round%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sBoots of Speed: Allows you to run faster%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sClaws of Attack +%d: An additional %d hp will be removed from the enemy on every hit%s",theforward,CLAWSOFATTACK,CLAWSOFATTACK,backward)
	pos += format(temp[pos], 1024-pos, "%sCloak of Shadows: Makes you partially invisible, invisibility is increased when holding the knife%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sMask of Death: You will receive health for every hit on the enemy%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sNecklace of Immunity: You will be immune to enemy ultimates",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sOrb of Frost: Slows your enemy down when you hit him%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sPeriapt of Health: Receive extra health%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sTome of Experience: Purchasing a tome increases your experience by a fixed amount, the tome is consume%s%s",theforward,backward,lastone)
	show_motd(index,temp,"Shopmenu Item Information")
}
// **************************************************************************
// END items_info subroutine
// **************************************************************************


// **************************************************************************
// BEGIN items_info2 subroutine
// Display info about items in the shopmenu2
// **************************************************************************
public native_items_info2(id, num)
{
	if (!warcraft3)
		return

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0
	pos += format(temp[pos], 1024-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 1024-pos, "The following items are available for purchase at the shop (say shopmenu2):<p><ul>")
	pos += format(temp[pos], 1024-pos, "%sScroll of Respawning: You will respawn after death%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sMole Protectant: Protects you from moles (unless they hit you in the head)%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sAmulet of the Cat: You cannot be heard when running/going up ladders%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sSock of the Feather: Allows you to levitate %s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sHelm of Excellence: Immunity to headshots%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sFlaming Gloves of Warmth: Given an HE grenade every %s seconds%s",theforward, get_cvar_num("FT_glove_timer"),backward)
	pos += format(temp[pos], 1024-pos, "%sRing of Regeneration +1: Gives 1 health every 2 seconds, you can have 5 max (type rings to receive 5)%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sChameleon: You are disguised as the enemy!!%s",theforward,backward)
	pos += format(temp[pos], 1024-pos, "%sMole: Teleports you to the enemy spawn under disguise at the start of the round %s%s",theforward,backward,lastone)

	show_motd(index,temp,"Shopmenu2 Item Information")
}
// **************************************************************************
// END items_info2 subroutine
// **************************************************************************

public native_change_race(id, num)
{
	if(!warcraft3)
		return;

	new index = get_param(1)

	if(check_player(index))
		return;

	temp[0] = 0
	new pos = 0

	pos += format(temp[pos], 2048-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<table border=0><tr><td><font color=#FFB000>")
	pos += format(temp[pos], 2048-pos, "<center><b>Ultimate Warcraft3</b><br><a href='http://www.uwc3.com/'><b>http://www.uwc3.com/</b></a></center><p>")
	pos += format(temp[pos], 2048-pos, "Ultimate Warcraft3(UWC3)is a raceless version of the Warcraft3 plugin you may be familiar with.<p>")
	pos += format(temp[pos], 2048-pos, "UWC3 provides players with 34 playable levels and a freeform skills system which allows you to mix and match skills of your choosing. In addition UWC3 offers players a choice of 10 ultimate skills and you can train up to 3 different ultimates, as well as other special abilities. UWC3 also offers an optional Enhancement system to provide your character with even more customization. <p>")
	pos += format(temp[pos], 2048-pos, "Interested? Read on! Say <b>/help</b> for UWC3's help menu and say <b>/commands</b> to see the list of UWC3 spoken commands. Say <b>/news</b> to read up on the latest changes to the plugin.<p>")
	pos += format(temp[pos], 2048-pos, "</td></tr></table>")
	show_motd(index, temp, "Ultimate Warcraft3")
}

// **************************************************************************
// BEGIN player_skills subroutine
// Displays the rank and skills for all players
// **************************************************************************
public player_skills(id)
{
	if(!warcraft3)
		return PLUGIN_CONTINUE

	client_print(id, print_chat, "[%s] /Playerskills is deprecated, please use /who instead.", MOD)

	return PLUGIN_HANDLED
}
// **************************************************************************
// END player_skills subroutine
// **************************************************************************

public client_putinserver(id)
{
	is_connected[id] = true;
}

public client_disconnect(id)
{
	is_connected[id] = false;
}