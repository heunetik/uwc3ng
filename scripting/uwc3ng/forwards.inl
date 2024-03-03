// Uwc3ng forwards

new fwd_ReportKill = -1;

public FWD_Register()
{
	//reportKill(killer, victim, weaponIndex, weaponName); 
	//iKiller, iVictim, iWeapon, szWpnName
	fwd_ReportKill = CreateMultiForward( "wc3_reportkill", ET_IGNORE, FP_CELL, FP_CELL, FP_CELL, FP_STRING );
}

public FWD_Destroy()
{
	DestroyForward( fwd_ReportKill );
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
