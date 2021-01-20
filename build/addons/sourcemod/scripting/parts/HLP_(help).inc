static bool:IsNewPlayer[MAXPLAYERS + 1]

void HelpShowing(ClientSpawnedEvent clientSpawnedEvent)
{
	ClientActivated.Subscribe(OnClientActivated)
	clientSpawnedEvent.Subscribe(OnClientSpawned)

	AddCommandListener(HLP_Cmd_Say, "say")
	AddCommandListener(HLP_Cmd_Say, "say_team")
}

static void OnClientActivated(int client)
{
	IsNewPlayer[client] = true
}

static void OnClientSpawned(int client)
{
	new team = GetClientTeam(client);
	if (team == CS_TEAM_CT || team == CS_TEAM_T)
	{
		if (IsNewPlayer[client])
		{
			ShowClientHelp(client);
			IsNewPlayer[client] = false
		}
	}
}

ShowClientHelp(client)
{
	new Handle:panel = CreatePanel();
	decl String:line[64]
	Format(line, sizeof(line), "%T", "HELP_KICK_BALL", client, "E")
	DrawPanelText(panel, line)
	Format(line, sizeof(line), "%T", "HELP_UPGRADE_MENU", client, "R")
	DrawPanelText(panel, line);
	Format(line, sizeof(line), "%T", "HELP_TURBO", client, "G")
	DrawPanelText(panel, line)
	Format(line, sizeof(line), "%T", "HELP_CURVE_LEFT", client, "Shift+A")
	DrawPanelText(panel, line);
	Format(line, sizeof(line), "%T", "HELP_CURVE_RIGHT", client, "Shift+D")
	DrawPanelText(panel, line)
	Format(line, sizeof(line), "%T", "HELP_HELP", client, "help")
	DrawPanelText(panel, line)
	SendPanelToClient(panel, client, HelpPanelHandler, 20)
	CloseHandle(panel)
}

public HelpPanelHandler(Handle:menu, MenuAction:action, param1, param2)
{
}

public Action:HLP_Cmd_Say(client, const String:command[], argc) 
{
	decl String:text[192]
	if(!GetCmdArgString(text, sizeof(text)))
	{
		return Plugin_Continue
	}
	StripQuotes(text)
	if (StrEqual(text, "help", false))
	{
		ShowClientHelp(client)
		return Plugin_Handled
	}
	return Plugin_Continue
}