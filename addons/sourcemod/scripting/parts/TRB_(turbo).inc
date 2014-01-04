static Handle:TurboTimers[MAXPLAYERS+1]
static Handle:TurboEnableTimers[MAXPLAYERS+1]

static TurboDurationUpgradeId
static TurboCooldownUpgradeId
static IsInTurbo[MAXPLAYERS+1];
static bool:IsTurboEnabled[MAXPLAYERS+1];

public TRB_Init()
{
	TurboDurationUpgradeId = CreateUpgrade("turbo_duration", 5.0, 10.0, 5, 1, "s")
	TurboCooldownUpgradeId = CreateUpgrade("turbo_cooldown", 10.0, 5.0, 5, 1, "s")
	AddCommandListener(Cmd_Turbo, "drop")
}

public TRB_OnMapStart()
{
	ForEachClient(ClearClientTurbo)
}

public TRB_Event_MatchEndRestart(Handle:event, const String:name[], bool:dontBroadcast)
{	
	ForEachClient(ClearClientTurbo)
}

public TRB_Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{	
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	new team = GetClientTeam(client);
	if (team != CS_TEAM_NONE)
	{
		ClearClientTurbo(client)
		SetEntProp(client, Prop_Send, "m_iProgressBarDuration", 0)
	}
}

public TRB_Event_PlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	ClearClientTurbo(client)
}

Client_UpdateSpeed(client)
{
	new Float:multiplier = GetPlayerSpeedMultiplier(client)
	new Float:turboSpeedMultiplier = IsInTurbo[client] ? 0.3 : 0.0
	new Float:totalMultiplier = multiplier + turboSpeedMultiplier
	g_PlayerSpeedMultiplier[client] = totalMultiplier
}

public Action:Cmd_Turbo(client, const String:command[], argc) 
{
	Client_StartTurboMode(client)
	return Plugin_Handled
}

Client_StartTurboMode(client)
{
	if (IsTurboEnabled[client] && !IsInTurbo[client])
	{
		IsInTurbo[client] = true
		new Float:turboDuration = GetPlayerUpgradeValue(client, TurboDurationUpgradeId)
		TurboTimers[client] = CreateTimer(turboDuration, Timer_EndTurbo, client)		
		SetEntPropFloat(client, Prop_Send, "m_flProgressBarStartTime", GetGameTime())
		SetEntProp(client, Prop_Send, "m_iProgressBarDuration", RoundFloat(turboDuration))
		Client_UpdateSpeed(client)
	}
}

Client_EndTurboMode(client)
{
	TurboTimers[client] = INVALID_HANDLE
	IsInTurbo[client] = 0
	IsTurboEnabled[client] = false
	Client_UpdateSpeed(client)
	SetEntProp(client, Prop_Send, "m_iProgressBarDuration", 0)
}

Client_IsInTurboMode(client)
{
	return IsInTurbo[client]
}

public ClearClientTurbo(client)
{
	if (TurboTimers[client] != INVALID_HANDLE)
	{
		KillTimer(TurboTimers[client])
		TurboTimers[client] = INVALID_HANDLE
	}
	if (TurboEnableTimers[client] != INVALID_HANDLE)
	{
		KillTimer(TurboEnableTimers[client])
		TurboEnableTimers[client] = INVALID_HANDLE
	}	
	IsInTurbo[client] = 0
	IsTurboEnabled[client] = true
	Client_UpdateSpeed(client)
}

public Action:Timer_EndTurbo(Handle:timer, any:client) 
{
	Client_EndTurboMode(client)
	TurboEnableTimers[client] = CreateTimer(GetPlayerUpgradeValue(client, TurboCooldownUpgradeId), Timer_EnableTurbo, client)
	return Plugin_Continue
}

public Action:Timer_EnableTurbo(Handle:timer, any:client)
{
	IsTurboEnabled[client] = true
	if (TurboEnableTimers[client] != INVALID_HANDLE)
	{
		KillTimer(TurboEnableTimers[client])
		TurboEnableTimers[client] = INVALID_HANDLE
	}
	return Plugin_Continue
}