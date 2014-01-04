static Handle:CreditsForPublicGoalConVar

static bool:IsPublic
static AdditionCreditsCount

public TU_Init()
{
	CreditsForPublicGoalConVar = CreateConVar("sj_credits_for_goal", "2", "Number of credits that get all players at the goal (public mode only)", 0, true, 0.0)
}

public TU_OnGoal(team, scorer)
{
	if (IsPublic)
	{
		new creditsForGoal = GetConVarInt(CreditsForPublicGoalConVar)
		for (new i = 1; i <= MaxClients; i++)
		{
			AddPlayerCredits(i, creditsForGoal)
		}
		AdditionCreditsCount += creditsForGoal
	}
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
	IsPublic = true
}

public TU_OnStartMatch()
{
	AdditionCreditsCount = 0
	IsPublic = false
}