static Handle:CreditCostConVar
static Handle:RewardsEnabledConVar
static Handle:InterceptionRewardConVar
static Handle:GoalRewardConVar
static Handle:GoalTeamRewardConVar
static Handle:AssistingRewardConVar
static Handle:LevelCreditCostMultConVar
static Handle:LevelCreditCostAddConVar
static Handle:CreditsPerLevelConVar

static PlayerCash[MAXPLAYERS+1]
static PlayerTotalCash[MAXPLAYERS+1]
static PlayerLevel[MAXPLAYERS+1]


public RS_Init()
{
	RewardsEnabledConVar = CreateConVar("sj_rewards_enabled", "0", "Enable rewards", 0, true, 0.0, true, 1.0)

	CreditsPerLevelConVar = CreateConVar("sj_credits_per_level_addition", "1", "Credits for level", 0, true, 0.0)

	CreditCostConVar = CreateConVar("sj_initial_credit_cost", "500", "Cost of one credit", 0, true, 1.0)
	LevelCreditCostMultConVar = CreateConVar("sj_credit_cost_per_level_multiplier", "1.2", "Level cost multiplier", 0, true, 1.0)
	LevelCreditCostAddConVar = CreateConVar("sj_credit_cost_per_level_addition", "0", "Level cost multiplier", 0, true, 0.0)
	
	InterceptionRewardConVar = CreateConVar("sj_interception_reward", "50", "Reward for ball interception", 0, true, 0.0)
	GoalTeamRewardConVar = CreateConVar("sj_goal_team_reward", "100", "Reward ALL players in team for goal scoring", 0, true, 0.0)
	GoalRewardConVar = CreateConVar("sj_goal_reward", "300", "Reward goal scoring", 0, true, 0.0)
	AssistingRewardConVar = CreateConVar("sj_assisting_reward", "200", "Reward assisting", 0, true, 0.0)
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
	for (new client = 1; client <= MaxClients; client++)
	{
		if (IsClientInGame(client) && GetClientTeam(client) == team)
		{
			AddPlayerCash(scorer, GetConVarInt(GoalTeamRewardConVar))
		}
	}
	AddPlayerCash(scorer, GetConVarInt(GoalRewardConVar))
}

public RS_OnClientAssisted(client)
{
	AddPlayerCash(client, GetConVarInt(AssistingRewardConVar))
}

AddPlayerCash(client, value)
{	
	if (!GetConVarBool(RewardsEnabledConVar))
	{
		return
	}
	new creditCost = GetCreditCostForPlayer(client)

	PlayerTotalCash[client] += value
	
	PlayerCash[client] += value
	if (PlayerCash[client] >= creditCost)
	{
		PlayerCash[client] -= creditCost
		AddPlayerCredits(client, GetConVarInt(CreditsPerLevelConVar))
		PlayerLevel[client]++
		PrintSJMessage(client, "You have reached level %i and got %i credits. Next level cost: $%i", PlayerLevel[client], GetConVarInt(CreditsPerLevelConVar), GetCreditCostForPlayer(client))
		AddPlayerCash(client, 0)
	}
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
		creditCost = (creditCost * costMultiplier) + costAddition
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