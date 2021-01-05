#include "soccerjam/tools/cycles"
#include "soccerjam/tools/func_caller"

PrintSJMessageAll(String:text[], any:...)
{
	decl String:message[100];
	VFormat(message, sizeof(message), text, 2);
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			PrintSJMessage(i, message);
		}
	}
}

PrintSJMessage(client, String:text[], any:...)
{
	decl String:message[100];
	VFormat(message, sizeof(message), text, 3);
	PrintToChat(client, "\x01\x0B\x04[\x03SJ\x04] \x03%s", message);
}

GetClientOpponentSJTeam(client)
{
	new team = GetClientTeam(client);
	switch (team)
	{
		case CS_TEAM_CT:
		{
			return CS_TEAM_T;
		}
		case CS_TEAM_T:
		{
			return CS_TEAM_CT;
		}
	}
	return CS_TEAM_NONE;
}

SJ_IncreaseTeamScore(team)
{
	TeamScore[team]++;
	SJ_UpdateTeamScores();
}

SJ_UpdateTeamScores()
{
	CS_SetTeamScore(CS_TEAM_T, TeamScore[CS_TEAM_T]);
	SetTeamScore(CS_TEAM_T, TeamScore[CS_TEAM_T]);
	CS_SetTeamScore(CS_TEAM_CT, TeamScore[CS_TEAM_CT]);
	SetTeamScore(CS_TEAM_CT, TeamScore[CS_TEAM_CT]);
}

SJ_ResetTeamScores()
{
	TeamScore[CS_TEAM_T] = 0;
	TeamScore[CS_TEAM_CT] = 0;
	SJ_UpdateTeamScores();
}

MapFunctionPrefix(&Function:func, const String:prefix[], const String:functionName[])
{
	decl String:realFunctionName[MAX_FUNCTION_NAME_LENGTH]
	Format(realFunctionName, sizeof(realFunctionName), "%s_%s", prefix, functionName)
	func = GetFunctionByName(INVALID_HANDLE, realFunctionName)
}