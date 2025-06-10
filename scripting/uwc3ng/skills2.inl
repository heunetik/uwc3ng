// Uwc3ng skills2 functions (skills 8-15)

// Spiked Carapace
public SKILL_SpikedCarapace( iAttacker, iVictim, iDamage, iHitPlace )
{
	new iLevel = p_data_skill[iVictim][SKILL_SPIKED];
	
	// Has this player spiked carapace?
	if ( iLevel )
	{
		new iTemp = floatround( float( iDamage ) * p_spiked[iLevel-1] );
		
		// Give the victim some armor...
		new CsArmorType:armorType;
		new iCurArmor = cs_get_user_armor( iVictim, armorType );
		new const iMaxArmor = 100;
			
		// Then set their armor to be the max
		if ( iCurArmor + iTemp > iMaxArmor )
		{
			cs_set_user_armor( iVictim, iMaxArmor, armorType );
		}
			
		// Just give them some bonus armor
		else
		{
			cs_set_user_armor( iVictim, iCurArmor + iTemp, armorType );
		}
				
		if ( is_user_alive( iAttacker ) )
		{
			// Damage our attacker!
			UWC3NG_DoDamage( iAttacker, iVictim, iTemp, CSW_CARAPACE, iHitPlace );

			// Make the user glow!
			SHARED_Glow( iAttacker, ( 3 * iTemp ), 0, 0, 0 );
			
			// Create a screen fade
			Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 255, 0, 0, iTemp );
		}
	}
	
	return;
}

// Repair Armor
public SKILL_WcRepair( id )
{
	// Can we execute the ultimate?
	if ( !ULTIMATE_CheckExecute( id, SKILL_REPAIR ) )
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
	
	// Amount of repairs are calculated with the level from the player
	if ( p_data[id][P_REPAIR] >= ( p_data[id][PLAYER_LVL] / REPAIR_DIVISOR ) )
	{
		UWC3NG_StatusText( id, TXT_SKILL, "You have already performed the maximum number of repairs for this round" );
		
		return;
	}
	
	// Get the names from the two players
	static SkillOwnerName[32], FriendName[32];
	get_user_name( id, SkillOwnerName, charsmax( SkillOwnerName ) );
	get_user_name( FriendID, FriendName, charsmax( FriendName ) );
	
	// Get the armor from the friend
	new CsArmorType:armorType;
	new iCurArmor = cs_get_user_armor( FriendID, armorType );
	
	// Need this player repair?
	if ( iCurArmor <= 0 || iCurArmor >= 100 )
	{
		static ShownText[256];
		formatex( ShownText, charsmax( ShownText ), "%s does not need armor repair at this time", FriendName );
		UWC3NG_StatusText( id, TXT_SKILL, ShownText );
				
		return;
	}
	
	// Alright. Add one
	p_data[id][P_REPAIR]++;
	
	// Calculate the Bonus armor
	new iBonusArmor = iCurArmor + floatround( REPAIR_BONUS * float( p_data[id][PLAYER_LVL] ) );
	
	// Check the bonus armor
	if ( iBonusArmor > 100 )
	{
		iBonusArmor = 100;
	}
	
	// Set the new armor
	cs_set_user_armor( FriendID, iBonusArmor, armorType );
	
	// Create pentagram symbols
	static origin[3];
	get_user_origin( FriendID, origin );
	Create_Pentagram( origin, g_iSprites[SPRITE_P_REPAIR] )
	
	// Print a message to the friend
	static ShownText[256];
	formatex( ShownText, charsmax( ShownText ), "%s has made some repairs to your armor", SkillOwnerName );
	UWC3NG_StatusText( id, TXT_SKILL, ShownText );
	
	// Play the repair sound
	emit_sound( id, CHAN_ITEM, g_szSounds[SOUND_REPAIR], 1.0, ATTN_NORM, 0, PITCH_NORM );
	
	// Calculate the xps which the player should get
	new iXPs = XP_Calc( id, REPAIR_HEAL );
		
	// Give him the xps
	new iBonusXP = XP_Give( id, iXPs );

	// All is okay, write him a message
	if ( iBonusXP != 0 )
	{
		client_print( id, print_chat, "%s You have been awarded %d XP for making repairs to your teammate's armor", MOD_NAME, iBonusXP );
	}
	
	return;
}

// Mend Wounds
public SKILL_WcMend( id )
{
	// Can we execute the ultimate?
	if ( !ULTIMATE_CheckExecute( id, SKILL_MEND ) )
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
	
	// Amount of heals are calculated with the level from the player
	if ( p_data[id][P_MEND] >= ( p_data[id][PLAYER_LVL] / MEND_DIVISOR ) )
	{
		UWC3NG_StatusText( id, TXT_SKILL, "You have already mended the maximum number of wounds for this round" );
		
		return;
	}
	
	// Get the names from the two players
	static SkillOwnerName[32], FriendName[32];
	get_user_name( id, SkillOwnerName, charsmax( SkillOwnerName ) );
	get_user_name( FriendID, FriendName, charsmax( FriendName ) );
	
	// Get the health from the friend and the maxhealth
	new iHealth = get_user_health( FriendID );
	new iMaxHealth = PLAYER_get_maxhealth( FriendID );
	
	
	// Need this player mend?
	if ( iHealth >= iMaxHealth )
	{
		static ShownText[256];
		formatex( ShownText, charsmax( ShownText ), "%s does not have wounds to be mended at this time", FriendName );
		UWC3NG_StatusText( id, TXT_SKILL, ShownText );
				
		return;
	}
	
	// Alright. Add one
	p_data[id][P_MEND]++;
	
	// Calculate the Bonus health
	new iBonusHealth = iHealth + floatround( MEND_BONUS * float( p_data[id][PLAYER_LVL] ) );
	
	// Check the bonus health
	if ( iBonusHealth > iMaxHealth )
	{
		iBonusHealth = iMaxHealth;
	}
	
	// Set the new health
	set_user_health( FriendID, iBonusHealth );
	
	// Create pentagram symbols
	static origin[3];
	get_user_origin( FriendID, origin );
	Create_Pentagram( origin, g_iSprites[SPRITE_P_MEND] )
	
	// Print a message to the friend
	static ShownText[256];
	formatex( ShownText, charsmax( ShownText ), "%s has provided basic medical care to you", SkillOwnerName );
	UWC3NG_StatusText( id, TXT_SKILL, ShownText );
		
	// Play the mend sound
	emit_sound( id, CHAN_ITEM, g_szSounds[SOUND_MEND], 1.0, ATTN_NORM, 0, PITCH_NORM );
	
	// Calculate the xps which the player should get
	new iXPs = XP_Calc( id, MEND_HEAL );
	
	// Give him the xps
	new iBonusXP = XP_Give( id, iXPs );

	// All is okay, write him a message
	if ( iBonusXP != 0 )
	{
		client_print( id, print_chat, "%s You have been awarded %d XP for healing the wounds of %s", MOD_NAME, iBonusXP, FriendName );
	}
	
	return;
}

// Unholy Aura
public SKILL_UnholyAura( id )
{
	// We shouldn't set the speed in the freezetime
	if ( !g_FreezeTimeOver )
	{
		return;
	}
	
	// Get maybe the right ID
	if ( id > TASK_SPEED )
	{
		id -= TASK_SPEED;
	}
	
	if ( !is_user_alive( id ) )
	{
		return;
	}
	
	// Player is stunned
	else if ( p_data_b[id][PB_STUNNED] )
	{
		set_user_maxspeed( id, 1.0 );
		
		return;
	}
	
	// User is slowed
	else if ( p_data_b[id][PB_SLOWED] )
	{
		set_user_maxspeed( id, get_pcvar_float( CVAR_uwc3ng_frost ) );
		
		return;
	}
	
	new Float:fNewSpeed = 0.0;
	new iSkillLevel = p_data_skill[id][SKILL_UNHOLYAURA];
	
	// Unholy Aura bonus
	if ( iSkillLevel )
	{
		fNewSpeed = p_unholy[iSkillLevel-1];
	}
	
	// Boots of Speed bonus
	if ( ITEM_Has( id, ITEM_BOOTS ) > ITEM_NONE )
	{
		new iClip, iAmmo;
		new iWeapon = get_user_weapon( id, iClip, iAmmo );
			
		// Then just apply the bonus!
		if ( fNewSpeed > 0.0 )
		{
			fNewSpeed *= ( 1.0 + get_pcvar_float( CVAR_uwc3ng_boots ) );
		}
			
		// User only has boots
		else
		{
			// Give them the bonus
			if ( g_iPlayerRole[id] == PLR_VIP )
			{
				fNewSpeed = CS_SPEED_VIP * ( 1.0 + get_pcvar_float( CVAR_uwc3ng_boots ) );
			}
			// Player zoomed in
			else if ( g_bPlayerZoomed[id] )
			{
				fNewSpeed = CS_WEAPON_SPEED_ZOOM[iWeapon] * ( 1.0 + get_pcvar_float( CVAR_uwc3ng_boots ) );
			}
			// Regular
			else
			{
				fNewSpeed = CS_WEAPON_SPEED[iWeapon] * ( 1.0 + get_pcvar_float( CVAR_uwc3ng_boots ) );
			}
		}
	}
	
	// The player has skilled agility, add the bonus
	if ( p_data_attrib[id][ATTRIB_AGILITY] > MAX_ATTRIBS_STARTPOINTS )
	{
		new iClip, iAmmo;
		new iWeapon = get_user_weapon( id, iClip, iAmmo );
			
		// Then just apply the bonus!
		if ( fNewSpeed > 0.0 )
		{
			fNewSpeed += 8.0 * ( p_data_attrib[id][ATTRIB_AGILITY] - MAX_ATTRIBS_STARTPOINTS );
		}
			
		// User only has the attrib
		else
		{
			// Give them the bonus
			if ( g_iPlayerRole[id] == PLR_VIP )
			{
				fNewSpeed = CS_SPEED_VIP + 8.0 * ( p_data_attrib[id][ATTRIB_AGILITY] - MAX_ATTRIBS_STARTPOINTS );
			}
			// Player zoomed in
			else if ( g_bPlayerZoomed[id] )
			{
				fNewSpeed = CS_WEAPON_SPEED_ZOOM[iWeapon] + 8.0 * ( p_data_attrib[id][ATTRIB_AGILITY] - MAX_ATTRIBS_STARTPOINTS );
			}
			// Regular
			else
			{
				fNewSpeed = CS_WEAPON_SPEED[iWeapon] + 8.0 * ( p_data_attrib[id][ATTRIB_AGILITY] - MAX_ATTRIBS_STARTPOINTS );
			}
		}
	}

	// Change the user's speed!
	if ( fNewSpeed > 0.0 && !p_data_b[id][PB_NORMALSPEED] )
	{
		set_user_maxspeed( id, fNewSpeed );

		return;
	}
	

	// We would never get here unless we didn't hit any if statement from above
	// Here we want to reset the user's speed
	new iClip, iAmmo;
	new iWeapon = get_user_weapon( id, iClip, iAmmo );
	
	// The player is the VIP
	if ( g_iPlayerRole[id] == PLR_VIP )
	{
		set_user_maxspeed( id, CS_SPEED_VIP );
	}
	
	// The player run with a zoomed weapon
	else if ( g_bPlayerZoomed[id] )
	{
		set_user_maxspeed( id, CS_WEAPON_SPEED_ZOOM[iWeapon] );
	}
	
	// The player has a normal weapon and run
	else
	{
		set_user_maxspeed( id, CS_WEAPON_SPEED[iWeapon] );
	}
	
	return;
}

// Hex
public SKILL_Hex( iAttacker, iVictim )
{
	new iSkillLevel = p_data_skill[iAttacker][SKILL_HEX];
	static Float:iHexChance;
	
	// the victim has mole, he is immun
	if ( iSkillLevel && !p_data_b[iVictim][PB_SKINSWITCHED] && !p_data_b[iVictim][PB_HEXED] )
	{
		// Add the hex chance
		iHexChance = p_hex[iSkillLevel-1];
		
		// The player has skilled intellect, add the bonus
		if ( p_data_attrib[iAttacker][ATTRIB_INTELLECT] >= INTELLECT_HEX )
		{
			iHexChance += INTELLECT_HEX_BONUS;
		}
		
		// Will the victim hexed?
		if ( random_float( 0.0, 1.0 ) <= iHexChance )
		{
			// Is the player immun?
			if ( p_data_b[iVictim][PB_CANT_RENDER] )
			{
				return;
			}
			
			// The user is hexed - yes
			p_data_b[iVictim][PB_HEXED] = true;
			
			// Change the skin from this player (1 = to zombie)
			SHARED_ChangeSkin( iVictim, 1 );
			
			// Set up the task that the hexed player jump in a few seconds
			if ( !is_user_bot( iVictim ) )
				PLAYER_Jump( TASK_JUMP + iVictim );
			
			// Get the two names
			static VictimName[32], AttackerName[32];
			get_user_name( iVictim, VictimName, charsmax( VictimName ) );
			get_user_name( iAttacker, AttackerName, charsmax( AttackerName ) );
			
			// Print a message
			client_print( iVictim, print_chat, "%s %s has morphed you into an undesirable form", MOD_NAME, AttackerName );
			client_print( iAttacker, print_chat, "%s You have morphed %s into an undesirable form", MOD_NAME, VictimName );
			
			// Play the hex sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_HEX], 1.0, ATTN_NORM, 0, PITCH_NORM );

			// Lets create a screen fade
			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 255, 255, 75 )
		}
	}
	
	return;
}

// Bash
public SKILL_Bash( iAttacker, iVictim )
{
	new iSkillLevel = p_data_skill[iAttacker][SKILL_BASH];
	static Float:iBashChance;
	
	// Has this player bash?
	if ( iSkillLevel )
	{
		// Add the bash chance
		iBashChance = p_bash[iSkillLevel-1];
		
		// The player has skilled intellect, add the bash bonus
		if ( p_data_attrib[iAttacker][ATTRIB_INTELLECT] >= INTELLECT_BASH )
		{
			iBashChance += INTELLECT_BASH_BONUS;
		}
		
		// Will this hit a "bash" hit?
		if ( random_float( 0.0, 1.0 ) <= iBashChance )
		{
			// Yes he is stunned
			p_data_b[iVictim][PB_STUNNED] = true;
			
			// Set him the new speed
			SKILL_UnholyAura( iVictim );
			
			// Remove the bash effect
			set_task( BASH_HOLD_TIME, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + iVictim );

			// Make the user glow!
			SHARED_Glow( iVictim, 0, 0, 0, BASH_GLOW_INTENSITY );
			
			// Create a screen fade
			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 255, 255, g_GlowLevel[iVictim][3] )
		}
	}
	
	return;
}

//-------------------------------------------------------------------------------- Start Serpent Ward --------------------
// Serpent Ward (Initial)
public SKILL_SerpentWard( id )
{
	static iSkillLevel;

	iSkillLevel = p_data_skill[id][SKILL_WARD];

	// User should have some!
	if ( iSkillLevel )
	{
		// Increase the available serpent wards
		p_data[id][P_SERPENTCOUNT] = p_serpent[iSkillLevel-1];
	}
	
	// User should not have any!
	else
	{
		// Remove all available serpents!
		p_data[id][P_SERPENTCOUNT] = 0;

		// We don't need to remove any of the wards b/c the task knows to check the skill level
	}
}

// Serpent Ward (player a serpent ward)
public SKILL_PlaceSerpentWard( id )
{

	if ( !ULTIMATE_CheckExecute( id, SKILL_WARD ) )
	{
		return;
	}

	// User is alive we can place a ward!
	else if ( p_data[id][P_SERPENTCOUNT] > 0 )
	{

		// Serpent Ward
		if ( SKILL_CanPlaceWard( id ) )
		{
			new parm[5], origin[3], MessageText[128];
			
			get_user_origin( id, origin );
			parm[0] = origin[0];
			parm[1] = origin[1];
			parm[2] = origin[2];
			parm[3] = id;
			parm[4] = get_user_team( id );

			_SKILL_DrawSerpentWard( parm );
			p_data[id][P_SERPENTCOUNT]--;
			
			// Create the messagetext
			formatex ( MessageText, charsmax( MessageText ), "Serpent Ward Placed, %d remaining", p_data[id][P_SERPENTCOUNT] );
			UWC3NG_StatusText( id, 0, MessageText );
		}
		else
		{
			UWC3NG_StatusText( id, 0, "You cannot place a serpent ward here" );
		}
	}
	
	// Can't place ward
	else
	{
		client_print( id, print_chat, "%s You have set the maximum of serpent wards", MOD_NAME );
	}
	
}

// Called when the bomb is planted
public SKILL_WARD_BombPlanted()
{
	bWardBombCheck = true;

	set_task( 1.0, "_SKILL_ResetBombCheck", 0 );
}

public _SKILL_ResetBombCheck()
{
	bWardBombCheck = false;
}

public _SKILL_DrawSerpentWard( parm[5] )
{

	if ( !UWC3NG_Check() )
	{
		return;
	}

	new id = parm[3];

	// User is no longer alive, don't draw wards
	if ( !is_user_alive( id ) )
	{
		return;
	}

	// User doesn't have this skill anymore! Remove the ward!
	if ( p_data_skill[id][SKILL_WARD] <= 0 )
	{
		return;
	}

	// Don't allow the ward to be by a hostage or the bom
	if ( bWardBombCheck )
	{
		if ( !SKILL_CanPlaceWard( id ) )
		{
			client_print( id, print_chat, "%s Serpent ward removed, you can't have them near bombs or hostages!", MOD_NAME );

			return;
		}
	}

	new origin[3], start[3], end[3], red, blue, green;
	origin[0]=parm[0];
	origin[1]=parm[1];
	origin[2]=parm[2];
	
	// Set up the top of the ward
	start[0] = origin[0];
	start[1] = origin[1];
	start[2] = origin[2] + 600;

	// Set up the bottom of the ward
	end[0] = origin[0];
	end[1] = origin[1];
	end[2] = origin[2] - 600;

	// Set up colors for eacht team
	if ( parm[4] == TEAM_T )
	{
		red	= 255;
		blue	= 0;
		green	= 0;
	}
	
	// CT
	else if ( parm[4] == TEAM_CT )
	{
		red	= 0;
		blue	= 255;
		green	= 0;
	}

	// Draw the serpent ward
	Create_TE_BEAMPOINTS( start, end, g_iSprites[SPRITE_LIGHTNING], 1, 5, 2, 500, 20, red, green, blue, 100, 100 );

	// Now lets check and see if any players are near that need to be damaged
	new players[32], numberofplayers;
	new targetid, targetorigin[3];

	get_players( players, numberofplayers, "a" );
	
	for ( new i = 0; i < numberofplayers; i++ )
	{
		targetid = players[i];

		if ( parm[4] != get_user_team( targetid ) )
		{
			get_user_origin( targetid, targetorigin );

			if ( get_distance( origin, targetorigin ) < 85 )
			{
				// The player is immun
				if ( ULTIMATE_IsImmun( targetid ) )
				{
					continue;
				}
				
				// Magic resistance check
				if ( p_data_resist[targetid][RESIST_MAGIC] > 0 )
				{
					if ( random_float( 0.0, 1.0 ) <= ( p_data_resist[targetid][RESIST_MAGIC] / MAX_RESIST_POINTS ) )
					{
						client_print( targetid, print_chat, "%s Your magic resistance protects you from a serpent ward attack", MOD_NAME );
						
						continue;
					}
				}

				// Damage the user
				UWC3NG_DoDamage( targetid, id, 10, CSW_SERPENTWARD, -1 );

				// Make the sound when they're attacked
				client_cmd( targetid, "speak ambience/thunder_clap.wav" );

				// Create screen fade on the owners screen
				Create_ScreenFade( id, (1<<10), (1<<10), (1<<12), red, green, blue, 55 );
			}
		}
	}

	set_task( 0.5, "_SKILL_DrawSerpentWard", TASK_LIGHT + id, parm, 5 );

	return;
}

bool:SKILL_CanPlaceWard( id )
{
	new vPlayerOrigin[3];
 	get_user_origin( id, vPlayerOrigin );

	// Make sure the user isn't trying to place a ward near a hostage
	if ( SHARED_NearObjective( vPlayerOrigin ) == OBJENT_HOSTAGE )
	{
		return false;
	}

	// Make sure the user isn't trying to place a ward near the bomb
 	new Float:vEntOrigin[3];
 	new vEntityOrigin[3];
 	
 	new iEnt = find_ent_by_model( -1, "grenade", "models/w_c4.mdl" )
 	
	if ( iEnt && is_valid_ent( iEnt ) ) 
	{
		
    	entity_get_vector( iEnt, EV_VEC_origin, vEntOrigin )
    
    	vEntityOrigin[0] = floatround( vEntOrigin[0] );
    	vEntityOrigin[1] = floatround( vEntOrigin[1] );
    	vEntityOrigin[2] = floatround( vEntOrigin[2] );
    	
    	if ( get_distance( vPlayerOrigin, vEntityOrigin ) < 250 )
		{
    		return false;
		}
	}
	
	return true;
}
//-------------------------------------------------------------------------------- End Serpent Ward --------------------

// Banish
public SKILL_Banish( iAttacker, iVictim )
{
	new iSkillLevel = p_data_skill[iAttacker][SKILL_BANISH];
	static Float:iBanishChance;
	
	// Has this player banish?
	if ( iSkillLevel )
	{
		// Add the banish chance
		iBanishChance = p_banish[iSkillLevel-1];
		
		// The player has skilled intellect, add the bonus
		if ( p_data_attrib[iAttacker][ATTRIB_INTELLECT] >= INTELLECT_BANISH )
		{
			iBanishChance += INTELLECT_BANISH_BONUS;
		}
		
		// Will this a banish hit?
		if ( random_float( 0.0, 1.0 ) <= iBanishChance )
		{
			// Slap him three times without to do damage
			user_slap( iVictim, 0 );
			user_slap( iVictim, 0 );
			user_slap( iVictim, 0 );
			
			// Now deal some damage
			UWC3NG_DoDamage( iVictim, iAttacker, BANISH_DAMAGE, CSW_BANISH, 0 );
			
			// Play the Banish sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_BANISH], 1.0, ATTN_NORM, 0, PITCH_NORM );
			
			// Make the user glow!
			SHARED_Glow( iVictim, 0, 0, 0, 100 );
			
			// Create a screen fade (purplish)
			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 144, 58, 255, g_GlowLevel[iAttacker][1] );
		}
	}
	
	return;
}

			
			
			
	
	
	
	
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
