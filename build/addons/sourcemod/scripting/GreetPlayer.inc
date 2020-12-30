void GreetPlayer(int userId)
{
    CreateTimer(10.0, Timer_PrintWelcomeMessage, userId)
}

static Action:Timer_PrintWelcomeMessage(Handle:timer, any:userId) 
{
	new client = GetClientOfUserId(userId)
	if (client && IsClientInGame(client))
	{
		PrintSJMessage(client, "SoccerJam: Source v%s", SOCCERJAMSOURCE_VERSION)
		PrintSJMessage(client, SOCCERJAMSOURCE_URL)
	}
}