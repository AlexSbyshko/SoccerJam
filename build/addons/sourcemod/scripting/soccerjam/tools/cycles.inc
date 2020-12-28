//functag public ClientFunc(client);

// funcenum ClientFunc
// {
//    public(client),
//    public(client, value),
// }

typeset ClientFunc
{
  function void (int client);
  function void (int client, int value);
}

ForEachClient(ClientFunc:func, value = 0)
{
	for (new client = 1; client <= MaxClients; client++)
	{
		Call_StartFunction(INVALID_HANDLE, func)
		Call_PushCell(client)
		Call_PushCell(value)
		Call_Finish()
	}
}

ForEachPlayer(ClientFunc:func)
{
	for (new client = 1; client <= MaxClients; client++)
	{
		if (IsClientInGame(client)
			&& (GetClientTeam(client) == CS_TEAM_T || GetClientTeam(client) == CS_TEAM_CT))
		{
			Call_StartFunction(INVALID_HANDLE, func)
			Call_PushCell(client)
			Call_Finish()
		}
	}
}