static int CtModelId
static int TModelId

void TeamModelSetting(ClientSpawnedEvent clientSpawnedEvent)
{
	clientSpawnedEvent.Subscribe(OnClientSpawned)

	TModelId = CreateModel("team_red")
	CtModelId = CreateModel("team_blue")
}

static void OnClientSpawned(int client)
{
	int team = GetClientTeam(client)
	if (team == CS_TEAM_CT)
	{
		SetEntityModelById(client, CtModelId)
	}
	else if (team == CS_TEAM_T)
	{
		SetEntityModelById(client, TModelId)
	}
}