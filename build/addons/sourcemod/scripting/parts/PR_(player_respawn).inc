void PlayerRespawning(ClientDiedEvent clientDiedEvent)
{
	clientDiedEvent.Subscribe(OnClientDied)
}

static void OnClientDied(int client)
{
	int userId = GetClientUserId(client)
	CreateTimer(2.0, Timer_PlayerRespawn, userId)
}

public Action Timer_PlayerRespawn(Handle timer, any userId)
{
	new client = GetClientOfUserId(userId)
	if (client)
	{
		if (!IsPlayerAlive(client) 
			&& GetClientTeam(client) != CS_TEAM_NONE
			&& GetClientTeam(client) != CS_TEAM_SPECTATOR
			&& !Match_IsWaitingPlayers())
		{
			CS_RespawnPlayer(client)
		}
	}
	return Plugin_Continue
}