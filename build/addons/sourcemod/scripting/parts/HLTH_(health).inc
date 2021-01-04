static ConVar PlayerHealthConVar

public HLTH_Init()
{
	PlayerHealthConVar = CreateConVar("sj_player_health", "70", "Default player's health", 0, true, 0.0)
}

public HLTH_Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{	
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	new team = GetClientTeam(client)
	if (team != CS_TEAM_NONE)
	{
		SetEntProp(client, Prop_Data, "m_iHealth", PlayerHealthConVar.IntValue)
	}
}