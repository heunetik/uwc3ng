// Uwc3ng clientcommands functions

// Someone want the skill menu (selectskill)
public COMMAND_SkillMenu( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	// Open the skill menu
	SKILL_ShowMenu( id, 0 );
	
	return PLUGIN_HANDLED;
}

// Someone want the drop skill menu (dropskill)
public COMMAND_DropSkillMenu( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	// Save that he want the drop skill menu
	p_data_b[id][SKILL_DROP] = true;
	
	// Open the skill menu
	SKILL_ShowMenu( id, 0 );
	
	return PLUGIN_HANDLED;
}

// Reset all skills from a player
public COMMAND_ResetSkills( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_ResetAll( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Repair Armor
public COMMAND_WcRepair( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_WcRepair( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Mend Wounds
public COMMAND_WcMend( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_WcMend( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Serpent Ward
public COMMAND_WcWard( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_PlaceSerpentWard( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Teleport
public COMMAND_WcTeleport( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_Teleport( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Dispell Hex
public COMMAND_WcDispell( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_DispellHex( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Big Bad Voodoo
public COMMAND_WcVoodoo( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_Big_Bad_Voodoo( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Gate
public COMMAND_WcGate( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_Gate( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Locust Swarm
public COMMAND_WcLocust( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_LocustSwarm( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Chain Lightnint
public COMMAND_WcLightning( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_ChainLightning( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Entangle Roots
public COMMAND_WcEntangle( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_EntangleRoots( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Flame Strike
public COMMAND_WcFlame( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_FlameStrike( id );
	
	return PLUGIN_HANDLED;
}

// Ultimate Suicide Bomber
public COMMAND_WcSuicide( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	SKILL_SuicideBomber( id );
	
	return PLUGIN_HANDLED;
}

// This is here to prevent the user from forcing a ResetHUD event
public COMMAND_fullupdate()
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}

	return PLUGIN_HANDLED;
}

// Open the shopmenu
public COMMAND_Shopmenu( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	ITEM_ShowShopmenu( id, 0 );
	
	return PLUGIN_HANDLED;
}

// Open the shopmenu2
public COMMAND_Shopmenu2( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	ITEM_ShowShopmenu( id, 1 );
	
	return PLUGIN_HANDLED;
}

// Buy all affordable rings
public COMMAND_Rings( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	ITEM_BuyRings( id );
	
	return PLUGIN_HANDLED;
}

// Open the select attrib menu
public COMMAND_SelectAttrib( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	ATTRIB_ShowSelectMenu( id );
	
	return PLUGIN_HANDLED;
}

// Execute resetattribs
public COMMAND_ResetAttribs( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	ATTRIB_ResetAll( id );
	
	return PLUGIN_HANDLED;
}

// Open the select resist menu
public COMMAND_SelectResist( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	RESIST_ShowSelectMenu( id );
	
	return PLUGIN_HANDLED;
}

// Execute resetresists
public COMMAND_ResetResists( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	RESIST_ResetAll( id );
	
	return PLUGIN_HANDLED;
}

// Execute level
public COMMAND_Level( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	client_print( id, print_chat, "%s Rank: %s [%d]   XP: %d", MOD_NAME, p_RankTitles[p_data[id][PLAYER_RANK]], p_data[id][PLAYER_LVL], p_data[id][PLAYER_XP] ); 
	
	return PLUGIN_HANDLED;
}

// Show playerskills
public COMMAND_PlayerSkills( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowPlayers( id );
	
	return PLUGIN_HANDLED;
}

// Show the charsheet
public COMMAND_Charsheet( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_CharacterSheet( id, 0 );
	
	return PLUGIN_HANDLED;
}

// Show whois motd
public COMMAND_Whois( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	new szCmd[32], szArg1[32];
	read_argv( 0, szCmd, 31 );
	read_argv( 1, szArg1, 31 );
		
	// Call the charsheet motd function
	if ( equal( szCmd, "whois" ) || equal( szCmd, "/whois" ) )
	{
		MOTD_CharSheetPlayer( id, szArg1 );
	}
	
	return PLUGIN_HANDLED;
}

// Skillsinformation 1
public COMMAND_Skills1( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowSkillsHelp( id, 1 );
	
	return PLUGIN_HANDLED;
}

// Skillsinformation 2
public COMMAND_Skills2( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowSkillsHelp( id, 2 );
	
	return PLUGIN_HANDLED;
}

// Skillsinformation 3
public COMMAND_Skills3( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowSkillsHelp( id, 3 );
	
	return PLUGIN_HANDLED;
}

// Skillsinformation 4
public COMMAND_Skills4( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowSkillsHelp( id, 4 );
	
	return PLUGIN_HANDLED;
}

// Skillsinformation 5
public COMMAND_Skills5( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowSkillsHelp( id, 5 );
	
	return PLUGIN_HANDLED;
}

// Attribinfo help
public COMMAND_AttribInfo( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowAttribsHelp( id );
	
	return PLUGIN_HANDLED;
}

// Resistanceinfo help
public COMMAND_ResistInfo( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowResistHelp( id );
	
	return PLUGIN_HANDLED;
}

// Show the commands help
public COMMAND_CommandsHelp( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowCommandHelp( id )
	
	return PLUGIN_HANDLED;
}

// Show tips
public COMMAND_ShowTips( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowTipsHelp( id );
	
	return PLUGIN_HANDLED;
}

// Show tips2
public COMMAND_ShowTips2( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowTips2Help( id );
	
	return PLUGIN_HANDLED;
}

// Delete all xps
public COMMAND_DeleteXP( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MENU_ResetSkills( id );
	
	return PLUGIN_HANDLED;
}

// Show changerace help 
public COMMAND_ChangeRace( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MOTD_ShowChangeraceHelp( id );
	
	return PLUGIN_HANDLED;
}

// SaveXP command
public COMMAND_SaveXP( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	// Check if we can save the xp
	if ( get_pcvar_num( CVAR_uwc3ng_save_xp ) )
	{
		client_print( id, print_chat, "%s Data is automatically saved", MOD_NAME );
	}
	
	else
	{
		client_print( id, print_chat, "%s SaveXP isn't enabled. Your XP will not automatically saved", MOD_NAME );
	}
	
	return PLUGIN_HANDLED;
}

// War3menu 
public COMMAND_War3Menu( id )
{
	// Make sure UWC3NG is loaded
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MENU_war3menu( id );
	
	return PLUGIN_HANDLED;
}

// This will be only called on bots if they do damage
public COMMAND_ExecuteUltimate( id )
{
	static iRandomNum;
	iRandomNum = random_num( 1, 8 );
	
	switch ( iRandomNum )
	{
		case 1: 
		{
			SKILL_PlaceSerpentWard( id );
		}
		
		case 2:
		{
			SKILL_Teleport( id );
		}
		
		case 3:
		{
			SKILL_Big_Bad_Voodoo( id );
		}
		
		case 4:
		{
			SKILL_Gate( id );
		}
		
		case 5:
		{
			SKILL_LocustSwarm( id );
		}
		
		case 6:
		{
			SKILL_ChainLightning( id );
		}
		
		case 7:
		{
			SKILL_FlameStrike( id );
		}
		
		case 8:
		{
			SKILL_EntangleRoots( id );
		}
	}
	
	return;
}

// Open the help menu
public COMMAND_Help( id )
{
	if ( !UWC3NG_Check() )
	{
		return PLUGIN_CONTINUE;
	}
	
	MENU_Helpmenu( id );
	
	return PLUGIN_HANDLED;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
