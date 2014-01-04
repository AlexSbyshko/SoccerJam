public WM_Event_PlayerActivate(Handle:event, const String:name[], bool:dontBroadcast)
{
	new userId = GetEventInt(event, "userid")
	CreateTimer(10.0, Timer_PrintWelcomeMessage, userId)	
}

public Action:Timer_PrintWelcomeMessage(Handle:timer, any:userId) 
{
	new client = GetClientOfUserId(userId)
	if (client && IsClientInGame(client))
	{
		PrintSJMessage(client, "SoccerJam: Source v%s", SOCCERJAMSOURCE_VERSION)
		PrintSJMessage(client, SOCCERJAMSOURCE_URL)
	}
}