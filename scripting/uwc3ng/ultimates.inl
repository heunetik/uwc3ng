// Uwc3ng Ultimate functions

// Check if a player can use their ultimate
public bool:ULTIMATE_CheckExecute( id, SkillID )
{
	// Dead people can't execute ultimates
	if ( !is_user_alive( id ) )
	{
		client_print( id, print_chat, "%s Shhhh, you are dead, act like it", MOD_NAME );
		
		return false;
	}
	
	// Wcgate can use only once a round except csdm
	if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 0 )
	{
		if ( g_PlayerUltimateDelay[id] > 0 && SkillID == SKILL_GATE )
		{
			static DisplayText[256];
			formatex ( DisplayText, charsmax( DisplayText ), "Ultimates not ready.^n(%d seconds remaining)", g_PlayerUltimateDelay[id] );
			UWC3NG_StatusText( id, TXT_ULTIMATE, DisplayText );
		
			return false;
		}
	}
	
	// CSDM isn't enabled
	else if ( p_data_b[id][USED_GATE] == true && SkillID == SKILL_GATE )
	{
		// The player hasn't skilled intellect, he can't gate twice
		if ( p_data_attrib[id][ATTRIB_INTELLECT] < INTELLECT_GATE || p_data_b[id][USED_GATE_TWICE] )
		{
			client_print( id, print_chat, "%s You have already used gate in this round", MOD_NAME );
		
			return false;
		}
		
		// We should save that this is the 2th time
		p_data_b[id][USED_GATE_TWICE] = true;
	}
	
	// Player has no ultimate
	if ( !p_data_skill[id][SkillID] )
	{
		UWC3NG_StatusText( id, TXT_ULTIMATE, "Ultimate not found." );
		
		return false;
	}
	
	// The ultimate delay isn't over
	if ( g_PlayerUltimateDelay[id] > 0 && SkillID != SKILL_REPAIR && SkillID != SKILL_MEND && SkillID != SKILL_DISPELLHEX && SkillID != SKILL_GATE && SkillID != SKILL_WARD )
	{
		static DisplayText[256];
		formatex ( DisplayText, charsmax( DisplayText ), "Ultimates not ready.^n(%d seconds remaining)", g_PlayerUltimateDelay[id] );
		UWC3NG_StatusText( id, TXT_ULTIMATE, DisplayText );
		
		return false;
	}
	
	// We can't use ultimates in the freezetime
	if ( !g_FreezeTimeOver )
	{
		client_print( id, print_chat, "%s You may not use this ability during freezetime", MOD_NAME );
		
		return false;
	}
	
	// Ultimate after a specific time available
	if ( !g_bUltRoundDelay )
	{
		client_print( id, print_chat, "%s You may not use this ability during round delay time", MOD_NAME );
		
		return false;
	}
	
	// Player is already searching
	if ( p_data_b[id][PB_ISSEARCHING] && SkillID != SKILL_REPAIR && SkillID != SKILL_MEND && SkillID != SKILL_DISPELLHEX && SkillID != SKILL_WARD )
	{
		UWC3NG_StatusText( id, 0, "You are already searching with another ultimate" );
		
		return false;
	}
	
	return true;
}

// This function will be called every second. Here we calculate the ultimate delay
public ULTIMATE_Delay()
{
	// Get all players
	static i, players[32], numofplayers;
	get_players( players, numofplayers );
	
	// Loop through every player 
	for ( i = 0; i < numofplayers; ++i )
	{
		// Remove one second from the ultimate delay from the player
		if ( g_PlayerUltimateDelay[players[i]] > 0 )
		{
			g_PlayerUltimateDelay[players[i]]--;
			
			// The delay is over. The player can use their ultimate
			if ( g_PlayerUltimateDelay[players[i]] == 0 )
			{
				// Show this in the first hud message
				UWC3NG_StatusText( players[i], TXT_ULTIMATE, "Ultimates are now ready." );
			
				// Play the ultimate ready sound
				client_cmd( players[i], "speak %s", g_szSounds[SOUND_ULTIMATEREADY] );
				
				// Suicide bomber is now available again
				if ( p_data_b[players[i]][PB_USED_SUICIDE] )
				{
					p_data_b[players[i]][PB_USED_SUICIDE] = false;
				}
			}
		}
	}
	
	return;
}

// Ultimates can now be used
public ULTIMATE_RemoveStartDelay()
{
	// Ultimates are now able to use
	g_bUltRoundDelay = true;
}

// Reset the cooldown
public ULTIMATE_ResetCooldown( id, iCooldown )
{
	g_PlayerUltimateDelay[id] = iCooldown;
}

// Ultimate Immun check
public bool:ULTIMATE_IsImmun( iVictim )
{
	// The player has blink
	if ( p_data_b[iVictim][PB_BLINK] || ITEM_Has( iVictim, ITEM_NECKLACE ) > ITEM_NONE )
	{
		// Remove one charge
		ULTIMATE_RemoveCharge( iVictim );
		
		return true;
	}
	
	return false;
}

// Ultimate was blocked
public ULTIMATE_Blocked( id )
{
	// Maybe play a sound here instead?
	client_print( id, print_chat, "%s Your ultimate has been blocked", MOD_NAME );

	// Play ultimate blocked sound
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_DISPELL], 1.0, ATTN_NORM, 0, PITCH_NORM );
}

public ULTIMATE_RemoveCharge( id )
{

	if ( ITEM_Has( id, ITEM_NECKLACE ) > ITEM_NONE )
	{
		ITEM_RemoveCharge( id, ITEM_NECKLACE );
	}
		
	client_print( id, print_chat, "%s You have blocked an enemy's ultimate!", MOD_NAME );

	// Play ultimate blocked sound
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_SPELLSHIELD], 1.0, ATTN_NORM, 0, PITCH_NORM );
}


/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
