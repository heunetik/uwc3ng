// Uwc3ng include file

// Will called after a player died
public UWC3NG_Death( iAttacker, iVictim, iHeadshot, iWeaponID )
{
	// The player is dead!
	p_data_b[iVictim][PB_DIEDLASTROUND] = true;
	
	// Suicide Bomber
	if ( p_data_skill[iVictim][SKILL_BOMBER] > 0 )
	{
		SKILL_Suicide( iVictim );
	}
	
	// Award xps
	XP_onDeath( iVictim, iAttacker, iWeaponID, iHeadshot )
	
	// Give the bot a chance to respawn
	if ( is_user_bot( iVictim ) && !get_pcvar_num( CVAR_uwc3ng_ignore_bots ) )
	{
		if ( random_float( 0.0, 1.0 ) <= BOT_RESPAWN )
		{
			// Set the page from the shopmenu2
			p_data[iVictim][SHOPMENU_SIDE] = 1;
			
			// Buy the item scroll
			_ITEM_HandleShopMenu( iVictim, ITEM_SCROLL );
		}
	}
	
	// Remove mole
	if ( p_data_b[iVictim][PB_SKINSWITCHED] )
	{
		p_data_b[iVictim][PB_SKINSWITCHED] = false;
		
		// Reset the model
		SHARED_ChangeSkin( iVictim, 0 );
	}
	
	// Player is nomore hexed
	if ( p_data_b[iVictim][PB_HEXED] )
	{
		p_data_b[iVictim][PB_HEXED] = false;
				
		// Change the skin
		SHARED_ChangeSkin( iVictim, 0 ); // 0 = reset skin
	}
	
	// Check for phoenix respawn
	SKILL_PhoenixSpawn( iVictim );
	
	// Does the user have a scroll?
	if ( ITEM_Has( iVictim, ITEM_SCROLL ) > ITEM_NONE )
	{
		ITEM_Scroll( iVictim );
	}
	
	// Check for Vengeance
	SKILL_Vengeance( iVictim, iAttacker );
	
	// Healing wave task
	if ( task_exists( iVictim + TASK_WAVE ) )
	{
		remove_task( iVictim + TASK_WAVE );
	}
	
	// Leather skin task
	if ( task_exists( iVictim + TASK_SKIN ) )
	{
		remove_task( iVictim + TASK_SKIN );
	}
	
	// Burning task
	if ( task_exists( iVictim + TASK_BURNING ) )
	{
		remove_task( iVictim + TASK_BURNING );
	}
	
	// Carrion Beetles disease
	if ( p_data_b[iVictim][PB_CARRIONDOT] )
		p_data_b[iVictim][PB_CARRIONDOT] = false;
	
	// Shadow Strike poison
	if ( p_data_b[iVictim][PB_SHADOWDOT] )
		p_data_b[iVictim][PB_SHADOWDOT] = false;
	
	// Remove items
	ITEM_UserDied( iVictim );
	
	return;
}

// Here we do damage to a player
public UWC3NG_DoDamage( iVictim, iAttacker, iDamage, iWeapon, iBodyPart )
{
	if ( iAttacker == 0 || iVictim == 0 )
	{
		return;
	}
	
	if ( !is_user_alive( iVictim ) )
	{
		return;
	}
	
	// The user is on godmode
	if ( p_data_b[iVictim][PB_GOD] )
	{
		return;
	}
	
	if ( iBodyPart == -1 )
	{
		iBodyPart = random_num( 1, 7 );
	}
	
	// Modify the amount of damage done based on the user's armor
	new Float:fNewDamage	= float( iDamage ) * ARMOR_RATIO;
	new Float:fArmorLost	= ( float( iDamage ) - fNewDamage ) * ARMOR_BONUS;
	new Float:fCurrentArmor	= float( get_user_armor( iVictim ) );

	// Does this use more armor than we have?
	if ( fArmorLost > fCurrentArmor )
	{
		fArmorLost = fCurrentArmor;
		fArmorLost *= ( 1 / ARMOR_BONUS );

		fNewDamage = float( iDamage ) - fArmorLost;

		set_user_armor( iVictim, 0 );
	}
	else
	{
		new iNewArmor = floatround( fCurrentArmor - ( 3.0 * fArmorLost ) );

		set_user_armor( iVictim, iNewArmor );
	}

	iDamage = floatround( fNewDamage );
	
	new iHeadshot = 0;

	if ( iBodyPart == 1 )
	{
		iHeadshot = 1;
	}
	
	// Psychostats Statistics is turned on!
	if ( get_pcvar_num( CVAR_uwc3ng_psychostats ) )
	{
		if ( CSW_WAR3_MIN <= iWeapon <= CSW_WAR3_MAX )
		{
			new iSkillWeapon = iWeapon - CSW_WAR3_MIN;
			
			// Make the "generic" the stomach
			if ( iBodyPart == -1 )
			{
				iBodyPart = HIT_STOMACH;
			}
		
			if ( iBodyPart == HIT_HEAD )
			{
				iStatsHead[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_CHEST )
			{
				iStatsChest[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_STOMACH )
			{
				iStatsStomach[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_LEFTARM )
			{
				iStatsLeftArm[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_RIGHTARM )
			{
				iStatsRightArm[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_LEFTLEG )
			{
				iStatsLeftLeg[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_RIGHTLEG )
			{
				iStatsRightLeg[iAttacker][iSkillWeapon]++;
			}

			iStatsHits[iAttacker][iSkillWeapon]++;
			iStatsShots[iAttacker][iSkillWeapon]++;
			iStatsDamage[iAttacker][iSkillWeapon] += iDamage;
		}
	}
	
	// Magical damage should go toward kill assist
	g_iDamageDealt[iAttacker][iVictim] += iDamage;
	
	new iHealth = get_user_health( iVictim );
	
	// User has been killed
	if ( iHealth - iDamage <= 0 )
	{
		UWC3NG_Kill( iVictim, iAttacker, iWeapon, iHeadshot );
	}

	// Just do the damage
	else
	{
		set_user_health( iVictim, iHealth - iDamage );
	}

	return;
}

// We kill a player
public UWC3NG_Kill( iVictim, iKiller, iWeapon, iHeadshot )
{
	new szWpnName[64], iRet = 0;
	UTIL_GetWeaponName( iWeapon, szWpnName, 63 );
	ExecuteForward( fwd_ReportKill, iRet, iKiller, iVictim, iWeapon, szWpnName );
	
	// Save stats information?
	if ( get_pcvar_num( CVAR_uwc3ng_psychostats ) )
	{
		if ( CSW_WAR3_MIN <= iWeapon <= CSW_WAR3_MAX )
		{
			new iSkillWeapon = iWeapon - CSW_WAR3_MIN;
			
			if ( SHARED_ValidPlayer( iKiller ) )
			{

				// Team kill
				if ( get_user_team( iVictim ) == get_user_team( iKiller ) )
				{
					iStatsTKS[iKiller][iSkillWeapon]++;
				}

				// Random chance so some skills that aren't headshots have a chance
				if ( ( iHeadshot || random_num( 0, 100 ) < 30 ) )
				{
					iStatsHS[iKiller][iSkillWeapon]++;
				}

				iStatsKills[iKiller][iSkillWeapon]++;
			}

			iStatsDeaths[iKiller][iSkillWeapon]++;
		}
	}
	
	new iVictimTeam = get_user_team( iVictim );
	new iKillerTeam = get_user_team( iKiller );

	// Create Death Message
	if ( is_user_alive( iVictim ) )
	{
		// Kill Victim
		UWC3NG_KillUser( iVictim, iKiller, iWeapon );

		// Update frags ( realtime )
		new iVictimFrags = get_user_frags( iVictim ) + 1;
		set_user_frags( iVictim, iVictimFrags );

		new iVictimDeaths = get_user_deaths( iVictim );

		Create_ScoreInfo( iVictim, iVictimFrags, iVictimDeaths, 0, iVictimTeam );

		// Get the weapon name
		new szWeaponName[32];
		UTIL_GetWeaponName( iWeapon, szWeaponName, 31 );

		Create_DeathMsg_CS( iKiller, iVictim, iHeadshot, szWeaponName );
	}

	// Award $300 for a Kill
	// Make sure they're not on the same team
	if ( iVictimTeam != iKillerTeam && iKiller != iVictim )
	{
		SHARED_SetUserMoney( iKiller, 300 );
	}
	
	// Get the killer's frags
	new iKillerFrags = get_user_frags( iKiller ) + 1;

	// Team Kill
	if ( iVictimTeam == iKillerTeam && iKiller != iVictim )
	{
		// Remove one from the killer's frags

		iKillerFrags = get_user_frags( iKiller ) - 2; // (-2 since when we init'd it was +1)

		set_user_frags( iKiller, iKillerFrags );
	}

	// Update frags ( realtime )
	if ( is_user_alive( iKiller ) )
	{
		new iKillerDeaths = get_user_deaths( iKiller );
		
		// If we don't do this then next round the number of kills will go back to what it was
		set_user_frags( iKiller, iKillerFrags );

		// Updates realtime
		Create_ScoreInfo( iKiller, iKillerFrags, iKillerDeaths, 0, iKillerTeam );
	}
	
	// Call all uwc3ng death options
	UWC3NG_Death( iKiller, iVictim, iHeadshot, iWeapon );
	
	// Log it so psychostats can pick the death up...
	UTIL_LogDeath( iVictim, iKiller, iWeapon );
	
	return;
}

// Kills a user without a deathmsg
public UWC3NG_KillUser( iVictim, iKiller, iWeapon )
{
	set_msg_block( gmsgDeathMsg, BLOCK_ONCE );
	user_kill( iVictim );
}

// Draw the sprites above the players for wcrepair and wcmend
public UWC3NG_DrawSprite( id )
{
	if ( id > TASK_DRAW )
	{
		id -= TASK_DRAW;
	}
	
	if ( !is_user_alive( id ) )
	{
		return;
	}
	
	// Get the life and the armor
	static iHealth, iArmor, CsArmorType: ArmorType;
	iHealth = get_user_health( id );
	iArmor = cs_get_user_armor( id, ArmorType );
	
	new bool:ShowHealth = false, bool:ShowArmor = false;
	
	// Check if the player need health
	if ( iHealth < PLAYER_get_maxhealth( id ) )
	{
		ShowHealth = true;
	}
	
	// Check if the player need armor
	if ( iArmor < 100 && iArmor > 0 )
	{
		ShowArmor = true;
	}
	
	// Get all players
	static players[32], numofplayers, i, iPlayerTeam;
	get_players( players, numofplayers, "a" );
	
	// Get the team
	iPlayerTeam = get_user_team( id );
	
	// Loop through all players 
	for ( i = 0; i < numofplayers; ++i )
	{
		// Isn't in the same team
		if ( get_user_team( players[i] ) != iPlayerTeam )
		{
			continue;
		}
		
		// The player has no wcrepair/mend
		if ( !p_data_skill[players[i]][SKILL_REPAIR] && !p_data_skill[players[i]][SKILL_MEND] )
		{
			continue;
		}
		
		// The player shouldn't see an icon for him self
		if ( players[i] == id )
		{
			continue;
		}
	
		// He needs armor and health, show the correct sprite
		if ( ShowHealth && ShowArmor )
		{
			Create_Sprite( id, players[i], g_iSprites[SPRITE_REPAIRMEND] );
		}
	
		// Only health
		else if ( ShowHealth )
		{
			Create_Sprite( id, players[i], g_iSprites[SPRITE_MEND] );
		}
	
		// Only armor
		else if ( ShowArmor )
		{
			Create_Sprite( id, players[i], g_iSprites[SPRITE_REPAIR] );
		}
	}
	
	// We have to draw again the sprite in a few seconds
	if ( ShowHealth || ShowArmor )
	{
		set_task( 5.0, "UWC3NG_DrawSprite", id + TASK_DRAW );
	}
	
	return;
}

// Function will print a message in the center of the screen (from wc3ft)
public UWC3NG_StatusText( id, iType, const fmt[] )
{
	// I.E. "You have gained a level"
	if ( iType == TXT_TOP_CENTER )
	{
		set_hudmessage( 200, 100, 0, -1.0, 0.25, HUDMESSAGE_FX_FADEIN, 1.0, 2.0, 0.1, 0.2, HUD_AUTO );
	}

	// Ultimate messages
	else if ( iType == TXT_ULTIMATE )
	{
		set_hudmessage( 255, 208, 0, -1.0, 0.85, HUDMESSAGE_FX_FADEIN, 6.0, 0.5, 0.1, 0.5, HUD_ULTIMATE );
	}
	
	// I.E. Suicide Bomb Armed
	else if ( iType == TXT_BLINK_CENTER )
	{
		set_hudmessage( 178, 14, 41, -1.0, -0.4, HUDMESSAGE_FX_FLICKER, 0.5, 1.7, 0.2, 0.2, HUD_AUTO );
	}
	
	// I.E. "You have evaded a shot"
	else if ( iType == TXT_SKILL )
	{
		set_hudmessage( 0, 0, 255, 0.75, 0.85, HUDMESSAGE_FX_FADEIN, 6.0, 3.0, 0.2, 0.7, HUD_SKILL );
	}
	
	else if ( iType == TXT_ULT_ITEM )
	{
		set_hudmessage( 255, 255, 255, 0.015, 0.78, 2, 0.02, 10.0, 0.01, 0.1, HUD_ULT_ITEM );
	}

  // Short duration blink message
	else if ( iType == TXT_BLINK_CENTER_SHRT )
	{
		set_hudmessage( 178, 14, 41, -1.0, -0.4, HUDMESSAGE_FX_FLICKER, 0.5, 0.7, 0.2, 0.2, HUD_AUTO );
	}
	
	// All other cases
	else
	{
		set_hudmessage( 255, 255, 10, -1.0, -0.4, HUDMESSAGE_FX_FLICKER, 0.5, 2.0, 0.2, 0.2, HUD_AUTO );
	}

	// Show the hudmessage
	show_hudmessage( id, fmt );
}

// Function simply checks if an enemy of id is near vOrigin and has a necklace/warden's blink (from wc3ft)
public UWC3NG_IsImmunePlayerNear( id, vOrigin[3] )
{
	new players[32], numplayers, vTargetOrigin[3], i;
	new iTeam = get_user_team( id );

	// Get all players
	get_players( players, numplayers, "a" );
	
	// Loop through all players and check for immunity
	for ( i = 0; i < numplayers; i++ )
	{
		
		// Make sure that the user we're looking at is on the opposite team of "id"
		if ( get_user_team( players[i] ) != iTeam )
		{	
			get_user_origin( players[i], vTargetOrigin );
			
			// Check the distance
			if ( get_distance( vOrigin, vTargetOrigin ) <= IMMUNITY_RADIUS )
			{

				// Does this player have a necklace or warden's blink?
				if ( ULTIMATE_IsImmun( players[i] ) )
				{
					return players[i];
				}
			}
		}
	}

	return 0;
}

public UWC3NG_ShowSpecInfo( id, iTargetID )
{
	// Get the target's name
	new szTargetName[32];
	get_user_name( iTargetID, szTargetName, 31 ); 
	
	new szMsg[512];
	
	// We save the names from the items
	static ItemOneName[32], ItemTwoName[32];

	// Create message
	formatex( szMsg, charsmax( szMsg ), "%s^nRank: %s [LvL %d]  XP: %d^nItems:", szTargetName, p_RankTitles[p_data[iTargetID][PLAYER_RANK]], p_data[iTargetID][PLAYER_LVL], p_data[iTargetID][PLAYER_XP] );
	
	// copy the names from the items in the array
	if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] != ITEM_NONE )
	{
		copy( ItemOneName, charsmax( ItemOneName ), ShopmenuShort_Names[g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE]] );
	}
			
	if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] != ITEM_NONE )
	{
		copy( ItemTwoName, charsmax( ItemTwoName ), ShopmenuShort_Names[g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO]] );
	}
			
	// The item is necklace. Add the charges
	switch ( g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] ) 
	{
		case ITEM_NECKLACE: // Necklace
		{
			formatex( ItemOneName, charsmax( ItemOneName ), "%s[%d]", ItemOneName, g_iNecklaceCharges[iTargetID] );
		}
				
		case ITEM_HELM: // Helm
		{
			formatex( ItemOneName, charsmax( ItemOneName ), "%s[%d]", ItemOneName, g_iHelmCharges[iTargetID] );
		}
				
		case ITEM_RING: // Ring
		{
			formatex( ItemOneName, charsmax( ItemOneName ), "%s[%d]", ItemOneName, g_iTotalRings[iTargetID] );
		}
	}
			
	// Slot two check
	switch ( g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] )
	{
		case ITEM_NECKLACE: // Necklace
		{
			formatex( ItemTwoName, charsmax( ItemTwoName ), "%s[%d]", ItemTwoName, g_iNecklaceCharges[iTargetID] );
		}
				
		case ITEM_HELM: // Helm
		{
			formatex( ItemTwoName, charsmax( ItemTwoName ), "%s[%d]", ItemTwoName, g_iHelmCharges[iTargetID] );
		}
				
		case ITEM_RING: // Ring
		{
			formatex( ItemTwoName, charsmax( ItemTwoName ), "%s[%d]", ItemTwoName, g_iTotalRings[iTargetID] );
		}
	}
			
	// The player has no items
	if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] == ITEM_NONE && g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] == ITEM_NONE )
	{
		// Player has no item, show him that and a little info
		formatex ( szMsg, charsmax( szMsg ), "%s No Items", szMsg );
	}
			
	// The player has two items
	else if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] > ITEM_NONE )
	{
		formatex ( szMsg, charsmax( szMsg ), "%s Magic Items: %s and %s", szMsg, ItemOneName, ItemTwoName );
	}
			
	// The player has only item slot one
	else if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] > ITEM_NONE )
	{
		formatex ( szMsg, charsmax( szMsg ), "%s Magic Item: %s", szMsg, ItemOneName );
	}
			
	// The player has only item slot two
	else if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] > ITEM_NONE )
	{
		formatex ( szMsg, charsmax( szMsg ), "%s Magic Item: %s", szMsg, ItemTwoName );
	}
	
	// Add the Health + Armor to the message
	formatex( szMsg, charsmax( szMsg ), "%s^nHealth: %d  Armor: %d", szMsg, get_user_health( iTargetID ), get_user_armor( iTargetID ) );
	
	// Format the message
	new Float:fSpecTime = SPEC_HOLDTIME;

	if ( get_pcvar_num( CVAR_uwc3ng_spec_position ) == 0 )
	{
		set_hudmessage( 255, 255, 255, 0.018, 0.9, 2, 1.5, fSpecTime, 0.02, 5.0, HUD_SPEC_INFO );
	}
	else
	{
		set_hudmessage( 255, 255, 255, 0.65, 0.9, 2, 1.5, fSpecTime, 0.02, 5.0, HUD_SPEC_INFO );
	}
	
	// Show the message
	show_hudmessage( id, szMsg );
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
