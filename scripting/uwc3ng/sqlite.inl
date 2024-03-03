// Uwc3ng SQLITE functions

#define TOTAL_SQLITE_TABLES 4

new const szTablesSQLite[TOTAL_SQLITE_TABLES][] = 
{
	"CREATE TABLE `uwc3ng_player` ( `player_id` INTEGER PRIMARY KEY AUTOINCREMENT, `player_steamid` varchar(25) NOT NULL default '', `player_ip` varchar(20) NOT NULL default '', `player_name` varchar(35) NOT NULL default '', `player_xp` int(16) default NULL );",
	"CREATE TABLE `uwc3ng_skill` ( `player_id` int(8) NOT NULL default '0', `skill_id` int(8) NOT NULL default '0', `skill_level` int(8) default NULL, PRIMARY KEY  (`player_id`,`skill_id`) );",
	"CREATE TABLE `uwc3ng_attrib` ( `player_id` int(8) NOT NULL default '0', `attrib_id` int(8) NOT NULL default '0', `attrib_level` int(8) NOT NULL default '0', PRIMARY KEY  (`player_id`,`attrib_id`) );",
	"CREATE TABLE `uwc3ng_resist` ( `player_id` int(8) NOT NULL default '0', `resist_id` int(8) NOT NULL default '0', `resist_level` int(8) NOT NULL default '0', PRIMARY KEY  (`player_id`,`resist_id`) );"
};

new const szTableNames[TOTAL_SQLITE_TABLES][] = 
{
	"uwc3ng_player",
	"uwc3ng_skill",
	"uwc3ng_attrib",
	"uwc3ng_resist"
};

// Initiate the connection to the SQLite database
public SQLITE_Init()
{
	new szError[256], iErrNum;

	// Set up the tuple that will be used for threading
	g_DBTuple = SQL_MakeDbTuple( "", "", "", "uwc3ng" );

	// Attempt to connect
	g_DBConn = SQL_Connect( g_DBTuple, iErrNum, szError, 255 );

	if ( !g_DBConn )
	{
		log_amx( "[SQLITE] Database Connection Failed: [%d] %s", iErrNum, szError );

		return;
	}


	server_print( "%s SQLite database connection successful", MOD_NAME );

	new Handle:query;

	// Create the default tables if we need to
	for ( new i = 0; i < TOTAL_SQLITE_TABLES; i++ )
	{
		query = SQL_PrepareQuery( g_DBConn, szTablesSQLite[i] );

		if ( !sqlite_TableExists( g_DBConn, szTableNames[i] ) )
		{
			if ( !SQL_Execute( query ) )
			{
				SQLITE_Error( query, szTablesSQLite[i], 1 );

				return;
			}
		}

		SQL_FreeHandle( query );
	}

	/*
		These probably should be subject to a CVAR
		Lets fine tune the database:
			"synchronous = NORMAL"	- Put back the 2.x behaviour (faster than the defalt
						  for 3.x)
			"synchronous = OFF"	- Way faster, but it might get corrupted data if a
						  server os system crash occurs
			"integrity_check"	- well it's what it says, we do have to check the
						  value it returns since it's important
		PRAGMA commands don't return anything so no need to check the result of the query
	*/	

	query = SQL_PrepareQuery( g_DBConn, "PRAGMA integrity_check" );
	
	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, "PRAGMA integrity_check", 2 );

		return;
	}
	
	// Get the integrity check value
	new szIntegrityCheck[64];
	if ( SQL_NumResults( query ) > 0 )
	{
		SQL_ReadResult( query, 0, szIntegrityCheck, charsmax( szIntegrityCheck ) );
	}

	// Free the result
	SQL_FreeHandle( query );

	// Check to make sure the integrity check passed
	if ( !equali(szIntegrityCheck, "ok") )
	{
		// Should we disable saving here?
		log_amx( "[SQLITE] SQL Lite integrity check failed, disabling saving XP." );

		set_pcvar_num( CVAR_uwc3ng_save_xp, 0 );

		return;
	}
	
	// Do some synchronous crap
	new szQuery[128];
	format( szQuery, charsmax( szQuery ), "PRAGMA synchronous = %d", SQLITE_SYNC_OFF );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 3 );

		return;
	}
	
	// Free the last handle
	SQL_FreeHandle( query );
}

// Close the SQLite connection
public SQLITE_Close()
{
	if ( g_DBTuple )
	{
		SQL_FreeHandle( g_DBTuple );
	}

	if ( g_DBConn )
	{
		SQL_FreeHandle( g_DBConn );
	}
}

public SQLITE_FetchUniqueID( id )
{
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}

	new szKey[66], szKeyName[32];
	Save_GetKey( id, szKey, charsmax( szKey ) );
	Save_GetKeyName( szKeyName, charsmax( szKeyName ) );

	new szQuery[512];
	format( szQuery, charsmax( szQuery ), "SELECT `player_id` FROM `uwc3ng_player` WHERE `%s` = '%s';", szKeyName, szKey );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 4 );

		return;
	}
	
	// If no rows we need to insert!
	if ( SQL_NumResults( query ) == 0 )
	{
		// Free the last handle!
		SQL_FreeHandle( query );

		// Insert this player!
		format( szQuery, charsmax( szQuery ), "INSERT INTO `uwc3ng_player` ( `player_id` , `%s` ) VALUES ( NULL , '%s' );", szKeyName, szKey );
		query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			SQLITE_Error( query, szQuery, 5 );

			return;
		}

		p_data[id][PLAYER_UNIQUEID] = SQL_GetInsertId( query );
	}

	// They have been here before - store their ID
	else
	{
		p_data[id][PLAYER_UNIQUEID] = SQL_ReadResult( query, 0 );
	}

	// Free the last handle!
	SQL_FreeHandle( query );
}

public SQLITE_Save( id )
{
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}
	
	// Get the UniqueID
	new iUniqueID = SAVE_GetUniqueID( id );
	
	new szKey[66], szKeyName[32];
	Save_GetKey( id, szKey, charsmax( szKey ) );
	Save_GetKeyName( szKeyName, charsmax( szKeyName ) );

	// Save the user's XP!
	new szQuery[512];
	formatex( szQuery, charsmax( szQuery ), "REPLACE INTO `uwc3ng_player` ( `player_id` , `%s` , `player_xp` ) VALUES ( '%d', '%s', '%d');", szKeyName, iUniqueID, szKey, p_data[id][PLAYER_XP] );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 6 );

		return;
	}

	// Now we need to save the skill levels!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		formatex( szQuery, 511, "REPLACE INTO `uwc3ng_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iSkillID, p_data_skill[id][iSkillID] );
		query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			SQLITE_Error( query, szQuery, 7 );

			return;
		}
	}
	
	// Now we need to save the attrib levels!
	for ( new iAttribID = 0; iAttribID < MAX_ATTRIBS; iAttribID++ )
	{
		formatex( szQuery, 511, "REPLACE INTO `uwc3ng_attrib` ( `player_id` , `attrib_id` , `attrib_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iAttribID, p_data_attrib[id][iAttribID] );
		query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			SQLITE_Error( query, szQuery, 7 );

			return;
		}
	}
	
	// Now we need to save the resist levels!
	for ( new iResistID = 0; iResistID < MAX_RESIST; iResistID++ )
	{
		formatex( szQuery, 511, "REPLACE INTO `uwc3ng_resist` ( `player_id` , `resist_id` , `resist_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iResistID, p_data_resist[id][iResistID] );
		query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			SQLITE_Error( query, szQuery, 7 );

			return;
		}
	}
	
	return;
}

public SQLITE_GetAllXP( id )
{
	// Make sure our connection is working
	if ( !SQLITE_Connection_Available() )
	{
		return;
	}

	// Then we have a problem and cannot retreive the user's XP
	if ( p_data[id][PLAYER_UNIQUEID] <= 0 )
	{
		client_print( id, print_chat, "%s Unable to retreive your XP from the database", MOD_NAME );

		log_amx( "[ERROR] Unable to retreive user's Unique ID" );

		return;
	}

	new szQuery[256];
	format(szQuery, charsmax( szQuery ), "SELECT `player_xp` FROM `uwc3ng_player` WHERE ( `player_id` = '%d' );", p_data[id][PLAYER_UNIQUEID] );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 8 );

		return;
	}

	// Set last saved XP to 0
	p_data[id][PLAYER_XP] = 0;

	// Read the XPs from the table
	p_data[id][PLAYER_XP] = SQL_ReadResult( query, 0 );
		
	// Free the handle
	SQL_FreeHandle( query );
	
	// Select the right Table
	format( szQuery, charsmax( szQuery ), "SELECT `skill_id`, `skill_level` FROM `uwc3ng_skill` WHERE ( `player_id` = '%d' );", p_data[id][PLAYER_UNIQUEID] );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	// Check if the DB ready
	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error ( query, szQuery ,8 );

		return;
	}
	
	// While we have a result!
	new iSkillID = 0;
	while ( SQL_MoreResults( query ) )
	{
		iSkillID = SQL_ReadResult( query, 0 );
		p_data_skill[id][iSkillID] = SQL_ReadResult( query, 1 );

		SQL_NextRow( query );
	}
	
	// Close the connection
	SQL_FreeHandle( query );
	
	// Select the right Table
	format( szQuery, charsmax( szQuery ), "SELECT `attrib_id`, `attrib_level` FROM `uwc3ng_attrib` WHERE ( `player_id` = '%d' );", p_data[id][PLAYER_UNIQUEID] );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	// Check if the DB ready
	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error ( query, szQuery, 8 );

		return;
	}
	
	// While we have a result!
	new iAttribID = 0;
	while ( SQL_MoreResults( query ) )
	{
		iAttribID = SQL_ReadResult( query, 0 );
		p_data_attrib[id][iAttribID] = SQL_ReadResult( query, 1 );

		SQL_NextRow( query );
	}
	
	// Close the connection
	SQL_FreeHandle( query );
	
	// Select the right Table
	format( szQuery, charsmax( szQuery ), "SELECT `resist_id`, `resist_level` FROM `uwc3ng_resist` WHERE ( `player_id` = '%d' );", p_data[id][PLAYER_UNIQUEID] );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	// Check if the DB ready
	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error ( query, szQuery, 8 );

		return;
	}
	
	// While we have a result!
	new iResistID = 0;
	while ( SQL_MoreResults( query ) )
	{
		iResistID = SQL_ReadResult( query, 0 );
		p_data_resist[id][iResistID] = SQL_ReadResult( query, 1 );

		SQL_NextRow( query );
	}
	
	// Close the connection
	SQL_FreeHandle( query );

	return;
}

// Verifies that the database connection is ok
bool:SQLITE_Connection_Available()
{
	if ( !g_DBConn )
	{
		return false;
	}

	return true;
}

// The id should be a unique number, so we know what function called it (useful for debugging)
SQLITE_Error( Handle:query, szQuery[], id )
{
	new szError[256];
	new iErrNum = SQL_QueryError( query, szError, 255 );

	log_amx( "[SQLITE] Error in querying database, location: %d", id );
	log_amx( "[SQLITE] Message: %s (%d)", szError, iErrNum );
	log_amx( "[SQLITE] Query statement: %s ", szQuery );

	// Free the handle
	SQL_FreeHandle( query );
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
