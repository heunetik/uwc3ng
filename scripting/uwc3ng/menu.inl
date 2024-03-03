// Uwc3ng Menu functions

// Show the uwc3ng menu
public MENU_war3menu( id )
{
	static MenuText[512], KEYS;
	
	// Add the header
	copy( MenuText, charsmax( MenuText ), "\yUWC3NG: Main Menu^n^n" );
	
	// Add every menupoint
	add( MenuText, charsmax( MenuText ), "\w1. Skills Menu^n" );
	add( MenuText, charsmax( MenuText ), "\w2. Item Menu^n" );
	add( MenuText, charsmax( MenuText ), "\w3. Help^n" );
	add( MenuText, charsmax( MenuText ), "\w4. UWC3NG Credits^n" );
	add( MenuText, charsmax( MenuText ), "\w5. Commands List^n" );
	add( MenuText, charsmax( MenuText ), "\w6. Your Character Sheet^n" );
	add( MenuText, charsmax( MenuText ), "^n\w0. Exit^n" );
	
	// Add the keys
	KEYS = (1<<9) | (1<<0) | (1<<1)| (1<<2) | (1<<3) | (1<<4) | (1<<5);
	
	// Show the menu
	show_menu( id, KEYS, MenuText, 25, "War3Menu" );
	
	return;
}

// Handle the war3menu
public MENU_HandleWar3( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	switch ( key )
	{
		case 9: // Exit
		{
			return;
		}
		
		case 0: // Skills Menu
		{
			MENU_Skills( id );
		}
		
		case 1: // Item Menu
		{
			MENU_ItemMenu( id );
		}
		
		case 2: // Help
		{
			MENU_Helpmenu( id );
		}
		
		case 3: // Uwc3ng credits
		{
			MOTD_ShowCreditsHelp( id );
		}
		
		case 4: // Commands list 
		{
			MOTD_ShowCommandHelp( id );
		}
		
		case 5: // Character Sheet
		{
			MOTD_CharacterSheet( id, 0 );
		}
	}
	
	return;
}

// Uwc3ng Skillsmenu
public MENU_Skills( id )
{
	static MenuText[512], KEYS;
	
	// Add the header
	copy( MenuText, charsmax( MenuText ), "\ySkills Options^n^n" );
		
	// Add every menupoint
	add( MenuText, charsmax( MenuText ), "\w1. Select Skills^n" );
	add( MenuText, charsmax( MenuText ), "\w2. 1st Page Skills Information^n" );
	add( MenuText, charsmax( MenuText ), "\w3. 2nd Page Skills Information^n" );
	add( MenuText, charsmax( MenuText ), "\w4. 3nd Page Skills Information^n" );
	add( MenuText, charsmax( MenuText ), "\w5. 4th Page Skills Information^n" );
	add( MenuText, charsmax( MenuText ), "\w6. 5th Page Skills Information^n" );
	add( MenuText, charsmax( MenuText ), "^n\w9. Back^n" );
	add( MenuText, charsmax( MenuText ), "^n\w0. Exit^n" );
	
	// Add the keys
	KEYS = (1<<9) | (1<<0) | (1<<1)| (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<8);
	
	// Show the menu
	show_menu( id, KEYS, MenuText, 25, "Skillsmenu" );
	
	return;
}

// Handle the skillsmenu
public MENU_HandleSkills( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	switch ( key )
	{
		case 9: // Exit
		{
			return;
		}
		
		case 0: // Select skills
		{
			COMMAND_SkillMenu( id );
		}
		
		case 8: // Back
		{
			MENU_war3menu( id );
		}
	}
	
	// If we're here, the player want skills information
	if ( key > 0 && key < 6 )
	{
		MOTD_ShowSkillsHelp( id, key );
	}
	
	return;
}

// Uwc3ng Item Menu
public MENU_ItemMenu( id )
{
	static MenuText[512], KEYS;
	
	// Add the header
	copy( MenuText, charsmax( MenuText ), "\yWhich Shopmenu?^n^n" );
		
	// Add every menupoint
	add( MenuText, charsmax( MenuText ), "\w1. Shopmenu 1^n" );
	add( MenuText, charsmax( MenuText ), "\w2. Shopmenu 2^n" );
	add( MenuText, charsmax( MenuText ), "\w9. Back^n" );
	add( MenuText, charsmax( MenuText ), "^n\w0. Exit^n" );
	
	// Add the keys
	KEYS = (1<<9) | (1<<0) | (1<<1) | (1<<8);
	
	// Show the menu
	show_menu( id, KEYS, MenuText, 25, "Itemsmenu" );
	
	return;
}

// Handle the itemmenu
public MENU_HandleItems( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	switch ( key )
	{
		case 9: // Exit
		{
			return;
		}
		
		case 0: // Shopmenu 1
		{
			COMMAND_Shopmenu( id );
		}
		
		case 1: // Shopmenu 2
		{
			COMMAND_Shopmenu2( id );
		}
		
		case 8: // Back
		{
			MENU_war3menu( id );
		}
	}
	
	return;
}

// Uwc3ng Help menu
public MENU_Helpmenu( id )
{
	static MenuText[512], KEYS;
	
	// Add the header
	copy( MenuText, charsmax( MenuText ), "\yHelp Menu^n^n" );
		
	// Add every menupoint
	add( MenuText, charsmax( MenuText ), "\w1. UWC3NG General Help^n" );
	add( MenuText, charsmax( MenuText ), "\w2. About UWC3NG Ultimates^n" );
	add( MenuText, charsmax( MenuText ), "\w3. UWC3NG Credits^n" );
	add( MenuText, charsmax( MenuText ), "\w4. (\rNew\w) Attributes Info^n" );
	add( MenuText, charsmax( MenuText ), "\w5. (\rNew\w) Resistances Info^n" );
	add( MenuText, charsmax( MenuText ), "\w6. Tips for Playing UWC3 pg.1^n" );
	add( MenuText, charsmax( MenuText ), "\w7. Tips for Playing UWC3 pg.2^n" );
	add( MenuText, charsmax( MenuText ), "^n\w9. Back^n" );
	add( MenuText, charsmax( MenuText ), "^n\w0. Exit^n" );
	
	// Add the keys
	KEYS = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<8)|(1<<9);
	
	// Show the menu
	show_menu( id, KEYS, MenuText, 25, "Helpmenu" );
	
	return;
}

// Handle help menu
public MENU_HandleHelp( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	switch ( key )
	{
		case 9: // Exit
		{
			return;
		}
		
		case 0: // General Help
		{
			MOTD_ShowInfoHelp( id );
		}
		
		case 1: // Ultimates
		{
			MOTD_ShowUltimatesHelp( id );
		}
		
		case 2: // Credits
		{
			MOTD_ShowCreditsHelp( id );
		}
		
		case 3: // Attributes
		{
			MOTD_ShowAttribsHelp( id );
		}
		
		case 4: // Resistances
		{
			MOTD_ShowResistHelp( id );
		}
		
		case 5: // Tips 1
		{
			MOTD_ShowTipsHelp( id );
		}
		
		case 6: // Tips 2
		{
			MOTD_ShowTips2Help( id );
		}
		
		case 8: // Back
		{
			MENU_war3menu( id );
		}
	}
	
	return;
}

// Uwc3ng reset skills menu
public MENU_ResetSkills( id )
{
	static MenuText[512], KEYS;
	
	// Add the header
	copy( MenuText, charsmax( MenuText ), "\yAre you sure you want to Reset your XP to 0?" );
		
	// Add every menupoint
	add( MenuText, charsmax( MenuText ), "^n^n\w1. Yes" );
	add( MenuText, charsmax( MenuText ), "^n\w2. No" );
		
	// Add the keys
	KEYS = (1<<0) | (1<<1);
	
	// Show the menu
	show_menu( id, KEYS, MenuText, 25, "ResetSkillmenu" );
	
	return;
}

// Handle reset skills menu
public MENU_HandleResetSkills( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	// he pressed yes
	if ( key == 0 )
	{
		// Reset the XPs to 1 (0 = not saved)
		p_data[id][PLAYER_XP] = 1;
		
		// Check the skills etc.
		XP_CheckLevel( id );
	}
	
	return;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
