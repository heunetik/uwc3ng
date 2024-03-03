// Uwc3ng Skill handler functions

public SKILL_ShowMenu( id, side )
{
	if ( !UWC3NG_Check() || !is_user_connected( id ) )
	{
		return;
	}
	
	// The header
	new MenuText[2048] = "";
	formatex ( MenuText, charsmax( MenuText ), "\ySelect Skill: (pg-%d) [%d pts remaining]\w^n", side+1, p_data[id][AVAILABLE_SKILL] );
	formatex ( MenuText, charsmax( MenuText ), "%s\y(say /skillsinfo%d for info about skills on this page) \w^n^n", MenuText, side+1 );
	
	// We have to save the side for checking keys later
	p_data[id][SKILL_SIDE] = side;
	
	// Here we save if the skill is an ultimate
	new bool:b_IsUltimate = false;
	
	// Calculate the side with the skills on side
	side *= MAX_SKILLS_ONSIDE;
	
	new i, KEY = 0; 
	for ( i = side; i < MAX_SKILLS; ++i )
	{
		// Detect if the skill is an ultimate
		b_IsUltimate = SKILL_is_ultimate( i, true );
		
		// This skill is available. Add a key to press for it
		if ( p_data[id][PLAYER_LVL] >= Skill_REQLevel[i] && !b_IsUltimate )
		{
			// Add every skill to the menu
			formatex ( MenuText, charsmax( MenuText ), "%s\w%d. %s - %d/%d^n", MenuText, ( i+1 - side ), Skill_Names[i], p_data_skill[id][i], Skill_Max_Points[i] );
		
			// we count it for the valid keys
			KEY++;
		}
		
		// It's an available ultimate
		else if ( p_data[id][PLAYER_LVL] >= Skill_REQLevel[i] && b_IsUltimate )
		{
			// Add every ultimate to the menu
			formatex ( MenuText, charsmax( MenuText ), "%s\w%d. %s - %d/%d (bind key %s)^n", MenuText, ( i+1 - side ), Skill_Names[i], p_data_skill[id][i], Skill_Max_Points[i], Ultimate_Bindings[i] );
			
			// we count it for the valid keys
			KEY++;
		}
		
		// Unavailable skill / ultimate
		else 
		{
			// Add every skill / ultimate to the menu
			formatex ( MenuText, charsmax( MenuText ), "%s\d%d. %s - %d/%d (lvl %d required)^n", MenuText, ( i+1 - side ), Skill_Names[i], p_data_skill[id][i], Skill_Max_Points[i], Skill_REQLevel[i] );
			
			// We have to count this key
			KEY++;
		}
		
		// The page is full
		if ( ( i + 1 - side ) == MAX_SKILLS_ONSIDE )
		{
			break;
		}
	}
	
	// Add the next page menupoint
	formatex ( MenuText, charsmax( MenuText ), "%s^n\w9. Next Page^n", MenuText, ( i+2 - side ) ); 
			
	// We add the exit menupoint
	formatex ( MenuText, charsmax( MenuText ), "%s^n\w0. Exit^n", MenuText );
	
	// Create the keys
	static PRESSKEY;
	for ( i = 0; i < KEY; ++i )
	{
		PRESSKEY |= (1<<i);
	}
	
	// Add exit & next page button
	PRESSKEY |= (1<<9) | (1<<8);
	
	// Show the player the menu
	show_menu ( id, PRESSKEY, MenuText, 25, "HandleSkill" );
	
	return;
}

public HandlePressMenu( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	// The player want the next side
	if ( key == 8 )
	{
		// This is the last side. Go to the first
		if ( p_data[id][SKILL_SIDE] == ( MAX_SKILLS / MAX_SKILLS_ONSIDE ) - 1 )
		{
			p_data[id][SKILL_SIDE] = 0;
		}
		
		// It's not the last side
		else
		{
			// Add one
			p_data[id][SKILL_SIDE]++;
		}
		
		// Show the Menu
		SKILL_ShowMenu( id, p_data[id][SKILL_SIDE] );
		
		return;
	}
	
	// The player exit the menu
	else if ( key == 9 )
	{
		// The player was maybe in the drop skill menu. Reset this data
		p_data_b[id][SKILL_DROP] = false;
		
		// Reset the side
		p_data[id][SKILL_SIDE] = 0;
		
		return;
	}
		
	// We calculate the menu entry
	static MenuEntry;
	MenuEntry = ( p_data[id][SKILL_SIDE] * MAX_SKILLS_ONSIDE ) + key;
	
	// The player choosed an invalid menupoint
	if ( p_data[id][PLAYER_LVL] < Skill_REQLevel[MenuEntry] )
	{
		client_print( id, print_chat, "%s You selected an invalid menupoint", MOD_NAME );
		
		return;
	}
	
	// Has the player available skillpoints?
	if ( p_data[id][AVAILABLE_SKILL] <= 0 && !p_data_b[id][SKILL_DROP] )
	{
		client_print( id, print_chat, "%s You have no skill points to spend", MOD_NAME );
		
		return;
	}
	
	// He wants to drop a skill
	else if ( p_data_b[id][SKILL_DROP] )
	{
		// He can't drop an nonskilled skill
		if ( p_data_skill[id][MenuEntry] <= 0 )
		{
			return;
		}
		
		// He wants to drop Critical Grenade but he has Napalm grenades skilled
		if ( MenuEntry == SKILL_CRITICALGRENADE && p_data_skill[id][SKILL_NAPALMGRENADE] )
		{
			client_print( id, print_chat, "%s You have to drop first %s to drop this skill", MOD_NAME, Skill_Names[SKILL_NAPALMGRENADE] );
			
			return;
		}
		
		// He wants to drop Vampiric Aura or Leather Skin but he has Steel Skin
		if ( ( MenuEntry == SKILL_VAMPIRIC || MenuEntry == SKILL_LSKIN ) && p_data_skill[id][SKILL_STEELSKIN] )
		{
			client_print( id, print_chat, "%s You have to drop first %s to drop this skill", MOD_NAME, Skill_Names[SKILL_STEELSKIN] );
			
			return;
		}
		
		// Remove the skill
		p_data_skill[id][MenuEntry]--;
		
		// Add one available skill
		p_data[id][AVAILABLE_SKILL]++;
		
		// He can use this skillpoint at the next round
		if ( CVAR_csdm_active <= 0 )
		{
			p_data_b[id][USED_DROP] = true;
		}
		
		// Print him a message
		client_print( id, print_chat, "%s You have %d skillpoint(s) available (use /selectskill)", MOD_NAME, p_data[id][AVAILABLE_SKILL] );
		
		// Update the skills
		SKILL_Update( id );
		
		// Re-open the menu
		SKILL_ShowMenu( id, p_data[id][SKILL_SIDE] );
		
		return;
	}
	
	// The player dropped in this round skills. He has to wait one round to select the new skills
	if ( p_data_b[id][USED_DROP] )
	{
		client_print( id, print_chat, "%s You must wait one round to select new skills after dropping them", MOD_NAME );
		
		return;
	}
	
	// The player is one the max level from this skill
	if ( p_data_skill[id][MenuEntry] == Skill_Max_Points[MenuEntry] )
	{
		client_print( id, print_chat, "%s You have reached the maximum level for this skill", MOD_NAME );
		
		return;
	}
	
	// Napalm grenades can only skilled when Critical Grenades is skilled
	if ( p_data_skill[id][SKILL_CRITICALGRENADE] != Skill_Max_Points[SKILL_CRITICALGRENADE] && MenuEntry == SKILL_NAPALMGRENADE )
	{
		client_print( id, print_chat, "%s You must be full trained in %s to train this skill", MOD_NAME, Skill_Names[SKILL_CRITICALGRENADE] );
		
		return;
	}
	
	// he skilled devotion aura. Give him the extra hp
	if ( MenuEntry == SKILL_DEVOTION )
	{
		// Devotion Aura
		SKILL_DevotionAura( id );
	}
	
	// Another restrictions for the skills
	if ( !SKILL_Collide( id, MenuEntry ) )
	{
		// player can't skill it because he has another skill 
		return;
	}
	
	// The skill is an ultimate and the player has reached the maximum of ultimates
	if ( SKILL_is_ultimate( MenuEntry, false ) && g_iCountUltimates[id] >= MAX_ULTIMATES )
	{
		client_print( id, print_chat, "%s You have reached the maximum of trainable ultimates", MOD_NAME );
		
		return;
	}
	
	// Set his new Skill +1
	p_data_skill[id][MenuEntry]++;
	
	// extra check for Serpent Ward 
	if ( MenuEntry == SKILL_WARD  && !p_data_skill[id][SKILL_WARD] )
	{
		// Tell him that he can use this ultimate at the next round
		client_print ( id, print_chat, "%s You can use Serpent Ward at the next round", MOD_NAME );
	}
	
	// This skill is an ultimate. We count it that he will skill it
	if ( SKILL_is_ultimate( MenuEntry, false ) && p_data_skill[id][MenuEntry] == Skill_Max_Points[MenuEntry] )
	{
		g_iCountUltimates[id]++;
	}
	
	// We remove one skillpoint because he set one
	p_data[id][AVAILABLE_SKILL]--;
	
	// Is he now on the maximum?
	if ( p_data_skill[id][MenuEntry] == Skill_Max_Points[MenuEntry] )
	{
		new PlayerName[32];
		get_user_name( id, PlayerName, charsmax( PlayerName ) );
		
		client_print ( 0, print_chat, "%s Announcement :: %s has mastered %s", MOD_NAME, PlayerName, Skill_Names[MenuEntry] );
	}
	
	// He isn't at the maximum
	else if ( p_data_skill[id][MenuEntry] > 0 )
	{
		// Show him a message an re-open the menu if he has more aviable skills to train
		client_print ( id, print_chat, "%s You have expanded your knowledge of %s", MOD_NAME, Skill_Names[MenuEntry] );
	}
	
	// He learned the basics
	else
	{
		client_print( id, print_chat, "%s You have learned the basics of %s", MOD_NAME, Skill_Names[MenuEntry] );
	}
	
	// The player has more available skills, reopen the menu!
	if ( p_data[id][AVAILABLE_SKILL] > 0 )
	{
		SKILL_ShowMenu( id, p_data[id][SKILL_SIDE] );
	}
	
	// He has available attribs
	else if ( p_data[id][AVAILABLE_ATTRIBS] > 0 )
	{
		ATTRIB_ShowSelectMenu( id );
	}
	
	// He has available resists
	else if ( p_data[id][AVAILABLE_RESISTS] > 0 )
	{
		RESIST_ShowSelectMenu( id );
	}
	
	// Update his new skills
	SKILL_Update( id );
	
	return;
}

// Check with the skillid if this skill an ultimate is
bool:SKILL_is_ultimate( SkillID, DetectAbilitiy )
{
	if ( !equal( Ultimate_Bindings[SkillID], "" ) )
	{
		if ( DetectAbilitiy )
		{
			return true;
		}
		
		// WcRepair, WcDispell and WcMend are not ultimates!
		else if ( SkillID != SKILL_REPAIR && SkillID != SKILL_DISPELLHEX && SkillID != SKILL_MEND )
		{
			return true;
		}
	}
	
	return false;
}

// check if a player has any ultimates learned
public bool:SKILL_hasUltimate(id) {
	new ultimateIds[9] = { 14, 17, 25, 26, 31, 33, 34, 35, 36 };
	for(new i = 0; i < sizeof(ultimateIds); i++)
	{
		if(p_data_skill[id][ultimateIds[i]] != 0)
		{
			return true;
		}
	}

	return false;
}

// Update the skills for a players
public SKILL_Update( id )
{
	if ( !is_user_alive( id ) )
	{
		return;
	}
	
	// Levitation
	SKILL_Levitation( id );
	
	// Leather Skin & Healing Wave
	SHARED_HandleTasks( id )
	
	// Unholy Aura
	SKILL_UnholyAura( id );
	
	// Invisibility
	SKILL_Invisible( id );
}

// Skills triggered on damage
public SKILL_Damage( iAttacker, iVictim, iDamage, iHitPlace, iWeapon )
{
	// Wtf, the player is away.
	if ( !is_user_connected( iVictim ) || !is_user_connected( iAttacker ) )
	{
		return;
	}
	
	// Just check this when the victim is alive
	if ( is_user_alive( iVictim ) ) 
	{
		// Impale
		SKILL_Impale( iAttacker, iVictim );
		
		// Spiked Carapace
		SKILL_SpikedCarapace( iAttacker, iVictim, iDamage, iHitPlace );
	
		// Hex
		SKILL_Hex( iAttacker, iVictim );
	
		// Bash
		SKILL_Bash( iAttacker, iVictim );
	
		// Banish
		SKILL_Banish( iAttacker, iVictim );
	
		// Critical Strike
		SKILL_CriticalStrike( iAttacker, iVictim, iDamage, iHitPlace, iWeapon );
	
		// Carrion Beetles
		SKILL_CarrionBeetles( iAttacker, iVictim, iHitPlace );
		
		// Shadow Strike
		SKILL_ShadowStrike( iAttacker, iVictim, iHitPlace );
	
		// Critical Grenade
		SKILL_CriticalGrenade( iAttacker, iVictim, iDamage, iWeapon, iHitPlace );
		
		// Trueshot Aura
		SKILL_TrueShotAura( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
	}
	
	// Siphon Mana
	SKILL_SiphonMana( iAttacker, iVictim, iDamage );
	
	// Vampiric Aura
	SKILL_VampiricAura( iAttacker, iDamage );
	
	// Steel Skin
	SKILL_SteelSkin( iAttacker, iVictim, iDamage );
	
	// Siphon Armor
	SKILL_SiphonArmor( iAttacker, iVictim );
	
	// Offensive items
	ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
}

// Check if a player should get available skillpoints
public SKILL_available_points( id )
{
	// Get all given skillpoints
	new iGivenPoints = SKILL_given_points( id );
	
	// Calculate the difference
	iGivenPoints = ( p_data[id][PLAYER_LVL] - iGivenPoints );
	
	// // Check if the player has more skillpoints spent as available
	if ( iGivenPoints < 0 )
	{
		// We have to reset all skills
		p_data_b[id][RESET_SKILLS] = true;
		SKILL_ResetAllNow( id );
		
		// jump out the loop and the function
		return;
	}
			
	// Save the available skillpoints
	p_data[id][AVAILABLE_SKILL] = iGivenPoints;
	
	return;
}

// Count all given skillpoints
SKILL_given_points( id )
{
	// We loop through all skills from this player and count the levels from these skills
	static i, iGivenPoints; iGivenPoints = 0;
	for ( i = 0; i < MAX_SKILLS; ++i )
	{
		iGivenPoints += p_data_skill[id][i];
	}
	
	return iGivenPoints;
}

// Reset all skills from a player
public SKILL_ResetAll( id )
{
	// print him a message that all skills will be reseted
	client_print( id, print_center, "Your skills will be reset next round" );
	
	// We have to save it because on the next round we should reset all skills
	p_data_b[id][RESET_SKILLS] = true;
}

// Reset all skills from a player now (called on new round or spawn when csdm is enabled)
public SKILL_ResetAllNow( id )
{
	if ( p_data_b[id][RESET_SKILLS] )
	{
		// We loop through all skills and set it to zero
		for ( new j = 0; j < MAX_SKILLS; ++j )
		{
			p_data_skill[id][j] = 0;
		}
		
		// Detect the new available skills
		SKILL_available_points( id );
	
		// Next round the skills should no more reseted
		p_data_b[id][RESET_SKILLS] = false;
			
		// Reset the counter for the ultimates
		g_iCountUltimates[id] = 0;
	}
}

// Check if the skill collides with another skill
SKILL_Collide( id, iSkill )
{
	new bError = false;
	
	// Which skill is it?
	switch ( iSkill )
	{
		case SKILL_VENGEANCE: 
		{ // is it vengeance
			if ( p_data_skill[id][SKILL_BOMBER] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s You would have to first give up your Suicide Bomber ultimate to train this", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_BOMBER:
		{ // is it suicide bomber
			if ( p_data_skill[id][SKILL_VENGEANCE] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s You would have to first give up your Vegeance ultimate to train this", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_THORNSAURA:
		{ // is it thornsaura
			if ( p_data_skill[id][SKILL_SPIKED] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s Only one damage shield skill is allowed, you would have to give up your Spiked Carapace", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_SPIKED:
		{ // is it spiked carapace
			if ( p_data_skill[id][SKILL_THORNSAURA] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s Only one damage shield skill is allowed, you would have to give up your Thorns Aura", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_MANA:
		{ // is it siphon mana
			if ( p_data_skill[id][SKILL_SIPHONARMOR] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s You would have to first give up your Siphon Armor skill to train this", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_SIPHONARMOR:
		{ // is it siphon armor
			if ( p_data_skill[id][SKILL_MANA] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s You would have to first give up your Siphon Mana skill to train this", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_SHADOWSTRIKE:
		{ // is it shadow strike
			if ( p_data_skill[id][SKILL_CARRIONBEETLES] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s You would have to first give up your Carrion Beetles skill to train this", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_CARRIONBEETLES:
		{ // is it carrion beetles
			if ( p_data_skill[id][SKILL_SHADOWSTRIKE] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s You would have to first give up your Shadow Strike skill to train this", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_CRITICAL:
		{ // is it critical strike
			if ( p_data_skill[id][SKILL_TRUESHOTAURA] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s You would have to first give up your Trueshot Aura skill to train this", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_TRUESHOTAURA:
		{ // is it trueshot aura
			if ( p_data_skill[id][SKILL_CRITICAL] > 0 )
			{
				// tell that the client
				client_print( id, print_chat, "%s You would have to first give up your Critical Strike skill to train this", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
		
		case SKILL_STEELSKIN:
		{ // is it steel skin
			if ( p_data_skill[id][SKILL_VAMPIRIC] < 1 || p_data_skill[id][SKILL_LSKIN] < 3 )
			{
				// tell that the client
				client_print( id, print_chat, "%s You must be trained in 1 level of Vampiric Aura and 3 levels of Leather Skin to train this skill", MOD_NAME );
				
				// Error occured
				bError = true;
			}
		}
	}
	
	// Error occured?
	if ( bError )
	{
		return false;
	}
	
	// All okay
	return true;
}
	
	
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
