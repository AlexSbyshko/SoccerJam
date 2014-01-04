static Handle:DisarmTimers[MAXPLAYERS+1]

static DisarmUpgradeId

static DisarmStatsId
static KnifeKillsStatsId

public DSRM_Init()
{
	DisarmUpgradeId = CreateUpgrade("disarm", 10.0, 90.0, 5, 1, "%")

	DisarmStatsId = CreateMatchStats("Most disarm")
	KnifeKillsStatsId = CreateMatchStats("Most knife kills")
}

public DSRM_OnMapStart()
{
	ForEachClient(ResetClientWeapon)
}

public DSRM_Event_MatchEndRestart(Handle:event, const String:name[], bool:dontBroadcast)
{	
	ForEachClient(ResetClientWeapon)
}

public DSRM_Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{	
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	new team = GetClientTeam(client)	
	if (team != CS_TEAM_NONE)
	{
		ResetClientWeapon(client)
	}
	
}

public DSRM_Event_PlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	new attacker = GetClientOfUserId(GetEventInt(event, "attacker"))
	ResetClientWeapon(client)
	if (attacker > 0 && client != attacker)
	{
		decl String:weaponName[MAX_NAME_LENGTH]
		GetEventString(event, "weapon", weaponName, sizeof(weaponName))
		if (StrEqual(weaponName, "weapon_knife"))
		{
			AddMatchStatsValue(KnifeKillsStatsId, attacker, 1)
		}
	}
}

public DSRM_Event_PlayerHurt(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	new attacker = GetClientOfUserId(GetEventInt(event, "attacker"))
	
	if (client == attacker)
	{
		return
	}	
	new rand = GetRandomInt(1, 100)

	if (GetPlayerUpgradeValue(attacker, DisarmUpgradeId) >= rand)
	{
		if (client == g_BallHolder) 
		{
			SetBallHolder(attacker)
		}
		else
		{
			Client_RemoveAllWeapons(client)
			DisarmTimers[client] = CreateTimer(4.0, Timer_GiveWeapon, client)
		}
		AddMatchStatsValue(DisarmStatsId, attacker, 1)
	}
}

public Action:Timer_GiveWeapon(Handle:timer, any:client)
{
	DisarmTimers[client] = INVALID_HANDLE
	GivePlayerWeapon(client)
	return Plugin_Continue
}

public ResetClientWeapon(client)
{
	if (DisarmTimers[client] != INVALID_HANDLE)
	{
		KillTimer(DisarmTimers[client])
		DisarmTimers[client] = INVALID_HANDLE
	}
}