public MVP_OnGoal(team, scorer)
{
	IncreaseMvpCount(scorer);
}

IncreaseMvpCount(client)
{
	new oldMvpCount = CS_GetMVPCount(client);
	CS_SetMVPCount(client, oldMvpCount + 1);
}