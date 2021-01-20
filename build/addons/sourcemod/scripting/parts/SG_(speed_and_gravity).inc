static Handle:BHSpeedMultiplierConVar
static Float:BallHolderSpeedMultiplier

void SpeedAndGravityManaging()
{
	ClientActivated.Subscribe(OnClientActivated)

	BHSpeedMultiplierConVar = CreateConVar("sj_ball_holder_speed_multiplier", "0.9", "Ball holder speed multiplier", 0, true, 0.0, true, 1.0)
	HookConVarChange(BHSpeedMultiplierConVar, OnCVBHSpeedMultiplierChanged)
	BallHolderSpeedMultiplier = GetConVarFloat(BHSpeedMultiplierConVar)
}

static void OnClientActivated(int client)
{
	SDKHook(client, SDKHook_PreThink, SG_OnClientThink)
}

public SG_OnClientThink(client)
{
	Client_FixSpeed(client)
}

static void Client_FixSpeed(client)
{
	if (IsClientInGame(client))
	{
		new Float:speedMultiplier = 1.0 * (IsBallHolder(client) ? BallHolderSpeedMultiplier : 1.0)
		new Float:gravityMultiplier = (1.0 / speedMultiplier) * 1.0
		SetEntPropFloat(client, Prop_Data, "m_flLaggedMovementValue", speedMultiplier)
		SetEntPropFloat(client, Prop_Data, "m_flGravity", gravityMultiplier)		
	}
}

public OnCVBHSpeedMultiplierChanged(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	BallHolderSpeedMultiplier = GetConVarFloat(cvar);
}