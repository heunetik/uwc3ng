// Uwc3ng XP functions

// Calculate the xps for the player
public XP_Calc( id, XPs )
{
	// The player is on level 0. He get the default xps
	if ( p_data[id][PLAYER_LVL] <= 0 )
	{
		return XPs;
	}
	
	// Okay, he isn't on level 0. Get the xp multiplier
	new Float:XPmultiplier = get_pcvar_float( CVAR_uwc3ng_xpmultiplier );
	
	// Should we work with the xp multiplier?
	if ( XPmultiplier )
	{
		// First, we divide the player level through the max level and add one
		new Float:AddedXPs = ( float( p_data[id][PLAYER_LVL] ) / float( MAX_LEVEL ) + 1.0 );
		
		// Now we multiple this with the default XPs
		XPs = floatround( float( XPs ) * AddedXPs );
		
		// Okay. Here comes the employment for the xp multiplier
		XPs = floatround( float( XPs ) * XPmultiplier );
	}
	
	// return the xps
	return XPs;
}

// Give or take some xps
public XP_Give( id, XPs )
{
	if ( !UWC3NG_Check() || !id )
	{
		return 0;
	}
	
	// Make sure we have the minimum amount of players
	if ( !XP_MinPlayers() )
	{
		return 0;
	}
	
	// Okay. Add the player the xps. We don't have to calculate because it's finally done. (look XP_Calc)
	p_data[id][PLAYER_XP] += XPs;
	
	// Check if the player gained or lost a level
	XP_CheckLevel( id );
	
	if ( is_user_alive( id ) )
	{
		// Show him the status text
		PLAYER_ShowStatusText( id );
	}
	
	return XPs;
}

// Give a complete Team xps for winning the round
public XP_GiveTeam( iTeam )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	// Get all players
	static players[32], numofplayers, i, iXPs, iBonusXP;
	get_players( players, numofplayers );
	
	// Loop through all players from the winning team and give they xps
	for ( i = 0; i < numofplayers; ++i )
	{
		if ( get_user_team( players[i] ) == iTeam )
		{
			// Calculate the xps for every user 
			iXPs = XP_Calc( players[i], WINNING_TEAM );
			
			// Give him the xps
			iBonusXP = XP_Give( players[i], iXPs );
			
			// Write him a message when all is okay
			if ( iBonusXP != 0 )
			{
				client_print( players[i], print_chat, "%s You recieved %d XP for winning the round", MOD_NAME, iBonusXP );
			}
		}
	}
	
	return;
}
			

// Calculate the level with the xps and the xp table
public XP_CalcLevel( id )
{
	// We loop now through all entrys and search for the level
	static i;
	for ( i = 0; i < MAX_LEVEL; ++i )
	{
		// The Player is on the last level
		if ( p_data[id][PLAYER_XP] >= g_XPforLevel[MAX_LEVEL] )
		{
			// Save the level in the array
			p_data[id][PLAYER_LVL] = MAX_LEVEL;
			
			// we can go out of the loop
			break;
		}
		
		// The player isn't on the last level, check all other levels
		if ( p_data[id][PLAYER_XP] >= g_XPforLevel[i] && p_data[id][PLAYER_XP] < g_XPforLevel[i+1] )
		{
			// Save the level in the array
			p_data[id][PLAYER_LVL] = i;
			
			// we can go out ot the loop
			break;
		}
	}
}

// Check if the player gained or lost a level
public XP_CheckLevel( id )
{
	// Save the old level
	new iOldLevel = p_data[id][PLAYER_LVL];
	
	// Calculate the new level by the xp table
	XP_CalcLevel( id );
	
	// WoW! He gained a level
	if ( iOldLevel < p_data[id][PLAYER_LVL] )
	{
		// Display him a hudmessage
		UWC3NG_StatusText( id, TXT_TOP_CENTER, "You gained a Level" );
		
		// Only play the sound if the user is alive!!
		if ( is_user_alive( id ) )
		{
			emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_LEVELUP], 1.0, ATTN_NORM, 0, PITCH_NORM );
		}
		
		// The player get skillpoints
		SKILL_available_points( id );
		
		// The player get maybe attribute points
		ATTRIB_Check_Available_points( id );
		
		// The player get maybe resistance points
		RESIST_Check_Available_points( id );
		
		// The player is a bot, he should random select the available points
		if ( is_user_bot( id ) && !get_pcvar_num( CVAR_uwc3ng_ignore_bots ) )
		{
			PLAYER_random_abilitys( id );
		}
		
		// Print a message if he has available skillpoints
		client_print( id, print_chat, "%s You have %d skillpoint(s) available (use /selectskill)", MOD_NAME, p_data[id][AVAILABLE_SKILL] );
		
		// Should he get a message that he has available attrib points?
		if ( p_data[id][AVAILABLE_ATTRIBS] > 0 )
		{
			client_print( id, print_chat, "%s You have %d attribute points available (use /selectattrib)", MOD_NAME, p_data[id][AVAILABLE_ATTRIBS] );
		}
		
		// Should he get a message that he has available resistant points?
		if ( p_data[id][AVAILABLE_RESISTS] > 0 )
		{
			client_print( id, print_chat, "%s You have %d resistance points available (use /selectresist)", MOD_NAME, p_data[id][AVAILABLE_RESISTS] );
		}
		
		// Look if the player gained a new rank
		new iOldRankID = p_data[id][PLAYER_RANK];
		PLAYER_Set_Rank( id );
		
		if ( p_data[id][PLAYER_RANK] > iOldRankID )
		{
			new PlayerName[32];
			get_user_name( id, PlayerName, charsmax( PlayerName ) );
			
			client_print( 0, print_chat, "%s Announcement :: %s is now %s", MOD_NAME, PlayerName, p_RankTitles[p_data[id][PLAYER_RANK]] ); 
		}
	}
	
	// He lost a level ;(
	if ( iOldLevel > p_data[id][PLAYER_LVL] )
	{
		// Display him a hudmessage
		UWC3NG_StatusText( id, TXT_TOP_CENTER, "You lost a Level" );
		
		// Reset all skills, attribs and resistant points
		p_data_b[id][RESET_ATTRIBS] = true;
		p_data_b[id][RESET_SKILLS] = true;
		p_data_b[id][RESET_RESISTS] = true;
		
		// Look if the player lost a rank
		PLAYER_Set_Rank( id );
	}
}

// Check if there are enough players on the server to take/give players xp
bool:XP_MinPlayers()
{
	new iNum = 0;

	// Don't ignore bots
	if ( get_pcvar_num( CVAR_uwc3ng_ignore_bots ) == 0 )
	{
		iNum = get_playersnum();
	}

	// Ignore bots, spectators, and HLTV
	else
	{
		new players[32], iNumCT;
		
		// Get all terrorists
		get_players( players, iNum, "ce", "TERRORIST" );
		
		// Get all counterterrorists
		get_players( players, iNumCT, "ce", "CT" );
		
		// Add the two
		iNum += iNumCT;
	}

	if ( iNum < get_pcvar_num( CVAR_uwc3ng_min_players ) )
	{
		return false;
	}

	return true;
}

// Set up the XP table (called after 0.8 seconds from the server start)
public XP_SetUpTable()
{
	static i, XPs;
	
	// Get the default xps for every level from the cvar
	XPs = get_pcvar_num( CVAR_uwc3ng_xp_for_level );
	
	// If we use shortterm xps
	if ( get_pcvar_num( CVAR_uwc3ng_save_xp ) )
	{
		// We divide through 10 for shortterm xps
		XPs /= 10;
	}
	
	// Okay. We use longterm xps
	else
	{
		XPs /= 3;
	}
		
	// We loop through every entry and fill it up. Level zero is 0 xp 
	for ( i = 1; i <= MAX_LEVEL; ++i )
	{
		// The first level get the default xps
		if ( i == 1 )
		{
			g_XPforLevel[i] = XPs;
		}
			
		// The first 10 level's should be easy to get
		else if ( i <= 10 )
		{
			g_XPforLevel[i] = floatround( float( g_XPforLevel[i-1] ) * 1.00 ) + XPs;
		}
			
		// 11 till 20 
		else if ( i <= 20 )
		{
			g_XPforLevel[i] = floatround( float( g_XPforLevel[i-1] ) * 1.10 ) + XPs;
		}
			
		// 21 till 30
		else if ( i <= 30 )
		{
			g_XPforLevel[i] = floatround( float( g_XPforLevel[i-1] ) * 1.25 ) + XPs;
		}
			
		// 31 till 40
		else if ( i <= 40 )
		{
			g_XPforLevel[i] = floatround( float( g_XPforLevel[i-1] ) * 1.35 ) + XPs;
		}
		
		// 41 till 50
		else if ( i <= 50 )
		{
			g_XPforLevel[i] = floatround( float( g_XPforLevel[i-1] ) * 1.45 ) + XPs;
		}
			
		// All other (should not called)
		else
		{
			g_XPforLevel[i] = floatround( float( g_XPforLevel[i-1] ) * 2.50 );
		}
	}
	
	// Configure the weapon multiplier
	fWpnXPMultiplier[CSW_USP] 				= 2.5;
	fWpnXPMultiplier[CSW_DEAGLE]				= 2.5;
	fWpnXPMultiplier[CSW_GLOCK18]				= 2.5;
	fWpnXPMultiplier[CSW_ELITE]				= 3.5;
	fWpnXPMultiplier[CSW_P228]				= 2.5;
	fWpnXPMultiplier[CSW_FIVESEVEN]				= 2.5;

	fWpnXPMultiplier[CSW_XM1014]				= 1.25;
	fWpnXPMultiplier[CSW_M3]				= 1.5;

	fWpnXPMultiplier[CSW_MP5NAVY]				= 1.0;
	fWpnXPMultiplier[CSW_UMP45]				= 1.25;
	fWpnXPMultiplier[CSW_P90]				= 1.25;
	fWpnXPMultiplier[CSW_TMP]				= 1.5;
	fWpnXPMultiplier[CSW_MAC10]				= 1.5;
	fWpnXPMultiplier[CSW_GALIL]				= 1.15;
	fWpnXPMultiplier[CSW_FAMAS]				= 1.15;

	fWpnXPMultiplier[CSW_AWP]				= 1.0;
	fWpnXPMultiplier[CSW_M4A1]				= 1.0;
	fWpnXPMultiplier[CSW_AK47]				= 1.0;
	fWpnXPMultiplier[CSW_AUG]				= 1.0;
	fWpnXPMultiplier[CSW_SG552]				= 1.0;
	fWpnXPMultiplier[CSW_G3SG1]				= 1.0;
	fWpnXPMultiplier[CSW_SG550]				= 1.0;
	fWpnXPMultiplier[CSW_M249]				= 1.25;
	fWpnXPMultiplier[CSW_SCOUT]				= 3.0;

	fWpnXPMultiplier[CSW_HEGRENADE]				= 1.75;
	fWpnXPMultiplier[CSW_KNIFE]				= 6.0;

	fWpnXPMultiplier[CSW_C4]				= 2.0;
	fWpnXPMultiplier[CSW_SMOKEGRENADE]			= 1.0;
	fWpnXPMultiplier[CSW_FLASHBANG]				= 1.0;
}

// Show a player the xp table in the console
public XP_TableShow( id )
{
	if ( !is_user_connected( id ) || !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	static i, iTotalXP;
	for ( i = 0; i <= MAX_LEVEL; ++i )
	{
		// Calculate all XPs
		iTotalXP += g_XPforLevel[i];
		
		// Show him the table
		client_print( id, print_console, "[Level %d] XP: %d  Total XP: %d", i, g_XPforLevel[i], iTotalXP );
	}
	
	return PLUGIN_HANDLED;
}

// Function from war3x thanks ryan!!!
public XP_GetAdminFlag()
{

    new szFlags[24];
    get_pcvar_string( CVAR_uwc3ng_admin_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}

// Give someone xps for killing the enemy
public XP_onDeath( iVictim, iAttacker, iWeaponIndex, iHeadshot )
{

	// We don't want to give XP to the world
	if ( iAttacker == 0 )
	{
		return;
	}

	// We don't want to give XP to suiciders
	else if ( iAttacker == iVictim )
	{
		return;
	}
	
	new iBonusXP, iXP = floatround( float( XP_Calc( iAttacker, KILL_ENEMY_XP ) ) * fWpnXPMultiplier[iWeaponIndex] );

	// Check for a team kill
	if ( get_user_team( iAttacker ) == get_user_team( iVictim ) && CVAR_csdm_active <= 0 )
	{
		// Remove XP since he killed his teammate
		iBonusXP = XP_Give( iAttacker, -1 * iXP );

		// This message should be displayed no matter what XP_kill_objectives is, b/c it's a no-no
		if ( iBonusXP != 0 )
		{
			client_print( iAttacker, print_chat, "%s You have lost %d XP for killing a teammate", MOD_NAME, -1 * iBonusXP );
		}
	}

	// Otherwise the player killed the other team
	else
	{
		// Award XP for a kill
		iBonusXP = XP_Give( iAttacker, iXP );
		
		if ( iBonusXP != 0 )
		{
			client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the enemy", MOD_NAME, iBonusXP );
		}

		// User had a headshot?  Give bonus XP!
		if ( iHeadshot )
		{
			new gXPs = XP_Calc( iAttacker, KILL_HEADSHOT_XP );
			iBonusXP = XP_Give( iAttacker, gXPs );
			
			if ( iBonusXP != 0 )
			{
				client_print( iAttacker, print_chat, "%s You have been awarded %d XP for getting a headshot", MOD_NAME, iBonusXP );
			}
		}

		new iAssistXP, iVictimMaxHealth;
		new Float:fMultiplier;
		
		// Get all players
		static players[32], numofplayers, id;
		get_players( players, numofplayers );

		// Award XP for other people doing damage to this victim
		for ( new i = 0; i < numofplayers; i++ )
		{
			// Save the current id to 'id'
			id = players[i];
			
			// Then this player dealt some damage to this player this round
			if ( g_iDamageDealt[id][iVictim] > 0 && iAttacker != id )
			{
				iVictimMaxHealth = PLAYER_get_maxhealth( iVictim );
				fMultiplier = float( g_iDamageDealt[id][iVictim] ) / float( iVictimMaxHealth );
				
				iAssistXP = XP_Calc( id, KILL_ENEMY_XP );

				// Need a ratio of XP to award to person who dealt damage
				iBonusXP = XP_Give( id, ( floatround( float( iAssistXP ) * fMultiplier ) / 2 ) );

				if ( iBonusXP != 0 )
				{
					client_print( id, print_chat, "%s You have been awarded %d XP for a kill assist!", MOD_NAME, iBonusXP );
				}

				// victim may respawn, so reset the counter
				g_iDamageDealt[id][iVictim] = 0;
			}
		}

		// User killed a hostage rescuer
		if ( g_iPlayerRole[iVictim] == PLR_HOSTAGE_RESCUER )
		{
			// Calculate the xps which the player should get
			new iXPs = XP_Calc( id, KILL_HOSTAGE_SAVER );
		
			iBonusXP = XP_Give( iAttacker, iXPs );
				
			if ( iBonusXP != 0 )
			{
				client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the hostage rescuer", MOD_NAME, iBonusXP );
			}
		}
		// User killed the bomb defuser
		else if ( g_iPlayerRole[iVictim] == PLR_BOMB_DEFUSER )
		{
			// Calculate the xps which the player should get
			new iXPs = XP_Calc( id, KILL_DEFUSER );
			
			iBonusXP = XP_Give( iAttacker, iXPs );
			
			if ( iBonusXP != 0 )
			{
				client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the bomb defuser", MOD_NAME, iBonusXP );
			}
		}
		// User killed the bomb planter
		else if ( g_iPlayerRole[iVictim] == PLR_BOMB_PLANTER )
		{
			// Calculate the xps which the player should get
			new iXPs = XP_Calc( id, KILL_PLANTER );
			
			iBonusXP = XP_Give( iAttacker, iXPs );
				
			if ( iBonusXP != 0 )
			{
				client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the bomb planter", MOD_NAME, iBonusXP );
			}
		}
		// User killed the bomb carrier
		else if ( g_iPlayerRole[iVictim] == PLR_BOMB_CARRIER )
		{
			// Calculate the xps which the player should get
			new iXPs = XP_Calc( id, KILL_BOMB_CARRIER );
			
			iBonusXP = XP_Give( iAttacker, iXPs );
				
			if ( iBonusXP != 0 )
			{
				client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the bomb carrier", MOD_NAME, iBonusXP );
			}
		}
		// user killed the VIP
		else if ( g_iPlayerRole[iVictim] == PLR_VIP )
		{
			// Calculate the xps which the player should get
			new iXPs = XP_Calc( id, KILL_VIP );
			
			iBonusXP = XP_Give( iAttacker, iXPs );
				
			if ( iBonusXP != 0 )
			{
				client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the VIP", MOD_NAME, iBonusXP );
			}
		}
		
		// Player died, so lets reset their data
		g_iPlayerRole[iVictim] = PLR_NONE;
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
