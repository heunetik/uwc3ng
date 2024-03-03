// Uwc3ng MySQL functions

// Maximum of tables
#define TOTAL_TABLES 4

new const szTables[TOTAL_TABLES][] = 
{
	"CREATE TABLE IF NOT EXISTS `uwc3ng_player` ( `player_id` int(8) unsigned NOT NULL auto_increment, `player_steamid` varchar(25) NOT NULL default '', `player_ip` varchar(20) NOT NULL default '', `player_name` varchar(35) NOT NULL default '', `player_xp` int(16) default NULL, PRIMARY KEY  (`player_id`), KEY `player_name` (`player_name`), KEY `player_ip` (`player_ip`), KEY `player_steamid` (`player_steamid`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `uwc3ng_skill` ( `player_id` int(8) unsigned NOT NULL default '0', `skill_id` int(8) default NULL, `skill_level` int(8) default NULL, PRIMARY KEY  (`player_id`,`skill_id` ) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `uwc3ng_attrib` ( `player_id` int(8) unsigned NOT NULL default '0', `attrib_id` int(8) default NULL, `attrib_level` int(8) default NULL, PRIMARY KEY (`player_id`, `attrib_id` ) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `uwc3ng_resist` ( `player_id` int(8) unsigned NOT NULL default '0', `resist_id` int(8) default NULL, `resist_level` int(8) default NULL, PRIMARY KEY (`player_id`, `resist_id` ) ) TYPE=MyISAM;"
};

// Initiate the connection to the MySQL database
public MYSQLX_Init()
{
	// Determine the database information
	new szHost[64], szUser[32], szPass[32], szDB[128], szError[256], iErrNum;

	get_pcvar_string( CVAR_uwc3ng_host, szHost, charsmax( szHost ) );
	get_pcvar_string( CVAR_uwc3ng_user, szUser, charsmax( szUser ) );
	get_pcvar_string( CVAR_uwc3ng_pass, szPass, charsmax( szPass ) );
	get_pcvar_string( CVAR_uwc3ng_name, szDB, charsmax( szDB ) );

	// Set up the tuple that will be used for threading
	g_DBTuple = SQL_MakeDbTuple( szHost, szUser, szPass, szDB );

	// Attempt to connect
	g_DBConn = SQL_Connect( g_DBTuple, iErrNum, szError, charsmax( szError ) );

	if ( !g_DBConn )
	{
		log_amx( "[MYSQLX] Database Connection Failed: [%d] %s", iErrNum, szError );

		return;
	}

	server_print( "%s MySQL X database connection successful", MOD_NAME );

	// Create tables!
	MYSQLX_CreateTables();
}

// Verifies that the database connection is ok
bool:MYSQLX_Connection_Available()
{
	if ( !g_DBConn )
	{
		return false;
	}

	return true;
}

// Create all of our tables!
MYSQLX_CreateTables()
{
	new Handle:query;

	// Create the default tables if we need to
	for ( new i = 0; i < TOTAL_TABLES; i++ )
	{
		query = SQL_PrepareQuery( g_DBConn, szTables[i] );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szTables[i], 1 );

			return;
		}

		SQL_FreeHandle( query );
	}
}

public MYSQLX_FetchUniqueID( id )
{
	// Make sure our connection is working
	if ( !MYSQLX_Connection_Available() )
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
		MYSQLX_Error( query, szQuery, 2 );

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
			MYSQLX_Error( query, szQuery, 3 );

			return;
		}

		// We read the new UniqueID
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

public MYSQLX_Save( id )
{
	// Make sure our connection is working
	if ( !MYSQLX_Connection_Available() )
	{
		return;
	}

	// Error checking when saving
	if ( p_data[id][PLAYER_UNIQUEID] <= 0 )
	{
		// Try again to fetch the uniqueID
		MYSQLX_FetchUniqueID( id );
		
		// Okay, we can't really get the uniqueID
		if ( p_data[id][PLAYER_UNIQUEID] <= 0 )
		{
			new szName[128];
			get_user_name( id, szName, 127 );

			log_amx( "Unable to save XP for user '%s', unique ID: %d", szName, p_data[id][PLAYER_UNIQUEID] );
			
			return;
		}
	}
	
	new szKey[66], szKeyName[32];
	Save_GetKey( id, szKey, charsmax( szKey ) );
	Save_GetKeyName( szKeyName, charsmax( szKeyName ) );

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, charsmax( szQuery ), "REPLACE INTO `uwc3ng_player` ( `player_id` , `%s` , `player_xp` ) VALUES ( '%d', '%s', '%d');", szKeyName, p_data[id][PLAYER_UNIQUEID], szKey, p_data[id][PLAYER_XP] );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		client_print( id, print_chat, "%s Error, unable to save your XP, please contact a server administrator", MOD_NAME );

		MYSQLX_Error( query, szQuery, 4 );

		return;
	}
	
	// We have to save the skills
	static i;
	for ( i = 0; i < MAX_SKILLS; ++i )
	{
		format( szQuery, charsmax( szQuery ), "REPLACE INTO `uwc3ng_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", p_data[id][PLAYER_UNIQUEID], i, p_data_skill[id][i] ); 
		query = SQL_PrepareQuery( g_DBConn, szQuery );
			
		// Check if the DB ready
		if ( !SQL_Execute( query ) )
		{
			client_print( id, print_chat, "%s Error, unable to save your XP, please contact a server administrator", MOD_NAME );
	
			MYSQLX_Error( query, szQuery, 5 );

			return;
		}
	}
		
	// We have to save the attribs
	for ( i = 0; i < MAX_ATTRIBS; ++i )
	{
		format( szQuery, charsmax( szQuery ), "REPLACE INTO `uwc3ng_attrib` ( `player_id` , `attrib_id` , `attrib_level` ) VALUES ( '%d', '%d', '%d' );", p_data[id][PLAYER_UNIQUEID], i, p_data_attrib[id][i] ); 
		query = SQL_PrepareQuery( g_DBConn, szQuery );
			
		// Check if the DB ready
		if ( !SQL_Execute( query ) )
		{
			client_print( id, print_chat, "%s Error, unable to save your XP, please contact a server administrator", MOD_NAME );
	
			MYSQLX_Error( query, szQuery, 5 );

			return;
		}
	}
		
	// We have to save the resistans
	for ( i = 0; i < MAX_RESIST; ++i )
	{
		format( szQuery, charsmax( szQuery ), "REPLACE INTO `uwc3ng_resist` ( `player_id` , `resist_id` , `resist_level` ) VALUES ( '%d', '%d', '%d' );", p_data[id][PLAYER_UNIQUEID], i, p_data_resist[id][i] ); 
		query = SQL_PrepareQuery( g_DBConn, szQuery );
			
		// Check if the DB ready
		if ( !SQL_Execute( query ) )
		{
			client_print( id, print_chat, "%s Error, unable to save your XP, please contact a server administrator", MOD_NAME );
	
			MYSQLX_Error( query, szQuery, 5 );

			return;
		}
	}
	
	return;
}

public MYSQLX_Save_T( id )
{
	// Make sure our connection is working
	if ( !MYSQLX_Connection_Available() )
	{
		return;
	}

	// Error checking when saving
	if ( p_data[id][PLAYER_UNIQUEID] <= 0 )
	{
		// Try again to fetch the uniqueID
		MYSQLX_FetchUniqueID( id );
		
		// Okay, we can't really get the uniqueID
		if ( p_data[id][PLAYER_UNIQUEID] <= 0 )
		{
			new szName[128];
			get_user_name( id, szName, 127 );

			log_amx( "Unable to save XP for user '%s', unique ID: %d", szName, p_data[id][PLAYER_UNIQUEID] );
			
			return;
		}
	}

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, charsmax( szQuery ), "UPDATE `uwc3ng_player` SET `player_xp`='%d' WHERE `player_id`='%d'", p_data[id][PLAYER_XP], p_data[id][PLAYER_UNIQUEID] );
	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save_T", szQuery );
	
	// Now we need to save the skill levels!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		// Then we need to save this!
		if ( p_data_skill[id][iSkillID] != 0 )
		{
			format( szQuery, 511, "REPLACE INTO `uwc3ng_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", p_data[id][PLAYER_UNIQUEID], iSkillID, p_data_skill[id][iSkillID] );
			SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save_T", szQuery );
		}
	}
	
	// Now we need to save the attrib levels!
	for ( new iAttribID = 0; iAttribID < MAX_ATTRIBS; iAttribID++ )
	{
		// Then we need to save this!
		if ( p_data_attrib[id][iAttribID] != MAX_ATTRIBS_STARTPOINTS )
		{
			format( szQuery, 511, "REPLACE INTO `uwc3ng_attrib` ( `player_id` , `attrib_id` , `attrib_level` ) VALUES ( '%d', '%d', '%d' );", p_data[id][PLAYER_UNIQUEID], iAttribID, p_data_attrib[id][iAttribID] );
			SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save_T", szQuery );
		}
	}
	
	// Now we need to save the resistant levels!
	for ( new iResistID = 0; iResistID < MAX_RESIST; iResistID++ )
	{
		// Then we need to save this!
		if ( p_data_resist[id][iResistID] != 0 )
		{
			format( szQuery, 511, "REPLACE INTO `uwc3ng_resist` ( `player_id` , `resist_id` , `resist_level` ) VALUES ( '%d', '%d', '%d' );", p_data[id][PLAYER_UNIQUEID], iResistID, p_data_resist[id][iResistID] );
			SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save_T", szQuery );
		}
	}

	return;
}

public _MYSQLX_Save_T( failstate, Handle:query, error[], errnum, data[], size )
{

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		SQL_GetQueryString( query, szQuery, charsmax( szQuery ) );
		
		MYSQLX_ThreadError( query, szQuery, error, errnum, failstate, 1 );
	}
}

public MYSQLX_GetAllXP( id )
{
	// Make sure our connection is working
	if ( !MYSQLX_Connection_Available() )
	{
		return;
	}
	
	// Error checking when loading
	if ( p_data[id][PLAYER_UNIQUEID] <= 0 )
	{
		// Try again to fetch the uniqueID
		MYSQLX_FetchUniqueID( id );
		
		// Okay, we can't really get the uniqueID
		if ( p_data[id][PLAYER_UNIQUEID] <= 0 )
		{
			new szName[128];
			get_user_name( id, szName, 127 );

			log_amx( "Unable to retrieve XP for user '%s', unique ID: %d", szName, p_data[id][PLAYER_UNIQUEID] );
			
			return;
		}
	}

	new szQuery[256];
	format(szQuery, charsmax( szQuery ), "SELECT `player_xp` FROM `uwc3ng_player` WHERE ( `player_id` = '%d' )", p_data[id][PLAYER_UNIQUEID] );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		client_print( id, print_chat, "%s Error, unable to retrieve XP, please contact a server administrator", MOD_NAME );

		MYSQLX_Error( query, szQuery, 6 );

		return;
	}

	// Set last saved XP to 0
	p_data[id][PLAYER_XP] = 0;

	// Read the XPs 
	p_data[id][PLAYER_XP] = SQL_ReadResult( query, 0 );
		
	// Free the handle
	SQL_FreeHandle( query );
	
	// Select the right Table
	format( szQuery, charsmax( szQuery ), "SELECT `skill_id`, `skill_level` FROM `uwc3ng_skill` WHERE ( `player_id` = '%d' );", p_data[id][PLAYER_UNIQUEID] );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	// Check if the DB ready
	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error ( query, szQuery, 6 );

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
	
	// Select the right Table
	format( szQuery, charsmax( szQuery ), "SELECT `attrib_id`, `attrib_level` FROM `uwc3ng_attrib` WHERE ( `player_id` = '%d' );", p_data[id][PLAYER_UNIQUEID] );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	// Check if the DB ready
	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error ( query, szQuery, 6 );

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
	
	// Select the right Table
	format( szQuery, charsmax( szQuery ), "SELECT `resist_id`, `resist_level` FROM `uwc3ng_resist` WHERE ( `player_id` = '%d' );", p_data[id][PLAYER_UNIQUEID] );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	// Check if the DB ready
	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error ( query, szQuery, 6 );

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

public MYSQLX_Close()
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

// The id should be a unique number, so we know what function called it (useful for debugging)
MYSQLX_Error( Handle:query, szQuery[], id )
{
	new szError[256];
	new iErrNum = SQL_QueryError( query, szError, 255 );

	log_amx( "[MYSQLX] Error in querying database, location: %d", id );
	log_amx( "[MYSQLX] Message: %s (%d)", szError, iErrNum );
	log_amx( "[MYSQLX] Query statement: %s ", szQuery );

	// Free the handle
	SQL_FreeHandle( query );

	// MySQL server has gone away (2006)
	if ( iErrNum == 2006 )
	{
		log_amx( "[MYSQLX] Attempting to re-establish connection to MySQL server" );
		
		// Close the connection
		MYSQLX_Close();

		// Re-open the connection
		MYSQLX_Init();
	}
}

MYSQLX_ThreadError( Handle:query, szQuery[], szError[], iErrNum, failstate, id )
{
	log_amx( "[MYSQLX] Threaded query error, location: %d", id );
	log_amx( "[MYSQLX] Message: %s (%d)", szError, iErrNum );
	log_amx( "[MYSQLX] Query statement: %s ", szQuery );

	// Connection failed
	if ( failstate == TQUERY_CONNECT_FAILED )
	{	
		log_amx( "[MYSQLX] Fail state: Connection Failed" );
	}

	// Query failed
	else if ( failstate == TQUERY_QUERY_FAILED )
	{
		log_amx( "[MYSQLX] Fail state: Query Failed" );
	}

	// Free the handle
	SQL_FreeHandle( query );
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
