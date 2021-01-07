const MAX_MATCH_STATS = 16
const STATS_NAME_LENGTH = 64

static StatsCount
static PlayerStats[MAXPLAYERS+1][MAX_MATCH_STATS]

static String:StatsNames[MAX_MATCH_STATS][STATS_NAME_LENGTH]

void MatchStatsManaging(ClientDisconnectingEvent clientDisconnectingEvent)
{
	clientDisconnectingEvent.Subscribe(OnClientDisconnecting)
}

public MSM_OnStartPublic()
{
	ResetAllClientsMatchStats()
}

public MSM_OnStartMatch()
{
	ResetAllClientsMatchStats()
}

static void OnClientDisconnecting(client)
{
	ResetClientMatchStats(client)
}

public MSM_OnEndPublic()
{
	ShowMatchStatsToAll()
}

public MSM_OnEndMatch()
{
	ShowMatchStatsToAll()
}

CreateMatchStats(const String:name[])
{
	strcopy(StatsNames[StatsCount], STATS_NAME_LENGTH, name)
	return StatsCount++
}

AddMatchStatsValue(statsId, client, value)
{
	PlayerStats[client][statsId] += value
}

SetMatchStatsValue(statsId, client, value)
{
	if (value > PlayerStats[client][statsId])
	{
		PlayerStats[client][statsId] = value
	}
}

ResetAllClientsMatchStats()
{
	for (new client = 1; client <= MaxClients; client++)
	{
		ResetClientMatchStats(client)
	}
}

ResetClientMatchStats(client)
{
	for (new statsId = 0; statsId < StatsCount; statsId++)
	{
		PlayerStats[client][statsId] = 0
	}
}

ShowMatchStatsToAll()
{
	for (new client = 1; client <= MaxClients; client++)
	{
		if (IsClientInGame(client))
		{
			ShowMatchStats(client)
		}
	}
}

ShowMatchStats(client)
{
	new Handle:panel = CreatePanel()
	
	new count = 0
	decl String:line[STATS_NAME_LENGTH]
	new statsClient
	new value
	for (new statsId = 0; statsId < StatsCount; statsId++)
	{
		statsClient = GetClientWithBiggerValue(statsId, value)
		if (statsClient > 0)
		{
			decl String:clientName[MAX_NAME_LENGTH]
			GetClientName(statsClient, clientName, sizeof(clientName))
			Format(line, sizeof(line), "%s: %s (%i)", StatsNames[statsId], clientName, value)
			DrawPanelText(panel, line)
			count++
		}
	}
	if (count > 0)
	{
		SetPanelTitle(panel, "Match Stats:")
		SendPanelToClient(panel, client, StatsPanelHandler, 20)
	}
	CloseHandle(panel)
}

GetClientWithBiggerValue(statsId, &value)
{
	new client = 0
	new maxValue = 0
	for (new i = 1; i < MaxClients; i++)
	{
		if (PlayerStats[i][statsId] > maxValue)
		{
			maxValue = PlayerStats[i][statsId]
			client = i
		}
	}
	value = maxValue
	return client
}

public StatsPanelHandler(Handle:menu, MenuAction:action, param1, param2)
{
}