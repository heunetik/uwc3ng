// Uwc3ng Items functions

// Item Setup Functions
public ITEM_Init()
{
	ITEM_COST[ITEM_ANKH]	    = 1500;			// Ankh of Reincarnation
	ITEM_COST[ITEM_BOOTS]        = 2500;			// Boots of Speed
	ITEM_COST[ITEM_CLAWS]	    = 1000;			// Claws of Attack
	ITEM_COST[ITEM_CLOAK]	    = 800;			// Cloak of Shadows
	ITEM_COST[ITEM_MASK]	    = 2000;			// Mask of Death
	ITEM_COST[ITEM_NECKLACE]    = 800;			// Necklace of Immunity
	ITEM_COST[ITEM_FROST]	    = 2000;			// Orb of Frost
	ITEM_COST[ITEM_HEALTH]	    = 1000;			// Periapt of Health
	ITEM_COST[ITEM_TOME]	    = 4000;			// Tome of Experience
	ITEM_COST[ITEM_SCROLL]	    = 6000;			// Scroll of Respawning
	ITEM_COST[ITEM_PROTECTANT]  = 1500;			// Mole Protectant
	ITEM_COST[ITEM_HELM]	    = 3000;			// Helm of Excellence
	ITEM_COST[ITEM_AMULET]	    = 1500;			// Amulet of the Cat
	ITEM_COST[ITEM_SOCK]	    = 1500;			// Sock of the Feather
	ITEM_COST[ITEM_GLOVES]	    = 1750;			// Flaming Gloves of Warmth
	ITEM_COST[ITEM_RING]	    = 1000;			// Ring of Regeneration + 1
	ITEM_COST[ITEM_CHAMELEON]   = 4500;			// Chameleon
	ITEM_COST[ITEM_MOLE]	    = 16000;			// Mole

	// Items are chargeable
	g_iFlag[ITEM_NECKLACE]		|= ITEM_CHARGEABLE;
	g_iFlag[ITEM_HELM]		|= ITEM_CHARGEABLE;
	g_iFlag[ITEM_RING]		|= ITEM_CHARGEABLE;

	// Items should be used when bought
	g_iFlag[ITEM_TOME]		|= ITEM_USEONBUY;

	// Items CAN be bought when dead
	g_iFlag[ITEM_ANKH]		|= ITEM_BUYWHENDEAD;
	g_iFlag[ITEM_SCROLL]		|= ITEM_BUYWHENDEAD;
	g_iFlag[ITEM_MOLE]		|= ITEM_BUYWHENDEAD;
	g_iFlag[ITEM_TOME]		|= ITEM_BUYWHENDEAD;

	// Items are used when the next round starts...
	g_iFlag[ITEM_ANKH]		|= ITEM_NEXTROUNDUSE;
	g_iFlag[ITEM_MOLE]		|= ITEM_NEXTROUNDUSE;
}

// Return the money from an item back
public ITEM_Cost( id, iItem )
{
	return ITEM_COST[iItem];
}


public bool:ITEM_CanBuy( id, iItem )
{
	// User doesn't have the money
	if ( cs_get_user_money( id ) < ITEM_Cost( id, iItem ) )
	{
		client_print( id, print_center, "You have insufficient funds!" );

		return false;
	}
	
	// User already owns the item and it's not a chargeable item!
	else if ( ITEM_Has( id, iItem ) > ITEM_NONE && !ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
	{
		client_print( id, print_center, "You already own that item" );

		return false;
	}
	
	// Make sure these items can be bought if the user is dead
	else if ( !is_user_alive( id ) && !ITEM_CheckFlag( iItem, ITEM_BUYWHENDEAD ) ) 
	{
		client_print( id, print_center, "You may not purchase that item when dead" );

		return false;
	}
	
	// User has necklace + blink, they don't need a necklace
	else if ( iItem == ITEM_NECKLACE && p_data_b[id][PB_BLINK] )
	{
		client_print( id, print_center, "You are already immune to ultimates through one of your skills!" );

		return false;
	}

	// User doesn't need an ankh if they're going to reincarnate
	else if ( iItem == ITEM_ANKH && p_data_skill[id][SKILL_REINCARNATION] == 3 )
	{
		client_print( id, print_center, "You will already reincarnate your weapons through one of your skills!" );

		return false;
	}
	
	// User has purchased the maximum allowed rings
	else if ( g_iTotalRings[id] >= 5 && iItem == ITEM_RING )
	{
		client_print( id, print_center, "You may not purchase more than 5 rings" );

		return false;
	}

	// User is already going to reincarnate weapons because they bought an ankh earlier (probably lost it when died)
	else if ( ( iItem == ITEM_ANKH && g_bPlayerBoughtAnkh[id] ) || ( iItem == ITEM_MOLE && g_bPlayerBoughtMole[id] ) )
	{
		client_print( id, print_center, "You already own that item" );

		return false;
	}

	return true;
}

public ITEM_Buy( id, iItem )
{

	
	// If the user can buy this item...
	if ( ITEM_CanBuy( id, iItem ) )
	{

		// User's items are full
		if ( ITEM_GetSlot( id ) == ITEM_SLOT_FULL && !ITEM_CheckFlag( iItem, ITEM_USEONBUY ) )
		{

			// We only care about items being full if this item isn't a use on buy
			if ( !ITEM_CheckFlag( iItem, ITEM_USEONBUY ) )
			{

				new bool:bShowReplaceMenu = false;

				// One time use...
				if ( !ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
				{
					bShowReplaceMenu = true;
				}

				// We also need to replace it if the item is chargeable but they don't own that item
				if ( ITEM_Has( id, iItem ) == ITEM_NONE && ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
				{
					bShowReplaceMenu = true;
				}

				if ( bShowReplaceMenu )
				{
					g_iFutureItem[id] = iItem;

					ITEM_ReplaceItem( id );

					return;
				}
			}
		}
	
		// We're clear!

		// Remove user's money
		SHARED_SetUserMoney( id, -1 * ITEM_Cost( id, iItem ) );

		ITEM_GiveItem( id, iItem );
	}

	return;
}

// Item Preset Function
public ITEM_GiveItem( id, iItem )
{

	// This item we should use instantly
	if ( ITEM_CheckFlag( iItem, ITEM_USEONBUY ) )
	{
		if ( iItem == ITEM_TOME )
		{
			ITEM_Tome( id );

			return;
		}
	}

	// They are just adding some charges
	else
	{
		// Actually set our item variable
		if ( is_user_alive( id ) )
		{
			ITEM_Equip( id, iItem );
		}

		// Display message to the user regarding the item they just purchased
		ITEM_DisplayMessage( id, iItem );

		// Give bonuses
		ITEM_GiveBonuses( id, iItem );

		// Play purchase sound
		emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_PICKUPITEM], 1.0, ATTN_NORM, 0, PITCH_NORM );
	}

	return;
}

public ITEM_DisplayMessage( id, iItem )
{
	// Display a message regarding the item they just purchased
	switch ( iItem )
	{
		case ITEM_ANKH:
		{
			client_print( id, print_chat,"%s You will reincarnate all of your weapons next round", MOD_NAME );
		}

		case ITEM_BOOTS:
		{
			client_print( id, print_chat,"%s You will now run %0.0f%% faster with each weapon", MOD_NAME, ( ( get_pcvar_float( CVAR_uwc3ng_boots ) ) * 100.00 ) );
		}

		case ITEM_CLAWS:
		{
			client_print( id, print_chat,"%s Each attack will now do an additional %d damage", MOD_NAME, get_pcvar_num( CVAR_uwc3ng_claw ) );
		}

		case ITEM_CLOAK:
		{
			new Float:fInvis = 100.0 * ( float( get_pcvar_num( CVAR_uwc3ng_cloak ) ) / 255.0 );
			client_print(id, print_chat,"%s You are now %0.0f%% visible", MOD_NAME, fInvis );
		}

		case ITEM_MASK:
		{
			new Float:fMask = ( 100.0 * get_pcvar_float( CVAR_uwc3ng_mask ) );
			client_print(id, print_chat,"%s You will now gain %0.0f%% of the damage done to enemies back as health", MOD_NAME, fMask );
		}

		case ITEM_NECKLACE:
		{
			client_print( id, print_chat, "%s You have been given %d charges, each charge will block an enemy ultimate", MOD_NAME, NECKLACE_CHARGES );
		}

		case ITEM_FROST:
		{
			new Float:fFrost = 100.0 * ( get_pcvar_float( CVAR_uwc3ng_frost ) / 260.0 );
			client_print(id, print_chat,"%s You will now slow enemies by %0.0f%% with each successful hit", MOD_NAME, fFrost );
		}

		case ITEM_HEALTH:
		{
			client_print(id, print_chat,"%s You will be given an additional %d health", MOD_NAME, get_pcvar_num( CVAR_uwc3ng_health ) );
		}

		case ITEM_SCROLL:
		{
			if ( is_user_alive( id ) )
			{
				client_print( id, print_chat, "%s If the round has not ended, you will respawn after you die", MOD_NAME );
			}

			else
			{
				client_print( id, print_chat, "%s You will now be respawned", MOD_NAME );
			}
		}

		case ITEM_PROTECTANT:
		{
			client_print( id, print_chat, "%s You will now be partially immune to moles", MOD_NAME );
		}

		case ITEM_HELM:
		{
			client_print( id, print_chat, "%s You have been given %d charges, each charge will block a headshot", MOD_NAME, HELM_CHARGES );
		}

		case ITEM_AMULET:
		{
			client_print( id, print_chat, "%s You will now be silent while walking and running", MOD_NAME );
		}

		case ITEM_SOCK:
		{
			client_print( id, print_chat, "%s You will now be able to jump higher", MOD_NAME );
		}

		case ITEM_GLOVES:
		{
			client_print( id, print_chat, "%s You will now be given a grenade once every %d seconds", MOD_NAME, get_pcvar_num( CVAR_uwc3ng_glove_timer ) );
		}

		case ITEM_RING:
		{
			client_print( id, print_chat, "%s You will now regenerate 1 HP every 2 seconds (you may purchase up to 5 rings)", MOD_NAME );
		}

		case ITEM_CHAMELEON:
		{
			client_print( id, print_chat, "%s You now look like the enemy", MOD_NAME );
		}

		case ITEM_MOLE:
		{
			client_print( id, print_chat, "%s You will be disguised as your enemy and appear at the enemy's spawn next round", MOD_NAME );
		}
	}
}

// Give the user bonuses for their items (except charges)
public ITEM_GiveAllBonuses( id )
{

	// Loop through all item slots
	for ( new i = ITEM_SLOT_ONE; i <= ITEM_SLOT_TWO; i++ )
	{
		
		// Do we have a valid item here?
		if ( g_iShopMenuItems[id][i] != ITEM_NONE )
		{

			// Don't want to give the user more charges for free do we?
			//  And we don't want to give the bonuses if this is a next round use item (i.e. if we do then mole for infinity - that doesn't seem nice)
			if ( !ITEM_CheckFlag( g_iShopMenuItems[id][i], ITEM_CHARGEABLE ) && !ITEM_CheckFlag( g_iShopMenuItems[id][i], ITEM_NEXTROUNDUSE ) )
			{
				ITEM_GiveBonuses( id, g_iShopMenuItems[id][i] );
			}
		}
	}
}

// Give our players their bonus!
ITEM_GiveBonuses( id, iItem )
{
	
	// Display a message regarding the item they just purchased
	switch ( iItem )
	{
		case ITEM_ANKH:
		{
			g_bPlayerBoughtAnkh[id] = true;
		}

		case ITEM_BOOTS:
		{
			SKILL_UnholyAura( id );
		}

		case ITEM_CLOAK:
		{
			SKILL_Invisible( id );
		}

		case ITEM_NECKLACE:
		{
			g_iNecklaceCharges[id] += NECKLACE_CHARGES;
		}

		case ITEM_HEALTH:
		{
			new iHealth = get_pcvar_num( CVAR_uwc3ng_health );
			iHealth += get_user_health( id );
			set_user_health( id, iHealth );
		}

		case ITEM_SCROLL:
		{
			if ( !is_user_alive( id ) )
			{
				ITEM_Scroll( id );
			}
		}

		case ITEM_HELM:
		{
			g_iHelmCharges[id] += HELM_CHARGES;
		}

		case ITEM_SOCK:
		{
			SKILL_Levitation( id );
		}

		case ITEM_GLOVES:
		{
			ITEM_Gloves( id );
		}

		case ITEM_RING:
		{
			g_iTotalRings[id] += RING_INCREMENT;

			if ( !task_exists( TASK_ITEM_RING + id ) )
			{
				_ITEM_Ring( id );
			}
		}

		case ITEM_CHAMELEON:
		{
			SHARED_ChangeSkin( id, 2 );
		}

		case ITEM_MOLE:
		{
			g_bPlayerBoughtMole[id] = true;
		}
	}
}

// Item Equip Function
public ITEM_Equip( id, iItem )
{
	new iItemSlot = ITEM_GetSlot( id );

	// Items are not full
	if ( iItemSlot != ITEM_SLOT_FULL )
	{
		// Remove the user's old item if necessary
		if ( g_iShopMenuItems[id][iItemSlot] > ITEM_NONE )
		{
			ITEM_Remove( id, iItemSlot, true );
		}
			
		// Set their new item
		g_iShopMenuItems[id][iItemSlot] = iItem;
	}

	return;
}

// Item Remove Functions
public ITEM_RemoveID( id, iItem )
{
	new iItemSlot = ITEM_Has( id, iItem );

	if ( iItemSlot > ITEM_NONE )
	{
		ITEM_Remove( id, iItemSlot, true );
	}

	return;
}

public ITEM_Remove( id, iItemSlot, bResetAnkhMole )
{
	new iItem = g_iShopMenuItems[id][iItemSlot];

	if ( g_iShopMenuItems[id][iItemSlot] != ITEM_SCROLL )
		g_iShopMenuItems[id][iItemSlot] = ITEM_NONE;

	switch( iItem )
	{
		case ITEM_ANKH:
		{
			if ( bResetAnkhMole )
			{
				g_bPlayerBoughtAnkh[id] = false;
			}
		}

		case ITEM_BOOTS:
		{
			SKILL_UnholyAura( id );
		}

		case ITEM_CLOAK:
		{
			SKILL_Invisible( id );
		}

		case ITEM_NECKLACE:
		{
			g_iNecklaceCharges[id] = 0;
		}

		case ITEM_HEALTH:
		{
			new iNewHealth = get_user_health( id ) - get_pcvar_num( CVAR_uwc3ng_health );
			
			// Lets not kill the user, give them 1 health
			iNewHealth = ( ( iNewHealth <= 0 ) ? 1 : iNewHealth );

			set_user_health( id, iNewHealth );
		}

		case ITEM_HELM:
		{
			g_iHelmCharges[id] = 0;
		}
		
		case ITEM_SOCK:
		{
			SKILL_Levitation( id );
		}

		case ITEM_GLOVES:
		{
			if ( task_exists( TASK_ITEM_GLOVES + id ) )
			{
				remove_task( TASK_ITEM_GLOVES + id );
			}
		}

		case ITEM_RING:
		{
			if ( task_exists( TASK_ITEM_RING + id ) )
			{
				remove_task( TASK_ITEM_RING + id );
			}
			
			// Set the number of rings to 0
			g_iTotalRings[id] = 0;
		}

		case ITEM_CHAMELEON:
		{
			SHARED_ChangeSkin( id, 0 );
		}

		case ITEM_MOLE:
		{
			if ( bResetAnkhMole )
			{
				g_bPlayerBoughtMole[id] = false;
			}
		}
	}

	return;
}

public ITEM_RemoveCharge( id, iItem )
{
	if ( ITEM_Has( id, iItem ) > ITEM_NONE )
	{
		switch ( iItem )
		{
			case ITEM_NECKLACE:
			{
				g_iNecklaceCharges[id] -= CHARGE_DISPOSE;
				
				if ( g_iNecklaceCharges[id] <= 0 )
				{
					ITEM_RemoveID( id, iItem );
				}
			}

			case ITEM_HELM:
			{
				g_iHelmCharges[id] -= CHARGE_DISPOSE;
				
				if ( g_iHelmCharges[id] <= 0 )
				{
					ITEM_RemoveID( id, iItem );
				}
			}

			case ITEM_RING:
			{
				g_iTotalRings[id] -= CHARGE_DISPOSE;
				
				if ( g_iTotalRings[id] <= 0 )
				{
					ITEM_RemoveID( id, iItem );
				}
			}
		}
	}

	return;
}


// Item Get Functions
public ITEM_GetSlot( id )
{
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[id][ITEM_SLOT_TWO] > ITEM_NONE )
		return ITEM_SLOT_FULL;

	else if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE )
		return ITEM_SLOT_TWO;

	return ITEM_SLOT_ONE;
}

public ITEM_Has( id, iItem )
{
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] == iItem )
		return ITEM_SLOT_ONE;

	else if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] == iItem )
		return ITEM_SLOT_TWO;

	return ITEM_NONE;
}

// Item Death Function
public ITEM_UserDied( id )
{
	// The user just died, remove all items
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE )
	{
		ITEM_Remove( id, ITEM_SLOT_ONE, false );
	}

	if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] > ITEM_NONE )
	{
		ITEM_Remove( id, ITEM_SLOT_TWO, false );
	}
}

// Item Specific Functions

public ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )
{

	// Claws of Attack
	if ( ITEM_Has( iAttacker, ITEM_CLAWS ) > ITEM_NONE )
	{	
		UWC3NG_DoDamage( iVictim, iAttacker, get_pcvar_num( CVAR_uwc3ng_claw ), iWeapon, iHitPlace );
		
		SHARED_Glow( iAttacker, (2 * get_pcvar_num( CVAR_uwc3ng_claw ) ), 0, 0, 0 );

		Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, g_GlowLevel[iVictim][0] );
	}

	// Mask of Death
	if ( ITEM_Has( iAttacker, ITEM_MASK ) > ITEM_NONE && is_user_connected( iAttacker ) )
	{
		new iHealth = get_user_health( iAttacker );
		new iBonusHealth = floatround( float( iDamage ) * get_pcvar_float( CVAR_uwc3ng_mask ) );
		
		new iVampiricBonus = p_data_b[iAttacker][PB_HEXED] ? 0 : p_data_skill[iAttacker][SKILL_VAMPIRIC];

		// Then the user already gets a bonus, lets lower the total amount the user is going to get
		if ( iVampiricBonus > 0 )
		{
			iBonusHealth /= iVampiricBonus;
		}
		
		// User needs to be set to max health
		if ( iHealth + iBonusHealth > PLAYER_get_maxhealth( iAttacker ) )
		{
			set_user_health( iAttacker, PLAYER_get_maxhealth( iAttacker ) );
		}
		
		// Give them bonus
		else
		{
			set_user_health( iAttacker, iHealth + iBonusHealth );
		}

		SHARED_Glow( iAttacker, 0, iBonusHealth, 0, 0 );

		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 0, 255, 0, g_GlowLevel[iAttacker][1] );
	}

	// Orb of Frost
	if ( ITEM_Has( iAttacker, ITEM_FROST ) > ITEM_NONE )
	{
		// Only slow them if they aren't slowed/stunned already
		if ( !p_data_b[iVictim][PB_SLOWED] && !p_data_b[iVictim][PB_STUNNED] )
		{

			p_data_b[iVictim][PB_SLOWED]	= true;

			SKILL_UnholyAura( iVictim );

			set_task( 1.0, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + iVictim );

			SHARED_Glow( iAttacker, 0, 0, 0, 100 );

			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 255, 255, g_GlowLevel[iVictim][3] );
		}
	}
}

public ITEM_Tome( id )
{
	new iXp = XP_Calc( id, get_pcvar_num( CVAR_uwc3ng_tome ) );
		
	new iBonusXP = XP_Give( id, iXp );

	if ( iBonusXP != 0 )
	{
		client_print( id, print_chat, "%s You just gained %d experience from purchasing a tome", MOD_NAME, iBonusXP );
	}

	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_TOME], 1.0, ATTN_NORM, 0, PITCH_NORM );

	return;
}

public ITEM_Gloves( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}

	if ( !SHARED_HasGrenade( id ) )
	{
		g_iGloveTimer[id] = 0;

		_ITEM_Glove_Give( id );
	}

	return;
}

public ITEM_Glove_Begin( id )
{
	// Then lets start a timer to give them a grenade!
	g_iGloveTimer[id] = get_pcvar_num( CVAR_uwc3ng_glove_timer );
	
	static StatusMsg[128];
	formatex( StatusMsg, charsmax( StatusMsg ), "%d second(s) until your next grenade", g_iGloveTimer[id] );

	UWC3NG_StatusText( id, TXT_BLINK_CENTER_SHRT, StatusMsg );

	g_iGloveTimer[id]--;

	set_task( 1.0, "_ITEM_Glove_Give", TASK_ITEM_GLOVES + id );
}

public _ITEM_Glove_Give( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}

	if ( id >= TASK_ITEM_GLOVES )
	{
		id -= TASK_ITEM_GLOVES;
	}

	if ( !is_user_alive( id ) )
	{
		return;
	}

	// Only need to save once! - this is b/c I'm not sure when the loss of a grenade is updated - and I wanted SHARED_HasGrenade to work @ all times!
	if ( g_iGloveTimer[id] == get_pcvar_num( CVAR_uwc3ng_glove_timer ) - 1 )
	{
		SHARED_Save_Weapons( id );
	}

	// If somehow they already got a grenade - stop this!
	new bool:bHasGrenade = false;
	if ( cs_get_user_bpammo( id, CSW_HEGRENADE ) > 0 )
	{
		bHasGrenade = true;
	}
	

	// Already have a grenade!!
	if ( bHasGrenade || SHARED_HasGrenade( id ) )
	{
		g_iGloveTimer[id] = 0;

		return;
	}


	if ( g_iGloveTimer[id] > 0 )
	{
		static StatusMsg[128];
		formatex( StatusMsg, charsmax( StatusMsg ), "%d second(s) until your next grenade", g_iGloveTimer[id] );
		
		UWC3NG_StatusText( id, TXT_BLINK_CENTER_SHRT, StatusMsg );

		g_iGloveTimer[id]--;

		set_task( 1.0, "_ITEM_Glove_Give", TASK_ITEM_GLOVES + id );

		return;
	}

	// Give him the nade
	give_item( id, "weapon_hegrenade" );

	// Display a message to the user
	UWC3NG_StatusText( id, TXT_BLINK_CENTER, "Enjoy a grenade!" )

	return;
}		

public ITEM_BuyRings( id )
{

	new iItemSlot = ITEM_GetSlot( id );

	// Items are full
	if ( iItemSlot == ITEM_SLOT_FULL && ITEM_Has( id, ITEM_RING ) == ITEM_NONE )
	{
		g_iFutureItem[id] = -3;

		ITEM_ReplaceItem( id );

		return;
	}

	new iMoney;
	new iAdditionalRings = 0;
	
	while ( g_iTotalRings[id] + iAdditionalRings < 5 )
	{
		iMoney = cs_get_user_money( id );

		if ( iMoney < ITEM_Cost( id, ITEM_RING ) )
		{
			break;
		}

		iAdditionalRings++;
		
		SHARED_SetUserMoney( id, -1 * ITEM_Cost( id, ITEM_RING ) );
	}

	// Then we need to give them some rings!
	if ( iAdditionalRings > 0 )
	{

		// Subtract 1 b/c ITEM_GiveItem will add one
		g_iTotalRings[id] += ( ( iAdditionalRings * RING_INCREMENT ) - ( RING_INCREMENT ) );

		ITEM_GiveItem( id, ITEM_RING );
	}

	return;
}

public _ITEM_Ring( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}

	if ( id >= TASK_ITEM_RING )
	{
		id -= TASK_ITEM_RING;
	}

	if ( !is_user_connected( id ) || ITEM_Has( id, ITEM_RING ) == ITEM_NONE )
	{
		return;
	}

	new iBonusHealth = g_iTotalRings[id];

	while ( iBonusHealth > 0 )
	{
		new iHealth =  get_user_health( id ) + 1;

		if ( iHealth <= PLAYER_get_maxhealth( id ) )
		{
			set_user_health( id, iHealth );
		}

		iBonusHealth--;
	}

	set_task( 2.0, "_ITEM_Ring", TASK_ITEM_RING + id );

	return;
}

public ITEM_Scroll( id )
{
	// Make sure the user isn't about to respawn when we do these checks
	if ( !p_data_b[id][PB_RESPAWNED] || !p_data_b[id][PB_RESPAWNED_TWICE] )
	{
		p_data_b[id][PB_RESPAWNED] = true;
		
		set_task( SPAWN_DELAY, "_SHARED_Spawn", TASK_SPAWN + id );
	}
	
	// Okay give him his money back
	else 
	{
		// Remove the Scroll
		if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] == ITEM_SCROLL )
			g_iShopMenuItems[id][ITEM_SLOT_ONE] = ITEM_NONE;
		else if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] == ITEM_SCROLL )
			g_iShopMenuItems[id][ITEM_SLOT_TWO] = ITEM_NONE;
			
		// The player should know why
		client_print( id, print_chat, "%s Unable to respawn because you respawned allready this round, here is your money back", MOD_NAME );

		SHARED_SetUserMoney( id, ITEM_Cost( id, ITEM_SCROLL ) );
	}
}

public bool:ITEM_CheckFlag( iItemID, iFlag )
{
	if ( g_iFlag[iItemID] & iFlag )
	{
		return true;
	}

	return false;	
}

// Show the shopmenu
public ITEM_ShowShopmenu( id, page )
{
	// The header
	new MenuText[512] = "", KEYS = (1<<9);
	
	if ( page == 0 )
	{
		copy ( MenuText, charsmax( MenuText ), "\yShopmenu 1\R$  Cost^n^n" );
	}
	
	else if ( page == 1 )
	{
		copy ( MenuText, charsmax( MenuText ), "\yShopmenu 2\R$  Cost^n^n" );
	}
	
	// Save the side
	p_data[id][SHOPMENU_SIDE] = page;
	
	// Calculate the right page
	page *= MAX_PAGE_ITEMS;
	
	// Loop through all items
	for ( new i = page; i < MAX_SHOPMENU_ITEMS; ++i )
	{
		// Add every item to the menutext
		formatex( MenuText, charsmax( MenuText ), "%s\w%d. %s\y\R%d^n", MenuText, ( i+1 - page ), ShopmenuLong_Names[i], ITEM_COST[i] ); 
		
		// Add every keypoint
		KEYS |= (1<<(i-page));
		
		// We're at the maximum. Go out of the loop
		if ( ( i + 1) == MAX_PAGE_ITEMS )
		{
			break;
		}
	}
	
	// Add the exit menupoint
	add( MenuText, charsmax( MenuText ), "^n\w0. Exit^n" );
	
	// Show the player the menu
	show_menu ( id, KEYS, MenuText, 25, "Shopmenu12" );
	
	return;
}

// Here we handle the pressed key from the shopmenu
public _ITEM_HandleShopMenu( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	// The player press the exit button
	if ( key == 9 )
	{
		// Reset the data
		p_data[id][SHOPMENU_SIDE] = 0;
		
		return;
	}
	
	// Calculate the pressed key with side
	new MenuEntry = ( p_data[id][SHOPMENU_SIDE] * MAX_PAGE_ITEMS ) + key;
	
	// Buy the item
	ITEM_Buy( id, MenuEntry );
	
	return;
}

// Replace menu
public ITEM_ReplaceItem( id )
{
	// The header
	new MenuText[256] = "";
	copy( MenuText, charsmax( MenuText ), "\yWhich item would you like to replace?^n" );
	
	// Create the keys for the menu
	new KEYS = (1<<9) | (1<<0) | (1<<1);
	
	// Get the two item names
	static MenuItemOne[32], MenuItemTwo[32];
	copy( MenuItemOne, charsmax( MenuItemOne ), ShopmenuLong_Names[g_iShopMenuItems[id][ITEM_SLOT_ONE]] );
	copy( MenuItemTwo, charsmax( MenuItemTwo ), ShopmenuLong_Names[g_iShopMenuItems[id][ITEM_SLOT_TWO]] );
	
	// Add the two items to the menu
	formatex( MenuText, charsmax( MenuText ), "%s\w1. %s^n", MenuText, MenuItemOne );
	formatex( MenuText, charsmax( MenuText ), "%s\w2. %s^n", MenuText, MenuItemTwo );
	
	// Add the exit button
	formatex( MenuText, charsmax( MenuText ), "%s^n\w0. Exit^n", MenuText );
	
	// Show the menu
	show_menu( id, KEYS, MenuText, 25, "ShopmenuReplace" );
	
	return;
}

// Handle replace menu
public ITEM_HandleReplaceItem( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	// He pressed the exit button
	if ( key == 9 )
	{
		return;
	}
	
	// Remove item from item slot one
	if ( key == 0 )
	{
		ITEM_Remove( id, ITEM_SLOT_ONE, true )
	}

	// Remove item from itemslot two
	else if ( key == 1 )
	{
		ITEM_Remove( id, ITEM_SLOT_TWO, true )
	}

	// Then the user typed "rings"
	if ( g_iFutureItem[id] == -3 )
	{
		ITEM_BuyRings( id );
	}
	else
	{
		ITEM_Buy( id, g_iFutureItem[id] );
	}
	
	return;
}
	
	
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
