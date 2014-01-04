public NFFK_Event_PrePlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
	if (!attacker || client == attacker)
	{
		Client_SetScore(client, Client_GetScore(client) + 1)
	}
	else
	{
		Client_SetScore(attacker, Client_GetScore(attacker) - 1)
	}
	Client_SetDeaths(client, Client_GetDeaths(client) - 1)
}