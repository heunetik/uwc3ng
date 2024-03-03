// Uwc3ng define & constant file

// -------------------------------------- Tasks --------------------------------------
// Task defines
#define TASK_LOADXP 32
#define TASK_GIVEITEMS 64
#define TASK_SILENCEBURST 96
#define TASK_WAVE 128
#define TASK_GLOW 160
#define TASK_SKIN 192
#define TASK_HUD 226
#define TASK_UDELAY 227
#define TASK_SPEED 229
#define TASK_JUMP 261
#define TASK_RESETSPEED 293
#define TASK_LIGHT 325
#define TASK_BLINKSTUCK 357
#define TASK_TELEPORT 389
#define TASK_RESETSPAWNS 422
#define TASK_RESETGOD 454
#define TASK_SPAWNREMOVEGOD 486
#define TASK_SPAWN 518
#define TASK_FUNNELS 550
#define TASK_LIGHTNING 582
#define TASK_LIGHTNINGNEXT 614
#define TASK_ULTPING 646
#define TASK_BIGBADATTACKER 678
#define TASK_ENTANGLEWAIT 710
#define TASK_BURNING 742
#define TASK_EXPLOSION 774
#define TASK_BEAMCYLINDER 806
#define TASK_MOLE 838
#define TASK_ITEM_RING 870
#define TASK_ITEM_GLOVES 902
#define TASK_GETXP 934
#define TASK_DRAW 967
#define TASK_SPAWNPLAYER 1000
#define TASK_CARRIONDOT 1033
#define TASK_SHADOWDOT 1066
#define TASK_ULT_START_DELAY 1100

// -------------------------------------- General --------------------------------------
// General define's
#define MOD_NAME "[UWC3NG]"

// Team defines
#define TEAM_T 1
#define TEAM_CT 2

// Save defines
#define DB_MYSQLX 0
#define DB_SQLITE 1

// Maximum is all
#define MAX_PLAYERS 33
#define MAX_SKILLS 40
#define MAX_LEVEL 50
#define MAX_ULTIMATES 3
#define MAX_SHOPMENU_ITEMS 18
#define MAX_PLAYER_ITEMS 2
#define MAX_PAGE_ITEMS 9
#define MAX_ATTRIBS 5
#define MAX_ATTRIBS_POINTS 20
#define MAX_ATTRIBS_STARTPOINTS 10
#define MAX_ATTRIBS_AVAILABLE 30
#define MAX_RESIST 5
#define MAX_RESIST_POINTS 100
#define MAX_RESIST_AVAILABLE 300

// Player data array defines (used for p_data var)
#define MAX_DATA 17
#define PLAYER_XP 0
#define PLAYER_LVL 1
#define PLAYER_UNIQUEID 2
#define PLAYER_RANK 3
#define SKILL_SIDE 4
#define AVAILABLE_SKILL 5
#define P_LASTARMOR 6
#define P_FLASHCOUNT 7
#define P_ARMORONDEATH 8
#define P_REPAIR 9
#define P_MEND 10
#define P_SERPENTCOUNT 11
#define P_DISPELL 12
#define SHOPMENU_SIDE 13
#define P_ULT_SEARCH 14
#define AVAILABLE_ATTRIBS 15
#define AVAILABLE_RESISTS 16

// Player bool data array defines (user for p_data_b var)
#define MAX_DATA_B 39
#define PLAYER_SPAWNGOD 0
#define PLAYER_DEFORPLAN 1
#define SKILL_DROP 2
#define PB_HEXED 3
#define USED_DROP 4
#define RESET_SKILLS 5
#define PB_SHIELD 6
#define PB_NIGHTVISION 7
#define PB_DEFUSE 8
#define PB_USP_SILENCED 9
#define PB_M4A1_SILENCED 10
#define PB_GLOCK_BURST 11
#define PB_FAMAS_BURST 12
#define PB_DIEDLASTROUND 13
#define PB_STUNNED 14
#define PB_BLINK 15
#define PB_GOD 16
#define PB_CANT_RENDER 17
#define PB_ISPLANTING 18
#define IGNOR_SPAWN 19
#define PB_PHOENIX 20
#define PB_RESPAWNED 21
#define PB_LIGHTNINGHIT 22
#define PB_ISSEARCHING 23
#define PB_BIGBAD_ATTACKER 24
#define PB_ISBURNING 25
#define PB_USED_SUICIDE 26
#define PB_SUICIDEATTEMPT 27
#define PB_SKINSWITCHED 28
#define PB_SLOWED 29
#define RESET_ATTRIBS 30
#define USED_GATE 31
#define USED_GATE_TWICE 32
#define PB_RESPAWNED_TWICE 33
#define RESET_RESISTS 34
#define PB_NORMALSPEED 35
#define RESIST_FILLRESIST 36
#define PB_CARRIONDOT 37
#define PB_SHADOWDOT 38


// How we save the data
#define SAVEBY_STEAMID 0
#define SAVEBY_IP 1
#define SAVEBY_NAME 2

// Type of SQlite database synchronization
#define SQLITE_SYNC_OFF	0
#define SQLITE_SYNC_NORMAL 1
#define SQLITE_SYNC_FULL 2

// Glow
#define MAXGLOW	150

// thanks twistedeuphoria - from objective proximity warning
#define MAX_OBJECTIVES 11
#define OBJENT_VIP_ESCAPE 0
#define OBJENT_HOSTAGE 1
#define OBJENT_BOMBSITE	2
#define OBJENT_HOSTAGE_ESCAPE 3

// Used to make sure a spawn point isn't used for another player
#define TOTAL_SPAWNS	64

// Achsen
#define XPOS 0
#define YPOS 1
#define ZPOS 2

// Spawn delay / Spawn protection
#define SPAWN_DELAY 0.2
#define SPAWN_PROTECTION 3.0

// Used for spectating info
#define SPEC_HOLDTIME 10.0

// Bot cast chance
#define BOT_CAST_ULT_CHANCE 0.80
#define BOT_RESPAWN 0.10

// -------------------------------------- Skills --------------------------------------
// p_data_skill defines (look vars.inl)
#define SKILL_LEVITATION 0
#define SKILL_REINCARNATION 1
#define SKILL_IMPALE 2
#define SKILL_DEVOTION 3
#define SKILL_HEALINGWAVE 4
#define SKILL_MANA 5
#define SKILL_LSKIN 6
#define SKILL_VAMPIRIC 7
#define SKILL_SPIKED 8
#define SKILL_REPAIR 9
#define SKILL_MEND 10
#define SKILL_UNHOLYAURA 11
#define SKILL_HEX 12
#define SKILL_BASH 13
#define SKILL_WARD 14
#define SKILL_BANISH 15
#define SKILL_THORNSAURA 16
#define SKILL_TELEPORT 17
#define SKILL_CRITICAL 18
#define SKILL_DISPELLHEX 19
#define SKILL_CARRIONBEETLES 20
#define SKILL_STEELSKIN 21
#define SKILL_SHADOWSTRIKE 22
#define SKILL_BLINK 23
#define SKILL_CRITICALGRENADE 24
#define SKILL_BIGBADVOODOO 25
#define SKILL_GATE 26
#define SKILL_INVISIBLE 27
#define SKILL_PHOENIX 28
#define SKILL_NAPALMGRENADE 29
#define SKILL_SIPHONARMOR 30
#define SKILL_LOCUST 31
#define SKILL_VENGEANCE 32
#define SKILL_CHAINLIGHTNING 33
#define SKILL_ENTANGLEROOTS 34
#define SKILL_FLAMESTRIKE 35
#define SKILL_BOMBER 36
#define SKILL_FANOFKNIVES 37
#define SKILL_EVASION 38
#define SKILL_TRUESHOTAURA 39

// Some skill defines
#define LEVITATION_FOR_SKILL 44
#define HEALING_WAVE_RANGE 750
#define HEALING_WAVE_HEAL 3
#define LEATHER_SKIN_RANGE 750
#define REPAIR_DIVISOR 5
#define MEND_DIVISOR 5
#define DISPELL_DIVISOR 5
#define REPAIR_BONUS 1.4
#define MEND_BONUS 1.4
#define HEXED_JUMP_TIME 3.0
#define BASH_GLOW_INTENSITY 100
#define BASH_HOLD_TIME 0.7
#define BANISH_DAMAGE 5
#define TELEPORT_RADIUS 50
#define BLINK_COOLDOWN 2.0
#define SHADOWSTRIKE_DAMAGE 15
#define SHADOWSTRIKE_DOT 5
#define DOT_TASKTIME 3.0
#define CARRIONBEETLE_DAMAGE 15
#define CARRIONBEETLE_DOT 3
#define IMMUNITY_RADIUS	500
#define BIGBADVOODOO_DURATION 2
#define NAPALM_MULTI 3
#define LOCUSTSWARM_DMG_MIN 30
#define LOCUSTSWARM_DMG_MAX 60
#define CHAINLIGHTNING_DAMAGE 50	
#define CHAINLIGHTNING_LINEWIDTH 80
#define LIGHTNING_RANGE	500
#define ENTANGLE_TIME 10.0
#define IMMOLATE_DAMAGE	35		
#define IMMOLATE_DOT_DAMAGE 5		
#define IMMOLATE_DOT 4
#define EXPLOSION_MAX_DAMAGE 50
#define EXPLOSION_RANGE 300
#define EXPLOSION_BLAST_RADIUS 250

// Menu defines
#define MAX_SKILLS_ONSIDE 8

// -------------------------------------- XPs & Roles --------------------------------------
// PlayerRole defines
#define PLR_NONE 0
#define PLR_BOMB_CARRIER 1
#define PLR_HOSTAGE_RESCUER 2
#define PLR_VIP 3
#define PLR_BOMB_PLANTER 4
#define PLR_BOMB_DEFUSER 5

// Kill someone
#define KILL_ENEMY_XP 70
#define KILL_HEADSHOT_XP 30

// Kill modifiers
#define KILL_HEADSHOT		30		// XP awarded for getting a headshot
#define KILL_HOSTAGE_SAVER	30		// XP awarded for killing the hostage saver
#define	KILL_DEFUSER		30		// XP awarded for killing the defuser
#define KILL_PLANTER		30		// XP awarded for killing the planter
#define KILL_BOMB_CARRIER	30		// XP awarded for killing the bomb carrier
#define KILL_VIP		50		// XP awarded for killing the VIP
#define KILL_RADIUS		250		// Nearby radius to award XP

// Objective Modifiers
#define DEFUSING_BOMB 80			// XP awarded when the user starts to defuse the bomb
#define DEFUSED_BOMB 160			// XP awarded when the user defuses the bomb
#define PLANTING_BOMB 80			// XP awarded when the user starts planting the bomb
#define PLANT_BOMB 160				// XP awarded when the user plants the bomb
#define SPAWN_BOMB 60				// XP awarded when the user spawns with the bomb
#define BOMB_PICKUP 60				// XP awarded when the user picks up the bomb
#define TOUCH_HOSTAGE 70			// XP awarded when the user touches a hostage
#define RESCUE_HOSTAGE 60			// XP awarded when the user rescues the hostage
#define KILL_HOSTAGE 60				// XP lost when killing a hostage
#define SPAWN_VIP 120				// XP awarded for spawning as the VIP
#define ESCAPE_VIP 200				// XP awarded for escaping as the VIP
#define DEFUSING_WITH_KIT 60			// XP awarded for defusing with kit
#define OBJ_RADIUS 500				// Nearby radius to award XP for helping complete objectives

// Other
#define REPAIR_HEAL 80				// XP awarded for repair someone
#define MEND_HEAL 80				// XP awarded for mend someone
#define PHOENIX_XP 120				// XP awarded for respawn someone
#define WINNING_TEAM 100			// XP awarded for win a round 
#define DISPELL_HEX 80				// XP awarded for dispelling a teammate's hex

// -------------------------------------- Hudmessages --------------------------------------
// Wc3ft hudmessage defines (look UWC3NG_StatusText on graphics.inl)
#define TXT_OTHER 0
#define TXT_TOP_CENTER 1
#define TXT_ULTIMATE 3
#define TXT_BLINK_CENTER 4
#define TXT_SKILL 5
#define TXT_ULT_ITEM 7
#define TXT_BLINK_CENTER_SHRT 8

#define HUDMESSAGE_FX_FADEIN    0               // hudmessage effect types
#define HUDMESSAGE_FX_FLICKER   1
#define HUDMESSAGE_FX_WRITEOUT  2

// HUD Channels
#define HUD_SPEC_INFO			1	// Info displayed in bottom corner of player being spectated
#define HUD_ULT_ITEM			2	// Display the two items and the status of the ultimates
#define HUD_ULTIMATE			1	// Ultimate messages (i.e. you have _ seconds remaining)
#define HUD_SKILL			3	// i.e. "you have evaded a shot"
#define HUD_AUTO			-1	// Automatically choose one!

// -------------------------------------- Precache --------------------------------------
// Sounds
#define MAX_SOUNDS 24

#define SOUND_LEVELUP 0
#define SOUND_REINCARNATION 1
#define SOUND_IMPALE 2
#define SOUND_ULTIMATEREADY 3
#define SOUND_HEX 4
#define SOUND_BANISH 5
#define SOUND_BLINK 6
#define SOUND_CARRION 7
#define SOUND_SHADOWSTRIKE 8
#define SOUND_DISPELL 9
#define SOUND_VOODOO 10
#define SOUND_TELEPORT 11
#define SOUND_LOCUSTSWARM 12
#define SOUND_LIGHTNING 13
#define SOUND_ULTIMATESCAN 14
#define SOUND_ENTANGLING 15
#define SOUND_IMMOLATE 16
#define SOUND_IMMOLATE_BURNING 17
#define SOUND_SUICIDE 18
#define SOUND_PICKUPITEM 19
#define SOUND_TOME 20
#define SOUND_SPELLSHIELD 21
#define SOUND_MEND 22
#define SOUND_REPAIR 23

// Sprites
#define MAX_SPRITES 20

#define SPRITE_REPAIR 0
#define SPRITE_MEND 1
#define SPRITE_REPAIRMEND 2
#define SPRITE_P_REPAIR 3
#define SPRITE_P_MEND 4
#define SPRITE_LIGHTNING 5
#define SPRITE_FLARE 6
#define SPRITE_SHOCKWAVE 7
#define SPRITE_BEETLE 8
#define SPRITE_SHADOWSTRIKE 9
#define SPRITE_TRAIL 10
#define SPRITE_SNOW 11
#define SPRITE_BEAM 12
#define SPRITE_IMMOLATE 13
#define SPRITE_BURNING 14
#define SPRITE_FIREBALL 15
#define SPRITE_SMOKE 16
#define SPRITE_FIRE 17
#define SPRITE_CARRIONBEETLES 18
#define SPRITE_SHADOWSTRIKEDOT 19

// -------------------------------------- Weapons --------------------------------------
#define CSW_WORLDSPAWN 0 

#if !defined CSW_GALIL
	#define CSW_GALIL 14
#endif
#if !defined CSW_FAMAS
	#define CSW_FAMAS 15
#endif

#define CSW_WAR3_MIN 51

#define	CSW_LIGHTNING 51
#define	CSW_SUICIDE 52
#define	CSW_IMMOLATE 53
#define	CSW_LOCUSTS 54
#define	CSW_SERPENTWARD 55
#define CSW_SHADOW 56
#define	CSW_THORNS 57
#define	CSW_CARAPACE 58
#define CSW_CARRION 59
#define CSW_ORB 60
#define CSW_CONCOCTION 61
#define CSW_BANISH 62

#define CSW_WAR3_MAX 62

#define MAX_CSW_S 12

#define HITGROUP_GENERIC		0 // none
#define HITGROUP_HEAD			1 // 1 << 1 = 2
#define HITGROUP_CHEST			2 // 1 << 2 = 4
#define HITGROUP_STOMACH		3 // 8
#define HITGROUP_LEFTARM		4 // 16
#define HITGROUP_RIGHTARM		5 // 32
#define HITGROUP_LEFTLEG		6 // 64
#define HITGROUP_RIGHTLEG		7 // 128

// -------------------------------------- Items --------------------------------------
// Item Charge Defines
#define NECKLACE_CHARGES 4
#define HELM_CHARGES 3
#define CHARGE_DISPOSE 1
#define RING_INCREMENT 1
#define INVIS_CLOAK_DIVISOR 1.5

// Item Variable Defines
#define ITEM_SLOT_ONE 0
#define ITEM_SLOT_TWO 1
#define ITEM_SLOT_FULL 2

#define ITEM_NONE		-1
#define ITEM_ANKH		0
#define ITEM_BOOTS		1
#define ITEM_CLAWS		2
#define ITEM_CLOAK		3
#define ITEM_MASK		4
#define ITEM_NECKLACE	    	5
#define ITEM_FROST		6
#define ITEM_HEALTH		7
#define ITEM_TOME		8
#define ITEM_SCROLL		9
#define ITEM_PROTECTANT     	10
#define ITEM_HELM		11
#define ITEM_AMULET		12
#define ITEM_SOCK		13
#define ITEM_GLOVES		14
#define ITEM_RING		15
#define ITEM_CHAMELEON	    	16
#define ITEM_MOLE		17

#define MAX_ITEMS_PAGE		9

#define ITEM_CHARGEABLE		1
#define ITEM_USEONBUY		2
#define ITEM_BUYWHENDEAD	4
#define ITEM_NEXTROUNDUSE	8

// ------------------------------------- Attribs ------------------------------------
#define ATTRIB_STRENGTH 0
#define ATTRIB_INTELLECT 1
#define ATTRIB_DEXTERITY 2
#define ATTRIB_AGILITY 3
#define ATTRIB_CONSTITUTION 4

// some attrib defines
#define AGILITY_HEALTH_BONUS 5 		 // Bonus health for each point agility
#define CON_HEAL_INTERVAL 5.0   	 // Task interval for Constitution health regen
#define DEX_EVADE_BONUS 0.01   		 // Dexterity evasion bonus

// Intellect defines
#define INTELLECT_IMPALE 11
#define INTELLECT_IMPALE_BONUS 0.10
#define INTELLECT_SIPHON 12
#define INTELLECT_SIPHON_BONUS 0.05
#define INTELLECT_BANISH 13
#define INTELLECT_BANISH_BONUS 0.10
#define INTELLECT_HEX 14
#define INTELLECT_HEX_BONUS 0.05
#define INTELLECT_BASH 15
#define INTELLECT_BASH_BONUS 0.10
#define INTELLECT_TRUESHOT 16
#define INTELLECT_TRUESHOT_BONUS 0.10
#define INTELLECT_TELEPORT 17
#define INTELLECT_TELEPORT_BONUS 10
#define INTELLECT_GATE 18
#define INTELLECT_LOCUST 19
#define INTELLECT_LOCUST_BONUS 15
#define INTELLECT_VENGEANCE 20

// -------------------------------------- Resistance ---------------------------------
#define RESIST_POISON 0
#define RESIST_DISEASE 1
#define RESIST_LIGHTNING 2
#define RESIST_FIRE 3
#define RESIST_MAGIC 4

// -------------------------------------- Armor --------------------------------------
// From ../multiplayer source/dlls/player.cpp
#define ARMOR_RATIO 0.7	// Armor Takes 30% of the damage (was .2 in the SDK)
#define ARMOR_BONUS 0.5	// Each Point of Armor is work 1/x points of health

// -------------------------------------- Ranks --------------------------------------
// UWC3 Rank titles
#define MAX_RANKS 12

new const p_RankTitles[MAX_RANKS][] = { 
	"Orc Fodder", 
	"Elf Acolyte", 
	"Human Squire", 
	"Undead Soldier", 
	"Skilled Combatant", 
	"Crypt Lord Zealot", 
	"Arch Lich", 
	"Blood Elf Champion", 
	"Demon Hunter", 
	"Emissary of Death", 
	"Warchief", 
	"World Destroyer" 
};

// -------------------------------------- Skills --------------------------------------
// Skill names
new const Skill_Names[MAX_SKILLS][] = {
	"Levitation",
	"Equipment Reincarnation",
	"Impale",
	"Devotion Aura",
	"Healing Wave",
	"Siphon Mana",
	"Leather Skin",
	"Vampiric Aura",
	"Spiked Carapace",
	"Repair Armor",
	"Mend Wounds",
	"Unholy Aura",
	"Hex",
	"Bash",
	"Serpent Ward",
	"Banish",
	"Thorns Aura",
	"Teleport",
	"Critical Strike",
	"Dispell Hex",
	"Carrion Beetles",
	"Steel Skin",
	"Shadow Strike",
	"Blink",
	"Critical Grenade",
	"Big Bad Voodoo",
	"Gate",
	"Invisibility",
	"Phoenix",
	"Napalm Grenades",
	"Siphon Armor",
	"Locust Swarm",
	"Vengeance",
	"Chain Lightning",
	"Entangle Roots",
	"Flame Strike",
	"Suicide Bomber",
	"Fan of Knives",
	"Evasion",
	"Trueshot Aura"
};

// Skill required level
new const Skill_REQLevel[MAX_SKILLS] = { 1, 1, 1, 2, 2, 2, 2, 3,		// Page 1
	3, 4, 4, 5, 5, 6, 6, 7,						// Page 2
	8, 8, 9, 9, 10, 11, 11, 11,					// Page 3	
	12, 12, 12, 13, 13, 13, 14, 14,					// Page 4
	15, 15, 16, 16, 17, 19, 24, 24 };				// Page 5

// Ultimate bindings
new const Ultimate_Bindings[MAX_SKILLS][] = {
	"",					// Levitation
	"",					// Equipment Reincarnation
	"",					// Impale
	"",					// Devotion Aura
	"",					// Healing Wave
	"",					// Siphon Mana
	"",					// Leather Skin
	"",					// Vampiric Aura
	"",					// Spiked Carapace
	"wcrepair",				// Repair Armor
	"wcmend",				// Mend Wounds
	"",					// Unholy Aura
	"",					// Hex
	"",					// Bash
	"wcward",				// Serpent Ward
	"",					// Banish
	"",					// Thorns Aura
	"wcteleport",				// Teleport
	"",					// Critical Strike
	"wcdispell",				// Dispell Hex
	"",					// Carrion Beetles
	"",					// Steel Skin
	"",					// Shadow Strike
	"",					// Blink
	"",					// Critical Grenade
	"wcvoodoo",				// Bid Bad Voodoo
	"wcgate",				// Gate
	"",					// Invisibility
	"",					// Phoenix
	"",					// Napalm Grenades
	"",					// Siphon Armor
	"wclocust",				// Locust Swarm
	"",					// Vengeance
	"wclightning",				// Chain Lightning
	"wcentangle",				// Entangle Roots
	"wcflame",				// Flame Strike
	"wcsuicide",				// Suicide Bomber
	"",					// Fan of Knives
	"",					// Evasion
	""					// Trueshot Aura
};

// Skill max points to spend
new const Skill_Max_Points[MAX_SKILLS] = { 10, 3, 10, 10, 10, 10, 10, 10,		// Page 1
	10, 1, 3, 10, 10, 10, 3, 10, 						// Page 2
	10, 1, 10, 1, 10, 1, 10, 5,						// Page 3
	10, 1, 1, 10, 5, 1, 10, 1,						// Page 4
	1, 1, 1, 1, 1, 3, 5, 10 };						// Page 5

// Skill const
new const Float:p_ankh[3] = { 0.333, 0.666, 1.0 }; // Equipment reincarnation
new const Float:p_impale[10] = { 0.04, 0.06, 0.08, 0.10, 0.13, 0.16, 0.19, 0.21, 0.24, 0.28 }; // Impale
new const p_devotion[10]  = { 10, 12, 14, 17, 20, 23, 26, 30, 34, 40 }; // Devotion Aura
new const Float:p_heal[10] = { 6.0, 5.5, 5.0, 4.5, 4.0, 3.5, 3.0, 2.5, 2.0, 1.5 }; // Healing Wave
new const Float:p_mana[10] = { 0.03, 0.04, 0.05, 0.06, 0.07, 0.09, 0.11, 0.13, 0.15, 0.17 }; // Siphon Mana
new const Float:p_lskin[10] = { 6.0, 5.5, 5.0, 4.5, 4.0, 3.5, 3.0, 2.5, 2.0, 1.5 }; // Leather Skin
new const Float:p_vampiric[10] = { 0.10, 0.12, 0.14, 0.16, 0.18, 0.21, 0.24, 0.27, 0.30, 0.34 }; // Vampiric Aura
new const Float:p_spiked[10] = { 0.06, 0.08, 0.10, 0.12, 0.15, 0.18, 0.21, 0.24, 0.27, 0.31 }; // Spiked Carapace
new const Float:p_unholy[10] = { 260.0, 265.0, 270.0, 275.0, 282.5, 290.0, 297.5, 305.0, 315.0, 325.0 }; // Unholy Aura
new const Float:p_hex[10] = { 0.04, 0.06, 0.08, 0.10, 0.12, 0.14, 0.16, 0.18, 0.20, 0.22 }; // Hex
new const Float:p_bash[10] = { 0.04, 0.06, 0.08, 0.10, 0.13, 0.16, 0.19, 0.21, 0.24, 0.28 }; // Bash
new const p_serpent[3] = { 1, 2, 3 }; // Serpent Ward
new const Float:p_banish[10] = { 0.04, 0.06, 0.08, 0.10, 0.13, 0.16, 0.19, 0.21, 0.24, 0.28 }; // Banish
new const Float:p_thorns[10] = { 0.06, 0.08, 0.10, 0.12, 0.15, 0.18, 0.21, 0.24, 0.27, 0.31 }; // Thorns Aura  
new const Float:p_critical[10] = { 0.04, 0.06, 0.08, 0.10, 0.13, 0.16, 0.19, 0.21, 0.24, 0.28 }; // Critical Strike
new const Float:p_carrion[10] = { 0.04, 0.06, 0.08, 0.10, 0.13, 0.16, 0.19, 0.21, 0.24, 0.28 }; // Carrion Beetles
new const Float:p_steel[10] = { 0.04, 0.06, 0.08, 0.10, 0.13, 0.16, 0.19, 0.21, 0.24, 0.28 }; // Steel Skin
new const Float:p_shadow[10] = { 0.04, 0.06, 0.08, 0.10, 0.13, 0.16, 0.19, 0.21, 0.24, 0.28 }; // Shadow Strike
new const Float:p_blink[5] = { 0.20, 0.40, 0.60, 0.80, 1.00 }; // Blink
new const Float:p_grenade[10] = { 1.08, 1.16, 1.24, 1.35, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0 }; // Critical Grenade
new const p_invisibility[10] = { 225, 215, 205, 195, 185, 175, 160, 155, 140, 135 }; // Invisibility
new const Float:p_phoenix[5] = { 0.20, 0.40, 0.60, 0.80, 1.00 } // Pheonix
new const Float:p_armor[10]  = { 0.04, 0.06, 0.08, 0.10, 0.13, 0.16, 0.19, 0.21, 0.24, 0.28 }; // Siphon Armor
new const Float:p_levitation[10] = { 0.8, 0.75, 0.7, 0.65, 0.60, 0.55, 0.5, 0.45, 0.4, 0.35 }; // Levitation
new const Float:p_fan[3] = { 0.05, 0.10, 0.15 }; // Fan of Knives
new const Float:p_evasion[5] = { 0.03, 0.05, 0.07, 0.09, 0.11 }; // Evasion
new const Float:p_trueshot[10] = { 0.04, 0.06, 0.08, 0.10, 0.13, 0.16, 0.19, 0.21, 0.24, 0.28 }; // Trueshot Aura

// ------------------------------------ Weapon speeds -------------------------------------
new const Float:CS_SPEED_VIP = 227.0;

new const Float:CS_WEAPON_SPEED[31] =
{
	0.0,
	250.0,      // CSW_P228
	0.0,
	260.0,      // CSW_SCOUT
	250.0,      // CSW_HEGRENADE
	240.0,      // CSW_XM1014
	250.0,      // CSW_C4
	250.0,      // CSW_MAC10
	240.0,      // CSW_AUG
	250.0,      // CSW_SMOKEGRENADE
	250.0,      // CSW_ELITE
	250.0,      // CSW_FIVESEVEN
	250.0,      // CSW_UMP45
	210.0,      // CSW_SG550
	240.0,      // CSW_GALI
	240.0,      // CSW_FAMAS
	250.0,      // CSW_USP
	250.0,      // CSW_GLOCK18
	210.0,      // CSW_AWP
	250.0,      // CSW_MP5NAVY
	220.0,      // CSW_M249
	230.0,      // CSW_M3
	230.0,      // CSW_M4A1
	250.0,      // CSW_TMP
	210.0,      // CSW_G3SG1
	250.0,      // CSW_FLASHBANG
	250.0,      // CSW_DEAGLE
	235.0,      // CSW_SG552
	221.0,      // CSW_AK47
	250.0,      // CSW_KNIFE
	245.0       // CSW_P90
};

new const Float:CS_WEAPON_SPEED_ZOOM[31] =
{
	0.0,
	0.0,
	0.0,
	220.0,      // CSW_SCOUT
	0.0,
	0.0,
	0.0,
	0.0,
	240.0,      // CSW_AUG
	0.0,
	0.0,
	0.0,
	0.0,
	150.0,      // CSW_SG550
	0.0,
	0.0,
	0.0,
	0.0,
	150.0,      // CSW_AWP
	0.0,
	0.0,
	0.0,
	0.0,
	0.0,
	150.0,      // CSW_G3SG1
	0.0,
	0.0,
	235.0,      // CSW_SG552
	0.0,
	0.0,
	0.0
};

// Spawnpoints
new const szSpawnEnt[2][] = {
	"info_player_start",
	"info_player_deathmatch"
};

// Counter-Strike/Condition Zero Skins
new SKIN_CT[4][]		= { "sas"		, "gsg9"	, "urban"		, "gign" };
new SKIN_T[4][]			= { "arctic"	, "leet"	, "guerilla"	, "terror" };

// Shopmenu Item names
new const ShopmenuLong_Names[MAX_SHOPMENU_ITEMS][] = {
	"Ankh of Reincarnation",
	"Boots of Speed",
	"Claws of Attack",
	"Cloak of Shadows",
	"Mask of Death",
	"Necklace of Immunity",
	"Orb of Frost",
	"Periapt of Health",
	"Tome of Experience",
	"Scroll of Respawning",
	"Mole Protectant",
	"Helm of Excellence",
	"Amulet of the Cat",
	"Sock of the Feater", 
	"Flaming Gloves of Warmth",
	"Ring of Regeneration +1",
	"Chameleon",
	"Mole"
}

// Shopmenu shot Item names
new const ShopmenuShort_Names[MAX_SHOPMENU_ITEMS][] = {
	"Ankh",
	"Boots",
	"Claw",
	"Cloak",
	"Mask",
	"Necklace",
	"Orb",
	"Periapt",
	"Tome",
	"Scroll",
	"Protectant",
	"Helm",
	"Amulet",
	"Sock",
	"Gloves",
	"Ring",
	"Chameleon",
	"Mole"
}

// Attribute names
new const Attribute_Names[MAX_ATTRIBS][] = {
	"Strength",
	"Intellect",
	"Dexterity",
	"Agility",
	"Constitution"
}

// Resistance Names
new const Resistance_Names[MAX_RESIST][] = {
	"Poison",
	"Disease",
	"Electricity",
	"Fire",
	"Magic"
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
