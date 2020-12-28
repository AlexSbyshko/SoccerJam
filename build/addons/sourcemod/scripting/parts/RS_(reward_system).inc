static Handle:CreditCostConVar
static Handle:RewardsEnabledConVar
static Handle:InterceptionRewardConVar
static Handle:GoalRewardConVar
static Handle:GoalTeamRewardConVar
static Handle:AssistingRewardConVar
static Handle:LevelCostMultConVar
static Handle:LevelCostAddConVar
static Handle:CreditsPerLevelConVar

static Handle:LevelCostCacheTrie

static PlayerCash[MAXPLAYERS+1]
static PlayerTotalCash[MAXPLAYERS+1]
static PlayerLevel[MAXPLAYERS+1]

static LevelUpSoundId


public RS_Init()
{
	RewardsEnabledConVar = CreateConVar("sj_rewards_enabled", "0", "Enable rewards", 0, true, 0.0, true, 1.0)

	CreditsPerLevelConVar = CreateConVar("sj_credits_per_level_addition", "1", "Credits for level", 0, true, 0.0)

	CreditCostConVar = CreateConVar("sj_first_level_cost", "500", "Level 1 cost", 0, true, 1.0)
	LevelCostMultConVar = CreateConVar("sj_level_cost_multiplier", "1.2", "Level cost multiplier", 0, true, 1.0)
	LevelCostAddConVar = CreateConVar("sj_level_cost_addition", "0", "Level cost addition", 0, true, 0.0)
	
	InterceptionRewardConVar = CreateConVar("sj_interception_reward", "50", "Reward for ball interception", 0, true, 0.0)
	GoalTeamRewardConVar = CreateConVar("sj_goal_team_reward", "100", "Reward ALL players in team for goal scoring", 0, true, 0.0)
	GoalRewardConVar = CreateConVar("sj_goal_reward", "300", "Reward goal scoring", 0, true, 0.0)
	AssistingRewardConVar = CreateConVar("sj_assisting_reward", "200", "Reward assisting", 0, true, 0.0)

	LevelCostCacheTrie = CreateTrie()

	LevelUpSoundId = CreateSound("level_up")
}

public RS_OnStartPublic()
{
	if (!GetConVarBool(RewardsEnabledConVar))
	{
		return
	}
	ForEachClient(ResetPlayerCash)
}

public RS_OnGetUpgradeInfo(client, String:info[512])
{
	if (!GetConVarBool(RewardsEnabledConVar))
	{
		return
	}
	Format(info, sizeof(info), "%s\nLevel: %i\nNext level: $%i\n\n", info, PlayerLevel[client], GetNextLevelCost(client))
}

public RS_OnBallReceived(ballHolder, oldBallOwner)
{
	if (!GetConVarBool(RewardsEnabledConVar))
	{
		return
	}
	new team = GetClientTeam(ballHolder)
	if (oldBallOwner > 0
		&& team != GetClientTeam(oldBallOwner))
	{
		new credits =  GetConVarInt(InterceptionRewardConVar)
		PrintSJMessage(ballHolder, "You have gained %i for %s", credits, "Interception")
		AddPlayerCash(ballHolder, credits)
	}
}

public RS_OnGoal(team, scorer)
{
	if (!GetConVarBool(RewardsEnabledConVar))
	{
		return
	}
	new credits
	for (new client = 1; client <= MaxClients; client++)
	{
		if (IsClientInGame(client) && GetClientTeam(client) == team)
		{
			credits =  GetConVarInt(GoalTeamRewardConVar)
			PrintSJMessage(client, "You have gained %i for %s", credits, "Team Goal")
			AddPlayerCash(scorer, credits)
		}
	}
	credits = GetConVarInt(GoalRewardConVar)
	PrintSJMessage(scorer, "You have gained %i for %s", credits, "Goal scoring")
	AddPlayerCash(scorer, credits)
}

public RS_OnClientAssisted(client)
{
	if (!GetConVarBool(RewardsEnabledConVar))
	{
		return
	}
	new credits =  GetConVarInt(AssistingRewardConVar)
	PrintSJMessage(client, "You have gained %i for %s", credits, "Goal Assisting")
	AddPlayerCash(client, credits)
}

public RS_OnClientResetUpgrades(client)
{
	if (!GetConVarBool(RewardsEnabledConVar))
	{
		return
	}
	AddPlayerCredits(client, GetConVarInt(CreditsPerLevelConVar) * PlayerLevel[client])
}

AddPlayerCash(client, value)
{	
	if (!GetConVarBool(RewardsEnabledConVar))
	{
		return
	}
	new creditCost = GetNextLevelCost(client)

	PlayerTotalCash[client] += value
	
	PlayerCash[client] += value
	if (PlayerCash[client] >= creditCost)
	{
		PlayerCash[client] -= creditCost
		AddPlayerCredits(client, GetConVarInt(CreditsPerLevelConVar))
		PlayerLevel[client]++
		PlaySoundByIdToClient(client, LevelUpSoundId)
		PrintSJMessage(client, "You have reached level %i and got %i credits. Next level cost: $%i", PlayerLevel[client], GetConVarInt(CreditsPerLevelConVar), GetNextLevelCost(client))
		AddPlayerCash(client, 0)
	}
	UpdateClientVisibleCash(client)
}

GetNextLevelCost(client)
{
	new playerLevel = PlayerLevel[client]
	return GetLevelCost(playerLevel + 1)
}

GetLevelCost(level)
{
	decl String:levelNumberString[8]
	IntToString(level, levelNumberString, sizeof(levelNumberString))
	
	new cost
	if (GetTrieValue(LevelCostCacheTrie, levelNumberString, cost))
	{
		return cost
	}

	cost = CalculateLevelCost(level)
	SetTrieValue(LevelCostCacheTrie, levelNumberString, cost)

	return cost
}

CalculateLevelCost(level)
{
	new initialCost = GetConVarInt(CreditCostConVar)
	new Float:creditCost = float(initialCost)
	
	if (level == 0)
	{
		return RoundFloat(creditCost)
	}
	
	new costAddition = GetConVarInt(LevelCostAddConVar)
	new Float:costMultiplier = GetConVarFloat(LevelCostMultConVar)
	
	
	for (new i = 0; i < level - 1; i++)
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
		new accountOffset = FindSendPropInfo("CCSPlayer", "m_iAccount")
		SetEntData(client, accountOffset, PlayerCash[client])
	}
}