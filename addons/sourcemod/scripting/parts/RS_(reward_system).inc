static Handle:CreditCostConVar
static Handle:RewardsEnabledConVar
static Handle:InterceptionRewardConVar
static Handle:GoalRewardConVar

static PlayerCash[MAXPLAYERS+1]
static PlayerTotalCash[MAXPLAYERS+1]


public RS_Init()
{
	CreditCostConVar = CreateConVar("sj_credit_cost", "500", "Cost of one credit", 0, true, 0.0)
	RewardsEnabledConVar = CreateConVar("sj_rewards_enabled", "0", "Enable rewards", 0, true, 0.0, true, 1.0)
	InterceptionRewardConVar = CreateConVar("sj_interception_reward", "150", "Reward for ball interception", 0, true, 0.0, true, 1.0)
	GoalRewardConVar = CreateConVar("sj_goal_reward", "500", "Reward goal scoring", 0, true, 0.0, true, 1.0)
}

public RS_OnStartPublic()
{
	ForEachClient(ResetPlayerCash)
}

public RS_OnBallReceived(ballHolder, oldBallOwner)
{
	new team = GetClientTeam(ballHolder)
	if (oldBallOwner > 0
		&& team != GetClientTeam(oldBallOwner))
	{
		AddPlayerCash(ballHolder, GetConVarInt(InterceptionRewardConVar))
	}
}

public RS_OnGoal(team, scorer)
{
	AddPlayerCash(scorer, GetConVarInt(GoalRewardConVar))
}

AddPlayerCash(client, value)
{
	if (IsMatchPublic() && GetConVarBool(RewardsEnabledConVar))
	{
		PlayerTotalCash[client] += value
		new creditCost = GetConVarInt(CreditCostConVar)
		
		new newCach = PlayerCash[client] + value
		PlayerCash[client] += value % creditCost

		new creditsCount = newCach / creditCost
		AddPlayerCredits(client, creditsCount)

		PlayerCash[client] = newCach % creditCost

		UpdateClientVisibleCash(client)
	}
}

public ResetPlayerCash(client)
{
	ClearClientUpgrades(client)
	PlayerTotalCash[client] = 0
	PlayerCash[client] = 0

	UpdateClientVisibleCash(client)
}

UpdateClientVisibleCash(client)
{
	if (IsClientInGame(client))
	{
		new accountOffset = FindSendPropOffs("CCSPlayer", "m_iAccount")
		SetEntData(client, accountOffset, PlayerCash[client])
	}
}