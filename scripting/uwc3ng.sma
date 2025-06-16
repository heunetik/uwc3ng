/* AMX Mod X
*   Ultimate Warcraft 3 Next Generation
*
*  by DA
*  http://www.wc3mods.net
*  http://www.fun-arena.com
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 2 of the License, or (at
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software Foundation,
*  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*  In addition, as a special exception, the author gives permission to
*  link the code of this program with the Half-Life Game Engine ("HL
*  Engine") and Modified Game Libraries ("MODs") developed by Valve,
*  L.L.C ("Valve"). You must obey the GNU General Public License in all
*  respects for all of the code used other than the HL Engine and MODs
*  from Valve. If you modify this file, you may extend this exception
*  to your version of the file, but you are not obligated to do so. If
*  you do not wish to do so, delete this exception statement from your
*  version.
*/

#pragma tabsize	0

#include <amxmodx>
#include <amxmisc>
#include <hamsandwich>
#include <cstrike>
#include <dbi>
#include <sqlx>
#include <fun>
#include <fakemeta>
#include <engine>
#include <csx>

#define VERSION "2.40.4"

// --- Main includes ---
#include "uwc3ng/constants.inl"		// Load the constants include file
#include "uwc3ng/vars.inl"		// Load the variables include file
#include "uwc3ng/save_common.inl"	// Load the saves include file (how and where we save all things)
#include "uwc3ng/forwards.inl" 		// Load the forwards file
#include "uwc3ng/cvars.inl" 		// Load the cvars include file
#include "uwc3ng/mysql.inl"		// Load the mysqlx include file
#include "uwc3ng/sqlite.inl"		// Load the sqlite include file
#include "uwc3ng/XP.inl"		// Load the xp include file
#include "uwc3ng/graphics.inl"		// Load the graphics file
#include "uwc3ng/events.inl"		// Load the events file
#include "uwc3ng/handle.inl"		// Load the handle file
#include "uwc3ng/player.inl"		// Load the player file
#include "uwc3ng/admin.inl"		// Load the admin file
#include "uwc3ng/clientcommands.inl"	// Load the clientcommands file
#include "uwc3ng/uwc3ng.inl"		// Load the uwc3ng file
#include "uwc3ng/shared.inl"		// Load the SHARED file
#include "uwc3ng/util.inl"		// Load the util file
#include "uwc3ng/motd.inl"		// Load the motd file
#include "uwc3ng/menu.inl"		// Load the menus file

// --- Skill includes ---
#include "uwc3ng/skillhandle.inl"	// Load the skill handle file
#include "uwc3ng/skills1.inl"		// Load the skills1 file (first 0-7 skills)
#include "uwc3ng/skills2.inl"		// Load the skills2 file (8-15 skills)
#include "uwc3ng/skills3.inl"		// Load the skills3 file (16-24 skills)
#include "uwc3ng/skills4.inl"		// Load the skills4 file (25-33 skills)
#include "uwc3ng/skills5.inl"		// Load the skills5 file (34-42 skills)

// --- Ultimate includes ---
#include "uwc3ng/ultimates.inl"		// Load the ultimates file

// --- Items ---
#include "uwc3ng/items.inl"		// Load the items file

// --- Attributes ---
#include "uwc3ng/attribs.inl"		// Load the attribs file

// --- Resistance ---
#include "uwc3ng/resist.inl"		// Load the resist file

public plugin_init() 
{
	register_plugin ( "UWC3 Next Generation", VERSION, "DA" );
	register_cvar ( "amx_uwc3ng", VERSION, FCVAR_SERVER );
	
	// Event's
	register_event ( "HLTV", "on_NewRound", "a", "1=0", "2=0" );
	register_event ( "SendAudio", "on_TerroristWin", "a", "2=%!MRAD_terwin" );
	register_event ( "SendAudio", "on_CTWin"	, "a", "2=%!MRAD_ctwin" );
	register_event ( "WeapPickup", "on_WeapPickup", "b" ); 
	register_event ( "CurWeapon", "on_CurWeapon", "be", "1=1" );
	register_event ( "ArmorType", "on_ArmorType", "be" );
	register_event ( "DeathMsg", "on_DeathMsg", "a" );
	register_event ( "SetFOV", "on_Zoom", "be" );
	register_event ( "Battery", "on_Battery"	, "be" );					
	register_logevent ( "on_EndRound", 2, "0=World triggered", "1=Round_End" );
	register_logevent ( "on_PlayerAction", 3, "1=triggered" );
	register_logevent ( "on_FreezeTimeComplete", 2, "0=World triggered", "1=Round_Start" );
	
	// hmm, a sandwich!
	RegisterHam ( Ham_Spawn, "player", "on_PlayerSpawn", 1 );
	RegisterHam ( Ham_TraceAttack, "player", "ham_TraceAttack" );
	
	// From wc3ft
	gmsgStatusText = get_user_msgid( "StatusText" );
	gmsgScreenFade = get_user_msgid( "ScreenFade" );
	gmsgScreenShake	= get_user_msgid( "ScreenShake"	);
	gmsgDeathMsg = get_user_msgid( "DeathMsg" );
	gmsgScoreInfo = get_user_msgid( "ScoreInfo" );
	gmsgBarTime = get_user_msgid( "BarTime" );
	
	// Register forwards (from fakemeta)
	register_forward ( FM_TraceLine, "TRIGGER_TraceLine" );
	register_forward ( FM_PlayerPreThink, "fwd_playerprethink" );
	
	// Register the menu's
	register_menu ( "HandleSkill", 1023, "HandlePressMenu" );
	register_menu ( "Shopmenu12", 1023, "_ITEM_HandleShopMenu" );
	register_menu ( "ShopmenuReplace", 1023, "ITEM_HandleReplaceItem" );
	register_menu ( "AttribMenu", 1023, "ATTRIB_HandleMenu" );
	register_menu ( "ResistMenu", 1023, "RESIST_HandleMenu" );
	register_menu ( "War3Menu", 1023, "MENU_HandleWar3" );
	register_menu ( "Skillsmenu", 1023, "MENU_HandleSkills" );
	register_menu ( "Itemsmenu", 1023, "MENU_HandleItems" );
	register_menu ( "Helpmenu", 1023, "MENU_HandleHelp" );
	register_menu ( "ResetSkillmenu", 1023, "MENU_HandleResetSkills" );
		
	// Load the cvars
	Load_Cvars();
	
	// Set up the items
	ITEM_Init();
	
	// Load the cvars from the cfg
	set_task( 0.4, "Load_Uwc3NG_CFG" );
	
	// Set up the XP table
	set_task( 0.8, "XP_SetUpTable" );
	
	// Determine the typ where we save our data
	set_task ( 1.0, "DetermineType" );
	
	// Start our connecrtions
	set_task ( 1.2, "Save_Init" );
	
	// Run the ultimate delay task
	set_task( 1.0, "ULTIMATE_Delay", TASK_UDELAY, "", 0, "b" );
	
	// Find the map objectives
	SHARED_FindObjectives();
	
	// Register all forwards
	FWD_Register()
}

public plugin_precache()
{
	// Path for the sounds
	copy( g_szSounds[SOUND_LEVELUP], 63, "uwc3ng/levelupcaster.wav" );
	copy( g_szSounds[SOUND_REINCARNATION], 63, "uwc3ng/reincarnation.wav" );
	copy( g_szSounds[SOUND_IMPALE], 63, "uwc3ng/impalehit.wav" );
	copy( g_szSounds[SOUND_ULTIMATEREADY], 63, "uwc3ng/resurrecttarget.wav" );
	copy( g_szSounds[SOUND_HEX], 63, "uwc3ng/PolymorphDone.wav" );
	copy( g_szSounds[SOUND_BANISH], 63, "uwc3ng/banishcaster.wav" );
	copy( g_szSounds[SOUND_BLINK], 63, "uwc3ng/blinkarrival.wav" );
	copy( g_szSounds[SOUND_CARRION], 63, "uwc3ng/carrionswarmlaunch1.wav" );
	copy( g_szSounds[SOUND_SHADOWSTRIKE], 63, "uwc3ng/shadowstrikebirth.wav" );
	copy( g_szSounds[SOUND_DISPELL], 63, "uwc3ng/DispelMagicTarget.wav" );
	copy( g_szSounds[SOUND_VOODOO], 63, "uwc3ng/divineshield.wav" );
	copy( g_szSounds[SOUND_TELEPORT], 63, "uwc3ng/massteleporttarget.wav" );
	copy( g_szSounds[SOUND_LOCUSTSWARM], 63, "uwc3ng/locustswarmloop.wav" );
	copy( g_szSounds[SOUND_LIGHTNING], 63, "uwc3ng/lightningbolt.wav" );
	copy( g_szSounds[SOUND_ULTIMATESCAN], 63, "turret/tu_ping.wav" );
	copy( g_szSounds[SOUND_ENTANGLING], 63, "uwc3ng/entanglingrootstarget1.wav" );
	copy( g_szSounds[SOUND_IMMOLATE], 63, "uwc3ng/ImmolationDecay1.wav" );
	copy( g_szSounds[SOUND_IMMOLATE_BURNING], 63, "ambience/flameburst1.wav" );
	copy( g_szSounds[SOUND_SUICIDE], 63, "ambience/particle_suck1.wav" );
	copy( g_szSounds[SOUND_PICKUPITEM], 63, "uwc3ng/pickupitem.wav" );
	copy( g_szSounds[SOUND_TOME], 63, "uwc3ng/Tomes.wav" );
	copy( g_szSounds[SOUND_SPELLSHIELD], 63, "uwc3ng/SpellShieldImpact1.wav" );
	copy( g_szSounds[SOUND_MEND], 63, "items/medshot5.wav" );
	copy( g_szSounds[SOUND_REPAIR], 63, "items/suitchargeok1.wav" );
	
	// Lets make sure we have all of the sound files!!!
	new i, szTmp[128], bool:bError = false;

	for ( i = 0; i < MAX_SOUNDS; ++i )
	{
		// We only need to look for uwc3ng files (other files should be w/base HLDS installation)
		if ( containi( g_szSounds[i], "uwc3ng" ) != -1 )
		{
			formatex( szTmp, charsmax( szTmp ), "sound/%s", g_szSounds[i] );

			if ( !file_exists( szTmp ) )
			{
				log_amx( "[ERROR] Missing sound file '%s'", szTmp );

				bError = true;
			}
		}
	}

	// All sound files exist!! Lets continue!
	if ( !bError )
	{
		// Precache all files
		for ( i = 0; i < MAX_SOUNDS; ++i )
		{
			precache_sound( g_szSounds[i] );
		}
	}

	// We need to throw a runtime error :/
	else
	{
		set_fail_state( "Sound files are missing, unable to load plugin" );
	}

	bError = false;
	
	// Path for the sprites
	copy( g_szSprites[SPRITE_REPAIR], 63, "sprites/uwc3ng/shield3.spr" );
	copy( g_szSprites[SPRITE_MEND], 63, "sprites/uwc3ng/medic3.spr" );
	copy( g_szSprites[SPRITE_REPAIRMEND], 63, "sprites/uwc3ng/medshield1.spr" );
	copy( g_szSprites[SPRITE_P_MEND], 63, "sprites/uwc3ng/mend1.spr" );
	copy( g_szSprites[SPRITE_P_REPAIR], 63, "sprites/uwc3ng/repair1.spr" );
	copy( g_szSprites[SPRITE_LIGHTNING], 63, "sprites/lgtning.spr" );
	copy( g_szSprites[SPRITE_FLARE], 63, "sprites/blueflare2.spr" );
	copy( g_szSprites[SPRITE_SHOCKWAVE], 63, "sprites/shockwave.spr" );
	copy( g_szSprites[SPRITE_BEETLE], 63, "sprites/agrunt1.spr" );
	copy( g_szSprites[SPRITE_SHADOWSTRIKE], 63, "sprites/animglow01.spr" );
	copy( g_szSprites[SPRITE_TRAIL], 63, "sprites/smoke.spr" );
	copy( g_szSprites[SPRITE_SNOW], 63, "sprites/snow.spr" );
	copy( g_szSprites[SPRITE_BEAM], 63, "sprites/zbeam4.spr" );
	copy( g_szSprites[SPRITE_BURNING], 63, "sprites/xfire.spr" );
	copy( g_szSprites[SPRITE_IMMOLATE], 63, "sprites/uwc3ng/fireball.spr" );
	copy( g_szSprites[SPRITE_FIREBALL], 63, "sprites/zerogxplode.spr" );
	copy( g_szSprites[SPRITE_SMOKE], 63, "sprites/steam1.spr" );
	copy( g_szSprites[SPRITE_FIRE], 63, "sprites/explode1.spr" );
	copy( g_szSprites[SPRITE_CARRIONBEETLES], 63, "sprites/uwc3ng/bm7.spr" );
	copy( g_szSprites[SPRITE_SHADOWSTRIKEDOT], 63, "sprites/uwc3ng/poison1.spr" );
		
	// Now lets make sure the sprites exist
	for ( i = 0; i < MAX_SPRITES; i++ )
	{

		// We only need to look for warcraft3 files (other files should be w/base HLDS installation)
		if ( containi( g_szSprites[i], "uwc3ng" ) != -1 )
		{
			if ( !file_exists( g_szSprites[i] ) )
			{
				log_amx( "[ERROR] Missing sprite file '%s'", g_szSprites[i] );

				bError = true;
			}
		}
	}
	
	// All sprite files exist!! Lets continue!
	if ( !bError )
	{
		// Precache all sprites
		for ( i = 0; i < MAX_SPRITES; ++i )
		{
			g_iSprites[i] = precache_model( g_szSprites[i] );
		}
	}
	
	// We need to throw a runtime error :/
	else
	{
		set_fail_state( "Sprite files are missing, unable to load plugin" );
	}
	
	// Zombie model for hex
	if ( file_exists( "models/player/zombie/zombie.mdl" ) )
	{
		precache_model( "models/player/zombie/zombie.mdl" );
	}
	
	// Player models for mole
	precache_model( "models/player/gign/gign.mdl" );
	precache_model( "models/player/sas/sas.mdl" );
	precache_model( "models/player/gsg9/gsg9.mdl" );
	precache_model( "models/player/urban/urban.mdl" );

	precache_model( "models/player/arctic/arctic.mdl" );
	precache_model( "models/player/leet/leet.mdl" );
	precache_model( "models/player/guerilla/guerilla.mdl" );
	precache_model( "models/player/terror/terror.mdl" );
}

public client_putinserver( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}

	// Check for steam ID pending
	static szPlayerID[32];
	get_user_authid( id, szPlayerID, charsmax( szPlayerID ) );
	
	// Then the player doesn't have a steam id, lets make them reconnect
	if ( equal(szPlayerID, "STEAM_ID_PENDING") )
	{
		client_cmd( id, "reconnect" );
	}
	
	// Check if the player is a bot
	new b_IsBot = false; 
	if ( is_user_bot( id ) )
	{
		b_IsBot = true;
	}
	
	// Check user's cvar if the user isn't a bot
	if ( !b_IsBot && get_pcvar_num( CVAR_uwc3ng_minmodels ) )
	{
		query_client_cvar( id, "cl_minmodels", "_UTIL_CheckMinModelsValue" );
	}
	
	// The user is connected. Reset the old data
	if ( !get_pcvar_num( CVAR_uwc3ng_save_xp ) )
	{
		// Reset the old data
		PLAYER_ResetData( id );
	}
	
	// The player is a bot. We give him a startlevel if this defined
	new iBotLevel = get_pcvar_num( CVAR_uwc3ng_bot_startlevel );
	if ( b_IsBot && iBotLevel > 0 && !get_pcvar_num( CVAR_uwc3ng_ignore_bots ) )
	{
	// An extra check
		if ( iBotLevel <= MAX_LEVEL )
		{
			p_data[id][PLAYER_XP] = g_XPforLevel[iBotLevel];
			p_data[id][PLAYER_LVL] = iBotLevel;
			p_data[id][AVAILABLE_SKILL] = iBotLevel;
			p_data[id][AVAILABLE_ATTRIBS] = ( iBotLevel > MAX_ATTRIBS_AVAILABLE ? MAX_ATTRIBS_AVAILABLE : iBotLevel );
			p_data[id][AVAILABLE_RESISTS] = ( iBotLevel * ( MAX_RESIST_AVAILABLE / MAX_LEVEL ) );
		}
	}
	
	// Detect the startlevel of each player
	PLAYER_set_startlevel( id );
		
	// Get the user's ID!
	Save_FetchUniqueID( id );
	
	// The player is a bot, give him random skills, attributes and resistances
	if ( b_IsBot && !get_pcvar_num( CVAR_uwc3ng_ignore_bots ) )
	{
		PLAYER_random_abilitys( id );
	}
	
	// Load the xps
	if ( !b_IsBot )
	{
		set_task( 1.0, "Player_GetAllXP", id + TASK_GETXP );
	}
	
	return;
}

public client_connect( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}

	client_cmd( id, "hud_centerid 0" );
	
	return;
}

public client_disconnected( id )
{
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	// Save the data from the player 
	Player_SaveXP( id, true );
	
	// Reset the complete data
	PLAYER_ResetData( id );
	
	// Psychostats
	if ( get_pcvar_num( CVAR_uwc3ng_psychostats ) )
	{
		static szWeapon[64];

		static szTeam[16], szName[32], szAuthid[32];
		new iWeap, iUserid = get_user_userid( id );

		if ( is_user_connected(id) )
		{
			get_user_team( id, szTeam, 15 );
		}
		get_user_name( id, szName, 31 );
		get_user_authid( id, szAuthid, 31 );

		for ( iWeap = CSW_WAR3_MIN; iWeap <=CSW_WAR3_MAX; iWeap++ )
		{
			format( szWeapon, 63, "" );

			switch( iWeap )
			{
				case CSW_LIGHTNING: copy( szWeapon, 63, "Chain Lightning" );
				case CSW_SUICIDE: copy( szWeapon, 63, "Suicide Bomber" );
				case CSW_IMMOLATE: copy( szWeapon, 63, "Immolate" );
				case CSW_LOCUSTS: copy( szWeapon, 63, "Locust Swarm" );
				case CSW_SERPENTWARD: copy( szWeapon, 63, "Serpent Ward" );
				case CSW_SHADOW: copy( szWeapon, 63, "Shadow Strike" );
				case CSW_THORNS: copy( szWeapon, 63, "Thorns Aura" );
				case CSW_CARAPACE: copy( szWeapon, 63, "Spiked Carapace" );
				case CSW_CARRION: copy( szWeapon, 63, "Carrion Beetles" );
				case CSW_ORB: copy( szWeapon, 63, "Orb of Annihilation" );
				case CSW_CONCOCTION: copy( szWeapon, 63, "Unstable Concoction" );
			}
			
			replace( szWeapon, 63, " ", "_" );

			new WEAPON = iWeap - CSW_WAR3_MIN;
			
			if ( iStatsShots[id][WEAPON] || iStatsHits[id][WEAPON] || iStatsKills[id][WEAPON] ||  iStatsHS[id][WEAPON] || iStatsTKS[id][WEAPON] || iStatsDamage[id][WEAPON] || iStatsDeaths[id][WEAPON] || iStatsHead[id][WEAPON] || iStatsChest[id][WEAPON] || iStatsStomach[id][WEAPON] || iStatsLeftArm[id][WEAPON] || iStatsRightArm[id][WEAPON] || iStatsLeftLeg[id][WEAPON] || iStatsRightLeg[id][WEAPON] )
			{

				// Log tge messafe
				log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats^" (weapon ^"%s^") (shots ^"%d^") (hits ^"%d^") (kills ^"%d^") (headshots ^"%d^") (tks ^"%d^") (damage ^"%d^") (deaths ^"%d^")", szName, iUserid, szAuthid, szTeam, szWeapon, iStatsShots[id][WEAPON], iStatsHits[id][WEAPON], iStatsKills[id][WEAPON], iStatsHS[id][WEAPON], iStatsTKS[id][WEAPON], iStatsDamage[id][WEAPON], iStatsDeaths[id][WEAPON] );
				log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats2^" (weapon ^"%s^") (head ^"%d^") (chest ^"%d^") (stomach ^"%d^") (leftarm ^"%d^") (rightarm ^"%d^") (leftleg ^"%d^") (rightleg ^"%d^")", szName, iUserid, szAuthid, szTeam, szWeapon, iStatsHead[id][WEAPON], iStatsChest[id][WEAPON], iStatsStomach[id][WEAPON], iStatsLeftArm[id][WEAPON], iStatsRightArm[id][WEAPON], iStatsLeftLeg[id][WEAPON], iStatsRightLeg[id][WEAPON] );
			
				iStatsShots[id][WEAPON]		= 0;
				iStatsHits[id][WEAPON]		= 0;
				iStatsKills[id][WEAPON]		= 0;
				iStatsHS[id][WEAPON]		= 0;
				iStatsTKS[id][WEAPON]		= 0;
				iStatsDamage[id][WEAPON]	= 0;
				iStatsDeaths[id][WEAPON]	= 0;
				iStatsHead[id][WEAPON]		= 0;
				iStatsChest[id][WEAPON]		= 0;
				iStatsStomach[id][WEAPON]	= 0;
				iStatsLeftArm[id][WEAPON]	= 0;
				iStatsRightArm[id][WEAPON]	= 0;
				iStatsLeftLeg[id][WEAPON]	= 0;
				iStatsRightLeg[id][WEAPON]	= 0;
			}
		}
	}
}

public plugin_end()
{
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	// Destry the forward
	FWD_Destroy()
	
	// Save all data from all players
	Player_SaveAll();
	
	// Close the connections
	Save_Close();
}

public plugin_natives()
{
	set_module_filter( "module_filter" );
}

public module_filter( const module[] )
{
	// We don't need to display a message, we'll display it later if we need to (when the DB is set up if XP Saving is on)
	if ( equal( module, "dbi" ) || equal( module, "sqlx" ) )
	{
		return PLUGIN_HANDLED;
	}
	
	// something is wrong. We need a module but can't find it
	else
	{
		log_amx( "[ERROR] Please enable the '%s' module in your modules.ini file", module );
		
		return PLUGIN_CONTINUE;
	}
}

public bool:UWC3NG_Check()
{
	if ( get_pcvar_num( uwc3ng_enabled ) == 1 )
	{
		return true;
	}
	
	return false;
}
