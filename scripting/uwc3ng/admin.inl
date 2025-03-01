// Uwc3ng admin functions

// Advanced Swear Filter and Punishment plugin uses this function
public ADMIN_ServerHandler()
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_HANDLED;
	}

	new szCmd[32], szArg1[32], szArg2[32];
	read_argv( 0, szCmd, 31 );
	read_argv( 1, szArg1, 31 );
	read_argv( 2, szArg2, 31 );
	
	new id	= str_to_num( szArg1 );
	new iXP = str_to_num( szArg2 );

	if ( equal( szCmd, "amx_takexp" ) )
	{
		iXP *= -1;

		iXP += p_data[id][PLAYER_XP];
	}

	else if ( equal( szCmd, "changexp" ) )
	{
		iXP += p_data[id][PLAYER_XP];
	}

	ADMIN_SetXP( id, iXP );

	return PLUGIN_HANDLED;
}

// This will handle every admin client command
public ADMIN_Handler( id )
{
	new szCmd[32];
	read_argv( 0, szCmd, 31 );

	// Make sure WC3 is loaded
	if ( !UWC3NG_Check() )
	{
		ADMIN_Print( id, "%s UWC3NG has been disabled by an admin of this server", MOD_NAME );

		return PLUGIN_HANDLED;
	}
	
	// Make sure the user is an admin
	if ( !( get_user_flags( id ) & XP_GetAdminFlag() ) )
	{
		client_print( id, print_console, "%s You have no access to that command", MOD_NAME );

		return PLUGIN_HANDLED;
	}

	// OK we're free to go!!!


	new szArg1[32], szArg2[32];
	read_argv( 1, szArg1, 31 );
	read_argv( 2, szArg2, 31 );

	// Give the user XP
	if ( equal( szCmd, "uwc3ng_givexp" ) || equal( szCmd, "amx_givexp" ) )
	{
		if ( read_argc() < 3 )
		{
			ADMIN_Print( id, "Format: %s <name|#id|auth|@TEAM|@ALL> <xp>", szCmd );

			return PLUGIN_HANDLED;
		}

		ADMIN_GiveXP( id, szArg1, str_to_num( szArg2 ) );
	}

	new szArgs[128];
	read_args( szArgs, 127 );

	ADMIN_Log( id, szCmd, "%s", szArgs );

	return PLUGIN_HANDLED;
}

// Actually set the user's XP
ADMIN_SetXP( id, iXP )
{
	if ( iXP < 0 )
	{
		iXP = 0;
	}

	// Give the xps
	p_data[id][PLAYER_XP] = iXP;

	// Check if the player gained or lost a level
	XP_CheckLevel( id );
	
	if ( is_user_alive( id ) )
	{
		// Show him the status text
		PLAYER_ShowStatusText( id );
	}
}

// Give the user some XP
ADMIN_GiveXP( id, szTarget[], iXP )
{
	new iTarget = 0, bool:bTargetFound = false;
	
	// Do this while we continue having a target!
	while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
	{
		ADMIN_SetXP( iTarget, p_data[id][PLAYER_XP] + iXP );

		client_print( iTarget, print_chat, "%s The admin gave you %d experience", MOD_NAME, iXP );

		bTargetFound = true;
	}

	// No target found :/
	if ( !bTargetFound )
	{
		ADMIN_NoTargetFound( id, szTarget, ( iTarget == -2 ) );
	}
}

// Find a user based on szTarget
public FindTarget( iLastID, szTarget[] )
{
	
	new iTarget = -1;

	// Then we want to basically return everyone!
	if ( equali( szTarget, "@ALL" ) )
	{
		new players[32], iTotalPlayers, i;
		get_players( players, iTotalPlayers );
		
		// Loop through and search for the next target
		for ( i = 0; i < iTotalPlayers; i++ )
		{
			// Target found, so lets return the next one (if possible)!!
			if ( players[i] == iLastID && i + 1 != iTotalPlayers )
			{
				iTarget = players[i+1];
			}
		}

		// No target was found so return the first one
		if ( iTotalPlayers > 0 && iLastID == 0 )
		{
			iTarget = players[0];
		}
	}

	// Find a target based on the team
	else if ( szTarget[0] == '@' )
	{
		new iTeam = -1;
		
		// Terrorist
		if ( equali( szTarget, "@T" ) )
		{
			iTeam = TEAM_T;
		}
		
		// Counter-Terrorist
		else if ( equali( szTarget, "@CT" ) )
		{
			iTeam = TEAM_CT;
		}
				
		// Make sure a team was found
		if ( iTeam != -1 )
		{
			new players[32], iTotalPlayers, i, iFirstPlayer = -1, bool:bSaveNext = false;
			get_players( players, iTotalPlayers );

			// Loop through and search for the next target
			for ( i = 0; i < iTotalPlayers; i++ )
			{			
				// Make sure they're on the same team
				if ( iTeam == get_user_team( players[i] ) )
				{
					
					// This is the next available player
					if ( bSaveNext )
					{
						iTarget = players[i];
						break;
					}

					// If this is the previous target, we need to get the next one!
					if ( players[i] == iLastID )
					{
						bSaveNext = true;
					}
					
					// Save the FIRST player on this team
					if ( iFirstPlayer == -1 )
					{
						iFirstPlayer = players[i];
					}
				}
			}

			// No target was found so return the first one that matches the team (the target could still be -1 if iFirstPlayer wasn't found)
			if ( iLastID == 0 )
			{
				iTarget = iFirstPlayer;
			}
		}
	}

	// Otherwise search for a player
	else
	{
		// Initial search is by player name
		new iPlayer = find_player( "a", szTarget );
		
		// If not found, search by partial match
		if ( !iPlayer )
		{
			iPlayer = find_player( "bl", szTarget );

			if ( iPlayer )
			{

				// Then Multiple clients found
				if ( iPlayer != find_player( "blj", szTarget ) )
				{
					return -2;
				}
			}
		}

		// If not found, search by auth id
		if ( !iPlayer )
		{
			iPlayer = find_player( "c" , szTarget );
		}

		// If not found, search by user id
		if ( !iPlayer )
		{
			// Make sure we have a user id
			if ( szTarget[0] == '#' && szTarget[1] )
			{
				iPlayer = find_player( "k", str_to_num( szTarget[1] ) )
			}
		}
		
		// Yay we have a match!!!
		if ( iPlayer && iLastID != iPlayer )
		{
			iTarget = iPlayer;
		}
	}

	return iTarget;
}

ADMIN_NoTargetFound( id, szTarget[], bool:bMulti )
{
	// Multiple clients found
	if ( bMulti )
	{
		ADMIN_Print( id, "%s There is more than one client matching '%s'", MOD_NAME, szTarget );
	}

	else
	{
		ADMIN_Print( id, "%s Unable to find target(s) '%s'", MOD_NAME, szTarget );
	}

	// List what the available targets are
	ADMIN_Print( id, "%s Available targets are: @ALL, @CT, @T or the player's name/auth/#id", MOD_NAME );
}

// Function will print to server console or client console based on the ID number
ADMIN_Print( id, text[], {Float,_}:...)
{    
	// format the text as needed

	new szFormattedText[128];
	format_args( szFormattedText, 127, 1 );

	if ( id == 0 )
	{
		server_print( szFormattedText );
	}
	else
	{
		client_print( id, print_console, szFormattedText );
	}

	// Gets rid of compiler warning
	if ( text[0] == 0 )
	{
		return;
	}
}  

// Adapted from war3x's log file (I was lazy) ( me too geesu ;) )
ADMIN_Log( id, szCommand[], {Float,_}:... )
{

	new szLogFile[128];
	get_configsdir( szLogFile, 127 );
	formatex( szLogFile, 127, "%s/uwc3ng/uwc3ng_admin.log", szLogFile );

	new szFormattedText[128];
	format_args( szFormattedText, 127, 2 );

	if ( !file_exists( szLogFile ) )
	{
		write_file( szLogFile, "UWC3 : Next Generation", -1 );
		write_file( szLogFile, "Logging of admin commands", -1 );
		write_file( szLogFile, " ", -1 );
	}

	new szAdminName[32], szSteamID[32];
	if ( id > 0 )
	{
		get_user_name( id, szAdminName, 31 );
		get_user_authid( id, szSteamID, 31 );
	}
	else
	{
		copy( szAdminName, 31, "SERVER" );
		copy( szSteamID, 31, "SERVER" );
	}

	new szCurrentTime[32];
	get_time( "%m.%d.%Y %H:%M:%S", szCurrentTime, 31 );

	new szLogEntry[256];
	formatex( szLogEntry, 255, "[%s] %s (%s) used command: '%s %s'", szCurrentTime, szAdminName, szSteamID, szCommand, szFormattedText );

	write_file( szLogFile, szLogEntry, -1 );

	// Gets rid of compiler warning
	if ( szCommand[0] == 0 )
	{
		return;
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
