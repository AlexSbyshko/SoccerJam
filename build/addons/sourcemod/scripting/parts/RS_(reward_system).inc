static Handle:CreditCostConVar
static Handle:RewardsEnabledConVar
static Handle:InterceptionRewardConVar
static Handle:GoalRewardConVar
static Handle:LevelCreditCostMultConVar
static Handle:LevelCreditCostAddConVar

static PlayerCash[MAXPLAYERS+1]
static PlayerTotalCash[MAXPLAYERS+1]
static PlayerLevel[MAXPLAYERS+1]


public RS_Init()
{
	CreditCostConVar = CreateConVar("sj_initial_credit_cost", "500", "Cost of one credit (0 for disable rewards)", 0, true, 1.0)
	LevelCreditCostMultConVar = CreateConVar("sj_credit_cost_per_level_multiplier", "0.5", "Level cost multiplier", 0, true, 0.0)
	LevelCreditCostAddConVar = CreateConVar("sj_level_credit_cost_per_level_addition", "0", "Level cost multiplier", 0, true, 0.0)

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
	if (!GetConVarBool(RewardsEnabledConVar))
	{
		return
	}
	PrintToChatAll("add %i", value)

	new creditCost = GetCreditCostForPlayer(client)
	PrintToChatAll("credit cost: %i", creditCost)

	PlayerTotalCash[client] += value
	
	PlayerCash[client] += value
	if (PlayerCash[client] >= creditCost)
	{
		PlayerCash[client] -= creditCost
		PrintToChatAll("-%i", creditCost)
		PrintToChat(client, "+1 CREDIT")
		AddPlayerCredits(client, 1)
		PlayerLevel[client]++
		AddPlayerCash(client, 0)
	}
	PrintToChatAll("ost: %i next credit cost: %i", PlayerCash[client], GetCreditCostForPlayer(client))
	UpdateClientVisibleCash(client)
}

GetCreditCostForPlayer(client)
{
	new playerLevel = PlayerLevel[client]
	new initialCost = GetConVarInt(CreditCostConVar)
	new costAddition = GetConVarInt(LevelCreditCostAddConVar)
	new Float:costMultiplier = GetConVarFloat(LevelCreditCostMultConVar)

	
	new Float:creditCost = float(initialCost)
	for (new i = 0; i < playerLevel; i++)
	{
		creditCost = creditCost + (creditCost * costMultiplier) + costAddition
	}
	return RoundFloat(creditCost)
}

public ResetPlayerCash(client)
{
	ClearClientUpgrades(client)
	PlayerTotalCash[client] = 0
	PlayerCash[client] = 0
	PlayerLevel[client] = 0

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