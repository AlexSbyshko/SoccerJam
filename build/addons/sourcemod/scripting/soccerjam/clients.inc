stock Client_SetScore(client, value)
{
	SetEntProp(client, Prop_Data, "m_iFrags", value);
}

stock Client_SetDeaths(client, value)
{
	SetEntProp(client, Prop_Data, "m_iDeaths", value);
}

public Client_Respawn(client)
{
	CS_RespawnPlayer(client);
}

public Client_KillForPreparing(client)
{
	Client_Kill(client)
	PrintSJMessage(client, "Print 'ready' when you're ready")
}

public Client_Kill(client)
{
	ForcePlayerSuicide(client)
}

ClearAllClients()
{
	ForEachClient(ClearClient)
}

public ClearClient(client)
{
	if (IsClientInGame(client))
	{
		Client_SetScore(client, 0)
		Client_SetDeaths(client, 0)
	}
}