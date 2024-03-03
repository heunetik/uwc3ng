// Uwc3ng skills5 functions (34-42)

// Vengeance
public SKILL_Vengeance( id, iAttacker )
{
	// The player has no Vengeance
	if ( !p_data_skill[id][SKILL_VENGEANCE] )
	{
		return;
	}
	
	// The enemy can block this 
	else if ( ULTIMATE_IsImmun( iAttacker ) )
	{
		UWC3NG_StatusText( id, 0, "Vengeance Failed^nEnemy has Immunity" );
		
		return;
	}
	
	// We need to respawn the user!!
	if ( !is_user_alive( id ) )
	{

		// Make sure the user isn't changing teams and we're not at the end of the round
		if ( !g_EndRound && SHARED_IsOnTeam( id ) )
		{
			// The player has intellect, give him the bonus
			if ( p_data_attrib[id][ATTRIB_INTELLECT] >= INTELLECT_VENGEANCE && !p_data_b[id][PB_RESPAWNED_TWICE] && p_data_b[id][PB_RESPAWNED] )
			{
				p_data_b[id][PB_RESPAWNED] = false;
				p_data_b[id][PB_RESPAWNED_TWICE] = true;
			}
				
			// Player should only one time recieved
			if ( p_data_b[id][PB_RESPAWNED] )
			{
				return;
			}
			
			// He will respawn
			p_data_b[id][PB_RESPAWNED] = true;

			// Set up the respawn task
			set_task( SPAWN_DELAY, "_SHARED_Spawn", TASK_SPAWN + id );
		}
	}
	
	return;
}

// ---------------------------------------- Start Chain Lightning ----------------------------------
public SKILL_ChainLightning( id )
{
	if ( !ULTIMATE_CheckExecute( id, SKILL_CHAINLIGHTNING ) )
	{
		return;
	}
	
	// He is on "searching"
	p_data_b[id][PB_ISSEARCHING] = true;
	
	// With what ultimate is he searching?
	p_data[id][P_ULT_SEARCH] = SKILL_CHAINLIGHTNING;
	
	// Start the Searching task 
	new parm[2];
	parm[0] = id;
	parm[1] = 5; // 5 seconds after that the ultimate is disabled till the player press again the ultimate button
	
	_SHARED_PING( parm );
	
	return;
}
	
// Initial Ultimate call
public SKILL_ChainLightningInit( iCaster, iTarget, iBodyPart )
{

	// Display the lightning effect
	SKILL_ChainEffect( iCaster, iTarget, CHAINLIGHTNING_LINEWIDTH, CHAINLIGHTNING_DAMAGE, iBodyPart );
	

	// Now we need to search for the next "jump"
	new parm[5];
	parm[0] = iTarget;
	parm[1] = CHAINLIGHTNING_DAMAGE;
	parm[2] = CHAINLIGHTNING_LINEWIDTH;
	parm[3] = iCaster;
	parm[4] = iBodyPart;

	set_task( 0.2, "_SKILL_ChainLightningInit", TASK_LIGHTNING + iTarget, parm, 5 );
}

// Called on subsequent ultimate calls ( i.e. lightninghitnext )
public _SKILL_ChainLightningInit( parm[5] )
{
	// parm[0] = enemy
	// parm[1] = damage
	// parm[2] = linewidth
	// parm[3] = caster
	// parm[4] = bodypart

	new iEnemy = parm[0];

	if ( !is_user_alive( iEnemy ) )
	{
		return;
	}

	new iCaster	= parm[3];
	new iBodyPart	= parm[4];
	new iCasterTeam	= get_user_team( iCaster )

	new vOrigin[3];
	get_user_origin( iEnemy, vOrigin );

	new players[32], numberofplayers;
	get_players( players, numberofplayers, "a" );


	new i, iTarget = 0;
	new iClosestTarget = 0, iClosestDistance = 0;
	new iDistanceBetween = 0;
	new vTargetOrigin[3]

	// Loop through every player
	for ( i = 0; i < numberofplayers; i++ )
	{
		iTarget = players[i];
		
		// Make sure our target player isn't on the same team!
		if ( get_user_team( iTarget ) != iCasterTeam )
		{
			get_user_origin( iTarget, vTargetOrigin )
			
			iDistanceBetween = get_distance( vOrigin, vTargetOrigin );

			// Verify the user is within range and has not been previously hit
			if ( iDistanceBetween < LIGHTNING_RANGE && !p_data_b[iTarget][PB_LIGHTNINGHIT] )
			{

				// Make sure target isn't immune!
				if ( ULTIMATE_IsImmun( iTarget )  )
				{
					// Let the caster know the ultimate was blocked!
					ULTIMATE_Blocked( iCaster );
					
					// Just say the user was hit so they aren't tried to be hit the next round (possibility of removing multiple charges)
					p_data_b[iTarget][PB_LIGHTNINGHIT] = true;
				}
				
				// Lightning Resistance check
				else if ( p_data_resist[iTarget][RESIST_LIGHTNING] > 0 )
				{
					// Check if the player will resist this hit
					if ( random_float( 0.0, 1.0 ) <= ( p_data_resist[iTarget][RESIST_LIGHTNING] / MAX_RESIST_POINTS ) )
					{
						client_print( iTarget, print_chat, "%s Your resistance to electricity absorbs all of the Chain Lightning damage", MOD_NAME );
						client_print( iCaster, print_chat, "%s Your chain lightning attack is deflected by your target's electricity resistance", MOD_NAME );
						
						// Just say the user was hit so they aren't tried to be hit the next round (possibility of removing multiple charges)
						p_data_b[iTarget][PB_LIGHTNINGHIT] = true;
					}
				}

				// Then we can hit this user!
				else
				{
					
					// This user is closest!! Lets make a note of this...
					if ( iDistanceBetween < iClosestDistance || iClosestTarget == 0 )
					{
						iClosestDistance = iDistanceBetween;
						iClosestTarget = iTarget;
					}

				}//end immune check
			}//end distance/hit check
		}//end team check
	}//end for

	// Then we have a valid target!!!
	if ( iClosestTarget )
	{
		// Damage should be decreased by 2/3 on each jump
		parm[1] = floatround(float(parm[2])*2/3);

		// Decrease line width as well
		parm[2] = floatround(float(parm[2])*2/3);
		
		// Display the actual lightning
		SKILL_ChainEffect( iCaster, iClosestTarget, parm[2], parm[1], iBodyPart );

		// Lets call this again on our next target!
		parm[0] = iClosestTarget;
		set_task( 0.2, "_SKILL_ChainLightningInit", TASK_LIGHTNINGNEXT + iCaster, parm, 5 );
	}

	// No valid target found - reset all lightning hit variables
	else
	{
		for ( i = 0; i < numberofplayers; i++ )
		{
			iTarget = players[i];

			p_data_b[iTarget][PB_LIGHTNINGHIT] = false;
		}
	}

	return;
}

public SKILL_ChainEffect( iCaster, iTarget, iLineWidth, iDamage, iBodyPart )
{

	// Make sure we set this user as hit, otherwise we'll hit him again
	p_data_b[iTarget][PB_LIGHTNINGHIT] = true;
	
	// Damage the user
	UWC3NG_DoDamage( iTarget, iCaster, iDamage, CSW_LIGHTNING, iBodyPart );
	
	// Create the lightning
	Create_TE_BEAMENTS( iCaster, iTarget, g_iSprites[SPRITE_LIGHTNING], 0, 15, 10, iLineWidth, 10, 255, 255, 255, 255, 0 );
	
	// Get the target's origin
	new vOrigin[3]
	get_user_origin( iTarget, vOrigin );
	
	// Create an elight on the target
	Create_TE_ELIGHT( iTarget, vOrigin, 100, 255, 255, 255, 10, 0 );

	// Play the lightning sound
	emit_sound( iCaster, CHAN_STATIC, g_szSounds[SOUND_LIGHTNING], 1.0, ATTN_NORM, 0, PITCH_NORM );

	return;
}
// -------------------------------------- End Chain Lightning --------------------------------------

// -------------------------------------- Start Entangle Roots -------------------------------------
public SKILL_EntangleRoots( id )
{
	if ( !ULTIMATE_CheckExecute( id, SKILL_ENTANGLEROOTS ) )
	{
		return;
	}
	
	// He is on "searching"
	p_data_b[id][PB_ISSEARCHING] = true;
	
	// With what ultimate is he searching?
	p_data[id][P_ULT_SEARCH] = SKILL_ENTANGLEROOTS;
	
	// Start the Searching task 
	new parm[2];
	parm[0] = id;
	parm[1] = 5; // 5 seconds after that the ultimate is disabled till the player press again the ultimate button
	
	_SHARED_PING( parm ); 
	
	return;
}
	
	
// Initiate Ultimate
public SKILL_Entangle( iCaster, iEnemy )
{
	// Magic resistance check
	if ( p_data_resist[iEnemy][RESIST_MAGIC] > 0 )
	{
		if ( random_float( 0.0, 1.0 ) <= ( p_data_resist[iEnemy][RESIST_MAGIC] / MAX_RESIST_POINTS ) )
		{
			client_print( iEnemy, print_chat, "%s Your magic resistance protects you from a entangle roots attack", MOD_NAME );
			client_print( iCaster, print_chat, "%s Your entangle roots attack is deflected by your target's magic resistance", MOD_NAME );
			
			return;
		}
	}

	// Follow the user until they stop moving...
	Create_TE_BEAMFOLLOW( iEnemy, g_iSprites[SPRITE_TRAIL], 10, 5, 10, 108, 23, 255 );
	
	// User is now stunned so we can't do any other stun abilities
	p_data_b[iEnemy][PB_STUNNED] = true;
	
	// Set the speed of the enemy (this will auto-stun them)
	SKILL_UnholyAura( iEnemy );
	
	// Start waiting for the user to stop...
	new parm[4];
	parm[0] = iEnemy;
	parm[1] = 0;
	parm[2] = 0;
	parm[3] = 0;
	_SKILL_EntangleWait( parm );
	
	// Drop the user's weapon
	if ( get_pcvar_num( CVAR_uwc3ng_entangle_drop ) )
	{
		new ammo, clip;
		new iWeapon = get_user_weapon( iEnemy, ammo, clip );
		
		// Only drop the weapon if it is the user's primary weapon
		if ( SHARED_IsPrimaryWeapon( iWeapon ) )
		{
			client_cmd( iEnemy, "drop" );
		}
	}
}



// Wait for the user to stop moving
public _SKILL_EntangleWait( parm[4] )
{

	new id = parm[0];

	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	new vOrigin[3];
	get_user_origin( id, vOrigin );
	
	// Checking to see if the user has actually stopped yet?
	if ( vOrigin[0] == parm[1] && vOrigin[1] == parm[2] && vOrigin[2] == parm[3] )
	{
		SKILL_UnholyAura( id );

		// Reset the user's speed in ENTANGLE_TIME amount of time
		set_task( ENTANGLE_TIME, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + id );
		
		// Entangle the user
		SKILL_EntangleEffect( id )
	}

	// If not lets run another check in 0.1 seconds
	else
	{
		parm[1] = vOrigin[0];
		parm[2] = vOrigin[1];
		parm[3] = vOrigin[2];

		set_task( 0.1, "_SKILL_EntangleWait", TASK_ENTANGLEWAIT + id, parm, 4 );
	}
	
	return;
}

public SKILL_EntangleEffect( id )
{

	// Get the user's origin
	new vOrigin[3];
	get_user_origin( id, vOrigin );
	
	// Play the entangle sound
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_ENTANGLING], 1.0, ATTN_NORM, 0, PITCH_NORM );

	new iStart[3], iEnd[3], iHeight;
	new iRadius	= 20, iCounter = 0;
	new x1, y1, x2, y2;

	// Some sweet crap that I don't understand courtesy of SpaceDude - draws the "cylinder" around the player
	while ( iCounter <= 7 )
	{
		if ( iCounter == 0 || iCounter == 8 )
			x1 = -iRadius;
		else if ( iCounter == 1 || iCounter == 7 )
			x1 = -iRadius * 100/141;
		else if ( iCounter == 2 || iCounter == 6 )
			x1 = 0;
		else if ( iCounter == 3 || iCounter == 5 )
			x1 = iRadius*100/141
		else if ( iCounter == 4 )
			x1 = iRadius

		if ( iCounter <= 4 )
			y1 = sqroot( iRadius*iRadius-x1*x1 );
		else
			y1 = -sqroot( iRadius*iRadius-x1*x1 );

		++iCounter;

		if ( iCounter == 0 || iCounter == 8 )
			x2 = -iRadius;
		else if ( iCounter == 1 || iCounter==7 )
			x2 = -iRadius*100/141;
		else if ( iCounter == 2 || iCounter==6 )
			x2 = 0;
		else if ( iCounter == 3 || iCounter==5 )
			x2 = iRadius*100/141;
		else if ( iCounter == 4 )
			x2 = iRadius;
		
		if ( iCounter <= 4 )
			y2 = sqroot( iRadius*iRadius-x2*x2 );
		else
			y2 = -sqroot( iRadius*iRadius-x2*x2 );

		iHeight = 16 + 2 * iCounter;

		while ( iHeight > -40 )
		{
			
			iStart[0]	= vOrigin[0] + x1;
			iStart[1]	= vOrigin[1] + y1;
			iStart[2]	= vOrigin[2] + iHeight;
			iEnd[0]		= vOrigin[0] + x2;
			iEnd[1]		= vOrigin[1] + y2;
			iEnd[2]		= vOrigin[2] + iHeight + 2;
			
			Create_TE_BEAMPOINTS( iStart, iEnd, g_iSprites[SPRITE_BEAM], 0, 0, ( floatround( ENTANGLE_TIME ) * 10 ), 10, 5, 10, 108, 23, 255, 0 );

			iHeight -= 16;
		}
	}

	return;
}
// ---------------------------------------- End Entangle Roots -------------------------------------

// ---------------------------------------- Start Flame Strike -------------------------------------
public SKILL_FlameStrike( id )
{
	if ( !ULTIMATE_CheckExecute( id, SKILL_FLAMESTRIKE ) )
	{
		return;
	}
	
	// He is on "searching"
	p_data_b[id][PB_ISSEARCHING] = true;
	
	// With what ultimate is he searching?
	p_data[id][P_ULT_SEARCH] = SKILL_FLAMESTRIKE;
	
	// Start the Searching task 
	new parm[2];
	parm[0] = id;
	parm[1] = 5; // 5 seconds after that the ultimate is disabled till the player press again the ultimate button
	
	_SHARED_PING( parm ); 
	
	return;
}
	
public SKILL_FlameStrikeInit( iCaster, iTarget )
{
	// Fire resistance check
	if ( p_data_resist[iTarget][RESIST_FIRE] > 0 )
	{
		if ( random_float( 0.0, 1.0 ) <= ( p_data_resist[iTarget][RESIST_FIRE] / MAX_RESIST_POINTS ) )
		{
			client_print( iTarget, print_chat, "%s Your fire resistance protects you from a flame strike attack", MOD_NAME );
			client_print( iCaster, print_chat, "%s Your flame strike attack is deflected by your target's fire resistance", MOD_NAME );
			
			return PLUGIN_HANDLED;
		}
	}
	
	emit_sound( iCaster, CHAN_STATIC, g_szSounds[SOUND_IMMOLATE], 0.5, ATTN_NORM, 0, PITCH_NORM );

	// Emit sound and create the fire exploding effect on the player

	new vTargetOrigin[3];
	get_user_origin( iTarget, vTargetOrigin );

	Create_TE_EXPLOSION( vTargetOrigin, vTargetOrigin, g_iSprites[SPRITE_IMMOLATE], 20, 24, 4 );
	Create_TE_EXPLOSION( vTargetOrigin, vTargetOrigin, g_iSprites[SPRITE_BURNING], 30, 24, 4 );

	Create_ScreenShake( iTarget, (10<<12), (2<<12), (5<<12) );

	// Do initial immolate damage and make their screen shake a little

	UWC3NG_DoDamage( iTarget, iCaster, IMMOLATE_DAMAGE, CSW_IMMOLATE, -1 )

	new parm_DoT[3];
	parm_DoT[0] = iCaster;
	parm_DoT[1] = iTarget;
	parm_DoT[2] = 1;

	// Start the ultimate DoT

	new TaskId = TASK_BURNING + iTarget;
	set_task( 1.0, "SKILL_FlameStrike_DoT", TaskId, parm_DoT, 3 );
	
	return PLUGIN_HANDLED;
}

public SKILL_FlameStrike_DoT( parm_DoT[3] )
{
	new iCaster = parm_DoT[0];
	new iTarget = parm_DoT[1];
	new iCounter = parm_DoT[2];
	
	// Check if all players are on the server or alive
	if ( !is_user_connected( iCaster ) || !is_user_alive( iTarget ) )
	{
		p_data_b[iTarget][PB_ISBURNING] = true;
		return PLUGIN_HANDLED;
	}
	
	// Stop DoT if the max number of ticks is reached

	if ( iCounter > IMMOLATE_DOT ) 
	{
		SKILL_FlameStrike_Remove( iTarget );
		return PLUGIN_HANDLED;
	}

	// Emit sound and show the burning effect on the player

	new vTargetOrigin[3];
	get_user_origin( iTarget, vTargetOrigin );

	emit_sound( iTarget, CHAN_STATIC, g_szSounds[SOUND_IMMOLATE_BURNING], 0.5, ATTN_NORM, 0, PITCH_NORM );

	Create_TE_SPRITE( vTargetOrigin, g_iSprites[SPRITE_FIRE], 3, 200 );

	// Do the DoT damage

	UWC3NG_DoDamage( iTarget, iCaster, IMMOLATE_DOT_DAMAGE, CSW_IMMOLATE, -1 )

	// If the target is still alive after this, make their screen glow orange and start the task again

	if ( is_user_alive( iTarget ) )
	{
		p_data_b[iTarget][PB_ISBURNING] = true;

		Create_ScreenFade( iTarget, (1<<10), (1<<10), (1<<12), 255, 108, 0, 160 );

		parm_DoT[2]++;

		set_task( 1.0, "SKILL_FlameStrike_DoT", TASK_BURNING + iTarget, parm_DoT, 3 );

	}

	return PLUGIN_HANDLED;
}


SKILL_FlameStrike_Remove( iTarget ) 
{
	// Clear global variables and stop the DoT task

	p_data_b[iTarget][PB_ISBURNING] = false;

	new TaskId = TASK_BURNING + iTarget;
	remove_task( TaskId, 0 );

	return PLUGIN_HANDLED;
}
// ----------------------------------- End Flame Strike --------------------------------------------

// ----------------------------------- Start Suicide Bomber ----------------------------------------
public SKILL_SuicideBomber( id )
{
	if ( !ULTIMATE_CheckExecute( id, SKILL_BOMBER ) )
	{
		return;
	}
	
	// Give him a warning
	if ( !p_data_b[id][PB_SUICIDEATTEMPT] )
	{
		UWC3NG_StatusText( id, TXT_BLINK_CENTER, "Suicide Bomb Armed^nPress Again To Detonate" );
		
		p_data_b[id][PB_SUICIDEATTEMPT] = true;
	}
	
	// The player had their warning, kill him!
	else
	{
		// Kill the player
		UWC3NG_KillUser( id, 0, 0 );
		
		p_data_b[id][PB_SUICIDEATTEMPT] = false;
	}
	
	return;
}

public SKILL_Suicide( id )
{
	// Ultimate has been used, so we can't use it again!
	if ( p_data_b[id][PB_USED_SUICIDE] )
	{
		return;
	}

	// If we ult while changing teams, you will kill your old teammates, we don't want that!!
	if ( !SHARED_IsOnTeam( id ) )
	{
		return;
	}
	
	// Freezetime is not over
	if ( !g_FreezeTimeOver )
	{
		return;
	}
	
	// Ultimate used 
	p_data_b[id][PB_USED_SUICIDE] = true;

	// Play the undead explosion sound!!!
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_SUICIDE], 1.0, ATTN_NORM, 0, PITCH_NORM );

	new parm[5], vOrigin[3];
	get_user_origin( id, vOrigin );

	parm[0] = id;
	parm[1] = 6;
	parm[2] = vOrigin[0];
	parm[3] = vOrigin[1];
	parm[4] = vOrigin[2];

	// Set up the tasks to damage + draw effects
	set_task( 0.5, "_SKILL_SuicideExplode", TASK_EXPLOSION + id, parm, 5 );
	set_task( 0.5, "_SKILL_SuicideBlastCircles", TASK_BEAMCYLINDER + id, parm, 5 );
	
	// Create an implosion effect where they died
	Create_TE_IMPLOSION( vOrigin, 100, 20, 5 );
	
	// Set up an ultimate delay (in case the user respawns)
	ULTIMATE_ResetCooldown( id, get_pcvar_num( CVAR_uwc3ng_ult_cooldown ) );
}


// This is only use in _SKILL_SuicideExplode so declaring it here should be fine
new bool:bIgnoreDmg[MAX_PLAYERS] = { false, ... };

// Draw the explosions
public _SKILL_SuicideExplode( parm[5] )
{
	new id = parm[0];

	if ( get_pcvar_num( CVAR_uwc3ng_psychostats ) )
	{
		new WEAPON = CSW_SUICIDE - CSW_WAR3_MIN;

		iStatsShots[id][WEAPON]++;
	}

	new vOrigin[3], vPosition[3];
	vOrigin[0] = parm[2];
	vOrigin[1] = parm[3];
	vOrigin[2] = parm[4];

	vPosition[0] = vOrigin[0] + random_num( -100, 100 );
	vPosition[1] = vOrigin[1] + random_num( -100, 100 );
	vPosition[2] = vOrigin[2] + random_num( -50, 50 );
	
	Create_TE_EXPLOSION( vOrigin, vPosition, g_iSprites[SPRITE_FIREBALL], (random_num(0,20) + 20), 12, TE_EXPLFLAG_NONE );
	
	Create_TE_Smoke( vOrigin, vPosition, g_iSprites[SPRITE_SMOKE], 60, 10 );
	
	new players[32], numberofplayers;
	get_players( players, numberofplayers, "a" );

	new i, iTargetID, vTargetOrigin[3], iDamage, iDistance;
	new iMultiplier = ( EXPLOSION_MAX_DAMAGE * EXPLOSION_MAX_DAMAGE ) / EXPLOSION_RANGE;
	new iTeam = get_user_team( id );
	
	// Check all players and see if they should be damaged
	for ( i = 0; i < numberofplayers; i++ )
	{
		iTargetID = players[i];
		
		// Get origin of target
		get_user_origin( iTargetID, vTargetOrigin );

		// Get distance in b/t target and caster
		iDistance = get_distance( vOrigin, vTargetOrigin );
		
		// Make sure this user is close enough to do damage + isn't immune + isn't on the same team + isn't already immune to all damage
		if ( iDistance < EXPLOSION_RANGE && iTeam != get_user_team( iTargetID ) && !bIgnoreDmg[iTargetID] )
		{
			if ( ULTIMATE_IsImmun( iTargetID ) || p_data_b[iTargetID][PLAYER_SPAWNGOD] || p_data_b[iTargetID][PB_GOD] )
			{
				ULTIMATE_Blocked( id );
				
				bIgnoreDmg[iTargetID] = true;
			}
			
			// Magic resistance check
			else if ( p_data_resist[iTargetID][RESIST_MAGIC] > 0 )
			{
				if ( random_float( 0.0, 1.0 ) <= ( p_data_resist[iTargetID][RESIST_MAGIC] / MAX_RESIST_POINTS ) )
				{
					client_print( iTargetID, print_chat, "%s Your magic resistance protects you from a suicide bomber attack", MOD_NAME );
					client_print( id, print_chat, "%s Your suicide bomber attack is deflected by your target's magic resistance", MOD_NAME );
					
					bIgnoreDmg[iTargetID] = true;
				}
			}
			
			// The user isn't immune!
			else
			{
				// Calculate the damage to be done
				iDamage = ( EXPLOSION_RANGE - iDistance) * iMultiplier;
				iDamage = sqroot( iDamage );
				
				// Damage them!!!!
				UWC3NG_DoDamage( iTargetID, id, iDamage, CSW_SUICIDE, -1 );
				
				// Lets shake up their screen a bit
				Create_ScreenShake( iTargetID, (1<<14), (1<<13), (1<<14) );
			}
		}

		// Reset the "don't damage" rule
		if ( parm[1] - 1 <= 0 )
		{
			bIgnoreDmg[iTargetID] = false;
		}
	}

	--parm[1];
	
	// Lets keep going if we have some left!
	if ( parm[1] > 0 )
	{
		set_task( 0.1, "_SKILL_SuicideExplode", TASK_EXPLOSION + id, parm, 5 );
	}
}

// Draw the blast circles
public _SKILL_SuicideBlastCircles( parm[5] )
{
	new vOrigin[3], vPosition[3];

	vOrigin[0] = parm[2];
	vOrigin[1] = parm[3];
	vOrigin[2] = parm[4] - 16;

	vPosition[0] = vOrigin[0];
	vPosition[1] = vOrigin[1];
	vPosition[2] = vOrigin[2] + EXPLOSION_BLAST_RADIUS;

	Create_TE_BEAMCYLINDER( vOrigin, vOrigin, vPosition, g_iSprites[SPRITE_SHOCKWAVE], 0, 0, 6, 16, 0, 188, 220, 255, 255, 0 );

	vOrigin[2] = ( vOrigin[2] - EXPLOSION_BLAST_RADIUS ) + ( EXPLOSION_BLAST_RADIUS / 2 );

	Create_TE_BEAMCYLINDER( vOrigin, vOrigin, vPosition, g_iSprites[SPRITE_SHOCKWAVE], 0, 0, 6, 16, 0, 188, 220, 255, 255, 0 );
}
// -------------------------------- End Suicide Bomber ---------------------------------------------

// Evasion
public SKILL_Evasion( id, iHitZone )
{
	static iSkillLevel, Float:iEvasionChance;

	// Check to see if they should evade this shot?
	iSkillLevel = p_data_skill[id][SKILL_EVASION];

	if ( iSkillLevel > 0 ) 
	{
		// Add the evasion chance
		iEvasionChance = p_evasion[iSkillLevel-1];
		
		// The player has skilled dexterity, add the bonus
		if ( p_data_attrib[id][ATTRIB_DEXTERITY] > MAX_ATTRIBS_STARTPOINTS )
		{
			iEvasionChance += DEX_EVADE_BONUS * ( p_data_attrib[id][ATTRIB_DEXTERITY] - MAX_ATTRIBS_STARTPOINTS );
		}
		
		if ( random_float( 0.0, 1.0 ) <= iEvasionChance )
		{
			new iGlowIntensity = random_num( 20, 50 );
		
			// Head shot
			if ( iHitZone == HIT_HEAD )
			{
				iGlowIntensity += 250;
			}

			// Chest
			else if ( iHitZone == HIT_CHEST )
			{
				iGlowIntensity += 75;
			}
		
			// Make the user glow!
			SHARED_Glow( id, 0, 0, iGlowIntensity, 0 );

			Create_ScreenFade( id, (1<<10), (1<<10), (1<<12), 0, 0, 255, g_GlowLevel[id][2] );

			return 1;
		}
	}

	return 0;
}

// Trueshot Aura
public SKILL_TrueShotAura( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )
{
	static iSkillLevel;
	static Float:iTrueshotBonus;

	iSkillLevel = p_data_skill[iAttacker][SKILL_TRUESHOTAURA];
	if ( iSkillLevel > 0 )
	{
		// Add the bonus from the const
		iTrueshotBonus = p_trueshot[iSkillLevel-1];
		
		// The player has intellect skilled, add the bonus
		if ( p_data_attrib[iAttacker][ATTRIB_INTELLECT] >= INTELLECT_TRUESHOT )
		{
			iTrueshotBonus += INTELLECT_TRUESHOT_BONUS;
		}
		
		static iTempDamage;
		iTempDamage = floatround( float( iDamage ) * iTrueshotBonus );
		
		// Damage the user
		UWC3NG_DoDamage( iVictim, iAttacker, iTempDamage, iWeapon, iHitPlace );

		// Make the user glow!
		SHARED_Glow( iVictim, ( 2 * iTempDamage ), 0, 0, 0 );

		// Create a screen fade
		Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, iTempDamage );
	}
	
	return;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
