static HealthUpgradeId

public HLTH_Init()
{
	HealthUpgradeId = CreateUpgrade("health", 70.0, 200.0, 5, 1, "hp", UpdatePlayerHealth)
}

public HLTH_Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{	
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	new team = GetClientTeam(client)
	if (team != CS_TEAM_NONE)
	{
		UpdateClientHealth(client)
	}
}

public UpdatePlayerHealth(client, Float:health)
{
	if (client > 0 && IsClientInGame(client))
	{
		UpdateClientHealth(client)
	}
}

static UpdateClientHealth(client)
{
	new health = GetPlayerMaxHealth(client);
	SetEntProp(client, Prop_Data, "m_iHealth", health)
}

static GetPlayerMaxHealth(client)
{
	new Float:ugradeMaxHealthValue = GetPlayerUpgradeValue(client, HealthUpgradeId)
	return RoundFloat(ugradeMaxHealthValue)
}