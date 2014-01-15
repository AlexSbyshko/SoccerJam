enum MatchState
{
	MatchState_Preparing,
	MatchState_FirstHalf,
	MatchState_HalfTime,
	MatchState_SecondHalf,
	MatchState_OverTime,
	MatchState_Public,
}

static Handle:OnStartOverTimeForward
static Handle:OnStartPublicForward
static Handle:OnStartMatchForward
static Handle:OnEndPublicForward
static Handle:OnEndFirstHalfForward
static Handle:OnEndMatchForward

static MatchState:CurrentState
static Handle:CvarGoalsToWin

static MatchTimeIsUpSoundId

// API //

public IsMatchPublic()
{
	return CurrentState == MatchState_Public
}

public GetWinLimit()
{
	return GetConVarInt(CvarGoalsToWin)
}

/////////

public MTCH_Init()
{
	RegAdminCmd("sj_match_start", MTCH_Cmd_StartMatch, ADMFLAG_CHANGEMAP)
	RegAdminCmd("sj_public_start", MTCH_Cmd_StartPublic, ADMFLAG_CHANGEMAP)
	
	OnStartOverTimeForward = CreateForward(ET_Ignore)
	RegisterCustomForward(OnStartOverTimeForward, "OnStartOverTime")

	OnStartPublicForward = CreateForward(ET_Ignore)
	RegisterCustomForward(OnStartPublicForward, "OnStartPublic")

	OnStartMatchForward = CreateForward(ET_Ignore)
	RegisterCustomForward(OnStartMatchForward, "OnStartMatch")

	OnEndPublicForward = CreateForward(ET_Ignore)
	RegisterCustomForward(OnEndPublicForward, "OnEndPublic")

	OnEndFirstHalfForward = CreateForward(ET_Ignore)
	RegisterCustomForward(OnEndFirstHalfForward, "OnEndFirstHalf")

	OnEndMatchForward = CreateForward(ET_Ignore)
	RegisterCustomForward(OnEndMatchForward, "OnEndMatch")
	
	CvarGoalsToWin = CreateConVar("sj_goals_to_win", "15", "How many goals need to win in public mode", 0, true, 1.0)
	MatchTimeIsUpSoundId = CreateSound("match_timeisup")
	AddCommandListener(MTCH_Cmd_Say, "say")
	AddCommandListener(CMD_JoinClass, "joinclass")
}

public MTCH_OnMapStart()
{
	Match_StartPublic()
}

public MTCH_Event_MatchEndRestart(Handle:event, const String:name[], bool:dontBroadcast)
{
	Match_StartPublic()
}

public Match_StartPublic()
{
	CurrentState = MatchState_Public
	ClearAllClients()
	g_BallSpawnTeam = CS_TEAM_NONE
	CS_TerminateRound(0.0, CSRoundEnd_GameStart)
	FireOnStartPublic()
}

static Match_StartPreparing()
{
	CurrentState = MatchState_Preparing
	ForEachPlayer(Client_KillForPreparing)
	FireOnStartMatch()
}

public Match_StartFirstHalf()
{
	CurrentState = MatchState_FirstHalf
	SJ_ResetTeamScores()
	g_BallSpawnTeam = CS_TEAM_NONE	
	Half_Start()
}

public Match_StartHalfTime()
{
	CurrentState = MatchState_HalfTime
	ForEachPlayer(Client_KillForPreparing)
	PrintCenterTextAll("HALF TIME")
}

public Match_StartSecondHalf()
{
	CurrentState = MatchState_SecondHalf
	g_BallSpawnTeam = CS_TEAM_NONE
	Half_Start()
}

public Match_StartOverTime()
{
	PrintCenterTextAll("OVERTIME")
	CurrentState = MatchState_OverTime
	g_BallSpawnTeam = CS_TEAM_NONE	
	FireOnStartOverTime()
}

static Match_End()
{
	FireOnEndMatch()
	CreateSjTimer(10, Match_StartPublic, "public mode start in ")
}

public MTCH_OnHalfEnd()
{
	switch (CurrentState)
	{
		case MatchState_FirstHalf:
		{
			Match_StartHalfTime()
			PlaySoundByIdToAll(MatchTimeIsUpSoundId)
			FireOnEndFirstHalf()
		}
		case MatchState_SecondHalf:
		{
			if (!Match_CheckForWin())
			{
				CreateSjTimer(5, Match_StartOverTime, "OverTime start in ")
			}
			PlaySoundByIdToAll(MatchTimeIsUpSoundId)
		}
	}
}

public MTCH_OnGoal(team, scorer)
{	
	new CSRoundEndReason:reason = (team == CS_TEAM_T) ? CSRoundEnd_TerroristWin : CSRoundEnd_CTWin
	switch (CurrentState)
	{
		case MatchState_FirstHalf:
		{
			CS_TerminateRound(6.0, reason, true)
			FireMvpEvent(scorer)
		}
		case MatchState_SecondHalf:
		{
			CS_TerminateRound(6.0, reason, true)
			FireMvpEvent(scorer)
		}
		case MatchState_OverTime:
		{
			if (Match_CheckForWin())
			{
				CurrentState = MatchState_Public
				CS_TerminateRound(6.0, reason, true)
				FireMvpEvent(scorer)
			}
		}
		case MatchState_Public:
		{
			if (!CheckForGameEnd())
			{
				CS_TerminateRound(6.0, reason, true)
				FireMvpEvent(scorer)
			}
		}
	}
}

FireMvpEvent(client)
{
	new Handle:event = CreateEvent("round_mvp") 
	SetEventInt(event, "userid", GetClientUserId(client))
	const MVP_REASON = 0
	SetEventInt(event, "reason", MVP_REASON)
	FireEvent(event)
}

public MTCH_Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{	
	CheckForStartMatch()
}

public MTCH_Event_PlayerDisconnect(Handle:event, const String:name[], bool:dontBroadcast)
{	
	CheckForStartMatch()
}

static CheckForStartMatch()
{
	switch (CurrentState)
	{
		case MatchState_Preparing:
		{
			if (IsTeamsReady())
			{
				CreateSjTimer(10, Match_StartFirstHalf, "Match start in ", true)
			}
		}
		case MatchState_HalfTime:
		{
			if (IsTeamsReady())
			{
				CreateSjTimer(10, Match_StartSecondHalf, "2nd Half start in ", true)
			}
		}
	}
}

static bool:IsTeamsReady()
{
	return Team_GetClientCount(CS_TEAM_T, CLIENTFILTER_DEAD) == 0
		&& Team_GetClientCount(CS_TEAM_CT, CLIENTFILTER_DEAD) == 0
}

static Match_OnClientReady(client)
{
	if (Match_IsWaitingPlayers())
	{
		Client_Respawn(client)
	}
}

static bool:Match_CheckForWin()
{
	new ctScore = GetTeamScore(CS_TEAM_CT)
	new tScore = GetTeamScore(CS_TEAM_T)
	if (ctScore > tScore)
	{
		Match_End()
		return true
	}
	else if (tScore > ctScore)
	{
		Match_End()
		return true
	}
	return false
}

bool:Match_IsWaitingPlayers()
{
	return CurrentState == MatchState_Preparing
		|| CurrentState == MatchState_HalfTime
}

static bool:CheckForGameEnd()
{
	new goalsToWin = GetConVarInt(CvarGoalsToWin)
	new ctScore = GetTeamScore(CS_TEAM_CT)
	new tScore = GetTeamScore(CS_TEAM_T)
	if (ctScore >= goalsToWin
			|| tScore >= goalsToWin)
	{
		FireOnEndPublic()
		Game_End()
		return true
	}
	return false
}

public Action:MTCH_Cmd_StartMatch(client, argc) 
{
	Match_StartPreparing()
	return Plugin_Handled
}

public Action:MTCH_Cmd_StartPublic(client, argc) 
{
	Match_StartPublic()
	return Plugin_Handled
}

public Action:MTCH_Event_PlayerTeam(Handle:event, const String:name[], bool:dontBroadcast)
{	
	CheckForStartMatch()
	return Plugin_Continue
}

public Action:MTCH_Cmd_Say(client, const String:command[], argc) 
{
	decl String:text[192]
	if(!GetCmdArgString(text, sizeof(text)))
	{
		return Plugin_Continue
	}
	StripQuotes(text)
	if (StrEqual(text, "ready", false))
	{
		Match_OnClientReady(client)
	}
	return Plugin_Continue
}

public Action:CMD_JoinClass(client, const String:command[], argc) 
{
	if (Match_IsWaitingPlayers())
	{
		FakeClientCommandEx(client, "spec_mode")		
	}
	else
	{
		CreateTimer(1.0, Timer_PlayerRespawn, GetClientUserId(client))
	}
	return Plugin_Handled
}

FireOnStartOverTime()
{
	Call_StartForward(OnStartOverTimeForward)
	Call_Finish()
}

FireOnStartPublic()
{
	Call_StartForward(OnStartPublicForward)
	Call_Finish()
}

FireOnStartMatch()
{
	Call_StartForward(OnStartMatchForward)
	Call_Finish()
}

FireOnEndPublic()
{
	Call_StartForward(OnEndPublicForward)
	Call_Finish()
}

FireOnEndFirstHalf()
{
	Call_StartForward(OnEndFirstHalfForward)
	Call_Finish()
}

FireOnEndMatch()
{
	Call_StartForward(OnEndMatchForward)
	Call_Finish()
}