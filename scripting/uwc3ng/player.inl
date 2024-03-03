// Uwc3ng player functions

// This function check if a player is in range from another player
public bool:PLAYER_is_in_range( playerID, friendID, Range )
{
	// We get the position from the two players
	static playerOrigin[3], friendOrigin[3];
	
	get_user_origin( playerID, playerOrigin );
	get_user_origin( friendID, friendOrigin );
	
	// We get the distance
	new distance = get_distance( playerOrigin, friendOrigin );
	
	// is he in range?
	if ( distance <= Range )
	{
		return true;
	}
	
	// He is not in range :(
	return false;
}

// returns the maxhealth from a player
public PLAYER_get_maxhealth( id )
{
	new iLevel = p_data_skill[id][SKILL_DEVOTION];
	new iLevelStrength = p_data_attrib[id][ATTRIB_STRENGTH];
	new iHealth = 0;
	
	// The player has devotion aura
	if ( iLevel )
	{
		// The player has devotion aura, he has more maxhealth
		iHealth = p_devotion[iLevel-1];
	}
	
	// The player has attribs strength
	if ( iLevelStrength > MAX_ATTRIBS_STARTPOINTS )
	{
		iHealth += AGILITY_HEALTH_BONUS * ( iLevelStrength - MAX_ATTRIBS_STARTPOINTS );
	}
	
	// CS default health is 100
	return ( 100 + iHealth );
}

// This function will the player on a spezific time jump
public PLAYER_Jump( id )
{
	id -= TASK_JUMP;
	
	// Is the user alive?
	if ( is_user_alive( id ) )
	{
		// Is the player hexed and not the round over?
		if ( p_data_b[id][PB_HEXED] && !g_EndRound )
		{
			// Let him jump
			client_cmd( id, "+jump;wait;-jump" );
		
			// Set the task again
			set_task( HEXED_JUMP_TIME, "PLAYER_Jump", id + TASK_JUMP );
		}
	}
	
	return;
}

// This function will create random skills, attributes and resistance for a bot
public PLAYER_random_abilitys( id )
{
	// The player is a bot. We give him here random skills, attributes and resistance if the bot has a startlevel
	new iBotPoints = p_data[id][AVAILABLE_SKILL], iRandomSkill, i;
	if ( iBotPoints > 0 )
	{
		// Reset the available skills
		p_data[id][AVAILABLE_SKILL] = 0;
		
		// Skills
		for ( i = 0; i < iBotPoints; ++i )
		{
			iRandomSkill = random_num( 0, ( MAX_SKILLS - 1 ) );
			
			// Set up the random skill
			if ( p_data_skill[id][iRandomSkill] != Skill_Max_Points[iRandomSkill] )
			{
				p_data_skill[id][iRandomSkill]++;
			}
		}
	}
	
	iBotPoints = p_data[id][AVAILABLE_ATTRIBS];
	if ( iBotPoints > 0 )
	{
		// Reset the available attribs
		p_data[id][AVAILABLE_ATTRIBS] = 0;
		
		// Attributes
		static iRandomAttrib
		for ( i = 0; i < iBotPoints; ++i )
		{
			// Get a random attrib
			iRandomAttrib = random_num( 0, ( MAX_ATTRIBS - 1 ) );
			
			// Set up the attrib
			if ( p_data_attrib[id][iRandomAttrib] != MAX_ATTRIBS_POINTS )
			{
				p_data_attrib[id][iRandomAttrib]++;
			}
		}
	}
	
	iBotPoints = p_data[id][AVAILABLE_RESISTS];
	if ( iBotPoints > 0 )
	{
		// Reset the available resists
		p_data[id][AVAILABLE_RESISTS] = 0;
		
		// Resistances
		static iRandomResist;
		for ( i = 0; i < iBotPoints; ++i )
		{
			// Get a random resistant
			iRandomResist = random_num( 0, ( MAX_RESIST - 1 ) );
			
			// Set up the resist
			if ( p_data_resist[id][iRandomResist] != MAX_RESIST_POINTS )
			{
				p_data_resist[id][iRandomResist]++;
			}
		}
	}
	
	return;
}

// Set the startlevel of each player
public PLAYER_set_startlevel( id )
{
	new iPlayerLevel = get_pcvar_num( CVAR_uwc3ng_startlevel );
	if ( iPlayerLevel > 0 && p_data[id][PLAYER_XP] < g_XPforLevel[iPlayerLevel] && !is_user_bot( id ) )
	{
		p_data[id][PLAYER_XP] = g_XPforLevel[iPlayerLevel];
		p_data[id][PLAYER_LVL] = iPlayerLevel;
	}
	
	return;
}

// Show the StatusText
public PLAYER_ShowStatusText( id )
{
	// Bots not interested in the status text
	if ( is_user_bot( id ) )
	{
		return;
	}
	
	new vTempTxT[128];
	
	// The player hasn't maybe a rank. Set it up
	if ( !p_data[id][PLAYER_RANK] )
	{
		PLAYER_Set_Rank( id );
	}
	
	// Create the message for the status text
	formatex( vTempTxT, charsmax( vTempTxT ), "Rank: %s [%d]   XP: %d", p_RankTitles[p_data[id][PLAYER_RANK]], p_data[id][PLAYER_LVL], p_data[id][PLAYER_XP] ); 
			
	// Display the message
	Create_StatusText( id, 0, vTempTxT );
}

// Set the rank for a player
public PLAYER_Set_Rank( id )
{
	// Calculate one rank for how much levels
	new Float:iRankOnLevel = float( MAX_LEVEL ) / float( MAX_RANKS - 1 );
	
	// Set up the rank id
	p_data[id][PLAYER_RANK] = floatround( float( p_data[id][PLAYER_LVL] ) / iRankOnLevel );
}

// Reset the player data
public PLAYER_ResetData( id )
{
	// Reset the complete p_data
	static i;
	for ( i = 0; i < MAX_DATA; ++i )
	{
		p_data[id][i] = 0;
	}
	
	// Reset the complete p_data_b
	for ( i = 0; i < MAX_DATA_B; ++i )
	{
		p_data_b[id][i] = false;
	}
	
	// Reset xp assist
	for ( i = 0; i < MAX_PLAYERS; i++ )
	{
		g_iDamageDealt[id][i] = 0;
	}
	
	// Reset the attribs
	for ( i = 0; i < MAX_ATTRIBS; ++i )
	{
		p_data_attrib[id][i] = MAX_ATTRIBS_STARTPOINTS;
	}
	
	// Reset the resistance
	for ( i = 0; i < MAX_RESIST; ++i )
	{
		p_data_resist[id][i] = 0;
	}
	
	// Reset the skills
	for ( i = 0; i < MAX_SKILLS; ++i )
	{
		p_data_skill[id][i] = 0;
	}
	
	// Reset the counter for the ultimates
	g_iCountUltimates[id] = 0;
	
	// Reset the armor setting
	bIgnoreArmorSet[id] = false;
	
	// Reset items when the user disconnect!
	g_iShopMenuItems[id][ITEM_SLOT_ONE] = ITEM_NONE;
	g_iShopMenuItems[id][ITEM_SLOT_TWO] = ITEM_NONE;
}
	

	
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
