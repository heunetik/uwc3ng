// Uwc3ng graphics

// Create the hudmessage for the ultimates and the items
public Create_Hud()
{
	if ( !UWC3NG_Check() )
	{
		return;
	}
	
	// Get all players
	static i, players[32], numofplayers, DisplayMessage[128];
	get_players( players, numofplayers );
	
	// Here we save if the player's ultimate is ready
	new bool:p_UltReady = false;
	
	// We save the names from the items
	static ItemOneName[32], ItemTwoName[32];
	
	// Loop through all players
	for ( i = 0; i < numofplayers; ++i )
	{
		// Is the player alive?
		if ( is_user_alive( players[i] ) )
		{
			
			// Is the ultimate ready?
			p_UltReady = false;
			DisplayMessage = "";
			
			if ( g_PlayerUltimateDelay[players[i]] <= 0 )
			{
				p_UltReady = true;
			}
			
			if ( SKILL_hasUltimate(players[i]) )
			{
				formatex ( DisplayMessage, charsmax( DisplayMessage ), "[ Ultimates %s ]", ( p_UltReady ? "Ready" : "Not Ready" ) );
			}
			
			// Two necklace, helms and rings? what?
			if ( g_iShopMenuItems[players[i]][ITEM_SLOT_ONE] == g_iShopMenuItems[players[i]][ITEM_SLOT_TWO] )
			{
				g_iShopMenuItems[players[i]][ITEM_SLOT_TWO] = ITEM_NONE;
			}
			
			// copy the names from the items in the array
			if ( g_iShopMenuItems[players[i]][ITEM_SLOT_ONE] != ITEM_NONE )
			{
				copy( ItemOneName, charsmax( ItemOneName ), ShopmenuShort_Names[g_iShopMenuItems[players[i]][ITEM_SLOT_ONE]] );
			}
			
			if ( g_iShopMenuItems[players[i]][ITEM_SLOT_TWO] != ITEM_NONE )
			{
				copy( ItemTwoName, charsmax( ItemTwoName ), ShopmenuShort_Names[g_iShopMenuItems[players[i]][ITEM_SLOT_TWO]] );
			}
			
			// The item is necklace. Add the charges
			switch ( g_iShopMenuItems[players[i]][ITEM_SLOT_ONE] ) 
			{
				case ITEM_NECKLACE: // Necklace
				{
					formatex( ItemOneName, charsmax( ItemOneName ), "%s[%d]", ItemOneName, g_iNecklaceCharges[players[i]] );
				}
				
				case ITEM_HELM: // Helm
				{
					formatex( ItemOneName, charsmax( ItemOneName ), "%s[%d]", ItemOneName, g_iHelmCharges[players[i]] );
				}
				
				case ITEM_RING: // Ring
				{
					formatex( ItemOneName, charsmax( ItemOneName ), "%s[%d]", ItemOneName, g_iTotalRings[players[i]] );
				}
			}
			
			// Slot two check
			switch ( g_iShopMenuItems[players[i]][ITEM_SLOT_TWO] )
			{
				case ITEM_NECKLACE: // Necklace
				{
					formatex( ItemTwoName, charsmax( ItemTwoName ), "%s[%d]", ItemTwoName, g_iNecklaceCharges[players[i]] );
				}
				
				case ITEM_HELM: // Helm
				{
					formatex( ItemTwoName, charsmax( ItemTwoName ), "%s[%d]", ItemTwoName, g_iHelmCharges[players[i]] );
				}
				
				case ITEM_RING: // Ring
				{
					formatex( ItemTwoName, charsmax( ItemTwoName ), "%s[%d]", ItemTwoName, g_iTotalRings[players[i]] );
				}
			}

			// The player has two items
			if ( g_iShopMenuItems[players[i]][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[players[i]][ITEM_SLOT_TWO] > ITEM_NONE )
			{
				formatex ( DisplayMessage, charsmax( DisplayMessage ), "%s Magic Items: %s and %s", DisplayMessage, ItemOneName, ItemTwoName );
			}
			
			// The player has only item slot one
			else if ( g_iShopMenuItems[players[i]][ITEM_SLOT_ONE] > ITEM_NONE )
			{
				formatex ( DisplayMessage, charsmax( DisplayMessage ), "%s Magic Item: %s", DisplayMessage, ItemOneName );
			}
			
			// The player has only item slot two
			else if ( g_iShopMenuItems[players[i]][ITEM_SLOT_TWO] > ITEM_NONE )
			{
				formatex ( DisplayMessage, charsmax( DisplayMessage ), "%s Magic Item: %s", DisplayMessage, ItemTwoName );
			}
			
			// How we have to show the message?
			if ( get_pcvar_num( CVAR_uwc3ng_hudmessage_display ) == 1 )
			{
				// Show the message as status text
				Create_StatusText( players[i], 0, DisplayMessage );
			}
			
			// Hudmessage
			else 
			{
				// Okay. Show him the hudmessage
				UWC3NG_StatusText( players[i], TXT_ULT_ITEM, DisplayMessage );
			}
		}
	}
	
	// Set up the next task
	set_task( 0.5, "Create_Hud", TASK_HUD );
	
	return;
}
			

// Create a status text where we display the XPs, Level and the rank from the player
stock Create_StatusText(id, linenumber, text[]){

	message_begin( MSG_ONE, gmsgStatusText, {0,0,0}, id )
	write_byte( linenumber )			// line number of status bar text
	write_string( text )				// status bar text
	message_end()
}

// Create a screenfade for the user
stock Create_ScreenFade(id, duration, holdtime, fadetype, red, green, blue, alpha){

	message_begin( MSG_ONE,gmsgScreenFade,{0,0,0},id )			
	write_short( duration )			// fade lasts this long duration
	write_short( holdtime )			// fade lasts this long hold time
	write_short( fadetype )			// fade type (in / out)
	write_byte( red )				// fade red
	write_byte( green )				// fade green
	write_byte( blue )				// fade blue
	write_byte( alpha )				// fade alpha
	message_end()
}

// Create a screenshake for a player
stock Create_ScreenShake(id, amount, duration, frequency){

	message_begin(MSG_ONE,gmsgScreenShake,{0,0,0},id) 
	write_short( amount )				// ammount 
	write_short( duration )				// lasts this long 
	write_short( frequency )			// frequency
	message_end()
}

// Creates an implosion effect
stock Create_TE_IMPLOSION(position[3], radius, count, life){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte ( TE_IMPLOSION )
	write_coord( position[0] )			// position (X)
	write_coord( position[1] )			// position (Y)
	write_coord( position[2] )			// position (Z)
	write_byte ( radius )				// radius
	write_byte ( count )				// count
	write_byte ( life )					// life in 0.1's
	message_end()
}

// Create a deathmsg
stock Create_DeathMsg_CS(killer_id,victim_id,headshot,weaponname[]){

	message_begin(MSG_ALL,gmsgDeathMsg)
	write_byte(killer_id)
	write_byte(victim_id)
	write_byte(headshot)
	write_string(weaponname)
	message_end()
}

// Create a scoreinfo
stock Create_ScoreInfo(id,frags,deaths,playerClass,team ){

	message_begin(MSG_ALL,gmsgScoreInfo)
	write_byte(id)
	write_short(frags)
	write_short(deaths)
	write_short(playerClass)
	write_short(team)
	message_end()
}

// Create the sprite above a player if he needs heal or armor (from uwc3)
stock Create_Sprite(id, idself, sprite){
	
	message_begin(MSG_ONE,SVC_TEMPENTITY,{0,0,0},idself)
	write_byte(TE_PLAYERATTACHMENT)
	write_byte(id)
	write_coord(30)       // z axis for sprite
	write_short(sprite)  // which sprite to display
	write_short(50)
	message_end()
}

// Create pentagram symbols
stock Create_Pentagram(origin[3],sprite){
	
	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte( TE_SPRITETRAIL )
	write_coord( origin[0])
	write_coord( origin[1])
	write_coord( origin[2] + 40)
	write_coord( origin[0])
	write_coord( origin[1])
	write_coord( origin[2])
	write_short( sprite )
	write_byte( 4 )   // count
	write_byte( 3 )   // life
	write_byte( 4 )    // scale
	write_byte( 20 )   // velocity
	write_byte( 10 )   // randomness in velocity
	message_end()
}

stock Create_TE_SPRITETRAIL(start[3], end[3], iSprite, count, life, scale, velocity, random ){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte( TE_SPRITETRAIL )
	write_coord( start[0] )				// start position (X)
	write_coord( start[1] )				// start position (Y)
	write_coord( start[2] )				// start position (Z)
	write_coord( end[0] )				// end position (X)
	write_coord( end[1] )				// end position (Y)
	write_coord( end[2] )				// end position (Z)
	write_short( iSprite )				// sprite index
	write_byte( count )					// count
	write_byte( life)					// life in 0.1's
	write_byte( scale)					// scale in 0.1's
	write_byte( velocity )				// velocity along vector in 10's
	write_byte( random )				// randomness of velocity in 10's
	message_end()
}

stock Create_TE_BEAMCYLINDER(origin[3], center[3], axis[3], iSprite, startFrame, frameRate, life, width, amplitude, red, green, blue, brightness, speed){

	message_begin( MSG_PAS, SVC_TEMPENTITY, origin )
	write_byte( TE_BEAMCYLINDER )
	write_coord( center[0] )			// center position (X)
	write_coord( center[1] )			// center position (Y)
	write_coord( center[2] )			// center position (Z)
	write_coord( axis[0] )				// axis and radius (X)
	write_coord( axis[1] )				// axis and radius (Y)
	write_coord( axis[2] )				// axis and radius (Z)
	write_short( iSprite )				// sprite index
	write_byte( startFrame )			// starting frame
	write_byte( frameRate )				// frame rate in 0.1's
	write_byte( life )					// life in 0.1's
	write_byte( width )					// line width in 0.1's
	write_byte( amplitude )				// noise amplitude in 0.01's
	write_byte( red )					// color (red)
	write_byte( green )					// color (green)
	write_byte( blue )					// color (blue)
	write_byte( brightness )			// brightness
	write_byte( speed )					// scroll speed in 0.1's
	message_end()
}

stock Create_TE_BEAMFOLLOW(entity, iSprite, life, width, red, green, blue, alpha){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte( TE_BEAMFOLLOW )
	write_short( entity )			// entity
	write_short( iSprite )			// model
	write_byte( life )				// life
	write_byte( width )				// width
	write_byte( red )				// red
	write_byte( green )				// green
	write_byte( blue )				// blue
	write_byte( alpha )				// brightness
	message_end()
}

stock Create_BarTime(id, duration, flag){

	message_begin( MSG_ONE, gmsgBarTime, {0,0,0}, id )
	write_byte( duration ) // duration 
	write_byte( flag )
	message_end() 
}

stock Create_TE_TELEPORT(position[3]){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY) 
	write_byte( TE_TELEPORT ) 
	write_coord( position[0] ) 
	write_coord( position[1] ) 
	write_coord( position[2] ) 
	message_end()
}

stock Create_TE_ELIGHT(entity, start[3], radius, red, green, blue, life, decayRate){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte( TE_ELIGHT )
	write_short( entity )			// entity
	write_coord( start[0] )			// initial position
	write_coord( start[1] )			// initial position
	write_coord( start[2] )			// initial position
	write_coord( radius )			// radius
	write_byte( red )				// red
	write_byte( green )				// green
	write_byte( blue )				// blue
	write_byte( life )				// life
	write_coord( decayRate )		// decay rate
	message_end()
}

stock Create_TE_LARGEFUNNEL(position[3], iSprite, flags){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )	
	write_byte( TE_LARGEFUNNEL )
	write_coord( position[0] )		// origin, x
	write_coord( position[1] )		// origin, y
	write_coord( position[2] )		// origin, z
	write_short( iSprite )			// sprite (0 for none)
	write_short( flags )			// 0 for collapsing, 1 for sending outward
	message_end() 
}

stock Create_TE_BEAMENTS(startEntity, endEntity, iSprite, startFrame, frameRate, life, width, noise, red, green, blue, alpha, speed){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte( TE_BEAMENTS )
	write_short( startEntity )		// start entity
	write_short( endEntity )		// end entity
	write_short( iSprite )			// model
	write_byte( startFrame )		// starting frame
	write_byte( frameRate )			// frame rate
	write_byte( life )				// life
	write_byte( width )				// line width
	write_byte( noise )				// noise amplitude
	write_byte( red )				// red
	write_byte( green )				// green
	write_byte( blue )				// blue
	write_byte( alpha )				// brightness
	write_byte( speed )				// scroll speed
	message_end()
}

stock Create_TE_BEAMPOINTS(start[3], end[3], iSprite, startFrame, frameRate, life, width, noise, red, green, blue, alpha, speed){

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY )
	write_byte( TE_BEAMPOINTS )
	write_coord( start[0] )
	write_coord( start[1] )
	write_coord( start[2] )
	write_coord( end[0] )
	write_coord( end[1] )
	write_coord( end[2] )
	write_short( iSprite )			// model
	write_byte( startFrame )		// start frame
	write_byte( frameRate )			// framerate
	write_byte( life )				// life
	write_byte( width )				// width
	write_byte( noise )				// noise
	write_byte( red)				// red
	write_byte( green )				// green
	write_byte( blue )				// blue
	write_byte( alpha )				// brightness
	write_byte( speed )				// speed
	message_end()
}

stock Create_TE_EXPLOSION( origin[3], origin2[3], iSprite, scale, frameRate, flags ){

	message_begin( MSG_PVS, SVC_TEMPENTITY, origin )
	write_byte( TE_EXPLOSION )
	write_coord( origin2[0] )			// position (X)
	write_coord( origin2[1] )			// position (Y)
	write_coord( origin2[2]	)			// position (Z)
	write_short( iSprite )				// sprite index
	write_byte( scale )					// scale in 0.1's
	write_byte( frameRate )				// framerate
	write_byte( flags )					// flags
	message_end()
}

stock Create_TE_SPRITE(position[3], iSprite, scale, alpha){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY ) 
	write_byte( TE_SPRITE ) 
	write_coord( position[0] )		// position)
	write_coord( position[1] ) 
	write_coord( position[2] ) 
	write_short( iSprite )			// sprite index
	write_byte( scale)				// scale in 0.1's
	write_byte( alpha )				// brightness
	message_end() 
}

stock Create_TE_Smoke(originSight[3], position[3], iSprite, scale, framerate){

	message_begin( MSG_PVS, SVC_TEMPENTITY, originSight )
	write_byte( TE_SMOKE )
	write_coord( position[0] )			// Position
	write_coord( position[1] )
	write_coord( position[2] )
	write_short( iSprite )				// Sprite index
	write_byte( scale )					// scale * 10
	write_byte( framerate  )			// framerate
	message_end()
}

stock Create_CarrionShadowDOT( id, iSprite ){
	
	message_begin( MSG_ALL, SVC_TEMPENTITY )
	write_byte( TE_PLAYERATTACHMENT )
	write_byte( id ) 
	write_coord( 0 )
	write_short( iSprite )
	write_short( 25 )
	message_end()
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
