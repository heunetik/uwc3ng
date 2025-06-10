// Uwc3ng skills1 functions (includes first 8 skills)

// Set up the levitation for a player
public SKILL_Levitation( id )
{
	// Can't set gravity if user isn't connected!
	if ( !is_user_alive( id ) )
	{
		return;
	}
	
	// If gravity is less than this, lets not change per-user b/c it BLOWS ASS in game
	if ( CVAR_sv_gravity == 0 || get_pcvar_num( CVAR_sv_gravity ) > 650 )
	{
		static iSkillLevel;
		iSkillLevel = p_data_b[id][PB_HEXED] ? 0 : p_data_skill[id][SKILL_LEVITATION];

		new Float:fGravityLevel = 1.0;

		// If this user does want their gravity to be changed!
		if ( iSkillLevel > 0 ) 
		{
			fGravityLevel = p_levitation[iSkillLevel-1];
		}

		// Set the user's gravity based on the item
		if ( ITEM_Has( id, ITEM_SOCK ) > ITEM_NONE )
		{
			
			// User has levitation + sock, give them an extra bonus
			if ( fGravityLevel < 1.0 )
			{
				fGravityLevel /= 2.0;

			}

			// User just has sock
			else
			{
				fGravityLevel = get_pcvar_float( CVAR_uwc3ng_sock );
			}
		}

		// Set the user's gravity!
		set_user_gravity( id, fGravityLevel );
	}

	return;
}

// Reincarnation for item and skill
public SKILL_CS_Reincarnation( id )
{
	// Giving weapons will only screw things up w/CSDM - so lets not do that!
	if ( CVAR_csdm_active > 0 )
	{
		return;
	}

	new bool:bGiveWeapons = false;
	
	// Check based on skill or if the user has an item
	if ( p_data_b[id][PB_DIEDLASTROUND] )
	{
		static iSkillLevel;
		iSkillLevel = p_data_skill[id][SKILL_REINCARNATION];
		
		// Orc's Reincarnation
		if ( iSkillLevel > 0 )
		{

			if ( random_float( 0.0, 1.0 ) <= p_ankh[iSkillLevel-1] )
			{
				bGiveWeapons = true;		
			}
		}
		
		// Ankh
		if ( g_bPlayerBoughtAnkh[id] )
		{
			bGiveWeapons = true;

			g_bPlayerBoughtAnkh[id] = false;
		}	
	}

	if ( bGiveWeapons )
	{
		client_cmd( id, "speak %s", g_szSounds[SOUND_REINCARNATION] );
		
		// Screen fade green
		Create_ScreenFade( id, (1<<10), (1<<10), (1<<12), 0, 255, 0, 255 );
		
		// Make the user glow!
		SHARED_Glow( id, 0, 100, 0, 0 );
		
		// Give the user their weapons from last round
		set_task( 0.3, "SHARED_Give_Player_Weapons", TASK_GIVEITEMS + id );
	}
	else
	{
		if ( !cs_get_user_nvg( id ) )
		{
			p_data_b[id][PB_NIGHTVISION] = false;
		}
	}

	return;	
}

// Impale
public SKILL_Impale( iAttacker, iVictim )
{
	new iLevel = p_data_skill[iAttacker][SKILL_IMPALE];
	new Float:iImpaleChance;
	
	// Has this player impale?
	if ( iLevel )
	{
		// Add the impale chance
		iImpaleChance = p_impale[iLevel-1];
		
		// The player has skilled intellect, give him the bonus
		if ( p_data_attrib[iAttacker][ATTRIB_INTELLECT] >= INTELLECT_IMPALE )
		{
			iImpaleChance += INTELLECT_IMPALE_BONUS;
		}
		
		// Is this an impale hit?
		if ( random_float( 0.0, 1.0 ) <= iImpaleChance )
		{
			// Play the impale sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_IMPALE], 1.0, ATTN_NORM, 0, PITCH_NORM );
			
			// Get the name from the victim
			static PlayerName[32];
			get_user_name( iVictim, PlayerName, charsmax( PlayerName ) );
			
			// Print a message to the two players
			client_print( iAttacker, print_chat, "%s You cause the ground to shake beneath %s", MOD_NAME, PlayerName );
			client_print( iVictim, print_chat, "%s The ground shakes beneath your feet", MOD_NAME );
			
			// Let us shake the screen from the victim
			Create_ScreenShake( iVictim, (255<<14), (10<<14), (255<<14) );
		}
	}
	
	return;
}

// Devotion Aura
public SKILL_DevotionAura( id )
{
	new iLevel = p_data_skill[id][SKILL_DEVOTION];
	new iLevelStrength = p_data_attrib[id][ATTRIB_STRENGTH];
	new iBonusHealth = 0;
		
	// Has this player devotion aura?
	if ( iLevel )
	{
		iBonusHealth = p_devotion[iLevel-1];
	}
	
	// Add attribute strength bonus
	if ( iLevelStrength > MAX_ATTRIBS_STARTPOINTS )
	{
		iBonusHealth += AGILITY_HEALTH_BONUS * ( iLevelStrength - MAX_ATTRIBS_STARTPOINTS );
	}
	
	// Add the new health
	if ( iBonusHealth != 0 )
	{
		set_user_health( id, ( 100 + iBonusHealth ) );
	}
	
	return;
}

// Healing Wave
public SKILL_HealingWave( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	// Get maybe the right id
	if ( id >= TASK_WAVE )
	{
		id -= TASK_WAVE;
	}
	
	if ( !is_user_alive( id ) )
	{
		return;
	}
	
	// Get the level
	new iLevel = p_data_skill[id][SKILL_HEALINGWAVE];
	new iLevelConst = p_data_attrib[id][ATTRIB_CONSTITUTION];
	
	// Calculate the true Constitution level
	iLevelConst -= MAX_ATTRIBS_STARTPOINTS;
	
	// Has this player healing wave?
	if ( iLevel > 0 || iLevelConst > 0 )
	{
		// Here we save the time for the task
		new Float:iHealTime = p_heal[0];
	
		// Set up the first task for healing wave
		if ( iLevel )
		{
			iHealTime = p_heal[iLevel-1];
		}
	
		set_task( iHealTime, "SKILL_HealingWave", id + TASK_WAVE );
		
		// Get all players
		static i, players[32], numofplayers, iTeam;
		static origin[3], iHealth, iMaxHealth, iAmountHeal;
		get_players( players, numofplayers );
		
		// Get the team from the skill owner
		iTeam = get_user_team( id );
		
		// Loop through all players
		for ( i = 0; i < numofplayers; ++i )
		{
			// They should be in the same team
			if ( get_user_team( players[i] ) == iTeam )
			{
				// Is the player in range?
				if ( PLAYER_is_in_range( id, players[i], HEALING_WAVE_RANGE ) )
				{
					// Get the health/maxhealth from this player
					iHealth = get_user_health( players[i] );
					iMaxHealth = PLAYER_get_maxhealth( players[i] );
					
					// Calculate the amount of heal depending on constitution
					iAmountHeal = ( HEALING_WAVE_HEAL + iLevelConst );
					
					// We're at the maximum. Switch to the next player
					if ( iHealth == iMaxHealth )
					{
						continue;
					}
					
					// Add health
					iHealth += iAmountHeal;
					
					// oops, we are about the maxhealth, reduce it
					if ( iHealth > iMaxHealth )
						iHealth = iMaxHealth;
					
					// Set the new health
					set_user_health( players[i], iHealth );
					
					// Get the origin from this player
					get_user_origin( players[i], origin );
					
					// Create an implosion
					Create_TE_IMPLOSION( origin, 100, 8, 1 );
				}
			}
		}
	}
	
	return;
}

// Siphon Mana
public SKILL_SiphonMana( iAttacker, iVictim, iDamage )
{
	new iLevel = p_data_skill[iAttacker][SKILL_MANA];
	static Float:iSiphonChance;
	
	// Victim is not alive, what do we here?
	if ( !is_user_alive( iVictim ) )
	{
		return;
	}
	
	// Has this player siphon mana?
	if ( iLevel )
	{
		// Add the siphon chance
		iSiphonChance = p_mana[iLevel-1];
		
		// The player has skilled intellect, add the bonus
		if ( p_data_attrib[iAttacker][ATTRIB_INTELLECT] >= INTELLECT_SIPHON )
		{
			iSiphonChance += INTELLECT_SIPHON_BONUS;
		}
		
		// Get the money which the attacker should get
		new iMoney = floatround( iSiphonChance * float( cs_get_user_money( iVictim ) ) );
		
		// Set the money from the victim
		SHARED_SetUserMoney( iVictim, -1 * iMoney );
		
		// Set the money from the attacker
		SHARED_SetUserMoney( iAttacker, iMoney );
		
		// Make the user glow!
		SHARED_Glow( iVictim, 0, iDamage, 0, 0 );
		
		// Create a screen fade (purplish)
		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 144, 58, 255, g_GlowLevel[iAttacker][1] );
	}
	
	return;
}

// Leather Skin
public SKILL_LeatherSkin( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	// Get maybe the right id
	if ( id >= TASK_SKIN )
	{
		id -= TASK_SKIN;
	}
	
	if ( !is_user_alive( id ) )
	{
		return;
	}
	
	// Get the level
	new iLevel = p_data_skill[id][SKILL_LSKIN];
	
	// Has this player leather skin?
	if ( iLevel )
	{
		// Set up the task
		set_task( p_lskin[iLevel-1], "SKILL_LeatherSkin", id + TASK_SKIN );
		
		// Get all players
		static i, players[32], numofplayers, iTeam;
		static origin[3], CsArmorType:armorType, iArmor;
		get_players( players, numofplayers );
		
		// Get the team from the skill owner
		iTeam = get_user_team( id );
		
		// Loop through all players
		for ( i = 0; i < numofplayers; ++i )
		{
			// They should be in the same team
			if ( get_user_team( players[i] ) == iTeam )
			{
				// Is the player in range?
				if ( PLAYER_is_in_range( id, players[i], LEATHER_SKIN_RANGE ) )
				{
					// Get the armor from this player
					iArmor = cs_get_user_armor( players[i], armorType );
					
					// We're at the maximum. Switch to the next player
					if ( iArmor >= 100 || iArmor <= 0 )
					{
						continue;
					}
					
					// Add one armor
					iArmor++;
					
					// Set the new armor
					cs_set_user_armor( players[i], iArmor, armorType );
					
					// Get the origin from this player
					get_user_origin( players[i], origin );
					
					// Create an implosion
					Create_TE_IMPLOSION( origin, 100, 8, 1 );
				}
			}
		}
	}
	
	return;
}

// Vampiric Aura
public SKILL_VampiricAura( iAttacker, iDamage )
{
	new iLevel = p_data_skill[iAttacker][SKILL_VAMPIRIC];
	
	// Has this user vampiric aura?
	if ( iLevel )
	{
		static iHealth, iMaxHealth, iBonusHealth;

		iHealth = get_user_health( iAttacker );
		iMaxHealth = PLAYER_get_maxhealth( iAttacker );
		iBonusHealth = floatround( float( iDamage ) * p_vampiric[iLevel-1] );
		
		// Give the user health!
		if ( iHealth < iMaxHealth )
		{

			// Then give the user his maximum health
			if ( iHealth + iBonusHealth > iMaxHealth )
			{
				set_user_health( iAttacker, iMaxHealth );
			}

			// Otherwise just give iMaxHealth
			else
			{
				set_user_health( iAttacker, iHealth + iBonusHealth );
			}
		}
		
		// Make the attacker glow
		SHARED_Glow( iAttacker, 0, ( 2 * iBonusHealth ), 0, 0 );
		
		// Give the attacker a nice screen fade
		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 0, 255, 0, iBonusHealth );
	}
	
	return;
}

/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
