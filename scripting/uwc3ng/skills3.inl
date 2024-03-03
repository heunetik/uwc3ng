// Uwc3mg skills3 functions (16-24)

// Thorns Aura
public SKILL_ThornsAura( iAttacker, iVictim, iDamage, iHitPlace )
{
	static iSkillLevel;

	// Thorns Aura ( attacker could be dead... i.e. nade )
	iSkillLevel = p_data_skill[iVictim][SKILL_THORNSAURA];
	if ( iSkillLevel && is_user_alive( iAttacker ) )
	{
		static iAdditionalDamage;
		iAdditionalDamage = floatround( float( iDamage ) * p_thorns[iSkillLevel-1] );
		
		// Damage the user
		UWC3NG_DoDamage( iAttacker, iVictim, iAdditionalDamage, CSW_THORNS, iHitPlace );

		// Make the user glow!
		SHARED_Glow( iAttacker, ( 3 * iAdditionalDamage ), 0, 0, 0 );
		
		// Create a screen fade
		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 0, 0, 255, iAdditionalDamage )
	}
}

// ------------------------------------ Start Teleport ---------------------------------------------
SKILL_Teleport( id )
{
	
	if ( !ULTIMATE_CheckExecute( id, SKILL_TELEPORT ) )
	{
		return;
	}
	
	// User can't Blink when he/she's stunned
	if ( p_data_b[id][PB_STUNNED] )
	{
		UWC3NG_StatusText( id, 0, "You can't teleport when you're stunned!" );

		return;
	}
	
	new vOldLocation[3], vNewLocation[3]
	
	// Get the player's current location
	get_user_origin( id, vOldLocation );
	
	// Get where the player is looking (where the player will teleport)
	get_user_origin( id, vNewLocation, 3 );
	
	// Play the blink sound!
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_BLINK], 1.0, ATTN_NORM, 0, PITCH_NORM );
	
	// Save the ultimate delay in a var
	new iUltimateDelay =  get_pcvar_num( CVAR_uwc3ng_ult_cooldown );
	
	// The player has intellect skilled, add the bonus
	if ( p_data_attrib[id][ATTRIB_INTELLECT] == INTELLECT_TELEPORT )
	{
		iUltimateDelay -= INTELLECT_TELEPORT_BONUS;
	}

	new iPlayerOldCheck = UWC3NG_IsImmunePlayerNear( id, vOldLocation );
	new iPlayerNewCheck = UWC3NG_IsImmunePlayerNear( id, vNewLocation );
	new iLosesCharge = 0;
	if ( iPlayerOldCheck > 0 )
	{
		iLosesCharge = iPlayerOldCheck;
	}
	else if ( iPlayerNewCheck > 0 )
	{
		iLosesCharge = iPlayerNewCheck;
	}

	// Make sure a nearby enemy doesn't have immunity
	if ( iLosesCharge > 0 )
	{
		UWC3NG_StatusText( id, 0, "Teleport Failed^nEnemy has Immunity" );
		
		// Reset the user's ultimate
		ULTIMATE_ResetCooldown( id, iUltimateDelay );

		// Display ultimate blocked message
		ULTIMATE_Blocked( id );

		return;
	}

	// Check to see if the user is teleporting where they shouldn't
	if ( SHARED_NearObjective( vNewLocation ) == OBJENT_VIP_ESCAPE )
	{
		UWC3NG_StatusText( id, 0, "Teleport failed, you cannot teleport so close to the escape zone!" );
		
		// Reset the user's ultimate
		ULTIMATE_ResetCooldown( id, iUltimateDelay );

		return;
	}

	// If we teleport them back, make sure they don't get teleported into the ground
	vOldLocation[ZPOS] += 15;

	// Change coordinates to make sure player won't get stuck in the ground/wall
	vNewLocation[XPOS] += ( ( vNewLocation[XPOS] - vOldLocation[XPOS] > 0 ) ? -50 : 50 );
	vNewLocation[YPOS] += ( ( vNewLocation[YPOS] - vOldLocation[YPOS] > 0 ) ? -50 : 50 );
	vNewLocation[ZPOS] += 40;			

	
	// Set up some origins for some special effects!!!
	new vCenterOrigin[3], vAxisOrigin[3];
	vCenterOrigin[0]	= vOldLocation[0];
	vCenterOrigin[1]	= vOldLocation[1];
	vCenterOrigin[2]	= vOldLocation[2] + 10;
	vAxisOrigin[0]		= vOldLocation[0];
	vAxisOrigin[1]		= vOldLocation[1];
	vAxisOrigin[2]		= vOldLocation[2] + 10 + TELEPORT_RADIUS;

	// Lets create some beam cylinders!
	Create_TE_BEAMCYLINDER( vOldLocation, vCenterOrigin, vAxisOrigin, g_iSprites[SPRITE_SHOCKWAVE], 0, 0, 3, 60, 0, 255, 255, 255, 255, 0 );
	
	// Modify our effects a bit for another cylinder
	vCenterOrigin[2]	+= 80;
	vAxisOrigin[2]		+= 80;
	
	// And draw another cylinder!!!
	Create_TE_BEAMCYLINDER( vOldLocation, vCenterOrigin, vAxisOrigin, g_iSprites[SPRITE_SHOCKWAVE], 0, 0, 3, 60, 0, 255, 255, 255, 255, 0 );

	// Planting the bomb then teleporting = bad, lets stop this...
	client_cmd( id, "-use" );

	// Then lets try to slay some bitches...
	if ( get_pcvar_num( CVAR_uwc3ng_blink_protection ) )
	{

		// Lets go ahead and make this check before they teleport...
		if ( SKILL_BlinkProtection( id, vNewLocation ) )
		{
			ULTIMATE_ResetCooldown( id, iUltimateDelay );

			return;
		}
	}

	// Teleport the player!!!
	set_user_origin( id, vNewLocation );

	// Check if Blink landed you in a wall, if so, abort
	new parm[5];
	parm[0] = id;
	parm[1] = vOldLocation[0];
	parm[2] = vOldLocation[1];
	parm[3] = vOldLocation[2];
	parm[4] = vNewLocation[2];
	
	// [FS#65]
	if ( !g_EndRound )
	{
		set_task( 0.1, "_SKILL_BlinkStuck", TASK_BLINKSTUCK + id, parm, 5 );
	}
	
	ULTIMATE_ResetCooldown( id, iUltimateDelay );

	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_BLINK], 1.0, ATTN_NORM, 0, PITCH_NORM );

	return;
}

// Function will check to see if a user is stuck in a wall
public _SKILL_BlinkStuck( parm[] )
{

	new id = parm[0]	 

	if ( !is_user_connected( id ) )
	{
		return;
	}

	new vOldLocation[3], vOrigin[3];

	vOldLocation[0] = parm[1];
	vOldLocation[1] = parm[2];
	vOldLocation[2] = parm[3];

	get_user_origin( id, vOrigin );
	
	// Then the user is stuck :/
	if ( parm[4] == vOrigin[2] )
	{

		UWC3NG_StatusText( id, 0, "Teleport Failed^nBad Destination" );
		
		// This will try to move the user back - if this fails then they will be teleported back to their spawn instead of left stuck!
		SHARED_Teleport( id, vOldLocation );

		ULTIMATE_ResetCooldown( id, floatround( BLINK_COOLDOWN ) );
	}

	// Otherwise they teleported correctly!
	else
	{

		// Sprays white bubbles everywhere
		new vStartOrigin[3];
		vStartOrigin[0] = vOrigin[0];
		vStartOrigin[1] = vOrigin[1];
		vStartOrigin[2] = vOrigin[2] + 40;
		
		Create_TE_SPRITETRAIL( vStartOrigin, vOrigin, g_iSprites[SPRITE_FLARE], 30, 10, 1, 50, 10 );

		// Flash the player
		if ( get_pcvar_num( CVAR_uwc3ng_blink_dizziness ) == 1 )
		{
			Create_ScreenFade( id, (1<<15), (1<<10), (1<<12), 0, 0, 255, 180 );
		}

		// Lets just flash them with a nice white screen :)
		else
		{
			Create_ScreenFade( id, (1<<15), (1<<10), (1<<12), 255, 255, 255, 255 );
		}
	}	
	
	return;
}


// Function will make sure the user isn't in an invalid location in a map
SKILL_BlinkProtection( id, vOrigin[3] )
{

	new bool:bSlay = false;
	new Float:fOrigin[3];
	
	// Convert origin to float
	IVecFVec( vOrigin, fOrigin );
	
	// User shouldn't be teleporting into the sky!
	if ( point_contents( fOrigin ) == CONTENTS_SKY )
	{
		bSlay = true;
	}

	// Get the current map name
	new szMapName[32];
	get_mapname( szMapName, 32 );

	
	if ( !bSlay )
	{
		new x = vOrigin[0];
		new y = vOrigin[1];
		new z = vOrigin[2];
			
		// Don't teleport too high
		if ( equali( szMapName, "de_dust" ) )
		{
			if ( z > 220 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "awp_assault" ) )
		{
			if( z > 520 && y > 2400 && y < 2600 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "de_dust_cz" ) )
		{
			if ( z > 220 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "de_aztec_cz" ) )
		{
			if ( z > 300 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "cs_assault_upc" ) )
		{
			if( z > 650 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "de_aztec" ) )
		{
			if( z > 300 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "de_cbble" ) || equali( szMapName, "de_cbble_cz" ) )
		{
			if ( z > 315 )
			{
				if ( ! ( ( x > -1320 && x < -1150 ) && ( y > 2600 && y < 2900 ) ) )
				{
					bSlay = true;
				}
			}           
		}

		else if ( equali( szMapName, "cs_assault" ) )
		{
			if ( z > 700 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "cs_militia" ) || equali( szMapName, "cs_militia_cz" ) )
		{
			if ( z > 500 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "cs_italy" ) )
		{
			if ( z > -220 && y < -2128 )
			{
					bSlay = true;
			}
			else if( z > 250 )
			{
				if ( ( x < -1000 && x > -1648 ) && ( y > 1900 && y < 2050 ) )
				{
					bSlay = true;
				}
				else if ( ( x < -1552 && x > -1648 ) && ( y > 1520 && y < 2050 ) )
				{
					bSlay = true;
				}
			}
		}

		else if ( equali( szMapName, "cs_italy_cz" ) )
		{
			if ( y > 2608 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "de_dust2" ) )
		{
			if ( z > 270 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "de_dust2_cz" ) )
		{
			if ( z > 270 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "fy_dustworld" ) )
		{
			if ( z > 82 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "fy_pool_day" ) )
		{
			if ( z > 190 )
			{
				bSlay = true;
			}
		}

		else if ( equali( szMapName, "as_oilrig" ) )
		{
			if ( x > 1700 )
			{
				bSlay = true;
			}
		}
	}

	
	// For some reason bSlay is always true on this map, but you should be able to teleport anywhere
	if ( equali( szMapName, "cs_mice_final" ) )
	{
		bSlay = false;
	}


	// Slay the user!!!
	if ( bSlay )
	{
		client_print( id, print_chat, "%s You have been slain for teleporting into a restricted area", MOD_NAME );
		user_kill( id );
	}

	return bSlay;
}
// ------------------------------------ End Teleport ---------------------------------------------

// Critical Strike
public SKILL_CriticalStrike( iAttacker, iVictim, iDamage, iHitPlace, iWeapon )
{
	new iSkillLevel = p_data_skill[iAttacker][SKILL_SHADOWSTRIKE];
	
	// Has this player critical strike?
	if ( iSkillLevel )
	{
		// Will this hit a critical hit?
		if ( random_float( 0.0, 1.0 ) <= p_critical[iSkillLevel-1] )
		{
			// Calculate the bonus dmg
			new iBonusDamage = floatround( float( iDamage ) * p_critical[iSkillLevel-1] );
			
			// Add the default damage to the bonus damage
			iBonusDamage += iDamage;
			
			// Damage our victim
			UWC3NG_DoDamage( iVictim, iAttacker, iBonusDamage, iWeapon, iHitPlace );
			
			// Make the user glow
			SHARED_Glow( iVictim, iBonusDamage, 0, 0, 0 );
			
			// Lets make a screenfade
			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, g_GlowLevel[iVictim][0] );
		}
	}
	
	return;
}

// Dispell Hex
public SKILL_DispellHex( id )
{
	if ( !ULTIMATE_CheckExecute( id, SKILL_DISPELLHEX ) )
	{
		return;
	}
	
	// Look the player at another player?
	static FriendID, iBody;
	get_user_aiming( id, FriendID, iBody );
	
	// Bad player. Go away ;o
	if ( !SHARED_ValidPlayer( FriendID ) || !is_user_alive( FriendID ) || get_user_team( FriendID ) != get_user_team( id ) )
	{
		return;
	}
	
	// Is the player hexed?
	if ( !p_data_b[FriendID][PB_HEXED] )
	{
		UWC3NG_StatusText( id, TXT_SKILL, "Your teammate isnt hexed at the moment" );
		
		return;
	}
	
	// Amount of repairs are calculated with the level from the player
	if ( p_data[id][P_DISPELL] >= ( p_data[id][PLAYER_LVL] / DISPELL_DIVISOR ) )
	{
		UWC3NG_StatusText( id, TXT_SKILL, "You have already performed the maximum number of dispells for this round" );
		
		return;
	}
	
	// Alright. Fist, we add one dispellpoint
	p_data[id][P_DISPELL]++;
	
	// We remove the mark from the player
	p_data_b[FriendID][PB_HEXED] = false;
	
	// Set the default model back
	SHARED_ChangeSkin( FriendID, 0 ); // 0 = reset skin
	
	// Remove the jump task if exists
	if ( task_exists( TASK_JUMP + FriendID ) )
	{
		remove_task( TASK_JUMP + FriendID );
	}
	
	// Set him the skills back
	SKILL_Update( FriendID );
	
	// Calculate the xps which the player should get
	new iXPs = XP_Calc( id, DISPELL_HEX );
		
	// Give him the xps
	new iBonusXP = XP_Give( id, iXPs );

	// All is okay, write him a message
	if ( iBonusXP != 0 )
	{
		client_print( id, print_chat, "%s You have been awarded %d XP for dispelling to your teammate's hex", MOD_NAME, iBonusXP );
	}
	
	// Write a message to the friend
	static SkillOwnerName[32];
	get_user_name( id, SkillOwnerName, charsmax( SkillOwnerName ) );
	
	client_print( FriendID, print_chat, "%s %s has dispelled your hex", MOD_NAME, SkillOwnerName );
	
	// Play the dispell sound
	emit_sound( FriendID, CHAN_STATIC, g_szSounds[SOUND_DISPELL], 1.0, ATTN_NORM, 0, PITCH_NORM );
	
	return;
}

// Carrion Beetles
public SKILL_CarrionBeetles( iAttacker, iVictim, iHitPlace )
{
	new iSkillLevel = p_data_skill[iAttacker][SKILL_CARRIONBEETLES];
	new Float:iDiseaseLevel = float( p_data_resist[iVictim][RESIST_DISEASE] );
	new Float:iCarrionChance;
	
	// Has this player Carrion beetles and isn't hexed?
	if ( iSkillLevel )
	{
		// Add the carrion chance
		iCarrionChance = p_carrion[iSkillLevel-1];
		
		// The player has skilled disease resistance, add the bonus
		if ( iDiseaseLevel )
		{
			iCarrionChance -= ( iDiseaseLevel / float( MAX_RESIST_POINTS ) ) * iCarrionChance;
		}
		
		// Will this hit a Carrion Beetles hit?
		if ( random_float( 0.0, 1.0 ) <= iCarrionChance )
		{
			new vVictimOrigin[3], vAttackerorigin[3];
			get_user_origin( iVictim, vVictimOrigin );
			get_user_origin( iAttacker, vAttackerorigin );
			
			// Create the Carrion Beetle effect
			Create_TE_SPRITETRAIL( vAttackerorigin, vVictimOrigin, g_iSprites[SPRITE_BEETLE], 15, 15, 1, 2, 6 );
			
			// Play the carrion beetle sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_CARRION], 1.0, ATTN_NORM, 0, PITCH_NORM );
			
			// Set up a var for the dmg
			new iDamage = CARRIONBEETLE_DAMAGE;
			
			// The player isn't diseased
			if ( !p_data_b[iVictim][PB_CARRIONDOT] )
			{
				iDamage += CARRIONBEETLE_DOT;
				
				// the player is now diseased, show all that sprite
				Create_CarrionShadowDOT( iVictim, g_iSprites[SPRITE_CARRIONBEETLES] );
			
				// The player is slowed
				p_data_b[iVictim][PB_NORMALSPEED] = true;
				set_task( 0.5, "SKILL_UnholyAura", ( iVictim + TASK_SPEED ) );
			
				// We should save the victim is diseased
				p_data_b[iVictim][PB_CARRIONDOT] = true;
			
				// print a message
				static AttackerName[32], VictimName[32];
				get_user_name( iAttacker, AttackerName, charsmax( AttackerName ) );
				get_user_name( iVictim, VictimName, charsmax( VictimName ) );
				client_print( iAttacker, print_chat, "%s Your Carrion Beetles infect %s with disease", MOD_NAME, VictimName );
				client_print( iVictim, print_chat, "%s Carrion Beetles from %s infect you with disease, seek a medic quickly!", MOD_NAME, AttackerName );
			
				//Set up the dot
				new parm[2];
				parm[0] = iVictim;
				parm[1] = iAttacker;
				set_task( DOT_TASKTIME, "SKILL_CarrionDot", ( TASK_CARRIONDOT + iVictim ), parm, 2 );
			}
			
			// Do the damage
			UWC3NG_DoDamage( iVictim, iAttacker, iDamage, CSW_CARRION, iHitPlace );
		}

		else if ( get_pcvar_num( CVAR_uwc3ng_psychostats ) )
		{
			new WEAPON = CSW_CARRION - CSW_WAR3_MIN;

			iStatsShots[iAttacker][WEAPON]++;
		}
	}
	
	return;
}

// Carrion Beetles desease damage
public SKILL_CarrionDot( parm[2] )
{
	new iVictim = parm[0];
	new iAttacker = parm[1];
	
	// is the player alive?
	if ( is_user_alive( iVictim ) && p_data_b[iVictim][PB_CARRIONDOT] && is_user_connected( iAttacker ) )
	{
		// Do the dot damage
		UWC3NG_DoDamage( iVictim, iAttacker, CARRIONBEETLE_DOT, CSW_CARRION, -1 );
		
		// And set up the new task
		set_task( DOT_TASKTIME, "SKILL_CarrionDot", ( TASK_CARRIONDOT + iVictim ), parm, 2 );
	}
	
	// the user died
	else 
	{
		// he should better no more slowed
		p_data_b[iVictim][PB_NORMALSPEED] = false;
		
		// Set the new speed
		set_task( 0.1, "SKILL_UnholyAura", ( iVictim + TASK_SPEED ) );
	}
	
	return;
}

// Steel Skin
public SKILL_SteelSkin( iAttacker, iVictim, iDamage )
{
	new iSkillLevel = p_data_skill[iAttacker][SKILL_STEELSKIN];
	
	// Has this player steel skin?
	if ( iSkillLevel )
	{
		// Will this hit a steel skin hit?
		if ( random_float( 0.0, 1.0 ) <= p_steel[iSkillLevel-1] )
		{
			static iArmor, CsArmorType:ArmorType;
			
			// Get the current armor
			iArmor = cs_get_user_armor( iAttacker, ArmorType );
			
			// Add the bonus armor
			iArmor += iDamage;
			
			// We're not give him more than he can have
			if ( iArmor > 100 )
			{
				iArmor = 100;
			}
			
			// Set the new armor
			cs_set_user_armor( iAttacker, iArmor, ArmorType );
			
			// Make the user glow
			SHARED_Glow( iAttacker, iArmor, 0, 0, 0 );
			
			// Lets make a screenfade
			Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 255, 0, 0, g_GlowLevel[iVictim][0] );
		}
	}
	
	return;
}

// Shadow Strike
public SKILL_ShadowStrike( iAttacker, iVictim, iHitPlace )
{
	new iSkillLevel = p_data_skill[iAttacker][SKILL_SHADOWSTRIKE];
	new Float:iResistLevel = float( p_data_resist[iVictim][RESIST_POISON] );
	new Float:iShadowChance;
	
	// Has this player ShadowStrike?
	if ( iSkillLevel )
	{
		// Add the shadow chance
		iShadowChance = p_shadow[iSkillLevel-1];
		
		// The player has skilled poison resistance, add the bonus
		if ( iResistLevel )
		{
			iShadowChance -= ( iResistLevel / float( MAX_RESIST_POINTS ) ) * iShadowChance;
		}
		
		// Will this hit a ShadowStrike hit?
		if ( random_float( 0.0, 1.0 ) <= iShadowChance )
		{
			new vVictimOrigin[3], vAttackerOrigin[3]
			get_user_origin( iVictim, vVictimOrigin );
			get_user_origin( iAttacker, vAttackerOrigin );
			
			// Create the shadow strike effect
			Create_TE_SPRITETRAIL( vAttackerOrigin, vVictimOrigin, g_iSprites[SPRITE_SHADOWSTRIKE], 50, 15, 1, 2, 6 );
			
			// Emit the shadow strike sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_SHADOWSTRIKE], 1.0, ATTN_NORM, 0, PITCH_NORM );

			// Set up the dmg var
			new iDamage = SHADOWSTRIKE_DAMAGE;
			
			// Is the player already poisoned?
			if ( !p_data_b[iVictim][PB_SHADOWDOT] )
			{
				// Add the poison damage
				iDamage += SHADOWSTRIKE_DOT;
				
				// Draw the sprite
				Create_CarrionShadowDOT( iVictim, g_iSprites[SPRITE_SHADOWSTRIKEDOT] );
			
				// We should know that he has poison
				p_data_b[iVictim][PB_SHADOWDOT] = true;
			
				// print a message
				static AttackerName[32], VictimName[32];
				get_user_name( iAttacker, AttackerName, charsmax( AttackerName ) );
				get_user_name( iVictim, VictimName, charsmax( VictimName ) );
				client_print( iAttacker, print_chat, "%s Your poison darts hit their mark on %s", MOD_NAME, VictimName );
				client_print( iVictim, print_chat, "%s Poison darts from %s take their toll on you, seek a medic quickly!", MOD_NAME, AttackerName );
			
				//Set up the dot
				new parm[2];
				parm[0] = iVictim;
				parm[1] = iAttacker;
				set_task( DOT_TASKTIME, "SKILL_ShadowDot", ( TASK_SHADOWDOT + iVictim ), parm, 2 );
			}
				
			// Damage the user
			UWC3NG_DoDamage( iVictim, iAttacker, iDamage, CSW_SHADOW, iHitPlace );
		}

		else if ( get_pcvar_num( CVAR_uwc3ng_psychostats ) )
		{
			new WEAPON = CSW_SHADOW - CSW_WAR3_MIN;

			iStatsShots[iAttacker][WEAPON]++;
		}
	}
	
	return;
}

// Shadow Strike poison damage
public SKILL_ShadowDot( parm[2] )
{
	new iVictim = parm[0];
	new iAttacker = parm[1];
	
	// is the player alive?
	if ( is_user_alive( iVictim ) && is_user_connected( iAttacker ) && p_data_b[iVictim][PB_SHADOWDOT] )
	{
		// Do the dot damage
		UWC3NG_DoDamage( iVictim, iAttacker, SHADOWSTRIKE_DOT, CSW_SHADOW, -1 );
		
		// And set up the new task
		set_task( DOT_TASKTIME, "SKILL_ShadowDot", ( TASK_SHADOWDOT + iVictim ), parm, 2 );
	}
	
	return;
}

// Blink
public SKILL_Blink( id )
{
	new iSkillLevel = p_data_skill[id][SKILL_BLINK];

	// User has the ability, lets initiate a "check"
	if ( iSkillLevel )
	{
		if ( random_float( 0.0, 1.0 ) <= p_blink[iSkillLevel-1] )
		{
			p_data_b[id][PB_BLINK] = true;
			client_print( id, print_chat, "%s You will now be immune to all ultimates", MOD_NAME );
		}
		
		// User shouldn't have blink!
		else
		{
			p_data_b[id][PB_BLINK] = false;
		}
	}

	// User shouldn't have blink!
	else
	{
		p_data_b[id][PB_BLINK] = false;
	}
}
	
	
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
