void FragsForKillsDisabling(ClientDyingEvent clientDyingEvent)
{
	clientDyingEvent.Subscribe(OnClientDying)
}

static void OnClientDying(int client, int killer)
{
	if (!killer || client == killer)
	{
		Client_SetScore(client, Client_GetScore(client) + 1)
	}
	else
	{
		Client_SetScore(killer, Client_GetScore(killer) - 1)
	}
	Client_SetDeaths(client, Client_GetDeaths(client) - 1)
}