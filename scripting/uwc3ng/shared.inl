// Uwc3ng SHARED functions

// Handle the tasks for leather skin and healing wave
public SHARED_HandleTasks( id )
{
	new iLevelSkin = p_data_skill[id][SKILL_LSKIN];
	new iLevelWave = p_data_skill[id][SKILL_HEALINGWAVE];
	new iLevelConst = p_data_attrib[id][ATTRIB_CONSTITUTION];
	
	// Calculate the true Constitution level
	iLevelConst -= MAX_ATTRIBS_STARTPOINTS;
	
	// Set up the first task for leather skin
	if ( iLevelSkin )
	{
		if ( !task_exists( id + TASK_SKIN ) )
		{
			set_task( p_lskin[iLevelSkin-1], "SKILL_LeatherSkin", id + TASK_SKIN );
		}
	}
	
	// Here we save the time for the task
	new Float:iHealTime = 0.0;
	
	// Set up the first task for healing wave
	if ( iLevelWave )
	{
		iHealTime = p_heal[iLevelWave-1];
	}
	
	else if ( iLevelConst )
	{
		iHealTime = p_heal[0];
	}
	
	if ( !task_exists( id + TASK_WAVE ) && iHealTime )
	{
		set_task( iHealTime, "SKILL_HealingWave", id + TASK_WAVE );
	}
	
	return;
}

// Set up the equipment reincarnation
public SHARED_Save_Weapons( id )
{
	if ( !is_user_alive( id ) )
	{
		return;
	}
	
	// Clear Array
	for( new i = 0; i < 32; i++ )
	{
		g_PlayerWeapons[id][i] = 0;
	}

	new num = 0;
	get_user_weapons( id, g_PlayerWeapons[id], num );
	
	return;
}

// Set up the quipment reincarnation
public SHARED_Give_Player_Weapons( id )
{
	id -= TASK_GIVEITEMS;
	
	if ( !is_user_alive( id ) )
	{
		return;
	}
	
	// Check to see if the user should have the bomb
	new bool:bGiveBomb = false;
	if ( cs_get_user_plant( id ) )
	{
		bGiveBomb = true;
	}

	// Remove all weapons
	strip_user_weapons( id );
	
	// Give armor
	if ( p_data[id][P_LASTARMOR] )
	{
		cs_set_user_armor( id, p_data[id][P_LASTARMOR], g_ArmorType[id] );
	}
	
	// Give a defuse kit
	if ( p_data_b[id][PB_DEFUSE] )
	{
		give_item(id, "item_thighpack");
		
		// Check for bomb on the back of chameleon
		if ( p_data_b[id][PB_SKINSWITCHED] )
			entity_set_int(id, EV_INT_body, 0);
	}

	if ( p_data_b[id][PB_NIGHTVISION] )
	{
		cs_set_user_nvg( id, 1 );
	}

	// Give the user a bomb
	if ( bGiveBomb )
	{
		give_item( id, "weapon_c4" );

		cs_set_user_plant( id, 1, 1 );
	}
	
	new iWeapID = 0, i = 0, bool:bPrimaryFound = false;
	for ( i = 0; i < 32; i++ )
	{
		iWeapID = g_PlayerLastWeapons[id][i];
		
		if ( iWeapID )
		{
			if ( iWeapID != CSW_C4 && iWeapID != CSW_FLASHBANG )
			{
				new szWeaponName[32], szAmmoName[32];
				get_weaponname( iWeapID, szWeaponName, 31 );
				SHARED_GetAmmoName( iWeapID, szAmmoName, 31 );
				
				if ( contain( szWeaponName, "weapon_" ) != -1 )
				{
					give_item( id, szWeaponName );

					if ( strlen( szAmmoName ) > 0 )
					{
						give_item( id, szAmmoName );
						give_item( id, szAmmoName );
						give_item( id, szAmmoName );
						give_item( id, szAmmoName );
						give_item( id, szAmmoName );
					}
				}
			}
		}

		if ( SHARED_IsPrimaryWeapon( iWeapID ) )
		{
			new szWeaponName[32];
			get_weaponname( iWeapID, szWeaponName, 31 );

			bPrimaryFound = true;
		}
	}

	// Only give it if the user doesn't have a primary weapon
	if ( p_data_b[id][PB_SHIELD] )
	{
		if ( !bPrimaryFound )
		{
			give_item( id, "weapon_shield" );
		}
	}

	// Give them their flash bangs back
	while ( p_data[id][P_FLASHCOUNT] > 0 )
	{
		give_item( id, "weapon_flashbang" );

		p_data[id][P_FLASHCOUNT]--;
	}

	set_task( 0.4, "SHARED_SetSilenceBurst", TASK_SILENCEBURST + id );

	return;
}

// Set up equipmenut reincarnation
public SHARED_SetSilenceBurst( id )
{
	id -= TASK_SILENCEBURST;
	
	if ( !is_user_alive( id ) )
	{
		return;
	}
	
	new iWeaponEnt = find_ent_by_owner( -1, "weapon_m4a1", id );
	if ( is_valid_ent( iWeaponEnt ) )
	{
		cs_set_weapon_silen( iWeaponEnt, p_data_b[id][PB_M4A1_SILENCED], 1 );
	}

	iWeaponEnt = find_ent_by_owner( -1, "weapon_usp", id );
	if ( is_valid_ent( iWeaponEnt ) )
	{
		cs_set_weapon_silen( iWeaponEnt, p_data_b[id][PB_USP_SILENCED], 1 );
	}

	iWeaponEnt = find_ent_by_owner( -1, "weapon_famas", id );
	if ( is_valid_ent( iWeaponEnt ) )
	{
		cs_set_weapon_burst( iWeaponEnt, p_data_b[id][PB_FAMAS_BURST] );
	}

	iWeaponEnt = find_ent_by_owner( -1, "weapon_glock18", id );
	if ( is_valid_ent( iWeaponEnt ) )
	{
		cs_set_weapon_burst( iWeaponEnt, p_data_b[id][PB_GLOCK_BURST] );
	}
}

// Set up equipment reincarnation
// Copy the weapons over right before reset hud is called, that way we don't lose our last round weapons when the round restarts
public SHARED_CopySavedWeapons( id )
{
	for ( new j = 0; j < 32; j++ )
	{
		g_PlayerLastWeapons[id][j] = g_PlayerWeapons[id][j];
	}
}

// Function will return an ammo name given a weapon ID
SHARED_GetAmmoName( iWeapID, szAmmoName[], len )
{
	switch( iWeapID )
	{
		case CSW_USP, CSW_MAC10, CSW_UMP45:
		{ 
			copy( szAmmoName, len, "ammo_45acp" );
		}
		case CSW_ELITE, CSW_GLOCK18, CSW_MP5NAVY, CSW_TMP:
		{
			copy( szAmmoName, len, "ammo_9mm" );
		}
		case CSW_FIVESEVEN, CSW_P90:
		{
			copy( szAmmoName, len, "ammo_57mm" );
		}
		case CSW_DEAGLE:
		{
			copy( szAmmoName, len, "ammo_50ae" );
		}
		case CSW_P228:
		{
			copy( szAmmoName, len, "ammo_357sig" );
		}
		case CSW_SCOUT, CSW_G3SG1, CSW_AK47:
		{
			copy( szAmmoName, len, "ammo_762nato" );
		}
		case CSW_XM1014, CSW_M3:
		{
			copy( szAmmoName, len, "ammo_buckshot" );
		}
		case CSW_AUG, CSW_SG550, CSW_GALIL, CSW_FAMAS, CSW_M4A1:
		{
			copy( szAmmoName, len, "ammo_556nato" );
		}
		case CSW_AWP:
		{
			copy( szAmmoName, len, "ammo_338magnum" );
		}
		case CSW_M249:
		{
			copy( szAmmoName, len, "ammo_556natobox" );
		}
		case CSW_SG552:
		{
			copy( szAmmoName, len, "ammo_556nato" );
		}
		default:
		{
			copy( szAmmoName, len, "" );
		}
	}
}

// Function checks to see if the weapon id is a primary weapon (used with Night Elf's Entangle)
SHARED_IsPrimaryWeapon( iWeaponID )
{
	if ( iWeaponID == CSW_GALIL || iWeaponID == CSW_FAMAS || iWeaponID == CSW_M3 || iWeaponID == CSW_XM1014 || iWeaponID == CSW_MP5NAVY || iWeaponID == CSW_TMP || iWeaponID == CSW_P90 || iWeaponID == CSW_MAC10 || iWeaponID == CSW_UMP45 || iWeaponID == CSW_AK47 || iWeaponID == CSW_SG552 || iWeaponID == CSW_M4A1 || iWeaponID == CSW_AUG || iWeaponID == CSW_SCOUT || iWeaponID == CSW_AWP || iWeaponID == CSW_G3SG1 || iWeaponID == CSW_SG550 || iWeaponID == CSW_M249 )
	{
		return true;
	}
	
	return false;
}

// Give someone money
public SHARED_SetUserMoney( id, iMoney )
{
	// Get the current money
	new PlayerMoney = cs_get_user_money( id );
	
	// Add the new money
	PlayerMoney += iMoney;
	
	// Check the money
	if ( PlayerMoney > 16000 )
	{
		PlayerMoney = 16000;
	}
	
	else if ( PlayerMoney < 0 )
	{
		PlayerMoney = 0;
	}
	
	// Set the new money
	cs_set_user_money( id, PlayerMoney );
	
	return;
}

// From wc3ft
public SHARED_Glow( id, iRed, iGreen, iBlue, iAll )
{
	
	// Don't glow if invisible
	if ( p_data_skill[id][SKILL_INVISIBLE] )
	{
		return;
	}

	// Only glow if the task doesn't exist!
	if ( task_exists( TASK_GLOW + id ) )
	{
		return;
	}
	
	if ( iAll )
	{
		g_GlowLevel[id][0]	= 0;
		g_GlowLevel[id][1]	= 0;
		g_GlowLevel[id][2]	= 0;
		g_GlowLevel[id][3]	+= iAll;
	}
	else if ( iRed )
	{
		g_GlowLevel[id][0]	+= iRed;
		g_GlowLevel[id][1]	= 0;
		g_GlowLevel[id][2]	= 0;
		g_GlowLevel[id][3]	= 0;
	}
	else if ( iGreen )
	{
		g_GlowLevel[id][0]	= 0;
		g_GlowLevel[id][1]	+= iGreen;
		g_GlowLevel[id][2]	= 0;
		g_GlowLevel[id][3]	= 0;
	}
	else if ( iBlue )
	{
		g_GlowLevel[id][0]	= 0;
		g_GlowLevel[id][1]	= 0;
		g_GlowLevel[id][2]	+= iBlue;
		g_GlowLevel[id][3]	= 0;
	}

	// Lets make sure its not over the max!
	g_GlowLevel[id][0] = ( ( g_GlowLevel[id][0] > MAXGLOW ) ? MAXGLOW : g_GlowLevel[id][0] );
	g_GlowLevel[id][1] = ( ( g_GlowLevel[id][1] > MAXGLOW ) ? MAXGLOW : g_GlowLevel[id][1] );
	g_GlowLevel[id][2] = ( ( g_GlowLevel[id][2] > MAXGLOW ) ? MAXGLOW : g_GlowLevel[id][2] );
	g_GlowLevel[id][3] = ( ( g_GlowLevel[id][3] > MAXGLOW ) ? MAXGLOW : g_GlowLevel[id][3] );
	

	_SHARED_Glow( id );
}

// From wc3ft
public _SHARED_Glow( id )
{
	
	if ( id >= TASK_GLOW )
	{
		id -= TASK_GLOW;
	}

	// User is no longer connected, so lets not continue this!
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	new iRed	= g_GlowLevel[id][0];
	new iGreen	= g_GlowLevel[id][1];
	new iBlue	= g_GlowLevel[id][2];
	new iAll	= g_GlowLevel[id][3];

	// Then we want to glow
	if ( iRed || iGreen || iBlue )
	{

		g_GlowLevel[id][0] = ( ( iRed > 5 )		? iRed - 5		: 0 );
		g_GlowLevel[id][1] = ( ( iGreen > 5 )	? iGreen - 5	: 0 );
		g_GlowLevel[id][2] = ( ( iBlue > 5 )	? iBlue - 5		: 0 );

		set_user_rendering( id, kRenderFxGlowShell, iRed, iGreen, iBlue, kRenderNormal, 16 );
	}

	else if ( iAll )
	{
		g_GlowLevel[id][3] = ( ( iAll > 5 )		? iAll - 5		: 0 );
		
		set_user_rendering( id, kRenderFxGlowShell, iAll, iAll, iAll, kRenderNormal, 16 );
	}

	// No more glowing!
	else
	{
		set_user_rendering( id );

		return;
	}

	set_task( 0.2, "_SHARED_Glow", TASK_GLOW + id );

	return;
}

// From wc3ft
public bool:SHARED_ValidPlayer( id )
{
	if ( id < 1 || id > MAX_PLAYERS )
	{
		return false;
	}

	return true;
}

// Reset Repair Armor and Mend wound (natually called on new round event)
public SHARED_MendRepairDispell_Reset( id )
{
	// Reset the data
	p_data[id][P_REPAIR] = 0;
	p_data[id][P_MEND] = 0;
	p_data[id][P_DISPELL] = 0;
}

// Change or reset the skin from a player
public SHARED_ChangeSkin( id, iOptions )
{
	// The skin should reseted
	if ( iOptions == 0 )
	{
		cs_reset_user_model( id );
		return;
	}

	// Get the team
	new iTeam = get_user_team( id );
	
	// The player should be a zombie
	if ( iOptions == 1 )
	{
		cs_set_user_model( id, "zombie" );

		// Color CT zombies blue so they're easier to tell apart
		if( iTeam == TEAM_CT ) {
			set_user_info( id, "topcolor", "160" );
			set_user_info( id, "bottomcolor", "160" );
		}
	}
	
	// Mole
	else if ( iOptions == 2 )
	{
		new szSkin[32]
		
		// Get one random model
		new iModelNum = random_num( 0, 3 );
			
		// Save which skin we're going to use
		if ( iTeam == TEAM_CT )
		{
			add( szSkin, charsmax( szSkin ), SKIN_T[iModelNum] );
		}
		
		// CT
		else
		{
			add( szSkin, charsmax( szSkin ), SKIN_CT[iModelNum] );
		}

		// Set the model and check for defuse
		cs_set_user_model( id, szSkin );
		if ( cs_get_user_defuse( id ) )
		{
			entity_set_int(id, EV_INT_body, 0);
		}
		
		// Save that we have switched the skin
		p_data_b[id][PB_SKINSWITCHED] = true;
	}
	
	return;
}

// Reset the maxspeed from a player
public SHARED_ResetMaxSpeed( id )
{
	id -= TASK_RESETSPEED;
	
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	// Is nomore stunned
	p_data_b[id][PB_STUNNED] = false;
	
	// Is nomore slowed
	p_data_b[id][PB_SLOWED] = false;
	
	// Set him the normal speed back
	SKILL_UnholyAura( id );
}

// Returns if an origin is near an objective (returns which objective) (from wc3ft)
public SHARED_NearObjective( vOrigin[3] )
{

	new i, Float:fOrigin[3];
	
	// Convert vector to float
	IVecFVec( vOrigin, fOrigin );

	// Check the distances
	for ( i = 0; i < g_iTotalObjectiveEnts; i++ )
	{
		new Float:fDistance = vector_distance( fOrigin, g_fObjectiveOrigin[i] );

		if ( fDistance < 250.0 )
		{
			return g_iObjectiveEntType[i];
		}
	}

	return -1;
}

// Find map objectives
public SHARED_FindObjectives()
{
	// We need to determine the objective entities (if they exist)
	new i, bool:bFound;
	for ( i = 0; i <= get_global_int( GL_maxEntities ); i++ )
	{
		if ( !is_valid_ent( i ) )
		{
			continue;
		}
		
		// More than were necessary was found
		if ( g_iTotalObjectiveEnts >= MAX_OBJECTIVES )
		{
			break;
		}

		bFound = false;

		new szClassName[64];
		entity_get_string( i, EV_SZ_classname, szClassName, 63 );
		
		// VIP Escape zone!!
		if ( equal( szClassName, "func_vip_safetyzone") )	
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_VIP_ESCAPE;
			bFound = true;
		}

		// Hostage zone
		else if ( equal( szClassName, "hostage_entity") )
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_HOSTAGE;
			bFound = true;
		}

		// Bomb zone
		else if ( equal( szClassName, "func_bomb_target") )
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_BOMBSITE;
			bFound = true;
		}

		// Hostage escape zone
		else if ( equal( szClassName, "func_escapezone") )
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_HOSTAGE_ESCAPE;
			bFound = true;
		}

		// We found an objective entity!!!
		if ( bFound )
		{
			g_iObjectiveEnt[g_iTotalObjectiveEnts] = i;

			get_brush_entity_origin( i, g_fObjectiveOrigin[g_iTotalObjectiveEnts] );

			// Increment the total number
			g_iTotalObjectiveEnts++;
		}
	}
	
	return;
}

// This will teleport a user to a location and test to make sure they were actually moved there (from wc3ft)
SHARED_Teleport( id, vOrigin[3] )
{
	// Increase so user doesn't get stuck in ground
	vOrigin[2] += 15;

	// Attempt to move the user
	set_user_origin( id, vOrigin );

	new iParm[4];
	iParm[0] = vOrigin[0];
	iParm[1] = vOrigin[1];
	iParm[2] = vOrigin[2];
	iParm[3] = id;

	// Set up the parameters
	set_task( 0.1, "_SHARED_Teleport", TASK_TELEPORT + id, iParm, 4 );
}

public _SHARED_Teleport( parm[] )
{
	new id = parm[3];
	new vOrigin[3];
	
	get_user_origin( id, vOrigin );


	// User is stuck, lets teleport them back to their spawn
	if ( vOrigin[2] == parm[2] )
	{
		// Find a spawn - ignore immunity and team reversal
		new iSpawnEnt = SHARED_FindFreeSpawn( id, false, false );
		
		// We can move the user yay!
		if ( iSpawnEnt > 0 )
		{
			new Float:fSpawnOrigin[3], vOrigin[3];
			
			// Get the origin of the spawn point
			entity_get_vector( iSpawnEnt, EV_VEC_origin, fSpawnOrigin );

			// Convert float vector to int vector
			FVecIVec( fSpawnOrigin, vOrigin );
			
			// Move the user
			SHARED_Teleport( id, vOrigin );

			client_print( id, print_chat, "%s You are stuck! Ahhh! Moving you back to your spawn!", MOD_NAME );
		}

		// We can't move the user - that sux0rsz
		else
		{
			client_print( id, print_chat, "%s Sorry, I know you're stuck, but I can't move you right now :/", MOD_NAME );
		}
	}
}

// Find a free spawn!
SHARED_FindFreeSpawn( id, bImmunityCheck = false, bReverseTeam = false )
{

	new iPlayersInVicinity, iSpawnID, iEntList[1], vOrigin[3];
	new ent = -1;
	new Float:fSpawnOrigin[3];
	new Float:fVicinity = 96.0;
	new bool:bFound = false;
	new bool:bImmunityNear = false;
	
	new iTeam = get_user_team( id );

	// Reverse the team IDs (i.e. Mole would want this)
	if ( bReverseTeam )
	{
		iTeam = ( ( iTeam == TEAM_CT ) ? TEAM_T : TEAM_CT );
	}

	// Need to determine which spawn point to look for based on the user's team
	iSpawnID = ( ( iTeam == TEAM_CT ) ? 0 : 1 );
	
	// Loop through each ent until we find a spawn entity that we want
	do {
		ent = find_ent_by_class( ent, szSpawnEnt[iSpawnID] );
		
		// Valid ent found
		if ( ent != 0 )
		{
			entity_get_vector( ent, EV_VEC_origin, fSpawnOrigin );
			
			// Convert float vector to int vector
			FVecIVec( fSpawnOrigin, vOrigin );

			// Check to see if there are players in this spawn
			iPlayersInVicinity = find_sphere_class( 0, "player", fVicinity, iEntList, 1, fSpawnOrigin );
			
			// We have a free spawn!!
			if ( iPlayersInVicinity == 0 )
			{

				// Make sure it isn't reserved
				if ( !SHARED_SpawnReserved( ent ) )
				{

					// Then we need to reserve it :)
					g_iSpawnReserved[g_iSpawnInc++] = ent;

					// We need to make sure there isn't anyone nearby that is immune
					if ( bImmunityCheck )
					{
						// Immune found
						if ( UWC3NG_IsImmunePlayerNear( id, vOrigin ) )
						{
							//ULT_RemoveCharge( id, 6 );

							bImmunityNear = true;
						}

						// We're clear!
						else
						{
							bImmunityNear = false;
							bFound = true;
						}
					}
					
					// We have a free spawn we can quit!
					else
					{
						bFound = true;
					}
				}
			}
		}
	}
	while ( ent && !bFound && !bImmunityNear )
	
	// Reset the spawn points...
	if ( !task_exists( TASK_RESETSPAWNS ) )
	{
		set_task( 0.3, "_SHARED_SpawnReset", TASK_RESETSPAWNS );
	}

	// Failed, nothing found
	if ( !bFound )
	{
		// Return a different value so they know the reason for failing
		if ( bImmunityCheck && bImmunityNear )
		{
			return -2;
		}

		return -1;
	}
	
	// Otherwise we found something!
	return ent;
}

// Reset our reserved spawn points
public _SHARED_SpawnReset()
{
	new i;
	
	g_iSpawnInc = 0;
	for ( i = 0; i < TOTAL_SPAWNS; i++ )
	{
		g_iSpawnReserved[i] = 0;
	}
}

// Determine if this entity is reserved for another player
public bool:SHARED_SpawnReserved( ent )
{
	new i;

	for ( i = 0; i < g_iSpawnInc; i++ )
	{
		if ( g_iSpawnReserved[i] == ent )
		{
			return true;
		}
	}

	return false;
}

// Function will return true if their active weapon is a knife
public bool:SHARED_IsHoldingKnife( id )
{
	new iClip, iAmmo, iWeapon;
	iWeapon = get_user_weapon( id, iClip, iAmmo );

	if ( iWeapon == CSW_KNIFE )
	{
		return true;
	}
	
	return false;
}

// Function used to spawn a player
public _SHARED_Spawn( id )
{
	// Respawning isn't necessary w/CSDM - so lets not do that!
	if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 )
	{
		return;
	}
	
	id -= TASK_SPAWN;
	
	// User is no longer connected or is not on a team
	if ( !is_user_connected( id ) || !SHARED_IsOnTeam( id ) )
	{
		return;
	}
	
	// Round has ended, lets give money back if they bought a scroll
	if ( g_EndRound )
	{
		if ( ITEM_Has( id, ITEM_SCROLL ) > ITEM_NONE )
		{
			// Remove the Scroll
			if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] == ITEM_SCROLL )
				g_iShopMenuItems[id][ITEM_SLOT_ONE] = ITEM_NONE;
			else if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] == ITEM_SCROLL )
				g_iShopMenuItems[id][ITEM_SLOT_TWO] = ITEM_NONE;
			
			// The player should know why
			client_print( id, print_chat, "%s Unable to respawn because the round is over, here is your money back", MOD_NAME );

			SHARED_SetUserMoney( id, ITEM_Cost( id, ITEM_SCROLL ) );
		}

		return;
	}

	if ( is_user_alive( id ) )
	{
		return;
	}
	
	// Reset items when the user spawns!
	g_iShopMenuItems[id][ITEM_SLOT_ONE] = ITEM_NONE;
	g_iShopMenuItems[id][ITEM_SLOT_TWO] = ITEM_NONE;

	// Give the user godmode for a little
	set_user_godmode( id, 1 );
	p_data_b[id][PLAYER_SPAWNGOD] = true;

	// Save their previous weapons!
	SHARED_CopySavedWeapons( id );
	
	// Ignore the armor settaging...
	bIgnoreArmorSet[id] = true;

	// We don't want to call a crap-ton of UWC3NG functions when we're spawning them 3 times do we ?
	p_data_b[id][IGNOR_SPAWN] = true;

	// Spawn the player
	ExecuteHamB( Ham_CS_RoundRespawn , id );

	// Reset the user's skin to normal
	SHARED_ChangeSkin( id, 0 );

	// The user should no longer be a mole!
	p_data_b[id][PB_SKINSWITCHED] = false;

	set_task( 0.2, "_SHARED_Spawn_Final", TASK_SPAWNPLAYER + id );
	set_task( 0.4, "SHARED_Give_Player_Weapons", TASK_GIVEITEMS + id );
	set_task( SPAWN_PROTECTION, "_SHARED_SpawnRemoveGod", TASK_SPAWNREMOVEGOD + id );

	return;
}

// Remove god
public _SHARED_SpawnRemoveGod( id )
{
	id -= TASK_SPAWNREMOVEGOD;
	
	if ( is_user_connected( id ) )
	{
		// Remove the godmode
		set_user_godmode( id, 0 );
		
		// Now call all spawn things
		on_PlayerSpawn( id );
		
		// Yes, remove!
		p_data_b[id][PLAYER_SPAWNGOD] = false;
		
		// Remove ignor spawn
		p_data_b[id][IGNOR_SPAWN] = false;
	}
	
	return;
}

// Function will just spawn a player again
public _SHARED_Spawn_Final( id )
{
	id -= TASK_SPAWNPLAYER;

	if ( !is_user_connected( id ) )
	{
		return;
	}

	bIgnoreArmorSet[id] = false;

	// If we don't give them a suit then they won't have a HUD
	give_item( id, "item_suit" );
	
	return;
}

// Is the player on a team?
public bool:SHARED_IsOnTeam( id )
{
	new iTeam = get_user_team( id );
	
	// Is he on a team?
	if ( iTeam == TEAM_T || iTeam == TEAM_CT )
	{
		return true;
	}
	
	// He isn't on a team
	return false;
}

// Ultimate pings
public _SHARED_PING( parm[] )
{
	new id = parm[0];
	new iTimeLeft = parm[1];
	
	// Decrement our timer
	parm[1]--;
	
	// User died or diconnected
	if ( !is_user_alive( id ) )
	{
		p_data_b[id][PB_ISSEARCHING] = false;
	}
	
	// This is the last "playing" of the sound, no target was found :/
	if ( iTimeLeft == 0 )
	{
		p_data_b[id][PB_ISSEARCHING] = false;

	}
	
	// Then we need to play the sound!
	if ( p_data_b[id][PB_ISSEARCHING] )
	{
		// Play the ping sound
		emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_ULTIMATESCAN], 1.0, ATTN_NORM, 0, PITCH_NORM );

		set_task( 1.0, "_SHARED_PING", TASK_ULTPING + id, parm, 2 );
	}

	return;
}

public SHARED_MoleCheck( id, bItemOnly )
{
	new parm[2];
	parm[1] = 0;

	// Only do a skill check if bItemOnly is false
	if ( !bItemOnly )
	{
		static iSkillLevel;
		iSkillLevel = p_data_skill[id][SKILL_FANOFKNIVES];

		// Mole from Fan of Knives?
		if ( iSkillLevel > 0 && random_float( 0.0, 1.0 ) <= p_fan[iSkillLevel-1] )
		{
			parm[1] = 1;
		}
	}
	
	// Mole from an item?
	if ( parm[1] == 0 )
	{
		if ( g_bPlayerBoughtMole[id] )
		{
			parm[1] = 2;
			
			ITEM_RemoveID(id, ITEM_MOLE);
			g_bPlayerBoughtMole[id] = false;
		}
	}

	// OK then lets mole!!
	if ( parm[1] > 0 )
	{
		parm[0] = id;
		
		set_task( 0.1, "_SHARED_Mole", TASK_MOLE + id, parm, 2 );
	}
}

public _SHARED_Mole( parm[2] )
{
	
	new id = parm[0];

	if ( !is_user_alive( id ) )
	{
		return;
	}

	// Lets search for a new spawn (ignore immunity, reverse team ids)	
	new ent = SHARED_FindFreeSpawn( id, false, true );
	
	// Free spawn found!!
	if ( ent > 0 )
	{
		new vOrigin[3], Float:fSpawnOrigin[3];
		
		// Get the origin of the spawn
		entity_get_vector( ent, EV_VEC_origin, fSpawnOrigin );
		
		// Convert float vector to int vector
		FVecIVec( fSpawnOrigin, vOrigin );
		
		// Change the user's skin
		SHARED_ChangeSkin( id, 2 );
		
		// Move the user
		set_user_origin( id, vOrigin );

		// Shake the user's screen
		Create_ScreenShake( id, (255<< 14), (10 << 14), (255<< 14) );
	}

	// No spawn found
	else
	{
		// Moving b/c of item - if no spot then give the user his/her money back!
		if ( parm[1] == 2 )
		{
			SHARED_SetUserMoney( id, ITEM_Cost( id, ITEM_MOLE ) );

			client_print( id, print_chat, "%s Sorry couldn't find a spot for you to mole, here's your money back", MOD_NAME );
		}

		// Moving b/c of skill
		
		{
			client_print( id, print_chat, "%s Sorry couldn't find a spot for you to mole", MOD_NAME )
		}
	}

	return; 
}

// Function returns true if the user has a grenade in his inventory
public SHARED_HasGrenade( id )
{
	new i, bool:bNadeFound = false;
	
	// Loop through all weapons and search for a grenade
	while ( g_PlayerWeapons[id][i] && !bNadeFound )
	{
		if ( g_PlayerWeapons[id][i] == CSW_HEGRENADE )
		{
			bNadeFound = true;
			break;
		}

		i++;
	}

	return bNadeFound;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
