// Uwc3ng motd functions

// Show the player the players which are on the server
public MOTD_ShowPlayers( id )
{
	if ( !is_user_connected( id ) )
	{
		return PLUGIN_HANDLED;
	}
	
	static message[4096], name[32], players[32];
	static numplayers, i, playerid; 
	
	// Get the players from the server
	get_players ( players, numplayers );
   
	// Okay, first we need the header
	formatex ( message, charsmax( message ), "<body bgcolor=#000000 text=#FFB000>^n" );
	formatex ( message, charsmax( message ), "%s<center><b>The following players are online</b></center><p>^n", message );
	formatex ( message, charsmax( message ), "%s<center><table width=800 border=1 cellpadding=4 cellspacing=4>^n", message );
	formatex ( message, charsmax( message ), "%s<tr><td>Name</td><td>Team</td><td>Level</td></tr>^n", message );

	// There we save the team
	static PlayerTeam[32];

	for ( i = 0; i < numplayers; ++i )
	{
		playerid = players[i];

		get_user_name( playerid, name, charsmax( name ) );
		
		switch ( get_user_team( playerid ) )
		{
			case TEAM_T: // Terrorists
			{
				PlayerTeam = "Terrorist";
			}
			
			case TEAM_CT: // Counterterrorists
			{
				PlayerTeam = "CT";
			}
			
			default: // Else
			{
				PlayerTeam = "Spectator";
			}
		}
		
		formatex ( message, charsmax( message ), "%s<tr><td>%s</td><td>%s</td><td>%d</td></tr>^n", message, name, PlayerTeam, p_data[playerid][PLAYER_LVL], name );
	}

	// We add the end
	formatex ( message, charsmax( message ), "%s</table></center>^n", message ); 
	
	// write it in a file
	new showplayersfile[64];
	get_configsdir( showplayersfile, charsmax( showplayersfile ) );
	
	// Format the right place
	formatex ( showplayersfile, charsmax( showplayersfile ), "%s/uwc3ng/gamehelp/players.txt", showplayersfile );
	
	// Check if the file exists - if it exists delete it because our is newer
	if ( file_exists( showplayersfile ) )
	{
		delete_file ( showplayersfile );
	}
	
	// write the new file
	write_file ( showplayersfile, message );
	
	// show the player the motd
	show_motd ( id, showplayersfile, "Players on the Server" );
	
	return PLUGIN_CONTINUE;
}

public MOTD_CharacterSheet( id, iShowID ) 
{
	static PlayerName[32], ShowMessage[2048];
	
	// Get username
	get_user_name( id, PlayerName, charsmax( PlayerName ) );
	
	// We calculate some stuff. Need it later
	static PlayerXP, p_NextLevelXP, p_XPNeeded;
	PlayerXP = p_data[id][PLAYER_XP];
	p_NextLevelXP = g_XPforLevel[MAX_LEVEL];
	
	if ( p_data[id][PLAYER_LVL] < MAX_LEVEL )
	{
		p_NextLevelXP = g_XPforLevel[p_data[id][PLAYER_LVL]+1];
	}
	
	p_XPNeeded = ( p_NextLevelXP - PlayerXP );
	
	// First, we add the header
	copy( ShowMessage, charsmax( ShowMessage ), "<body bgcolor=#000000><font color=#FFB000>" );
	
	// Add the name in the header
	formatex( ShowMessage, charsmax( ShowMessage ), "%s<center><b>Character Sheet: %s</b></center><p>", ShowMessage, PlayerName );
	
	// Header...
	add( ShowMessage, charsmax( ShowMessage ), "<center><table width=800 border=1 cellpadding=4 cellspacing=4>" );
	add( ShowMessage, charsmax( ShowMessage ), "<tr><td valign=top width=50%><font color=#FFB000>" );
	
	// Okay, let us begin to fill the motd with infos
	formatex( ShowMessage, charsmax( ShowMessage ), "%sPlayer Name: %s <br>", ShowMessage, PlayerName );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sPlayer Level: %d <br>", ShowMessage, p_data[id][PLAYER_LVL] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sPlayer Rank: %s <br>", ShowMessage, p_RankTitles[p_data[id][PLAYER_RANK]] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sSkillpts Available: %d <br>", ShowMessage, p_data[id][AVAILABLE_SKILL] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sAttribute Pts Available: %d <br>", ShowMessage, p_data[id][AVAILABLE_ATTRIBS] );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sResistance Pts Available: %d <br>", ShowMessage, p_data[id][AVAILABLE_RESISTS] );
		
	// We need some crappy stuff between this :P
	add( ShowMessage, charsmax( ShowMessage ), "</td><td valign=top width=50%><font color=#FFB000>" );
	
	// More infos...
	formatex( ShowMessage, charsmax( ShowMessage ), "%sPlayer XP: %d / %d <br>", ShowMessage, PlayerXP, p_NextLevelXP );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sXP Needed: %d <br>", ShowMessage, p_XPNeeded );
	
	// More crappy stuff
	add( ShowMessage, charsmax( ShowMessage ), "</td></tr><tr><td valign=top><font color=#FFB000>" );
	add( ShowMessage, charsmax( ShowMessage ), "<center><b>Attributes</b></center><p>" );
	
	// Attribute infos
	formatex( ShowMessage, charsmax( ShowMessage ), "%sStrength: %d / %d <br>", ShowMessage, p_data_attrib[id][ATTRIB_STRENGTH], MAX_ATTRIBS_POINTS );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sIntellect: %d / %d <br>", ShowMessage, p_data_attrib[id][ATTRIB_INTELLECT], MAX_ATTRIBS_POINTS );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sDexterity: %d / %d <br>", ShowMessage, p_data_attrib[id][ATTRIB_DEXTERITY], MAX_ATTRIBS_POINTS );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sAgility: %d / %d <br>", ShowMessage, p_data_attrib[id][ATTRIB_AGILITY], MAX_ATTRIBS_POINTS );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sConstitution: %d / %d <br>", ShowMessage, p_data_attrib[id][ATTRIB_CONSTITUTION], MAX_ATTRIBS_POINTS );
	
	// More crappy stuff
	add( ShowMessage, charsmax( ShowMessage ), "</td><td valign=top><font color=#FFB000>" );
	add( ShowMessage, charsmax( ShowMessage ), "<center><b>Resistances</b></center><p>" );
	
	// Resistance infos
	formatex( ShowMessage, charsmax( ShowMessage ), "%sPoison: %d / %d <br>", ShowMessage, p_data_resist[id][RESIST_POISON], MAX_RESIST_POINTS );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sDisease: %d / %d <br>", ShowMessage, p_data_resist[id][RESIST_DISEASE], MAX_RESIST_POINTS );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sElectricity: %d / %d <br>", ShowMessage, p_data_resist[id][RESIST_LIGHTNING], MAX_RESIST_POINTS );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sFire: %d / %d <br>", ShowMessage, p_data_resist[id][RESIST_FIRE], MAX_RESIST_POINTS );
	formatex( ShowMessage, charsmax( ShowMessage ), "%sMagic: %d / %d <br>", ShowMessage, p_data_resist[id][RESIST_MAGIC], MAX_RESIST_POINTS );
	
	// More crappy stuff
	add( ShowMessage, charsmax( ShowMessage ), "</td></tr><tr><td colspan=2 valign=top><font color=#FFB000>" );
	add( ShowMessage, charsmax( ShowMessage ), "<center><b>Skills Trained</b></center><p>" );
	
	// Add the skills
	static sMaster[10], iSkillCounter; iSkillCounter = 0;
	for ( new i = 0; i < MAX_SKILLS; ++i )
	{
		// Skill is trained, add it
		if ( p_data_skill[id][i] > 0 )
		{
			// Count the skills
			iSkillCounter++;
			
			// Is the skill on the maximum?
			if ( p_data_skill[id][i] == Skill_Max_Points[i] )
			{
				copy( sMaster, charsmax( sMaster ), "(Master)" );
			}
			
			// Isn't on the maximum
			else
			{
				// Reset the data
				sMaster = "";
			}
			
			// Add every skill
			formatex( ShowMessage, charsmax( ShowMessage ), "%s%s [level %d] %s <br>", ShowMessage, Skill_Names[i], p_data_skill[id][i], sMaster );
		}
	}
	
	// The player has no trained skills
	if ( !iSkillCounter )
	{
		add( ShowMessage, charsmax( ShowMessage ), "No skills trained" );
	}
	
	// Add the end lines
	add( ShowMessage, charsmax( ShowMessage ), "</td></tr>" );
	add( ShowMessage, charsmax( ShowMessage ), "</table></center>" );
	
	// Detect if we should show another player this charsheet
	if ( iShowID > 0 )
	{
		id = iShowID;
	}
	
	// Show the player the character sheet
	show_motd( id, ShowMessage, "Character Sheet" );
	
	return;
}

// Show charsheet from another player
public MOTD_CharSheetPlayer( id, Msg[] )
{
	new iTarget = 0, bool:bTargetFound = false;
	
	// Do this while we continue having a target!
	while ( ( iTarget = FindTarget( iTarget, Msg ) ) > 0 )
	{
		bTargetFound = true;
		
		MOTD_CharacterSheet( iTarget, id );
	}
	
	if ( !bTargetFound )
	{
		client_print( id, print_chat, "%s Can't find player %s", MOD_NAME, Msg );
	}
	
	return;
}

// Show attribs help
public MOTD_ShowAttribsHelp( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_attribs.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "UWC3NG Attribute Information!" );
	
	return;
}

// Show changerace help (if someone type /changerace)
public MOTD_ShowChangeraceHelp( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_changerace.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "This is UWC3NG!" );
	
	return;
}

// Show commands help 
public MOTD_ShowCommandHelp( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_commands.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "UWC3NG Command List!" );
	
	return;
}

// Show credits help 
public MOTD_ShowCreditsHelp( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_credits.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "UWC3NG Credits!" );
	
	return;
}

// Show info help 
public MOTD_ShowInfoHelp( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_info.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "UWC3NG Information!" );
	
	return;
}

// Show resist help 
public MOTD_ShowResistHelp( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_resists.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "UWC3NG Resistances Information!" );
	
	return;
}

// Show shopmenu2 help 
public MOTD_ShowShopmenu2Help( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_shopmenu2.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "Shopmenu2 Items" );
	
	return;
}

// Show shopmenu help 
public MOTD_ShowShopmenuHelp( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_shopmenu.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "Shopmenu Items" );
	
	return;
}

// Show skills page help (1-5) 
public MOTD_ShowSkillsHelp( id, page )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	formatex( FilePath, charsmax( FilePath ), "%s/uwc3ng/gamehelp/uwc3ng_skills_pg%d.htm", FilePath, page );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Add the name to the windows
	static WindowInfo[32];
	formatex( WindowInfo, charsmax( WindowInfo ), "Skills Information Page %d", page );
	
	// Show the motd
	show_motd( id, FilePath, WindowInfo );
	
	return;
}

// Show tips2 help 
public MOTD_ShowTips2Help( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_tips2.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "UWC3NG Tips 2" );
	
	return;
}

// Show tips help 
public MOTD_ShowTipsHelp( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_tips.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "UWC3NG Tips" );
	
	return;
}

// Show ultimates help 
public MOTD_ShowUltimatesHelp( id )
{
	static FilePath[256];
	get_configsdir( FilePath, charsmax( FilePath ) );
	
	// Add the place to the motd file
	add( FilePath, charsmax( FilePath ), "/uwc3ng/gamehelp/uwc3ng_ultimates.htm" );
	
	// Is the file there?
	if ( !file_exists( FilePath ) )
	{
		log_amx( "[ERROR] Can't find %s", FilePath );
		
		return;
	}
	
	// Show the motd
	show_motd( id, FilePath, "UWC3NG Ultimates" );
	
	return;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
