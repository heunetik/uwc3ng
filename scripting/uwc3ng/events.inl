// Uwc3ng events

// This will be called when a player get damage
public client_damage( iAttacker, iVictim, iDamage, iWeapon, iBodyPart, TA )
{
	if ( !UWC3NG_Check() || iAttacker == iVictim )
	{
		return;
	}
	
	// Bot should "auto" cast his/her ultimate on random
	if ( SHARED_ValidPlayer( iAttacker ) && is_user_bot( iAttacker ) && random_num( 0, 100 ) <= ( BOT_CAST_ULT_CHANCE * 100 ) && !get_pcvar_num( CVAR_uwc3ng_ignore_bots ) )
	{
		// Check for an actual ultimate is done in this function, why do it twice?
		COMMAND_ExecuteUltimate( iAttacker )
	}
	
	// XP assists
	g_iDamageDealt[iAttacker][iVictim] += iDamage;
	
	// Check for skills which will triggered on damage events (and items too)
	SKILL_Damage( iAttacker, iVictim, iDamage, iBodyPart, iWeapon );
	
	// Check if we have to draw a sprite for wcrepair and wcmend
	if ( get_pcvar_num( CVAR_uwc3ng_icons ) == 1 )
		UWC3NG_DrawSprite( iVictim );
	
	return;
}

// This will be called after the player spawned
public on_PlayerSpawn( id )
{
	if ( !UWC3NG_Check() || !is_user_alive( id ) )
	{
		return HAM_IGNORED;
	}
	
	// Ignor spawn
	if ( p_data_b[id][IGNOR_SPAWN] )
	{
		return HAM_IGNORED;
	}
	
	// CSDM is maybe activated. We call on every spawn the "new round" function
	on_CSDMRespawn( id );
	
	// If the user is a bot they should have a chance to buy an item
	if ( is_user_bot( id ) && !get_pcvar_num( CVAR_uwc3ng_ignore_bots ) )
	{
		new Float:fBotChance = get_pcvar_float( CVAR_uwc3ng_bot_buy_item );

		if ( fBotChance > 0 && random_float( 0.0, 1.0 ) <= fBotChance )
		{
			// First, which shopmenu does the bot want?
			static iRandomChance;
			iRandomChance = random_num( 0, 1 );
			
			// Shopmenu
			if ( iRandomChance == 0 )
			{
				_ITEM_HandleShopMenu( id, random_num( 0, 8 ) );
			}
			
			// Shopmenu 2
			else if ( iRandomChance == 1 )
			{
				// Set the page
				p_data[id][SHOPMENU_SIDE] = 1;
				
				// Call the menu
				_ITEM_HandleShopMenu( id, random_num( 0, 8 ) );
			}
		}
	}
		
	// Set up the skills from this player
	SKILL_Update( id );
	
	// Equipment reincarnation should only call on spawn. So we do it not in SKILL_Update
	if ( !p_data_b[id][PLAYER_SPAWNGOD] )
		SKILL_CS_Reincarnation( id );
	
	// Serpent Ward inital. Only called once for every spawn
	SKILL_SerpentWard( id );
	
	// Initial. Only called once for every spawn
	SKILL_Blink( id );
	
	// Phoenix Initial. Only called once for every spawn
	SKILL_PhoenixCheck( id );
	
	// Mend Wounds & Repair Armor & Dispell Hex reset
	SHARED_MendRepairDispell_Reset( id );
	
	// Devotion Aura
	SKILL_DevotionAura( id );
	
	// Remove the wards
	if ( task_exists( TASK_LIGHT + id ) )
		remove_task( TASK_LIGHT + id );
	
	// We reset this data now because we need it only for the reincarnation
	p_data_b[id][PB_DIEDLASTROUND] = false;
	
	// He is nomore planting
	p_data_b[id][PB_ISPLANTING] = false;
	
	// Check if we have to draw a sprite for wcrepair and wcmend
	if ( get_pcvar_num( CVAR_uwc3ng_icons ) == 1 )
		UWC3NG_DrawSprite( id );
	
	// Reset Assist XP handler!
	for ( new i = 0; i < MAX_PLAYERS; i++ )
	{
		g_iDamageDealt[id][i] = 0;
	}
	
	if ( !is_user_bot( id ) )
	{
		// Show the player the skills menu if he has available skillpoints
		if ( p_data[id][AVAILABLE_SKILL] > 0 )
		{
			SKILL_ShowMenu( id, 0 );
		}
	
		// Show the player the attribs menu if he has available attrib points
		else if ( p_data[id][AVAILABLE_ATTRIBS] > 0 )
		{
			ATTRIB_ShowSelectMenu( id );
		}
	
		// Show the player the resist menu if he has available resist points
		else if ( p_data[id][AVAILABLE_RESISTS] > 0 )
		{
			RESIST_ShowSelectMenu( id );
		}
	}
	
	return HAM_IGNORED;
}

// This will be called after the bomb exploded, all hostage rescued, vip escaped or a complete team is dead
public on_EndRound()
{
	// Set the global variable to true
	g_EndRound = true;
	
	// Remove some tasks
	static i, players[32], numofplayers;
	get_players( players, numofplayers )
	
	// Loop through all alive players
	for ( i = 0; i < numofplayers; ++i )
	{
		if ( is_user_alive( players[i] ) )
		{
			// Leather skin task
			if ( task_exists( players[i] + TASK_SKIN ) )
			{
				remove_task( players[i] + TASK_SKIN );
			}
			
			// Healing Wave task
			if ( task_exists( players[i] + TASK_WAVE ) )
			{
				remove_task( players[i] + TASK_WAVE );
			}
			
			// Burning Task
			if ( task_exists( players[i] + TASK_BURNING ) )
			{
				SKILL_FlameStrike_Remove( players[i] );
			}
			
			// Carrion Beetles disease
			if ( p_data_b[players[i]][PB_CARRIONDOT] )
				p_data_b[players[i]][PB_CARRIONDOT] = false;
			
			// Player is nomore hexed
			if ( p_data_b[players[i]][PB_HEXED] )
			{
				p_data_b[players[i]][PB_HEXED] = false;
			
				// Change the skin
				SHARED_ChangeSkin( players[i], 0 ); // 0 = reset skin
			}
			
			// Shadow Strike poison
			if ( p_data_b[players[i]][PB_SHADOWDOT] )
				p_data_b[players[i]][PB_SHADOWDOT] = false;
		}
	}
	
	// Is save end round enabled?
	if ( get_pcvar_num( CVAR_uwc3ng_save_end_round ) )
	{
		// Save all XP's
		Player_SaveAll();
	}
}

// CSDM is working, then we have to do all things that we had to do on new round
public on_CSDMRespawn( id )
{
	// CSDM is working. Do other things
	if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 )
	{
		if ( !SHARED_ValidPlayer( id ) )
		{
			return;
		}
		
		// Reset all skills if the player want that
		SKILL_ResetAllNow( id );
			
		// Reset all attribs if the player want that
		ATTRIB_ResetAllNow( id );
		
		// Reset all resists if the player want that
		RESIST_ResetAllNow( id );
		
		// The player can now be respawned from other ultimates / item
		p_data_b[id][PB_RESPAWNED] = false;
		p_data_b[id][PB_RESPAWNED_TWICE] = false;
		
		// Suicide Bomber
		p_data_b[id][PB_SUICIDEATTEMPT] = false;
		
		// Defuser or plant the bomb
		p_data_b[id][PLAYER_DEFORPLAN] = false;
		
		// Skin switched
		p_data_b[id][PB_SKINSWITCHED] = false;
		
		// Should we mole b/c of an item?
		SHARED_MoleCheck( id, true ); // Only check item!
		
		// Skill check
		SHARED_MoleCheck( id, false ); 
		
		// Set the global variable to false
		g_EndRound = false;
		
		// Give him some extra money
		SHARED_SetUserMoney( id, 300 );
	}
	
	return;
}

// This will be called bevore all player spawns and when the freezetime begins to count
public on_NewRound()
{
	// Set the global variable to false
	g_EndRound = false;
	
	// The round is over. The freezetime will begin now
	g_FreezeTimeOver = false;
	
	// We should reset some things
	static i, players[32], numofplayers;
	get_players( players, numofplayers );
	for ( i = 0; i < numofplayers; ++i )
	{
		// Reset all skills if the player want that
		SKILL_ResetAllNow( players[i] );
			
		// Reset all attribs if the player want that
		ATTRIB_ResetAllNow( players[i] );
		
		// Reset all resists if the player want that
		RESIST_ResetAllNow( players[i] );
		
		// Copy the weapons to another array so we don't lose our currently weapons
		SHARED_CopySavedWeapons( players[i] );
		
		// The round is over. The player can use the dropped skill
		p_data_b[players[i]][USED_DROP] = false;
		
		// The player can now be respawned from other ultimates / item
		p_data_b[players[i]][PB_RESPAWNED] = false;
		p_data_b[players[i]][PB_RESPAWNED_TWICE] = false;
		
		// Suicide Bomber
		p_data_b[players[i]][PB_USED_SUICIDE] = false;
		p_data_b[players[i]][PB_SUICIDEATTEMPT] = false;
		
		// Should we mole b/c of an item?
		SHARED_MoleCheck( players[i], true ); // Only check item!
		
		// Skill check
		SHARED_MoleCheck( players[i], false );
		
		// He can use gate
		p_data_b[players[i]][USED_GATE] = false;
		
		// He can use gate twice
		p_data_b[players[i]][USED_GATE_TWICE] = false
	}
	
	// The player's should later get xps for starting to defuse or planting the bomb
	for ( i = 0; i < sizeof( p_data_b ); ++i )
	{
		p_data_b[i][PLAYER_DEFORPLAN] = false;
	}
}

// This will be called after the Terrorists win the round
public on_TerroristWin()
{
	XP_GiveTeam( TEAM_T );
}

// This will be called after the Counter Terrorist win the round
public on_CTWin()
{
	XP_GiveTeam( TEAM_CT );
}

// Called if someone starts to planting the bomb
public bomb_planting( id )
{
	if ( !is_user_connected( id ) || !UWC3NG_Check() )
	{
		return;
	}
	
	// The player starts to planting the bomb - yes
	p_data_b[id][PB_ISPLANTING] = true;
	
	// We call this function only once for every player 
	if ( p_data_b[id][PLAYER_DEFORPLAN] )
	{
		return;
	}
	
	// Set it to true
	p_data_b[id][PLAYER_DEFORPLAN] = true;
	
	// He has now another role
	g_iPlayerRole[id] = PLR_BOMB_PLANTER;
	
	// Calculate the xps which the player should get
	new iXPs = XP_Calc( id, PLANTING_BOMB );
		
	// Give him the xps
	new iBonusXP = XP_Give( id, iXPs );

	// All is okay, write him a message
	if ( iBonusXP != 0 )
	{
		client_print( id, print_chat, "%s You have been awarded %d XP for starting to plant the bomb", MOD_NAME, iBonusXP );
	}
	
	return;
}

// Called if someone starts to defusing the bomb
public bomb_defusing( id )
{
	if ( !is_user_connected( id ) || !UWC3NG_Check() )
	{
		return;
	}
	
	// We call this function only once for every player 
	if ( p_data_b[id][PLAYER_DEFORPLAN] )
	{
		return;
	}
	
	// Set it to true
	p_data_b[id][PLAYER_DEFORPLAN] = true;
	
	// He has now another role
	g_iPlayerRole[id] = PLR_BOMB_DEFUSER;
	
	// Calculate the xps which the player should get
	new iXPs = XP_Calc( id, DEFUSING_BOMB );
		
	// Give him the xps
	new iBonusXP = XP_Give( id, iXPs );

	// All is okay, write him a message
	if ( iBonusXP != 0 )
	{
		client_print( id, print_chat, "%s You have been awarded %d XP for starting to defuse the bomb", MOD_NAME, iBonusXP );
	}
	
	return;
}

// Someone planted the bomb
public bomb_planted( id )
{
	if ( !is_user_connected( id ) || !UWC3NG_Check() )
	{
		return;
	}
	
	// No more Bomb planter
	g_iPlayerRole[id] = PLR_NONE;
	
	// Stop planting
	p_data_b[id][PB_ISPLANTING] = false;
	
	// Calculate the xps which the player should get
	new iXPs = XP_Calc( id, PLANT_BOMB );
	
	// Give him the xps
	new iBonusXP = XP_Give( id, iXPs );

	// All is okay, write him a message
	if ( iBonusXP != 0 )
	{	
		client_print ( id, print_chat, "%s You have been awarded %d XP for planting the bomb", MOD_NAME, iBonusXP );
	}
	
	// Player's in range should get xps
	static i, players[32], numofplayers;
	get_players( players, numofplayers );
	
	// Loop through all players
	for ( i = 0; i < numofplayers; ++i )
	{
		// The planter shouldn't get xps twice
		if ( players[i] != id )
		{
			// The player is in the terrorist team
			if ( get_user_team( players[i] ) == TEAM_T )
			{
				// The player is alive
				if ( is_user_alive( players[i] ) )
				{
					// The player is in the range
					if ( PLAYER_is_in_range( id, players[i], OBJ_RADIUS ) )
					{
						// Calculate the xps which the player should get
						iXPs = XP_Calc( players[i], PLANT_BOMB/2 );
					
						// Give him the xps
						iBonusXP = XP_Give( players[i], iXPs );
					
						// All is okay, write him a message
						if ( iBonusXP != 0 )
						{
							client_print( players[i], print_chat, "%s You have been awarded %d XP for supporting the bomb plant", MOD_NAME, iBonusXP );
						}
					} // end range check
				} // end alive check
			} // end team check
		} // end player check
	} // end loop
	
	return;
}

// Someone has defused the bomb
public bomb_defused( id )
{
	if ( !is_user_connected( id ) || !UWC3NG_Check() )
	{
		return;
	}
	
	// No more Bomb defuser
	g_iPlayerRole[id] = PLR_NONE;
	
	// Calculate the xps which the player should get
	new iXPs = XP_Calc( id, DEFUSED_BOMB );
	
	// Give him the xps
	new iBonusXP = XP_Give( id, iXPs );

	// All is okay, write him a message
	if ( iBonusXP != 0 )
	{	
		client_print ( id, print_chat, "%s You have been awarded %d XP for defusing the bomb", MOD_NAME, iBonusXP );
	}
	
	// Player's in range should get xps
	static i, players[32], numofplayers;
	get_players( players, numofplayers );
	
	// Loop through all players
	for ( i = 0; i < numofplayers; ++i )
	{
		// The defuser shouldn't get xps twice
		if ( players[i] != id )
		{
			// The player is in the terrorist team
			if ( get_user_team( players[i] ) == TEAM_CT )
			{
				// The player is alive
				if ( is_user_alive( players[i] ) )
				{
					// The player is in the range
					if ( PLAYER_is_in_range( id, players[i], OBJ_RADIUS ) )
					{
						// Calculate the xps which the player should get
						iXPs = XP_Calc( players[i], DEFUSED_BOMB/2 );
					
						// Give him the xps
						iBonusXP = XP_Give( players[i], iXPs );
					
						// All is okay, write him a message
						if ( iBonusXP != 0 )
						{
							client_print( players[i], print_chat, "%s You have been awarded %d XP for supporting the bomb defusal", MOD_NAME, iBonusXP );
						}
					} // end range check
				} // end alive check
			} // end team check
		} // end player check
	} // end loop
	
	return;
}

// This will be called after the freeze time is over
public on_FreezeTimeComplete()
{
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	// The freezetime is over. All should it know
	g_FreezeTimeOver = true;
	
	// Hudmessage (items + ult)
	if ( !task_exists( TASK_HUD ) )
	{
		Create_Hud();
	}
	
	// Set a task for the ultimate delay at round start
	if ( task_exists( TASK_ULT_START_DELAY ) )
	{
		// Reset the task if exists
		remove_task( TASK_ULT_START_DELAY );
	}
	set_task( float( get_pcvar_num( CVAR_uwc3ng_ult_round_start_cd ) ), "ULTIMATE_RemoveStartDelay", TASK_ULT_START_DELAY );
	
		
	
	// We loop through every player
	static i, players[32], numofplayers;
	get_players( players, numofplayers );
	
	for ( i = 0; i < numofplayers; ++i )
	{
		if ( is_user_alive( players[i] ) )
		{
			// Show him the status text
			PLAYER_ShowStatusText( players[i] );
			
			// Set their speed
			set_task( 0.5, "SKILL_UnholyAura", players[i] + TASK_SPEED );
		}
	}
}

// Called on certain player events (from wc3ft)
public on_PlayerAction()
{
	if ( !UWC3NG_Check() )
	{
		return;
	}

	new sArg[64], sAction[64], sName[64];
	new iUserId, id;

	read_logargv( 0,sArg,64 );
	read_logargv( 2,sAction,64 );
	parse_loguser( sArg,sName,64,iUserId );
	id = find_player( "k", iUserId );

	if ( id == 0 )
	{
		return;
	}

	// If the round is over do not award XP!
	if ( g_EndRound )
	{
		return;
	}

	// Spawned with the bomb 
	if ( equal( sAction, "Spawned_With_The_Bomb" ) )
	{
		// He is now the bomb carrier
		g_iPlayerRole[id] = PLR_BOMB_CARRIER;
		
		// Calculate the xps which the player should get
		new iXPs = XP_Calc( id, SPAWN_BOMB );
		
		// Give him the xps
		new iBonusXP = XP_Give( id, iXPs );

		// All is okay, write him a message
		if ( iBonusXP != 0 )
		{
			client_print( id, print_chat, "%s You have been awarded %d XP for spawning with the bomb", MOD_NAME, iBonusXP );
		}
	}
	// Dropped bomb 
	else if ( equal( sAction, "Dropped_The_Bomb" ) )
	{
		// He is now none
		g_iPlayerRole[id] = PLR_NONE;
		
		// Calculate the xps which the player should get
		new iXPs = XP_Calc( id, SPAWN_BOMB );
		
		// Take him the xps
		new iBonusXP = XP_Give( id, -1 * iXPs );

		// All is okay, write him a message
		if ( iBonusXP != 0 )
		{
			client_print( id, print_chat, "%s You have lost %d XP for dropping the bomb", MOD_NAME, -1 * iBonusXP );
		}
	}
	// Picked up bomb 
	else if ( equal( sAction, "Got_The_Bomb" ) )
	{ 
		// He picked up the bomb, he is now bomb carrier
		g_iPlayerRole[id] = PLR_BOMB_CARRIER;
		
		// Calculate the xps which the player should get
		new iXPs = XP_Calc( id, BOMB_PICKUP );
		
		// Give him the xps
		new iBonusXP = XP_Give( id, iXPs );

		// All is okay, write him a message
		if ( iBonusXP != 0 )
		{
			client_print( id, print_chat, "%s You have been awarded %d XP for picking up the bomb", MOD_NAME, iBonusXP );
		}
	}
	// Hostage touched 
	else if ( equal( sAction, "Touched_A_Hostage" ) )
	{
		// He touched a hostage, he is now Hostage rescuer
		g_iPlayerRole[id] = PLR_HOSTAGE_RESCUER;
		
		// Calculate the xps which the player should get
		new iXPs = XP_Calc( id, TOUCH_HOSTAGE );
		
		// Give him the xps
		new iBonusXP = XP_Give( id, iXPs );

		// All is okay, write him a message
		if ( iBonusXP != 0 )
		{
			client_print( id, print_chat, "%s You have been awarded %d XP for starting to rescue a hostage", MOD_NAME, iBonusXP );
		}
	}
	// Hostage rescued 
	else if ( equal( sAction, "Rescued_A_Hostage" ) )
	{
		// Calculate the xps which the player should get
		new iXPs = XP_Calc( id, RESCUE_HOSTAGE );
		
		// Give him the xps
		new iBonusXP = XP_Give( id, iXPs );

		// All is okay, write him a message
		if ( iBonusXP != 0 )
		{
			client_print( id, print_chat, "%s You have been awarded %d XP for rescuing a hostage", MOD_NAME, iBonusXP );
		}
	}
	// Hostage killed 
	else if ( equal( sAction, "Killed_A_Hostage" ) )
	{
		// Calculate the xps which the player should get
		new iXPs = XP_Calc( id, KILL_HOSTAGE );
		
		// Take him the xps
		new iBonusXP = XP_Give( id, -1 * iXPs );

		// All is okay, write him a message
		if ( iBonusXP != 0 )
		{
			client_print( id, print_chat, "%s You have lost %d XP for killing a hostage", MOD_NAME, -1 * iBonusXP );
		}
	}
	// VIP spawn 
	else if ( equal( sAction, "Became_VIP" ) )
	{ 
		// He is now a vip
		g_iPlayerRole[id] = PLR_VIP;
		
		// Calculate the xps which the player should get
		new iXPs = XP_Calc( id, SPAWN_VIP );
		
		// Give him the xps
		new iBonusXP = XP_Give( id, iXPs );

		// All is okay, write him a message
		if ( iBonusXP != 0 )
		{
			client_print( id, print_chat, "%s You have gained %d XP for spawning as the VIP", MOD_NAME, iBonusXP );
		}
	}
	// VIP escaped 
	else if ( equal( sAction, "Escaped_As_VIP" ) )
	{
		// Calculate the xps which the player should get
		new iXPs = XP_Calc( id, ESCAPE_VIP );
		
		// Give him the xps
		new iBonusXP = XP_Give( id, iXPs );

		// All is okay, write him a message
		if ( iBonusXP != 0 )
		{
			client_print( id, print_chat, "%s You have gained %d XP for escaping as the VIP", MOD_NAME, iBonusXP );
		}
	}
	// defuse with kit
	else if ( equal( sAction, "Begin_Bomb_Defuse_With_Kit" ) )
	{
		// Calculate the xps which the player should get
		new iXPs = XP_Calc( id, DEFUSING_WITH_KIT );
		
		// Give him the xps
		new iBonusXP = XP_Give( id, iXPs );

		// All is okay, write him a message
		if ( iBonusXP != 0 )
		{
			client_print( id, print_chat, "%s You have gained %d XP for defusing the bomb with kit", MOD_NAME, iBonusXP );
		}
	}
}

// Someone picked up a weapon
public on_WeapPickup( id )
{ 

	if ( !UWC3NG_Check() )
	{
		return;
	}

	SHARED_Save_Weapons( id );

	return;
}

// Current weapon
public on_CurWeapon( id )
{
	if ( !UWC3NG_Check() || !is_user_alive( id ) )
	{
		return;
	}

	new iCurWeapon = read_data( 2 );

	// Then a weapon strip was triggered - we don't want to save now do we?
	if ( iCurWeapon == 0 && read_data( 1 ) == 0 && read_data( 3 ) == 0 )
	{
		return;
	}
	
	// Set the speed
	SKILL_UnholyAura( id )
	
	// Record the last time a shot was fired
	fLastShotFired[id] = halflife_time();

	// Save the user's weapons
	SHARED_Save_Weapons( id );
	
	// Invisibility
	SKILL_Invisible( id )
	
	return;
}

// Someone dropped a weapon
public on_Drop( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}

	// Save the user's weapons
	SHARED_Save_Weapons( id );

	return;
}

// Function called when a user's armor changes
public on_Battery( id )
{	
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	if ( !bIgnoreArmorSet[id] )
	{
		// Store the previous value (we do this b/c the Battery event is triggered (and set to 0) before weapon reincarnation)
		p_data[id][P_LASTARMOR] = p_data[id][P_ARMORONDEATH];

		p_data[id][P_ARMORONDEATH] = read_data( 1 );
	}

	return;
}

public on_ArmorType(id)
{

	if ( !UWC3NG_Check() || !g_FreezeTimeOver  )
	{
		return;
	}
	
	// Save user's armor
	p_data[id][P_ARMORONDEATH] = cs_get_user_armor( id, g_ArmorType[id] );
	
	return;
}

// Hook the deathmessage
public on_DeathMsg()
{

	if ( !UWC3NG_Check() )
	{
		return;
	}

	new iAttacker	= read_data( 1 );
	new iVictim	= read_data( 2 );
	new iHeadshot	= read_data( 3 );
	new iWeaponID;
	get_user_attacker( iVictim, iWeaponID );

	on_Death( iVictim, iAttacker, iWeaponID, iHeadshot );
	
	return;
}

// Called on a deathmsg or a death
public on_Death( iVictim, iAttacker, iWeaponID, iHeadshot )
{
	// For some reason the damage passed by explosions is not actually correct (perhaps armor adjustments weren't done yet), so lets check
	if ( is_user_alive( iVictim ) && iWeaponID == CSW_C4 )
	{
		return;
	}
		
	// Check for NightVision
	p_data_b[iVictim][PB_NIGHTVISION] = (cs_get_user_nvg( iVictim )) ? true : false;

	// Check if a user had a shield on death
	p_data_b[iVictim][PB_SHIELD] = (cs_get_user_shield( iVictim )) ? true : false;

	// See if the user has a defuse kit
	p_data_b[iVictim][PB_DEFUSE] = (cs_get_user_defuse( iVictim )) ? true : false;

	// Save the number of flash grenades the user has
	p_data[iVictim][P_FLASHCOUNT] = cs_get_user_bpammo( iVictim, CSW_FLASHBANG );
	
	// Check to see if weapons were silenced on death
	new iWeaponUSPEnt = find_ent_by_owner( -1, "weapon_usp", iVictim );
	new iWeaponM4A1Ent = find_ent_by_owner( -1, "weapon_m4a1", iVictim );
		
	if ( is_valid_ent( iWeaponUSPEnt ) )
	{
		p_data_b[iVictim][PB_USP_SILENCED] = cs_get_weapon_silen( iWeaponUSPEnt ) ? true : false;
	}

	if ( is_valid_ent( iWeaponM4A1Ent ) )
	{
		p_data_b[iVictim][PB_M4A1_SILENCED] = cs_get_weapon_silen( iWeaponM4A1Ent ) ? true : false;
	}

	// Check to see if weapons were in burst mode on death
	new iWeaponGlock18Ent = find_ent_by_owner( -1, "weapon_glock18", iVictim );
	new iWeaponFamasEnt = find_ent_by_owner( -1, "weapon_famas", iVictim );

	if ( is_valid_ent( iWeaponGlock18Ent ) )
	{
		p_data_b[iVictim][PB_GLOCK_BURST] = cs_get_weapon_burst( iWeaponGlock18Ent ) ? true : false;
	}

	if ( is_valid_ent( iWeaponFamasEnt ) )
	{
		p_data_b[iVictim][PB_FAMAS_BURST] = cs_get_weapon_burst( iWeaponFamasEnt ) ? true : false;
	}
	
	UWC3NG_Death( iAttacker, iVictim, iHeadshot, iWeaponID );
	
	return;
}

// From war3x
public on_Zoom( id )
{

	if ( !UWC3NG_Check() )
	{
		return;
	}

	if ( read_data( 1 ) < 90 )
	{
		g_bPlayerZoomed[id] = true;
	}

	else
	{
		g_bPlayerZoomed[id] = false;
	}

	// We will need to change the user's speed on zooming
	SKILL_UnholyAura( id );

	return;
}

// Triggered if someone throw a grenade (from wc3ft)
public grenade_throw( index, greindex, wId )
{
	
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	// If user isn't alive do nothing!
	if ( !is_user_alive( index ) )
	{
		return;
	}
	
	// User has gloves!  Do we need to give them another grenade?
	if ( ITEM_Has( index, ITEM_GLOVES ) > ITEM_NONE )
	{
		ITEM_Glove_Begin( index );
	}

	new iSkillLevel = p_data_skill[index][SKILL_CRITICALGRENADE];
	
	// Make sure the user has the skill and we actually have a grenade index
	if ( greindex && iSkillLevel > 0 )
	{
		// Then draw it!
		if ( wId == CSW_HEGRENADE )
		{	
			// The player has skilled napalm grenades. The trail is now red
			if ( p_data_skill[index][SKILL_NAPALMGRENADE] )
			{
				Create_TE_BEAMFOLLOW( greindex, g_iSprites[SPRITE_TRAIL], 20, 9, 255, 0, 0, 196 );
			}
			
			// He has no napalm grenades
			else
			{
				Create_TE_BEAMFOLLOW( greindex, g_iSprites[SPRITE_TRAIL], 20, 9, 0, 255, 0, 196 );
			}
		}
	}
	
	return;
}

// Called when a user looks somewhere
public TRIGGER_TraceLine( Float:v1[3], Float:v2[3], noMonsters, pentToSkip )
{
	if ( !UWC3NG_Check() )
	{
		return FMRES_IGNORED;
	}

	new iAttacker = pentToSkip;
	new iVictim = get_tr(TR_pHit);
	new iHitZone = (1 << get_tr(TR_iHitgroup));
	
	// Make sure we have a valid victim
	if ( is_user_alive( iVictim ) )
	{
		// We need to have a valid player!
		if ( is_user_alive( iAttacker ) )
		{
			// This is a check for ultimates that need to "search" for a target
			if ( p_data_b[iAttacker][PB_ISSEARCHING] )
			{

				// Now lets make sure the person he's looking at is in view and isn't on the same team
				if ( get_user_team( iAttacker ) != get_user_team( iVictim ) ) 
				{
					
					// Check to see if the user should block this ultimate!
					if ( ULTIMATE_IsImmun( iVictim ) || p_data_b[iVictim][PLAYER_SPAWNGOD] || p_data_b[iVictim][PB_GOD] )
					{
						ULTIMATE_Blocked( iAttacker );
					}

					// Then the user's ult should work!
					else
					{
						// Well we do have a target so lets execute the user's ultimate!!
						if ( p_data_skill[iAttacker][SKILL_CHAINLIGHTNING] > 0 )
						{
							if ( p_data[iAttacker][P_ULT_SEARCH] == SKILL_CHAINLIGHTNING )
							{
								SKILL_ChainLightningInit( iAttacker, iVictim, iHitZone );
							}
						}
						
						// Entangle Roots
						if ( p_data_skill[iAttacker][SKILL_ENTANGLEROOTS] > 0 )
						{
							if ( p_data[iAttacker][P_ULT_SEARCH] == SKILL_ENTANGLEROOTS )
							{
								SKILL_Entangle( iAttacker, iVictim );
							}
						}
						
						// Flame Strike
						if ( p_data_skill[iAttacker][SKILL_FLAMESTRIKE] > 0 )
						{
							if ( p_data[iAttacker][P_ULT_SEARCH] == SKILL_FLAMESTRIKE )
							{
								SKILL_FlameStrikeInit( iAttacker, iVictim );
							}
						}
					}

					// No longer searching since we found a target
					p_data_b[iAttacker][PB_ISSEARCHING] = false;
					
					// We're not searching!
					p_data[iAttacker][P_ULT_SEARCH] = 0;

					// Set up the user's ultimate delay
					ULTIMATE_ResetCooldown( iAttacker, get_pcvar_num( CVAR_uwc3ng_ult_cooldown ) );
				}
			}
		}
	}
	
	return FMRES_IGNORED;
}

// Called when a player attacks an enemy
public ham_TraceAttack( iVictim, iAttacker, Float:damage, Float:direction[3], tracehandle, damagebits )
{
	if ( !UWC3NG_Check() )
	{
		return HAM_IGNORED;
	}
	
	// Make sure we have a valid victim
	if ( is_user_alive( iVictim ) && SHARED_ValidPlayer( iVictim ) )
	{
		// We need to have a valid player!
		if ( is_user_alive( iAttacker ) && SHARED_ValidPlayer( iAttacker ) )
		{
			// Get the hitzone
			new hitgroup = get_tr2( tracehandle, TR_iHitgroup );
				
			// This is a nice check for Helm of Excellence
			if ( ITEM_Has( iVictim, ITEM_HELM ) > ITEM_NONE )
			{
				// Friendly fire is off! - This means same team shouldn't remove a charge!
				if ( !get_pcvar_num( CVAR_mp_friendlyfire ) )
				{
					if ( get_user_team( iAttacker ) == get_user_team( iVictim ) )
					{
						return HAM_IGNORED;
					}
				}

				// If its a headshot then we want to block it
				if ( hitgroup == HIT_HEAD )
				{
					// Do the check to see if we should flash the screen orange
					new Float:fTime = halflife_time();
					new Float:fDifference = fTime - fLastShotFired[iAttacker];

					if ( fDifference < 0.1 && fDifference > 0.0 )
					{
						Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 0, 0, 255, 150 );

						// Lets remove a charge from the helm!
						ITEM_RemoveCharge( iVictim, ITEM_HELM );
					}
									
					return HAM_SUPERCEDE;
				}
			}

			// Check to see if this user has night elf's evasion
			if ( p_data_skill[iVictim][SKILL_EVASION] > 0 )
			{
				if ( SHARED_ValidPlayer( iAttacker ) )
				{
					// Friendly fire is off! - This means we shouldn't evade since no damage will be done!
					if ( !get_pcvar_num( CVAR_mp_friendlyfire ) )
					{
						if ( get_user_team( iAttacker ) == get_user_team( iVictim ) )
						{
							return HAM_IGNORED;
						}
					}
					
					// Then we should evade this shot!
					if ( SKILL_Evasion( iVictim, hitgroup ) )
					{
						UWC3NG_StatusText( iVictim, TXT_SKILL, "You have evaded a shot!" );

						return HAM_SUPERCEDE;
					}
				}
			}
			
			// Mole protectant
			if ( p_data_b[iAttacker][PB_SKINSWITCHED] && ITEM_Has( iVictim, ITEM_PROTECTANT ) > ITEM_NONE )
			{	
				// Friendly fire is off! - This means we shouldn't evade since no damage will be done!
				if ( !get_pcvar_num( CVAR_mp_friendlyfire ) )
				{
					if ( get_user_team( iAttacker ) == get_user_team( iVictim ) )
					{
						return HAM_IGNORED;
					}
				}
					
				client_print( iVictim, print_chat, "%s A shot by a mole was deflected", MOD_NAME );
				return HAM_SUPERCEDE;
			}

			// Check for Big Bad Voodoo's ultimate!
			if ( p_data_b[iVictim][PB_GOD] )
			{
				new bool:bBlockDamage = true;
			
				// Do we allow this person to be attacked by this player?
				if ( p_data_b[iAttacker][PB_BIGBAD_ATTACKER] )
				{
					bBlockDamage = false;
				}
				
				// Check to see if immunity is available for the attacker
				else if ( p_data_b[iAttacker][PB_BLINK] )
				{
					// Display message about user's ultimate being blocked!
					ULTIMATE_Blocked( iVictim );

					// This user can attack someone with big bad voodoo!
					p_data_b[iAttacker][PB_BIGBAD_ATTACKER] = true;

					// The player should get damage
					bBlockDamage = false;
					
					// Reset the attacker dmg
					set_task( get_pcvar_float( CVAR_uwc3ng_ult_cooldown ), "_SKILL_ResetBigBadAttacker", TASK_BIGBADATTACKER + iAttacker );
				}
				
				// Block the damage!
				if ( bBlockDamage )
				{
					return HAM_SUPERCEDE;
				}
			}
		}
	}
	
	return HAM_IGNORED;
}
	
	

// Player prethink (for speed etcetc)
public fwd_playerprethink( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}

	if ( is_user_alive( id ) )
	{
		// This is used so we can't hear the undead's footsteps at level 3
		if ( ( p_data_skill[id][SKILL_UNHOLYAURA] || p_data_attrib[id][ATTRIB_AGILITY] ) && !p_data_b[id][PB_STUNNED] && !p_data_b[id][PB_SLOWED] )
		{
			new Float:vel[3];
			entity_get_vector( id, EV_VEC_velocity, vel );
			
			// When this is true, the user is walking...  lets hope :P
			if ( vector_length( vel ) < 225.0 )
			{
				entity_set_int( id, EV_INT_flTimeStepSound, 999 );
			}

			// Otherwise if we just set it on the previous tick we need to set it back
			else if ( entity_get_int(id, EV_INT_flTimeStepSound) > 500 )
			{
				entity_set_int( id, EV_INT_flTimeStepSound, 200 );
			}
		}
		
		// Amulet of the Cat
		if ( ITEM_Has( id, ITEM_AMULET ) > ITEM_NONE )
		{
			entity_set_int( id, EV_INT_flTimeStepSound, 999 );
		}
	}
		
	// User is dead
	else
	{

		// Check to see if spectated player has changed
		new iTarget = entity_get_int( id, EV_INT_iuser2 );
			
		new Float:fTime = halflife_time();

		// Then we are looking at a new player or the last hud message has disappeared
		if ( g_iSpectatingID[id] != iTarget || g_fLastSpecDisplay[id] <= fTime )
		{
			// We have a valid target!!
			if ( SHARED_ValidPlayer( iTarget ) && iTarget != id )
			{
				UWC3NG_ShowSpecInfo( id, iTarget );

				g_fLastSpecDisplay[id] = fTime + SPEC_HOLDTIME;
			}

			g_iSpectatingID[id] = iTarget;
		}
	}
	
	return;
}

// All we want to check is if the user was killed by the bomb
public client_death( iAttacker, iVictim, iWeapon, hitplace, TK )
{
	// Check out who the inflictor was
	new iInflictor = entity_get_edict( iVictim, EV_ENT_dmg_inflictor );

	// Check to see if the death was from the bomb
	if ( !SHARED_ValidPlayer( iInflictor ) && iWeapon != CSW_HEGRENADE && iInflictor )
	{
		if ( is_valid_ent ( iInflictor ) )
		{
			new szClassName[64];
			entity_get_string( iInflictor, EV_SZ_classname, szClassName, 63 );

			// Check the classname of our inflictor
			if ( equali( szClassName, "grenade" ) || equali( szClassName, "env_explosion" ) )
			{
				iWeapon = CSW_C4;
				iAttacker = 0;

				// Well if this isn't set, shouldn't it be?
				if ( !p_data_b[iVictim][PB_DIEDLASTROUND] )
				{
					on_Death( iVictim, iAttacker, iWeapon, 0 );
				}
			}
		}
	}
	
	return;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
