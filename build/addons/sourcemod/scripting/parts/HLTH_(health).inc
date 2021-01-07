static ConVar PlayerHealthConVar

void SpawnHealthSetting(ClientSpawnedEvent clientSpawnedEvent)
{
	clientSpawnedEvent.Subscribe(OnClientSpawned)
	PlayerHealthConVar = CreateConVar("sj_player_health", "70", "Default player's health", 0, true, 0.0)
}

static void OnClientSpawned(int client)
{
	new team = GetClientTeam(client)
	if (team != CS_TEAM_NONE)
	{
		SetEntProp(client, Prop_Data, "m_iHealth", PlayerHealthConVar.IntValue)
	}
}