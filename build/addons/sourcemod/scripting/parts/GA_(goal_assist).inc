const MAX_ASSISTANTS = 2

static Handle:AssistantsArray

static AssistCountStatsId

void GoalAssistProcessing(ClientTeamChangingEvent clientTeamChangingEvent)
{
	BallReceived.Subscribe(OnBallReceived)
	GoalScored.Subscribe(OnGoalScored)
	clientTeamChangingEvent.Subscribe(OnClientTeamChanging)

	AssistantsArray = CreateArray()

	AssistCountStatsId = CreateMatchStats("Most assists")
}

static void OnBallReceived(ballHolder, oldBallOwner)
{
	if (oldBallOwner > 0
		&& oldBallOwner != ballHolder
		&& GetClientTeam(ballHolder) == GetClientTeam(oldBallOwner))
	{
		RemoveClientFromArray(ballHolder)
		RemoveClientFromArray(oldBallOwner)
		PushArrayCell(AssistantsArray, oldBallOwner)
	}
	else
	{
		ClearArray(AssistantsArray)
	}
}

static void OnClientTeamChanging(int client)
{
	RemoveClientFromArray(client)
}

static void OnGoalScored(int team, int scorer)
{
	for (new client = 1; client <= MaxClients; client++)
	{
		if (IsClientInGame(client))
		{
			ShowAssistants(client)
		}
	}	
}

static RemoveClientFromArray(client)
{
	new arraySize = GetArraySize(AssistantsArray)
	for (new i = arraySize - 1; i >= 0; i--)
	{
		new currentClient = GetArrayCell(AssistantsArray, i)
		if (currentClient == client)
		{
			RemoveFromArray(AssistantsArray, i)
		}
	}
}

ShowAssistants(client)
{
	new arraySize = GetArraySize(AssistantsArray)
	if (arraySize > 0)
	{
		new Handle:panel = CreatePanel()
		DrawPanelItem(panel, "", ITEMDRAW_SPACER)
		new currentClient
		new count = 0
		for (new i = arraySize - 1; i >= 0; i--)
		{
			currentClient = GetArrayCell(AssistantsArray, i)
			decl String:clientName[MAX_NAME_LENGTH]
			GetClientName(currentClient, clientName, sizeof(clientName))
			DrawPanelText(panel, clientName)

			AddMatchStatsValue(AssistCountStatsId, currentClient, 1)
			count++
			if (count >= MAX_ASSISTANTS)
			{
				break
			}
		}
		SetPanelTitle(panel, "Assistants:")
		SendPanelToClient(panel, client, StatsPanelHandler, 20)
		CloseHandle(panel)
	}
}

public ShowAssistantsHandler(Handle:menu, MenuAction:action, param1, param2)
{
}