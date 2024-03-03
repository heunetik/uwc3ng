// Uwc3ng cvar functions

public Load_Cvars()
{
	// Uwc3ng enabled or disabled?
	uwc3ng_enabled = register_cvar ( "uwc3ng_enabled", "1" );
	
	// Where we save the xps and skills etc?
	CVAR_uwc3ng_savexp = register_cvar ( "uwc3ng_savexp_sql", "1" ); // 0 = MySQL, 1 = SQLLITE
	
	// Should we save the xps?
	CVAR_uwc3ng_save_xp = register_cvar ( "uwc3ng_save_xp", "1" ); // 1 = enabled, 0 = disabled
	
	// How should we save the xps?
	CVAR_uwc3ng_save_by = register_cvar ( "uwc3ng_save_by", "0" ); // 0 = SteamID, 1 = IP, 2 = Name
	
	// SQL cvars
	CVAR_uwc3ng_host = register_cvar ( "uwc3ng_host", "127.0.0.1" ); // The host from the db
	CVAR_uwc3ng_user = register_cvar ( "uwc3ng_user", "root" ); // The username from the db login
	CVAR_uwc3ng_pass = register_cvar ( "uwc3ng_pass", "" ); // The password from the db login
	CVAR_uwc3ng_name = register_cvar ( "uwc3ng_name", "uwc3ng" ); // The database name 
	
	// XP modifiers
	CVAR_uwc3ng_xp_for_level = register_cvar ( "uwc3ng_xp_for_level", "5000" ); // XPs base for every level
	CVAR_uwc3ng_min_players = register_cvar ( "uwc3ng_min_players", "4" ); // Maximum for players to get / lose xps
	CVAR_uwc3ng_xpmultiplier = register_cvar ( "uwc3ng_xpmultiplier", "1.0" ); // Multiple all xps with this value
	CVAR_uwc3ng_startlevel = register_cvar ( "uwc3ng_startlevel", "0" ); // Define the startlevel of each player if he connects to the server
	CVAR_uwc3ng_resist_xpmodifier = register_cvar ( "uwc3ng_resist_xpmodifier", "0.5" ); // Longterm XP resistpoints modifier
	CVAR_uwc3ng_attrib_xpmodifier = register_cvar ( "uwc3ng_attrib_xpmodifier", "0.5" ); // Longterm XP attribpoints modifier
	CVAR_uwc3ng_save_end_round = register_cvar ( "uwc3ng_save_end_round", "1" ); // Save xp at the end of each round (may cause lag, default is 1)

	// Psychostats
	CVAR_uwc3ng_psychostats = register_cvar ( "uwc3ng_enable_psychostats", "0" ); // 1 = Enable log for psychostats, 0 = disable log for psychostats
	
	// Admin cvars
	CVAR_uwc3ng_admin_flag = register_cvar ( "uwc3ng_admin_flag", "m" ); // Admins with this flag can execute the admin commands
	
	// Gameplay
	CVAR_uwc3ng_blink_protection = register_cvar ( "uwc3ng_teleport_protection", "1" ); // slays people who abuse blink on some maps (skywalking, map exploiting, default is 1)
	CVAR_uwc3ng_ult_cooldown = register_cvar ( "uwc3ng_ultimate_cooldown", "20" ); // Ultimate cooldown 
	CVAR_uwc3ng_blink_dizziness = register_cvar ( "uwc3ng_teleport_dizziness", "0" ); // What type of diziness? (0 is flashbang effect, 1 is fading blue screen, default is 0)
	CVAR_uwc3ng_entangle_drop = register_cvar ( "uwc3ng_entangle_drop", "0" );
	CVAR_uwc3ng_spec_position = register_cvar ( "uwc3ng_spec_position", "0" );
	CVAR_uwc3ng_minmodels = register_cvar ( "uwc3ng_minmodels", "1" ); // Enable or disable minmodels check
	CVAR_uwc3ng_icons = register_cvar ( "uwc3ng_icons", "1" ); // Enable or Disable icons for wcrepair and wcmend
	CVAR_uwc3ng_ult_round_start_cd = register_cvar ( "uwc3ng_ult_round_start_cd", "5" );
	CVAR_uwc3ng_hudmessage_display = register_cvar ( "uwc3ng_hudmessage_display", "0" ); // Show items in a hudmessage (0) in a status text (1)
	
	// Items
	CVAR_uwc3ng_boots = register_cvar ( "uwc3ng_boots", "0.10" );
	CVAR_uwc3ng_claw = register_cvar ( "uwc3ng_claw", "6" );
	CVAR_uwc3ng_cloak = register_cvar ( "uwc3ng_cloak", "150" );
	CVAR_uwc3ng_mask = register_cvar ( "uwc3ng_mask", "0.3" );
	CVAR_uwc3ng_frost = register_cvar ( "uwc3ng_frost", "125" );
	CVAR_uwc3ng_health = register_cvar ( "uwc3ng_health", "15" );
	CVAR_uwc3ng_glove_timer = register_cvar ( "uwc3ng_glove_timer", "10" );
	CVAR_uwc3ng_tome = register_cvar ( "uwc3ng_tome", "60" );
	CVAR_uwc3ng_sock = register_cvar ( "uwc3ng_sock", "0.5" );
	
	// Bots
	CVAR_uwc3ng_ignore_bots = register_cvar ( "uwc3ng_ignore_bots", "0" ); // 0 = ignore no bots, 1 = ignore bots
	CVAR_uwc3ng_bot_startlevel = register_cvar ( "uwc3ng_bot_startlevel", "0" ); // On which level should the bot start?
	CVAR_uwc3ng_bot_buy_item = register_cvar ( "uwc3ng_bot_buy_item", "0.33" ); // 0.33 = 33% item buy chance
	
	// Handle the chat
	register_clcmd ( "say", "HandleSay" );
	register_clcmd ( "say_team", "HandleSay" );
	
	// Admin console commands
	register_clcmd ( "amx_givexp", "ADMIN_Handler" );
	register_clcmd ( "uwc3ng_givexp", "ADMIN_Handler" );
	
	// Console commands
	register_clcmd ( "xp_table", "XP_TableShow" ); // Show the xp table in the console
	register_clcmd ( "selectskill", "COMMAND_SkillMenu" ); // Show the skill menu
	register_clcmd ( "/selectskill", "COMMAND_SkillMenu" ); // Show the skill menu
	register_clcmd ( "dropskill", "COMMAND_DropSkillMenu" ); // Show the drop skill menu
	register_clcmd ( "/dropskill", "COMMAND_DropSkillMenu" ); // Show the drop skill menu
	register_clcmd ( "resetskills", "COMMAND_ResetSkills" ); // Reset all skills
	register_clcmd ( "/resetskills", "COMMAND_ResetSkills" ); // Reset all skills
	register_clcmd ( "drop", "on_Drop" ); // Event that will triggered when someone dropped a weapon
	register_clcmd ( "fullupdate", "COMMAND_fullupdate" ); // fullupdate fix
	register_clcmd ( "wcrepair", "COMMAND_WcRepair" ); // Ultimate Repair Armor
	register_clcmd ( "wcmend", "COMMAND_WcMend" ); // Ultimate Mend Wounds
	register_clcmd ( "wcward", "COMMAND_WcWard" ); // Ultimate Serpent Ward
	register_clcmd ( "wcteleport", "COMMAND_WcTeleport" ); // Ultimate Teleport
	register_clcmd ( "wcdispell", "COMMAND_WcDispell" ); // Ultimate Dispell Hex
	register_clcmd ( "wcvoodoo", "COMMAND_WcVoodoo" ); // Ultimate Big Bad Voodoo
	register_clcmd ( "wcgate", "COMMAND_WcGate" ); // Ultimate Gate
	register_clcmd ( "wclocust", "COMMAND_WcLocust" ); // Ultimate Locust Swarm
	register_clcmd ( "wclightning", "COMMAND_WcLightning" ); // Ultimate Chain Lightning
	register_clcmd ( "wcentangle", "COMMAND_WcEntangle" ); // Ultimate Entangle Roots
	register_clcmd ( "wcflame", "COMMAND_WcFlame" ); // Ultimate Flame Strike
	register_clcmd ( "wcsuicide", "COMMAND_WcSuicide" ); // Ultimate Suicide Bomber
	register_clcmd ( "shopmenu", "COMMAND_Shopmenu" ); // Open the shopmenu
	register_clcmd ( "/shopmenu", "COMMAND_Shopmenu" ); // Open the shopmenu
	register_clcmd ( "shopmenu2", "COMMAND_Shopmenu2" ); // Open the shopmenu2
	register_clcmd ( "/shopmenu2", "COMMAND_Shopmenu2" ); // Open the shopmenu2
	register_clcmd ( "selectattrib", "COMMAND_SelectAttrib" ); // Open the select attrib menu
	register_clcmd ( "/selectattrib", "COMMAND_SelectAttrib" ); // Open the select attrib menu
	register_clcmd ( "resetattribs", "COMMAND_ResetAttribs" ); // Execute resetattribs
	register_clcmd ( "/resetattribs", "COMMAND_ResetAttribs" ); // Execute resetattribs
	register_clcmd ( "selectresist", "COMMAND_SelectResist" ); // Open the select resist menu
	register_clcmd ( "/selectresist", "COMMAND_SelectResist" ); // Open the select resist menu
	register_clcmd ( "resetresists", "COMMAND_ResetResists" ); // Execute resetresists
	register_clcmd ( "/resetresists", "COMMAND_ResetResists" ); // Execute resetresists
	register_clcmd ( "level", "COMMAND_Level" ); // Execute level
	register_clcmd ( "/level", "COMMAND_Level" ); // Execute level
	register_clcmd ( "playerskills", "COMMAND_PlayerSkills" ); // Show the players on the server
	register_clcmd ( "/playerskills", "COMMAND_PlayerSkills" ); // Show the players on the server
	register_clcmd ( "who", "COMMAND_PlayerSkills" ); // Show the players on the server
	register_clcmd ( "/who", "COMMAND_PlayerSkills" ); // Show the players on the server
	register_clcmd ( "charsheet", "COMMAND_Charsheet" ); // Show the charsheet
	register_clcmd ( "/charsheet", "COMMAND_Charsheet" ); // Show the charsheet
	register_clcmd ( "charactersheet", "COMMAND_Charsheet" ); // Show the charsheet
	register_clcmd ( "/charactersheet", "COMMAND_Charsheet" ); // Show the charsheet
	register_clcmd ( "character", "COMMAND_Charsheet" ); // Show the charsheet
	register_clcmd ( "/character", "COMMAND_Charsheet" ); // Show the charsheet
	register_clcmd ( "whois", "COMMAND_Whois" ); // Show charsheet motd
	register_clcmd ( "/whois", "COMMAND_Whois" ); // Show charsheet motd
	register_clcmd ( "skillsinfo1", "COMMAND_Skills1" ); // Skillsinformation 1
	register_clcmd ( "skillsinfo2", "COMMAND_Skills2" ); // Skillsinformation 2
	register_clcmd ( "skillsinfo3", "COMMAND_Skills3" ); // Skillsinformation 3
	register_clcmd ( "skillsinfo4", "COMMAND_Skills4" ); // Skillsinformation 4
	register_clcmd ( "skillsinfo5", "COMMAND_Skills5" ); // Skillsinformation 5
	register_clcmd ( "/skillsinfo1", "COMMAND_Skills1" ); // Skillsinformation 1
	register_clcmd ( "/skillsinfo2", "COMMAND_Skills2" ); // Skillsinformation 2
	register_clcmd ( "/skillsinfo3", "COMMAND_Skills3" ); // Skillsinformation 3
	register_clcmd ( "/skillsinfo4", "COMMAND_Skills4" ); // Skillsinformation 4
	register_clcmd ( "/skillsinfo5", "COMMAND_Skills5" ); // Skillsinformation 5
	register_clcmd ( "attribinfo", "COMMAND_AttribInfo" ); // Show the attrib info
	register_clcmd ( "/attribinfo", "COMMAND_AttribInfo" ); // Show the attrib info
	register_clcmd ( "commands", "COMMAND_CommandsHelp" ); // Show the commands help
	register_clcmd ( "/commands", "COMMAND_CommandsHelp" ); // Show the commands help
	register_clcmd ( "tips", "COMMAND_ShowTips" ); // Show tips
	register_clcmd ( "/tips", "COMMAND_ShowTips" ); // Show tips
	register_clcmd ( "tips2", "COMMAND_ShowTips2" ); // Show tips 2
	register_clcmd ( "/tips2", "COMMAND_ShowTips2" ); // Show tips 2
	register_clcmd ( "deletexp", "COMMAND_DeleteXP" ); // Delete all xps
	register_clcmd ( "/deletexp", "COMMAND_DeleteXP" ); // Delete all xps
	register_clcmd ( "changerace", "COMMAND_ChangeRace" ); // Show changerace help
	register_clcmd ( "/changerace", "COMMAND_ChangeRace" ); // Show changerace help
	register_clcmd ( "savexp", "COMMAND_SaveXP" ); // Savexp
	register_clcmd ( "/savexp", "COMMAND_SaveXP" ); // Savexp
	register_clcmd ( "war3menu", "COMMAND_War3Menu" ); // Open the war3menu
	register_clcmd ( "/war3menu", "COMMAND_War3Menu" ); // Open the war3menu
	register_clcmd ( "wc3menu", "COMMAND_War3Menu" ); // Open the war3menu
	register_clcmd ( "/wc3menu", "COMMAND_War3Menu" ); // Open the war3menu
	register_clcmd ( "help", "COMMAND_Help" ); // Open the help menu
	register_clcmd ( "/help", "COMMAND_Help" ); // Open the help menu
	
	// CVAR pointers from other plugins / cvars
	CVAR_sv_gravity = get_cvar_pointer ( "sv_gravity" );
	CVAR_csdm_active = get_cvar_pointer ( "csdm_active" );
	CVAR_mp_friendlyfire = get_cvar_pointer ( "mp_friendlyfire" );
}

// Load the uwc3ng.cfg
public Load_Uwc3NG_CFG()
{
	static configpath[256];
	
	// Get the configs dir
	get_configsdir( configpath, charsmax( configpath ) );
	
	// Add the file path
	add( configpath, charsmax( configpath ), "/uwc3ng/uwc3ng.cfg" );
	
	// Execute the file if it exists
	if ( file_exists( configpath ) )
	{
		server_cmd( "exec %s", configpath );
	}
	
	// File doesn't exist
	else
	{
		log_amx( "[ERROR] Config file '%s' missing!", configpath );
		
		// Set plugin to unload
		set_fail_state( "Config file is missing, unable to load plugin" );
	}
}
	
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
