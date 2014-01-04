static Handle:BHSpeedMultiplierConVar
static SpeedUpgradeId
static Float:BallHolderSpeedMultiplier

public SG_Init()
{
	SpeedUpgradeId = CreateUpgrade("speed", 100.0, 150.0, 5, 1, "%", UpdatePlayerSpeed)
	CreateUpgrade("gravity", 100.0, 50.0, 5, 1, "%", UpdatePlayerGravity)

	BHSpeedMultiplierConVar = CreateConVar("sj_ball_holder_speed_multiplier", "0.9", "Ball holder speed multiplier", 0, true, 0.0, true, 1.0)
	HookConVarChange(BHSpeedMultiplierConVar, OnCVBHSpeedMultiplierChanged)
	BallHolderSpeedMultiplier = GetConVarFloat(BHSpeedMultiplierConVar)
}

public SG_Event_PlayerActivate(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	SDKHook(client, SDKHook_PreThink, SG_OnClientThink)
}

public SG_OnClientThink(client)
{
	Client_FixSpeed(client)
}

Client_FixSpeed(client)
{
	if (IsClientInGame(client))
	{
		new Float:speedMultiplier = g_PlayerSpeedMultiplier[client] * (IsBallHolder(client) ? BallHolderSpeedMultiplier : 1.0)
		new Float:gravityMultiplier = (1.0 / speedMultiplier) * g_PlayerGravityMultiplier[client]
		SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", speedMultiplier)
		SetEntPropFloat(client, Prop_Data, "m_flGravity", gravityMultiplier)		
	}
}

Float:GetPlayerSpeedMultiplier(client)
{
	return GetPlayerUpgradeValue(client, SpeedUpgradeId) * 0.01
}

public UpdatePlayerSpeed(client, Float:speedPct)
{
	g_PlayerSpeedMultiplier[client] = speedPct * 0.01
}

public UpdatePlayerGravity(client, Float:gravityPct)
{
	g_PlayerGravityMultiplier[client] = gravityPct * 0.01
}

public OnCVBHSpeedMultiplierChanged(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	BallHolderSpeedMultiplier = GetConVarFloat(cvar);
}