static Handle:GoalCreditsAdditionGoalConVar

static AdditionCreditsCount

public TU_Init()
{
	GoalCreditsAdditionGoalConVar = CreateConVar("sj_goal_credits_addition", "0", "Number of credits that get all players at the goal", 0, true, 0.0)
}

public TU_OnGoal(team, scorer)
{
	new creditsForGoal = GetConVarInt(GoalCreditsAdditionGoalConVar)
	for (new i = 1; i <= MaxClients; i++)
	{
		AddPlayerCredits(i, creditsForGoal)
	}
	AdditionCreditsCount += creditsForGoal
}

public TU_Event_PrePlayerDisconnect(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	ClearClientUpgrades(client)
	AddPlayerCredits(client, AdditionCreditsCount)
}

public TU_OnStartPublic()
{
	AdditionCreditsCount = 0
}

public TU_OnStartMatch()
{
	AdditionCreditsCount = 0
}

public TU_OnClientResetUpgrades(client)
{
	AddPlayerCredits(client, AdditionCreditsCount)
}