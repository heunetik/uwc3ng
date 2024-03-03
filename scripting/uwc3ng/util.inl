// Uwc3ng Util functions (from wc3ft)

public UTIL_GetWeaponName( iWeapon, szWpnName[], len )
{
	switch ( iWeapon )
	{
		case CSW_KNIFE:			copy( szWpnName, len, "knife"		);
		case CSW_USP:           copy( szWpnName, len, "usp"			);
		case CSW_GLOCK18:       copy( szWpnName, len, "glock18"		);
		case CSW_DEAGLE:        copy( szWpnName, len, "deagle"		);
		case CSW_P228:          copy( szWpnName, len, "p228"		);
		case CSW_ELITE:         copy( szWpnName, len, "elite"		);
		case CSW_FIVESEVEN:     copy( szWpnName, len, "fiveseven"	);
		case CSW_M3:            copy( szWpnName, len, "m3"			);
		case CSW_XM1014:        copy( szWpnName, len, "xm1014"		);
		case CSW_MP5NAVY:       copy( szWpnName, len, "mp5navy"		);
		case CSW_TMP:           copy( szWpnName, len, "tmp"			);
		case CSW_P90:           copy( szWpnName, len, "p90"			);
		case CSW_MAC10:         copy( szWpnName, len, "mac10"		);
		case CSW_UMP45:         copy( szWpnName, len, "ump45"		);
		case CSW_AK47:          copy( szWpnName, len, "ak47"		);
		case CSW_SG552:         copy( szWpnName, len, "sg552"		);
		case CSW_M4A1:          copy( szWpnName, len, "m4a1"		);
		case CSW_AUG:           copy( szWpnName, len, "aug"			);
		case CSW_SCOUT:         copy( szWpnName, len, "scout"		);
		case CSW_AWP:           copy( szWpnName, len, "awp"			);
		case CSW_SG550:         copy( szWpnName, len, "sg550"		);
		case CSW_G3SG1:         copy( szWpnName, len, "g3sg1"		);
		case CSW_GALI:          copy( szWpnName, len, "galil"		);
		case CSW_FAMAS:         copy( szWpnName, len, "famas"		);
		case CSW_M249:          copy( szWpnName, len, "m249"		);
		case CSW_HEGRENADE:     copy( szWpnName, len, "grenade"		);
		case CSW_WORLDSPAWN:    copy( szWpnName, len, "world"													);

		// Custom "weapons"
		case CSW_LIGHTNING: copy( szWpnName, len, "Chain Lightning" );
		case CSW_SUICIDE: copy( szWpnName, len, "Suicide Bomber" );
		case CSW_IMMOLATE: copy( szWpnName, len, "Immolate" );
		case CSW_LOCUSTS: copy( szWpnName, len, "Locust Swarm" );
		case CSW_SERPENTWARD: copy( szWpnName, len, "Serpent Ward" );
		case CSW_SHADOW: copy( szWpnName, len, "Shadow Strike" );
		case CSW_THORNS: copy( szWpnName, len, "Thorns Aura" );
		case CSW_CARAPACE: copy( szWpnName, len, "Spiked Carapace" );
		case CSW_CARRION: copy( szWpnName, len, "Carrion Beetles" );
		case CSW_ORB: copy( szWpnName, len, "Orb of Annihilation" );
		case CSW_CONCOCTION: copy( szWpnName, len, "Unstable Concoction" );
		case CSW_BANISH: copy( szWpnName, len, "Banish" );
	}
}

// Log the death of a user (from wc3ft)
UTIL_LogDeath( iVictim, iKiller, iWeapon )
{
	new szKillerName[32], szVictimName[32];
	new szKillerTeam[32], szVictimTeam[32];
	new szKillerAuth[32], szVictimAuth[32];
	new szWeaponName[32];

	new iKillerUserID = get_user_userid( iKiller );
	new iVictimUserID = get_user_userid( iVictim );

	UTIL_GetWeaponName( iWeapon, szWeaponName, 31 );

	get_user_name(		iKiller	, szKillerName	, 31 );
	get_user_name(		iVictim	, szVictimName	, 31 );
	get_user_team(		iKiller	, szKillerTeam	, 31 );
	get_user_team(		iVictim	, szVictimTeam	, 31 );
	get_user_authid(	iKiller	, szKillerAuth	, 31 );
	get_user_authid(	iVictim	, szVictimAuth	, 31 );

	log_message("^"%s<%d><%s><%s>^" killed ^"%s<%d><%s><%s>^" with ^"%s^"", szKillerName, iKillerUserID, szKillerAuth, szKillerTeam, szVictimName, iVictimUserID, szVictimAuth, szVictimTeam, szWeaponName );
}

public _UTIL_CheckMinModelsValue( id, const cvar[], const value[] )
{
	if ( equali( cvar,"cl_minmodels" ) && strcmp( value, "0" ) )
	{
		client_cmd( id, "echo ^"======== Ultimate Warcraft 3 Next Generation ========^"" );
		client_cmd( id, "echo ^"You were kicked because cl_minmodels is set to 1 on your client, please change this to 0.^"" );
		client_cmd( id, "echo ^"Type ^"cl_minmodels 0^" in your console and press enter to do this.^"" );
		client_cmd( id, "echo ^"=========================================^"" );
		server_cmd( "kick #%d ^"cl_minmodels 1 is not allowed on this server^"", get_user_userid( id ) );
	} 
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
