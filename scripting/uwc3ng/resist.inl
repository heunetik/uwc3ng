// Uwc3ng Resistances functions

// Display the select resist menu
public RESIST_ShowSelectMenu( id )
{
	// The header
	new MenuText[512] = "", KEYS = (1<<9) | (1<<8) | (1<<7);
	formatex( MenuText, charsmax( MenuText ), "\ySelect a Resistance to enhance [%d pts remaining]: \w^n", p_data[id][AVAILABLE_RESISTS] );
	add( MenuText, charsmax( MenuText ), "\y(say /resistinfo for info about resistances) \w^n^n" );
	
	// Loop through all resistens
	for ( new i = 0; i < MAX_RESIST; ++i )
	{
		// Add every resist to the menutext
		formatex( MenuText, charsmax( MenuText ), "%s\w%d. %s Resistance     [ %d/%d ]^n", MenuText, i+1, Resistance_Names[i], p_data_resist[id][i], MAX_RESIST_POINTS ); 
		
		// Add every keypoint
		KEYS |= (1<<i);
	}
	
	// Add the Set fill menupoint
	if ( !p_data_b[id][RESIST_FILLRESIST] )
	{
		add( MenuText, charsmax( MenuText ), "^n\w8. Set fill Resists^n" );
	}
	
	// Add the Set one menupoint
	else
	{
		add( MenuText, charsmax( MenuText ), "^n\w8. Set one Resist^n" );
	}
	
	// Add the attrib menupoint
	add( MenuText, charsmax( MenuText ), "^n\w9. Attributes Menu^n" );
	
	// Add the exit menupoint
	add( MenuText, charsmax( MenuText ), "\w0. Exit^n" );
	
	// Show the player the menu
	show_menu ( id, KEYS, MenuText, 25, "ResistMenu" );
	
	return;
}

// Handle the pressed key from the select resist menu
public RESIST_HandleMenu( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	// The player exit the menu
	if ( key == 9 )
	{
		// Reset the data
		p_data_b[id][RESIST_FILLRESIST] = false;
		
		return;
	}
	
	// the player want fill / one resist menupoint
	else if ( key == 7 )
	{
		p_data_b[id][RESIST_FILLRESIST] = ( p_data_b[id][RESIST_FILLRESIST] ? false : true );
	
		// Show him the menu again
		RESIST_ShowSelectMenu( id )
		
		// Go out of this shit
		return;
	}
	
	// The player want to the attributes menu
	else if ( key == 8 )
	{
		ATTRIB_ShowSelectMenu( id );
		
		return;
	}
	
	// The player is already on the last attrib level
	if ( p_data_resist[id][key] == MAX_RESIST_POINTS )
	{
		client_print( id, print_chat, "%s You have already reached the maximum resistance level", MOD_NAME );
		
		return;
	}
	
	// The player has no available attribute points
	else if ( p_data[id][AVAILABLE_RESISTS] <= 0 )
	{
		client_print( id, print_chat, "%s You do not have any Resistance points to spend, earn more XP", MOD_NAME );
		
		return;
	}
	
	// The player want to fill the complete resist
	if ( p_data_b[id][RESIST_FILLRESIST] )
	{
		static iSpendResists = 0;
		while ( p_data_resist[id][key] != MAX_RESIST_POINTS )
		{
			// Alright. Add one point
			p_data_resist[id][key]++;
			
			// we count all spend resistance for the message 
			iSpendResists++;
	
			// Remove one from the available points
			p_data[id][AVAILABLE_RESISTS]--;
			
			// We have no available resists - go out!
			if ( p_data[id][AVAILABLE_RESISTS] == 0 )
			{
				break;
			}
		}
		
		// Print him a message
		client_print( id, print_chat, "%s You spend %d Resistance points to %s", MOD_NAME, iSpendResists, Resistance_Names[key] );
	}
	
	// Fill only with one point
	else
	{
		// Alright. Add one point
		p_data_resist[id][key]++;
	
		// Remove one from the available points
		p_data[id][AVAILABLE_RESISTS]--;
		
		// Print him a message
		client_print( id, print_chat, "%s You spend 1 Resistance point to %s", MOD_NAME, Resistance_Names[key] );
	}
	
	// He has more available resist points re-open the menu
	if ( p_data[id][AVAILABLE_RESISTS] > 0 )
	{
		RESIST_ShowSelectMenu( id );
		
		return;
	}
	
	// He has available attribs
	else if ( p_data[id][AVAILABLE_ATTRIBS] > 0 )
	{
		ATTRIB_ShowSelectMenu( id );
	}
	
	// The player has more available skills, reopen the menu!
	else if ( p_data[id][AVAILABLE_SKILL] > 0 )
	{
		SKILL_ShowMenu( id, p_data[id][SKILL_SIDE] );
	}
	
	// Reset the data
	p_data_b[id][RESIST_FILLRESIST] = false;
	
	return;
}

// Check available resists points
public RESIST_Check_Available_points( id )
{
	// Get the selected attrib points
	new iSelectedResists = RESIST_SelectedResists( id );
	
	// Calculate the default available resists for this player
	new iAvailablePoints = ( p_data[id][PLAYER_LVL] * ( MAX_RESIST_AVAILABLE / MAX_LEVEL ) );
	
	// Check if we have Longterm XP (no savexp)
	if ( !get_pcvar_num( CVAR_uwc3ng_save_xp ) )
	{
		iAvailablePoints /= get_pcvar_float( CVAR_uwc3ng_resist_xpmodifier );
	}
	
	if ( iAvailablePoints > MAX_RESIST_AVAILABLE ) 
	{
		iAvailablePoints = MAX_RESIST_AVAILABLE;
	}
	
	// Calculate the difference
	iAvailablePoints = ( iAvailablePoints -  iSelectedResists );
	
	// Check if the player has more resistpoints spent as available
	if ( iAvailablePoints < 0 )
	{
		// We have to reset all resists
		p_data_b[id][RESET_RESISTS] = true;
		RESIST_ResetAllNow( id );
		
		// Jump out the loop and the function
		return;
	}
		
	// Save the available resistpoints
	p_data[id][AVAILABLE_RESISTS] = iAvailablePoints;
	
	return;
}
	
	
// Check selected resists points
RESIST_SelectedResists( id )
{
	// Loop through all resists and count the selected resists points
	static i, iResistCounter; iResistCounter = 0;
	for ( i = 0; i < MAX_RESIST; ++i )
	{
		iResistCounter += p_data_resist[id][i];
	}
	
	return iResistCounter;
}

// Reset all resists
public RESIST_ResetAll( id )
{
	// print him a message that all resists will be reseted
	client_print( id, print_center, "Your resistances will be reset next round" );
	
	// We have to save it because on the next round we should reset all resists
	p_data_b[id][RESET_RESISTS] = true;
}

// Reset now all resists (called on new round or spawn if csdm enabled)
public RESIST_ResetAllNow( id )
{
	if ( p_data_b[id][RESET_RESISTS] )
	{
		// We loop through all resists and set it to start points
		for ( new j = 0; j < MAX_RESIST; ++j )
		{
			p_data_resist[id][j] = 0;
		}
			
		// Detect new available resist points
		RESIST_Check_Available_points( id );
			
		// Next round the resists should no more reseted
		p_data_b[id][RESET_RESISTS] = false;
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
