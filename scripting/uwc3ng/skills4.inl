// Uwc3ng skills4 functions (25-33)

// Critical Grenade & Napalm Grenades
public SKILL_CriticalGrenade( iAttacker, iVictim, iDamage, iWeapon, iHitPlace )
{
	new iSkillLevel = p_data_skill[iAttacker][SKILL_CRITICALGRENADE];
	new iSkillLevelNapalm = p_data_skill[iAttacker][SKILL_NAPALMGRENADE];
	
	// Has this player critical grenade?
	if ( iSkillLevel )
	{
		// Is the weapon a hegrenade?
		if ( iWeapon == CSW_HEGRENADE )
		{
			// The victim has necklace or blink, he is immune!
			if ( ULTIMATE_IsImmun( iVictim ) )
			{
				// Print him a message that the victim is immun
				ULTIMATE_Blocked( iAttacker );
				
				return;
			}
			
			static iBonusDamage;
			iBonusDamage = floatround( float( iDamage ) * p_grenade[iSkillLevel-1] );
			
			// The player has skilled napalm grenade. This will be crititcal ;P
			if ( iSkillLevelNapalm )
			{
				SKILL_FlameStrikeInit( iAttacker, iVictim );
			}
			
			// Magic resistance check
			if ( p_data_resist[iVictim][RESIST_MAGIC] > 0 )
			{
				if ( random_float( 0.0, 1.0 ) <= ( p_data_resist[iVictim][RESIST_MAGIC] / MAX_RESIST_POINTS ) )
				{
					client_print( iVictim, print_chat, "%s Your magic resistance protects you from a critical grenade attack", MOD_NAME );
					client_print( iAttacker, print_chat, "%s Your critical grenade attack is deflected by your target's magic resistance", MOD_NAME );
					
					return;
				}
			}
				
			// Damage the user!
			UWC3NG_DoDamage( iVictim, iAttacker, iBonusDamage, iWeapon, iHitPlace );

			// Make the user glow
			SHARED_Glow( iVictim, iBonusDamage, 0, 0, 0 );

			// Lets make a screenfade
			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, g_GlowLevel[iVictim][0] );
		}
	}
	
	return;
}

// ---------------------------------------------- Start Big Bad Voodoo -----------------------------
public SKILL_Big_Bad_Voodoo( id )
{
	if ( !ULTIMATE_CheckExecute( id, SKILL_BIGBADVOODOO ) )
	{
		return;
	}
	
	// User is on godmode
	p_data_b[id][PB_GOD] = true;
		
	// Need this for invisible and zombie rendering
	p_data_b[id][PB_CANT_RENDER] = true;

	// Reset the cooldown
	ULTIMATE_ResetCooldown( id, get_pcvar_num( CVAR_uwc3ng_ult_cooldown ) + BIGBADVOODOO_DURATION );

	// Create a bartime to show the player how long the big bad voodoo runs
	Create_BarTime( id, BIGBADVOODOO_DURATION, 0 );
	
	// Play the big bad voodoo sound
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_VOODOO], 1.0, ATTN_NORM, 0, PITCH_NORM );

	// Make him invisible
	set_user_rendering( id, kRenderFxGlowShell, 255, 245, 50, kRenderNormal, 16 );

	new vOrigin[3];
	get_user_origin( id, vOrigin );
	vOrigin[2] += 75;

	// Create the light
	Create_TE_ELIGHT( id, vOrigin, 100, 255, 245, 200, BIGBADVOODOO_DURATION, 0 );

	// Remove the big bad voodoo
	set_task( float( BIGBADVOODOO_DURATION ), "_SKILL_Remove", TASK_RESETGOD + id );

	return;
}

public _SKILL_Remove( id )
{
	if ( id >= TASK_RESETGOD )
	{
		id -= TASK_RESETGOD;
	}

	if ( !is_user_connected( id ) )
	{
		return;
	}

	// Now we can render 
	p_data_b[id][PB_CANT_RENDER] = false;

	// no more on godmode
	p_data_b[id][PB_GOD] = false;
	
	// Set back the invisibility
	SKILL_Invisible( id );

	return;
}

// This function is called to reset the ability of an attacker to attack a user running big bad voodoo
public _SKILL_ResetBigBadAttacker( id )
{

	if ( !UWC3NG_Check() )
	{
		return;
	}

	if ( id >= TASK_BIGBADATTACKER )
	{
		id -= TASK_BIGBADATTACKER;
	}

	if ( !is_user_connected( id ) )
	{
		return;
	}

	p_data_b[id][PB_BIGBAD_ATTACKER] = false;

	client_print( id, print_chat, "%s You can no longer damage players with Big Bad Voodoo activated", MOD_NAME );
}
// ------------------------------------- End Big Bad Voodoo ----------------------------------------

// Gate
public SKILL_Gate( id )
{
	if ( !ULTIMATE_CheckExecute( id, SKILL_GATE ) )
	{
		return;
	}
	
	
	// User can't use ultimate while planting or defusing!
	if ( p_data_b[id][PB_ISPLANTING] )
	{
		client_print( id, print_center, "You cannot use your ultimate while planting the bomb!" );
		return;
	}

	// teleport back to spawn
	// Lets find a free spawn and obey immunity rules
	new ent = SHARED_FindFreeSpawn( id, true );
		
	// No spawn found
	if ( ent < 0 )
	{
		// Immune person nearby
		if ( ent == -2 )
		{
			// The user's ultimate was blocked!
			ULTIMATE_Blocked( id );

			// He used in this round gate
			if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 )
			{
				// Set up a new ultimate delay
				ULTIMATE_ResetCooldown( id, get_pcvar_num( CVAR_uwc3ng_ult_cooldown ) );
			}
			
			// CSDM isn't enabled
			else 
			{
				p_data_b[id][USED_GATE] = true;
			}

			UWC3NG_StatusText( id, 0, "Gate Failed^nEnemy has Immunity" );
		}

		// No spawn found
		else
		{
			UWC3NG_StatusText( id, 0, "No free spawn was found to teleport to" );
		}
	}

	// Spawn found!
	else
	{
		new vOrigin[3], vNewOrigin[3], Float:fSpawnOrigin[3];
		
		// Get the user's origin
		get_user_origin( id, vOrigin );

		// Get the origin of the spawn
		entity_get_vector( ent, EV_VEC_origin, fSpawnOrigin );
			
		// Convert float vector to int vector
		FVecIVec( fSpawnOrigin, vNewOrigin );
			
		// Move the user
		set_user_origin( id, vNewOrigin );

		// Play teleport animation at old location
		Create_TE_TELEPORT( vOrigin );

		// Play the Vengeance sound		
		emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_TELEPORT], 1.0, ATTN_NORM, 0, PITCH_NORM );

		// He used in this round gate
		if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 )
		{
			// Set up a new ultimate delay
			ULTIMATE_ResetCooldown( id, get_pcvar_num( CVAR_uwc3ng_ult_cooldown ) );
		}
			
		// CSDM isn't enabled
		else 
		{
			p_data_b[id][USED_GATE] = true;
		}
	}
	
	return;
}

// Invisibility
public SKILL_Invisible( id )
{
	// This should be called on weapon change, on new round, when the user selects a new skill, and after an item is purchased
	// Do not want to set them as invisible if the player is currently rendering
	if ( p_data_b[id][PB_CANT_RENDER] || !is_user_alive( id ) )
	{
		return;
	}

	new iInvisLevel = 0;

	new iSkillLevel = p_data_skill[id][SKILL_INVISIBLE];

	// Has this player invisibility
	if ( iSkillLevel > 0 )
	{
		iInvisLevel = p_invisibility[iSkillLevel-1];
	}

	// User has a Cloak of Invisibility
	if ( ITEM_Has( id, ITEM_CLOAK ) > ITEM_NONE )
	{
		// Then we are already a little invis, lets give them a little bonus for purchasing the item
		if ( iInvisLevel > 0 )
		{
			iInvisLevel = floatround( float( iInvisLevel ) / INVIS_CLOAK_DIVISOR );
		}
		else
		{
			iInvisLevel = get_pcvar_num( CVAR_uwc3ng_cloak );
		}
	}
	
	// If the player is holding a knife they should be more invisible
	if ( SHARED_IsHoldingKnife( id ) )
	{
		iInvisLevel /= 2;
	}

	if ( iInvisLevel )
	{
		set_user_rendering( id, kRenderFxNone, 0, 0, 0, kRenderTransTexture, iInvisLevel );
	}

	// User should not be invisible
	else
	{
		set_user_rendering( id );
	}

	return;
}

// -------------------------------------- Start Phoenix --------------------------------------------
// Check to see if a player will become a Phoenix
public SKILL_PhoenixCheck( id )
{
	new iSkillLevel = p_data_skill[id][SKILL_PHOENIX];
	
	// Then the user could become a phoenix!
	if ( iSkillLevel > 0 )
	{
		// Should the user be a Phoenix
		if ( random_float( 0.0, 1.0 ) <= p_phoenix[iSkillLevel-1] ) 
		{
			p_data_b[id][PB_PHOENIX] = true;
		}
	}

	// Then the user shouldn't be a phoenix!
	else
	{
		p_data_b[id][PB_PHOENIX] = false;
	}
}

// This function is called when a user dies
public SKILL_PhoenixSpawn( id )
{
	
	// If this user is about to respawn, then we don't even want to check
	if ( g_EndRound )
	{
		return;
	}
	
	// The player was respawned this round
	if ( p_data_b[id][PB_RESPAWNED] || p_data_b[id][PB_RESPAWNED_TWICE] )
	{
		return;
	}

	new iTeam = get_user_team( id );
	new iPhoenixID = SKILL_PhoenixExists( iTeam );
	
	// Then we have a Phoenix!!
	if ( iPhoenixID != -1 )
	{

		// Lets make sure they're not going to revive themself
		if ( iPhoenixID != id )
		{
			// The caster is no longer a Phoenix
			p_data_b[iPhoenixID][PB_PHOENIX] = false;
			
			// The player will be respawned 
			p_data_b[id][PB_RESPAWNED] = true;
			
			// Respawn the user
			set_task( SPAWN_DELAY, "_SHARED_Spawn", TASK_SPAWN + id );

			new szCastername[32], szSpawnerName[32];

			get_user_name( iPhoenixID, szCastername,31 )
			get_user_name( id, szSpawnerName, 31 )

			client_print( id, print_chat, "%s %s has revived you", MOD_NAME, szCastername );
			client_print( iPhoenixID, print_chat, "%s You have revived %s, way to go !", MOD_NAME, szSpawnerName );
			
			// Calculate the xps which the player should get
			new iXPs = XP_Calc( id, PHOENIX_XP );
		
			// Give him the xps
			new iBonusXP = XP_Give( id, iXPs );

			// All is okay, write him a message
			if ( iBonusXP != 0 )
			{
				client_print( iPhoenixID, print_chat, "%s You have been awarded %d XP for reviving %s", MOD_NAME, iBonusXP, szSpawnerName );
			}
		}
	}

	return;
}

// Check to see if a Phoenix exists on a given team
SKILL_PhoenixExists( iTeam )
{
	new players[32], numberofplayers, id, i;
	get_players(players, numberofplayers, "a");

	// Loop through all the players to find someone with phoenix
	for ( i = 0; i < numberofplayers; i++ )
	{
		id = players[i];

		if ( get_user_team( id ) == iTeam )
		{
			if ( p_data_b[id][PB_PHOENIX] && !p_data_b[id][PB_HEXED] )
			{
				return id;
			}
		}
	}

	return -1;
}
// ------------------------------------- End Phoenix -----------------------------------------------	

// Siphon Ammo
public SKILL_SiphonArmor( iAttacker, iVictim )
{
	new iSkillLevel = p_data_skill[iAttacker][SKILL_SIPHONARMOR];
	
	// Has this player siphon ammo?
	if ( iSkillLevel )
	{
		// Get the armor from the victim
		static iVictimArmor, CsArmorType:VictimArmorType;
		iVictimArmor = cs_get_user_armor( iVictim, VictimArmorType );
		
		// Get the armor from the attacker
		static iAttackerArmor, CsArmorType:AttackerArmorType;
		iAttackerArmor = cs_get_user_armor( iAttacker, AttackerArmorType );
		
		// Calculate the bonus armor
		new iBonusArmor = floatround( float( iVictimArmor ) * p_armor[iSkillLevel-1] );
		
		// Calculate the armor for the victim
		iVictimArmor -= iBonusArmor;
		
		// Calculate the armor for the Attacker
		iAttackerArmor += iBonusArmor;
		
		// Check for victim
		if ( iVictimArmor < 0  )
		{
			iVictimArmor = 0;
		}
		
		// Check for attacker
		if ( iAttackerArmor > 100 )
		{
			iAttackerArmor = 100;
		}
		
		// Reduce the armor from the victim
		cs_set_user_armor( iVictim, iVictimArmor, VictimArmorType );
		
		// Give the attacker the new armor
		cs_set_user_armor( iAttacker, iAttackerArmor, AttackerArmorType );
		
		// Make the user glow!
		SHARED_Glow( iVictim, 0, iVictimArmor, 0, 0 );
		
		// Create a screen fade (purplish)
		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 144, 58, 255, g_GlowLevel[iAttacker][1] );
	}
	
	return;
}

// ------------------------------------ Start Locust Swarm -----------------------------------------
// Returns victim id
//  -1 if no target
//  -2 if ult blocked
SKILL_LocustGetTarget( id )
{
	new iTeam = get_user_team( id );

	// First thing we need to do is find an alive player that isn't immune to target :)
	new players[32], numberofplayers, i, iTargetID;
	new iTargets[33], iTotalTargets = 0;

	get_players( players, numberofplayers, "a" );

	for ( i = 0; i < numberofplayers; i++ )
	{
		iTargetID = players[i];

		if ( get_user_team( iTargetID ) != iTeam )
		{
			iTargets[iTotalTargets++] = iTargetID;
		}
	}
	
	// No victims found
	if ( iTotalTargets == 0 )
	{
		return -1;
	}

	// Lets find ourselves a victim
	new iVictim = 0, iRandomSpot;
	while ( iVictim == 0 )
	{
		iRandomSpot = random_num( 0, iTotalTargets );

		iVictim = iTargets[iRandomSpot];
	}

	// Then the user's ultimate was blocked :/
	if ( ULTIMATE_IsImmun( iVictim ) )
	{
		// Remove one charge
		ULTIMATE_RemoveCharge( iVictim );
		
		ULTIMATE_Blocked( id );

		// Reset the user's ultimate
		ULTIMATE_ResetCooldown( id, get_pcvar_num( CVAR_uwc3ng_ult_cooldown ) );

		return -2;
	}

	return iVictim;
}

public SKILL_LocustSwarm( id )
{
	if ( !ULTIMATE_CheckExecute( id, SKILL_LOCUST ) )
	{
		return;
	}
	
	// Psychostats
	if ( get_pcvar_num( CVAR_uwc3ng_psychostats ) )
	{
		new WEAPON = CSW_LOCUSTS - CSW_WAR3_MIN;

		iStatsShots[id][WEAPON]++;
	}

	new iVictim = SKILL_LocustGetTarget( id );

	// No target was found :/
	if ( iVictim == -1 )
	{
		UWC3NG_StatusText( id, TXT_BLINK_CENTER, "No valid targets found" );

		return;
	}

	// Ultimate was blocked - do nothing
	if ( iVictim == -2 )
	{
		return;
	}

	// Lets get the origin of the caster
	new vCasterOrigin[3];
	get_user_origin( id, vCasterOrigin );
	
	new parm[10];
	parm[0] = id;		// caster
	parm[1] = iVictim;	// victim
	parm[2] = vCasterOrigin[0];
	parm[3] = vCasterOrigin[1];
	parm[4] = vCasterOrigin[2];

	// Reset the cooldown
	ULTIMATE_ResetCooldown( id, get_pcvar_num( CVAR_uwc3ng_ult_cooldown ) );

	// Start the effects!!
	_SKILL_LocustEffect( parm );

	return;
}

public _SKILL_LocustEffect( parm[] )
{
	new iAttacker	= parm[0];
	new iVictim	= parm[1];

	if ( !is_user_alive( iVictim ) )
	{
		client_print( iAttacker, print_chat, "%s Victim is no longer targetable for Locust Swarm, finding new target!", MOD_NAME );

		// Need to find a new victim here...
		new iVictim = SKILL_LocustGetTarget( iAttacker );

		// No target was found :/
		if ( iVictim == -1 )
		{
			UWC3NG_StatusText( iAttacker, TXT_BLINK_CENTER, "No valid targets found" );
			
			// Set ultimate timer back to 0
			g_PlayerUltimateDelay[iAttacker] = 0;

			return;
		}

		// Ultimate was blocked - do nothing
		else if ( iVictim == -2 )
		{
			return;
		}


		// Lets get the new origin of the caster
		new vCasterOrigin[3];
		get_user_origin( iAttacker, vCasterOrigin );
		parm[1] = iVictim;	// victim
		parm[2] = vCasterOrigin[0];
		parm[3] = vCasterOrigin[1];
		parm[4] = vCasterOrigin[2];

		client_print( iAttacker, print_chat, "%s Victim is no longer targetable, try casting again!", MOD_NAME );
	}

	new MULTIPLIER = 150 // the lower the number the faster it reaches the target

	new vVictimOrigin[3], vFunnel[3];
	get_user_origin( iVictim, vVictimOrigin );

	// Origin of the funnel
	vFunnel[0] = parm[2];
	vFunnel[1] = parm[3];
	vFunnel[2] = parm[4];
	
	// Draw a funnel
	Create_TE_LARGEFUNNEL( vFunnel, g_iSprites[SPRITE_SNOW], 0 );
	
	// Now we need to calculate where the next funnel will be drawn
	new vDist[3];
	vDist[XPOS] = SKILL_HLP_Diff( vVictimOrigin[0], vFunnel[0] );
	vDist[YPOS] = SKILL_HLP_Diff( vVictimOrigin[1], vFunnel[1] );
	vDist[ZPOS] = SKILL_HLP_Diff( vVictimOrigin[2], vFunnel[2] );	
	
	new i;
	for ( i = 0; i < 3; i++ )
	{
		
		if ( SKILL_HLP_Diff( vVictimOrigin[i], vFunnel[i] - MULTIPLIER ) < vDist[i] )
		{
			vFunnel[i] -= MULTIPLIER;
		}
		else if ( SKILL_HLP_Diff( vVictimOrigin[i], vFunnel[0] + MULTIPLIER ) < vDist[i] )
		{
			vFunnel[i] += MULTIPLIER;
		}
		else
		{
			vFunnel[i] = vVictimOrigin[i];
		}
	}

	// Now we need to set the parms so (if/when) we call this function again it knows the next origin
	parm[2] = vFunnel[0];
	parm[3] = vFunnel[1];
	parm[4] = vFunnel[2];


	// Still not close enough to our target
	if ( !( vDist[XPOS] < 50 && vDist[YPOS] < 50 && vDist[ZPOS] < 50 ) )
	{
		set_task( 0.2, "_SKILL_LocustEffect", iAttacker + TASK_FUNNELS, parm, 5 );
	}

	// We're close enough, we can damage them!
	else
	{
		new iDamage = random_num( LOCUSTSWARM_DMG_MIN, LOCUSTSWARM_DMG_MAX );
		
		// The player has skilled intellect, add the bonus
		if ( p_data_attrib[iAttacker][ATTRIB_INTELLECT] >= INTELLECT_LOCUST )
		{
			iDamage += INTELLECT_LOCUST_BONUS;
		}

		UWC3NG_DoDamage( iVictim, iAttacker, iDamage, CSW_LOCUSTS, -1 );
		
		emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_LOCUSTSWARM], 1.0, ATTN_NORM, 0, PITCH_NORM );
		
		new szName[32];
		get_user_name( iVictim, szName, 31 );

		client_print( iAttacker, print_chat, "%s Locust Swarm hit %s for %d damage!", MOD_NAME, szName, iDamage );
	}
}

SKILL_HLP_Diff( iNum, iNum2 )
{
	if ( iNum > iNum2 )
	{
		return (iNum-iNum2);
	}
	else
	{
		return (iNum2-iNum);
	}

	return 0;
}
// ---------------------------------------- End Locust Swarm ---------------------------------------
		
		
		
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
