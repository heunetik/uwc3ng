// Uwc3ng Handle functions

public HandleSay( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	// Get the complete text from the chat
	static talk[32], szArg1[32], szArg2[32];
	read_args( talk, charsmax( talk ) );
	
	// Convert the szArg1 to lowercase and without quotes
	strtolower( talk );
	remove_quotes( talk );
		
	// Parse the chat
	strbreak( talk, szArg1, charsmax( szArg1 ), szArg2, charsmax( szArg2 ) );
	new shopmenuItem = UTIL_IsItemShorthand(szArg1);
	
	// The player said level
	if ( equal( szArg1, "level" ) || equal( szArg1, "/level" ) )
	{
		COMMAND_Level( id );
	}
	
	// The player said selectskill
	else if ( equal( szArg1, "selectskill" ) || equal( szArg1, "/selectskill" ) )
	{
		// Execute the skill menu
		COMMAND_SkillMenu( id );
	}
	
	// The player said dropskill
	else if ( equal( szArg1, "dropskill" ) || equal( szArg1, "/dropskill" )  )
	{
		// Execute the drop skill menu
		COMMAND_DropSkillMenu( id );
	}
	
	// The player said resetskills
	else if ( equal( szArg1, "resetskills" ) || equal( szArg1, "/resetskills" ) )
	{
		// Execute the resetskill function
		COMMAND_ResetSkills( id );
	}
	
	// The player said shopmenu
	else if ( equal( szArg1, "shopmenu" ) || equal( szArg1, "/shopmenu" ) )
	{
		ITEM_ShowShopmenu( id, 0 );
	}
	
	// The player said shopmenu2
	else if ( equal( szArg1, "shopmenu2" ) || equal( szArg1, "/shopmenu2" ) )
	{
		ITEM_ShowShopmenu( id, 1 );
	}
	
	// the player said selectattrib
	else if ( equal( szArg1, "selectattrib" ) || equal( szArg1, "/selectattrib" ) )
	{
		COMMAND_SelectAttrib( id );
	}
	
	// The player said resetattribs
	else if ( equal( szArg1, "resetattribs" ) || equal( szArg1, "/resetattribs" ) )
	{
		COMMAND_ResetAttribs( id );
	}
	
	// The player said selectresist
	else if ( equal( szArg1, "selectresist" ) || equal( szArg1, "/selectresist" )  )
	{
		COMMAND_SelectResist( id );
	}
	
	// The player said resetresists
	else if ( equal( szArg1, "resetresists" ) || equal( szArg1, "/resetresists" ) )
	{
		COMMAND_ResetResists( id );
	}
	
	// The player said playerskills
	else if ( equal( szArg1, "playerskills" ) || equal( szArg1, "/playerskills" ) )
	{
		COMMAND_PlayerSkills( id );
	}
	
	// the player said who
	else if ( equal( szArg1, "who" ) || equal( szArg1, "/who" ) )
	{
		COMMAND_PlayerSkills( id );
	}
	
	// The player said charsheet
	else if ( equal( szArg1, "charsheet" ) || equal( szArg1, "/charhseet" ) )
	{
		COMMAND_Charsheet( id );
	}
	
	// The player said skillsinfo1
	else if ( equal( szArg1, "skillsinfo1" ) || equal( szArg1, "/skillsinfo1" ) )
	{
		MOTD_ShowSkillsHelp( id, 1 );
	}
	
	// The player said skillsinfo2
	else if ( equal( szArg1, "skillsinfo2" ) || equal( szArg1, "/skillsinfo2" ) )
	{
		MOTD_ShowSkillsHelp( id, 2 );
	}
	
	// The player said skillsinfo3
	else if ( equal( szArg1, "skillsinfo3" ) || equal( szArg1, "/skillsinfo3" ) )
	{
		MOTD_ShowSkillsHelp( id, 3 );
	}
	
	// The player said skillsinfo4
	else if ( equal( szArg1, "skillsinfo4" ) || equal( szArg1, "/skillsinfo4" ) )
	{
		MOTD_ShowSkillsHelp( id, 4 );
	}
	
	// The player said skillsinfo1
	else if ( equal( szArg1, "skillsinfo5" ) || equal( szArg1, "/skillsinfo5" ) )
	{
		MOTD_ShowSkillsHelp( id, 5 );
	}
	
	// The player said attribinfo
	else if ( equal( szArg1, "attribinfo" ) || equal( szArg1, "/attribinfo" ) )
	{
		COMMAND_ResistInfo( id );
	}

	// The player said resistinfo
	else if ( equal( szArg1, "resistinfo" ) || equal( szArg1, "/resistinfo" ) )
	{
		COMMAND_ResistInfo( id );
	}
	
	// The player said commands
	else if ( equal( szArg1, "commands" ) || equal( szArg1, "/commands" ) )
	{
		COMMAND_CommandsHelp( id );
	}
	
	// The player said tips
	else if ( equal( szArg1, "tips" ) || equal( szArg1, "/tips" ) )
	{
		COMMAND_ShowTips( id );
	}
	
	// The player said tips2
	else if ( equal( szArg1, "tips2" ) || equal( szArg1, "/tips2" ) )
	{
		COMMAND_ShowTips2( id );
	}
	
	// The player said deletexp
	else if ( equal( szArg1, "deletexp" ) || equal( szArg1, "/deletexp" ) )
	{
		COMMAND_DeleteXP( id );
	}
	
	// The player said changerace
	else if ( equal( szArg1, "changerace" ) || equal( szArg1, "/changerace" ) )
	{
		COMMAND_ChangeRace( id );
	}
	
	// The player said savexp
	else if ( equal( szArg1, "savexp" ) || equal( szArg1, "/savexp" ) )
	{
		COMMAND_SaveXP( id );
	}
	
	// The player said war3menu
	else if ( equal( szArg1, "war3menu" ) || equal( szArg1, "/war3menu" ) )
	{
		COMMAND_War3Menu( id );
	}
	
	// The player said whois
	else if ( equal( szArg1, "whois" ) || equal( szArg1, "/whois" ) )
	{
		MOTD_CharSheetPlayer( id, szArg2 );
	}
	
	// The player said wc3menu
	else if ( equal( szArg1, "wc3menu" ) || equal( szArg1, "/wc3menu" ) )
	{
		COMMAND_War3Menu( id );
	}
	
	// the player said help
	else if ( equal( szArg1, "help" ) || equal( szArg1, "/help" ) )
	{
		COMMAND_Help( id );
	}

	else if ( equal( szArg1, "rings" ) || equal( szArg1, "/rings" ) )
	{
		COMMAND_Rings( id );
	}

	// shopmenu items
	else if ( shopmenuItem != -1 )
	{
		ITEM_Buy( id, shopmenuItem );
	}
	
	return;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
