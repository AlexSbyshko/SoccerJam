static Handle:CvarBallAutoRespawn
static Handle:TmrBallRespawn

public BAR_Init()
{
	CvarBallAutoRespawn = CreateConVar("sj_ball_respawn_time", "30", "Time after which the ball respawn (if no ball holder)", 0, true, 1.0)
}

public BAR_OnBallKicked(client)
{
	StartBallRespawnTimer()
}

public BAR_Event_PrePlayerDisconnect(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	if (client == g_BallHolder)
	{
		StartBallRespawnTimer()
	}
}

public BAR_OnBallReceived(client, oldBallOwner)
{
	StopBallRespawnTimer()
}

public BAR_OnBallSpawned(ballEntity, ballSpawnTeam)
{
	StopBallRespawnTimer()
}

public Action:BAR_Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	TmrBallRespawn = INVALID_HANDLE
	return Plugin_Continue
}

public BAR_OnGoal(team, scorer)
{
	StopBallRespawnTimer()
}

StartBallRespawnTimer()
{
	if (TmrBallRespawn == INVALID_HANDLE)
	{
		new Float:respawnTime = GetConVarFloat(CvarBallAutoRespawn)
		TmrBallRespawn = CreateTimer(respawnTime, Timer_RespawnBall, _, TIMER_FLAG_NO_MAPCHANGE)
	}
}

public BAR_OnMapStart()
{
	TmrBallRespawn = INVALID_HANDLE
}

public Action:Timer_RespawnBall(Handle:timer)
{
	StopBallRespawnTimer()
	RespawnBallWithNotify()
	return Plugin_Continue
}

StopBallRespawnTimer()
{
	ClearTimer(TmrBallRespawn)
}