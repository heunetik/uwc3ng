// Uwc3ng attribs functions

// Display the select attribs menu
public ATTRIB_ShowSelectMenu( id )
{
	// The header
	new MenuText[512] = "", KEYS = (1<<9) | (1<<8);
	formatex( MenuText, charsmax( MenuText ), "\ySelect an Attribute to enhance [%d pts remaining]: \w^n", p_data[id][AVAILABLE_ATTRIBS] );
	add( MenuText, charsmax( MenuText ), "\y(say /attribinfo for info about attributes) \w^n^n" );
	
	// Loop through all attribs
	for ( new i = 0; i < MAX_ATTRIBS; ++i )
	{
		// Add every attrib to the menutext
		formatex( MenuText, charsmax( MenuText ), "%s\w%d. %s     [ %d/%d ]^n", MenuText, i+1, Attribute_Names[i], p_data_attrib[id][i], MAX_ATTRIBS_POINTS ); 
		
		// Add every keypoint
		KEYS |= (1<<i);
	}
	
	// Add the "switch to resistance menu" menupoint
	add( MenuText, charsmax( MenuText ), "^n\w9. Resistances Menu^n" );
	
	// Add the exit menupoint
	add( MenuText, charsmax( MenuText ), "\w0. Exit^n" );
	
	// Show the player the menu
	show_menu ( id, KEYS, MenuText, 25, "AttribMenu" );
	
	return;
}

// Handle the pressed key from the select attrib menu
public ATTRIB_HandleMenu( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	// The player exit the menu
	if ( key == 9 )
	{
		return;
	}
	
	// The player switch to the resistance menu
	else if ( key == 8 )
	{
		RESIST_ShowSelectMenu( id );
		
		return;
	}
	
	// The player is already on the last attrib level
	if ( p_data_attrib[id][key] == MAX_ATTRIBS_POINTS )
	{
		client_print( id, print_chat, "%s You have already reached the maximum Attribute level", MOD_NAME );
		
		return;
	}
	
	// The player has no available attribute points
	else if ( p_data[id][AVAILABLE_ATTRIBS] <= 0 )
	{
		client_print( id, print_chat, "%s You do not have any attribute points to spend, earn more XP", MOD_NAME );
		
		return;
	}
	
	// Alright. Add one point
	p_data_attrib[id][key]++;
	
	// Remove one from the available points
	p_data[id][AVAILABLE_ATTRIBS]--;
	
	// Print him a message
	client_print( id, print_chat, "%s You spend 1 attribute point to %s", MOD_NAME, Attribute_Names[key] );
	
	// The player skilled Strength, give him instant the extra hp
	if ( key == ATTRIB_STRENGTH )
	{
		SKILL_DevotionAura( id );
	}
	
	// Update the skills
	SKILL_Update( id );
	
	// He has more available attrib points re-open the menu
	if ( p_data[id][AVAILABLE_ATTRIBS] > 0 )
	{
		ATTRIB_ShowSelectMenu( id );
	}
	
	// The player has more available skills, reopen the menu!
	else if ( p_data[id][AVAILABLE_SKILL] > 0 )
	{
		SKILL_ShowMenu( id, p_data[id][SKILL_SIDE] );
	}
	
	// He has available resists
	else if ( p_data[id][AVAILABLE_RESISTS] > 0 )
	{
		RESIST_ShowSelectMenu( id );
	}
	
	return;
}

// Check available attribute points
public ATTRIB_Check_Available_points( id )
{
	// Get the selected attrib points
	new iSelectedAttribs = ATTRIB_SelectedAttribs( id );
	
	// Calculate the default available attribs for this player
	new iAvailablePoints = p_data[id][PLAYER_LVL];
		
	// Look if we have Longterm XP
	if ( !get_pcvar_num( CVAR_uwc3ng_save_xp ) )
	{
		iAvailablePoints /= get_pcvar_float( CVAR_uwc3ng_attrib_xpmodifier );
	}
	
	if ( iAvailablePoints > MAX_ATTRIBS_AVAILABLE )
	{
		iAvailablePoints = MAX_ATTRIBS_AVAILABLE; 
	}
	
	// Calculate the difference
	iAvailablePoints = ( iAvailablePoints -  iSelectedAttribs );
	
	// Check if the player has more attribpoints spent as available
	if ( iAvailablePoints < 0 )
	{
		// We have to reset all skills
		p_data_b[id][RESET_ATTRIBS] = true;
		ATTRIB_ResetAllNow( id );
			
		// jump out the loop and the function
		return;
	}
	
	// Save the available attribpoints
	p_data[id][AVAILABLE_ATTRIBS] = iAvailablePoints;
	
	return;
}
	
	
// Check selected attribute points
ATTRIB_SelectedAttribs( id )
{
	// Loop through all attribs and count the selected attrib points
	static i, iAttribCounter; iAttribCounter = 0;
	for ( i = 0; i < MAX_ATTRIBS; ++i )
	{
		iAttribCounter += ( p_data_attrib[id][i] - MAX_ATTRIBS_STARTPOINTS );
	}
	
	return iAttribCounter;
}

// Reset all attribs
public ATTRIB_ResetAll( id )
{
	// print him a message that all attribs will be reseted
	client_print( id, print_center, "Your attribs will be reset next round" );
	
	// We have to save it because on the next round we should reset all attribs
	p_data_b[id][RESET_ATTRIBS] = true;
}

// Reset now all attribs (called on new round or spawn if csdm enabled)
public ATTRIB_ResetAllNow( id )
{
	if ( p_data_b[id][RESET_ATTRIBS] )
	{
		// We loop through all attribs and set it to start points
		for ( new j = 0; j < MAX_ATTRIBS; ++j )
		{
			p_data_attrib[id][j] = MAX_ATTRIBS_STARTPOINTS;
		}
			
		// Detect new available attrib points
		ATTRIB_Check_Available_points( id )
			
		// Next round the attribs should no more reseted
		p_data_b[id][RESET_ATTRIBS] = false;
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
