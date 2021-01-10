static Handle:CvarBallAutoRespawn
static Handle:TmrBallRespawn

void BallAutoReturning(
	MapStartedEvent mapStartedEvent,
	ClientDisconnectingEvent clientDisconnectingEvent,
	RoundStartedEvent roundStartedEvent,
	BallReceivedEvent ballReceivedEvent)
{
	mapStartedEvent.Subscribe(OnMapStarted)
	clientDisconnectingEvent.Subscribe(OnClientDisconnecting)
	roundStartedEvent.Subscribe(OnRoundStarted)
	ballReceivedEvent.Subscribe(OnOnBallReceived)

	CvarBallAutoRespawn = CreateConVar("sj_ball_respawn_time", "30", "Time after which the ball respawn (if no ball holder)", 0, true, 1.0)
}

public BAR_OnBallKicked(client)
{
	StartBallRespawnTimer()
}

static void OnClientDisconnecting(int client)
{
	if (client == g_BallHolder)
	{
		StartBallRespawnTimer()
	}
}

public BAR_OnBallHolderDeath(client)
{
	StartBallRespawnTimer()
}

static void OnOnBallReceived(client, oldBallOwner)
{
	StopBallRespawnTimer()
}

public BAR_OnBallSpawned(ballEntity, ballSpawnTeam)
{
	StopBallRespawnTimer()
}

static void OnRoundStarted()
{
	TmrBallRespawn = INVALID_HANDLE
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

static void OnMapStarted()
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