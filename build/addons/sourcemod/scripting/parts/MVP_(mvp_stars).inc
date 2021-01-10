void MvpForGoalScoring()
{
	GoalScored.Subscribe(OnGoalScored)
}

static void OnGoalScored(team, scorer)
{
	IncreaseMvpCount(scorer);
}

IncreaseMvpCount(client)
{
	new oldMvpCount = CS_GetMVPCount(client);
	CS_SetMVPCount(client, oldMvpCount + 1);
}