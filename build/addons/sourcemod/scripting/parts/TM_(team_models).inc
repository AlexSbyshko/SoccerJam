static CtModelId
static TModelId

public TM_Init()
{
	TModelId = CreateModel("team_red")
	CtModelId = CreateModel("team_blue")
}

public TM_Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	new team = GetClientTeam(client)
	if (team == CS_TEAM_CT)
	{
		SetEntityModelById(client, CtModelId)
	}
	else if (team == CS_TEAM_T)
	{
		SetEntityModelById(client, TModelId)
	}
}