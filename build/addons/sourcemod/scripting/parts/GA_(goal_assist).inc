const MAX_ASSISTANTS = 2

static Handle:OnClientAssistedForward

static Handle:AssistantsArray

static AssistCountStatsId

void GoalAssistProcessing()
{
	AssistantsArray = CreateArray()

	AssistCountStatsId = CreateMatchStats("Most assists")

	OnClientAssistedForward  = CreateForward(ET_Ignore, Param_Cell)
	RegisterCustomForward(OnClientAssistedForward, "OnClientAssisted")
}

public GA_OnBallReceived(ballHolder, oldBallOwner)
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

public GA_Event_PrePlayerTeam(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	RemoveClientFromArray(client)
}

public GA_OnGoal(team, scorer)
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
			FireOnClientAssisted(currentClient)
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

static FireOnClientAssisted(client)
{
	Call_StartForward(OnClientAssistedForward)
	Call_PushCell(client)
	Call_Finish()
}