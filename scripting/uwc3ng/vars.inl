// Uwc3ng public variables

// Pointers for the cvars
new uwc3ng_enabled, CVAR_uwc3ng_savexp, CVAR_uwc3ng_save_xp, CVAR_uwc3ng_host, 
     CVAR_uwc3ng_user, CVAR_uwc3ng_pass, CVAR_uwc3ng_name, CVAR_uwc3ng_save_by,
     CVAR_uwc3ng_min_players, CVAR_uwc3ng_ignore_bots, CVAR_uwc3ng_xpmultiplier,
     CVAR_uwc3ng_xp_for_level, CVAR_uwc3ng_admin_flag, CVAR_sv_gravity, 
     CVAR_csdm_active, CVAR_uwc3ng_psychostats, CVAR_uwc3ng_blink_protection,
     CVAR_uwc3ng_ult_cooldown, CVAR_uwc3ng_blink_dizziness, CVAR_uwc3ng_entangle_drop,
     CVAR_uwc3ng_spec_position, CVAR_mp_friendlyfire, CVAR_uwc3ng_boots, CVAR_uwc3ng_claw,
     CVAR_uwc3ng_cloak, CVAR_uwc3ng_mask, CVAR_uwc3ng_frost, CVAR_uwc3ng_health, CVAR_uwc3ng_glove_timer,
     CVAR_uwc3ng_tome, CVAR_uwc3ng_sock, CVAR_uwc3ng_bot_buy_item, CVAR_uwc3ng_bot_startlevel,
     CVAR_uwc3ng_startlevel, CVAR_uwc3ng_minmodels, CVAR_uwc3ng_icons, CVAR_uwc3ng_attrib_xpmodifier,
     CVAR_uwc3ng_resist_xpmodifier, CVAR_uwc3ng_save_end_round, CVAR_uwc3ng_ult_round_start_cd,
     CVAR_uwc3ng_hudmessage_display;

// Where we save our data?
new g_SaveType = -1;

// Database vars
new Handle:g_DBTuple, Handle:g_DBConn;

// Player Data arrays
new p_data[MAX_PLAYERS][MAX_DATA], p_data_skill[MAX_PLAYERS][MAX_SKILLS], bool:p_data_b[MAX_PLAYERS][MAX_DATA_B],
    g_PlayerWeapons[MAX_PLAYERS][32], g_PlayerLastWeapons[MAX_PLAYERS][32], CsArmorType:g_ArmorType[MAX_PLAYERS],
    g_PlayerUltimateDelay[MAX_PLAYERS], p_data_attrib[MAX_PLAYERS][MAX_ATTRIBS], p_data_resist[MAX_PLAYERS][MAX_RESIST];

// Player roles
new g_iPlayerRole[MAX_PLAYERS];

// Here we save the xp table. We have to add one :0
new g_XPforLevel[MAX_LEVEL+1];

// The pointer for the sounds & Sprites
new g_szSounds[MAX_SOUNDS][64], g_iSprites[MAX_SPRITES];

// Sprites path
new g_szSprites[MAX_SPRITES][64];

// is the round over?
new bool:g_EndRound = true;

// is the freezetime over?
new bool:g_FreezeTimeOver = false;

// Message vars
new gmsgStatusText, gmsgScreenFade, gmsgScreenShake, gmsgDeathMsg, gmsgScoreInfo, gmsgBarTime;

// Glow
new g_GlowLevel[MAX_PLAYERS][4];

// Used for advanced statistics
new iStatsHead[MAX_PLAYERS][MAX_CSW_S];
new iStatsChest[MAX_PLAYERS][MAX_CSW_S];
new iStatsStomach[MAX_PLAYERS][MAX_CSW_S];
new iStatsLeftArm[MAX_PLAYERS][MAX_CSW_S];
new iStatsRightArm[MAX_PLAYERS][MAX_CSW_S];
new iStatsLeftLeg[MAX_PLAYERS][MAX_CSW_S];
new iStatsRightLeg[MAX_PLAYERS][MAX_CSW_S];
new iStatsKills[MAX_PLAYERS][MAX_CSW_S];
new iStatsDeaths[MAX_PLAYERS][MAX_CSW_S];
new iStatsHS[MAX_PLAYERS][MAX_CSW_S];
new iStatsTKS[MAX_PLAYERS][MAX_CSW_S];
new iStatsShots[MAX_PLAYERS][MAX_CSW_S];
new iStatsHits[MAX_PLAYERS][MAX_CSW_S];
new iStatsDamage[MAX_PLAYERS][MAX_CSW_S];

// Zoomed weapon?
new bool:g_bPlayerZoomed[MAX_PLAYERS];

// Serpent Ward 
new bool:bWardBombCheck = false;

// Objectives
new g_iObjectiveEnt[MAX_OBJECTIVES];
new g_iObjectiveEntType[MAX_OBJECTIVES];
new Float:g_fObjectiveOrigin[MAX_OBJECTIVES][3]
new g_iTotalObjectiveEnts = 0;

// Spawns
new g_iSpawnReserved[TOTAL_SPAWNS];
new g_iSpawnInc = 0;

// Used for spectating
new g_iSpectatingID[33];
new Float:g_fLastSpecDisplay[33];

// Ultimate detection
new g_iCountUltimates[MAX_PLAYERS];

// Used with helm
new Float:fLastShotFired[MAX_PLAYERS];

// Holds the XP Multipliers per weapon
new Float:fWpnXPMultiplier[CSW_WAR3_MAX+1] = { 1.0, ... };

// Used to keep track of how much dmg one player does to another
new g_iDamageDealt[MAX_PLAYERS][MAX_PLAYERS];

// Item Variables
new g_iShopMenuItems[MAX_PLAYERS][MAX_PLAYER_ITEMS]; 				// Holds the player's current items
new g_iFutureItem[MAX_PLAYERS];                           				// Holds the item that the player wants to buy 
new g_iTotalRings[MAX_PLAYERS];							// Holds the number of rings the player has
new g_iNecklaceCharges[MAX_PLAYERS];						// Holds the number of charges left in the player's necklace
new g_iHelmCharges[MAX_PLAYERS];						// Holds the number of charges left in the player's helm
new bool:g_bPlayerBoughtAnkh[MAX_PLAYERS];					// Set to true when a user buys an ankh
new bool:g_bPlayerBoughtMole[MAX_PLAYERS];					// Set to true when a user buys a mole
new g_iGloveTimer[MAX_PLAYERS];							// Keeps track of how much time is left to give a user a grenade
new ITEM_COST[MAX_SHOPMENU_ITEMS];	         			         // Holds the price of an item
new g_iFlag[MAX_SHOPMENU_ITEMS] = { 0, ... };					// Stores information about items

// Ignore the armory setting
new bool:bIgnoreArmorSet[MAX_PLAYERS];

// Ultimate round start delay
new bool:g_bUltRoundDelay = false;
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
