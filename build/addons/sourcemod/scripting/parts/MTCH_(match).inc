enum MatchState
{
	MatchState_Preparing,
	MatchState_FirstHalf,
	MatchState_HalfTime,
	MatchState_SecondHalf,
	MatchState_OverTime,
	MatchState_Public,
}

#define CLIENTFILTER_ALL				0		// No filtering
#define CLIENTFILTER_BOTS			( 1	<< 1  )	// Fake clients
#define CLIENTFILTER_NOBOTS			( 1	<< 2  )	// No fake clients
#define CLIENTFILTER_AUTHORIZED		( 1 << 3  ) // SteamID validated
#define CLIENTFILTER_NOTAUTHORIZED  ( 1 << 4  ) // SteamID not validated (yet)
#define CLIENTFILTER_ADMINS			( 1	<< 5  )	// Generic Admins (or higher)
#define CLIENTFILTER_NOADMINS		( 1	<< 6  )	// No generic admins
// All flags below require ingame checking (optimization)
#define CLIENTFILTER_INGAME			( 1	<< 7  )	// Ingame
#define CLIENTFILTER_INGAMEAUTH		( 1 << 8  ) // Ingame & Authorized
#define CLIENTFILTER_NOTINGAME		( 1 << 9  )	// Not ingame (currently connecting)
#define CLIENTFILTER_ALIVE			( 1	<< 10 )	// Alive
#define CLIENTFILTER_DEAD			( 1	<< 11 )	// Dead
#define CLIENTFILTER_SPECTATORS		( 1 << 12 )	// Spectators
#define CLIENTFILTER_NOSPECTATORS	( 1 << 13 )	// No Spectators
#define CLIENTFILTER_OBSERVERS		( 1 << 14 )	// Observers
#define CLIENTFILTER_NOOBSERVERS	( 1 << 15 )	// No Observers
#define CLIENTFILTER_TEAMONE		( 1 << 16 )	// First Team (Terrorists, ...)
#define CLIENTFILTER_TEAMTWO		( 1 << 17 )	// Second Team (Counter-Terrorists, ...)

// Team Defines
#define	TEAM_INVALID	-1
#define TEAM_UNASSIGNED	0
#define TEAM_SPECTATOR	1
#define TEAM_ONE		2
#define TEAM_TWO		3
#define TEAM_THREE		4
#define TEAM_FOUR		5

static Handle:OnStartOverTimeForward
static Handle:OnStartPublicForward
static Handle:OnStartMatchForward
static Handle:OnEndPublicForward
static Handle:OnEndFirstHalfForward
static Handle:OnEndMatchForward

static Handle:CvarGoalsToWin

static MatchTimeIsUpSoundId

static MatchState:CurrentState

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

void MatchProcessing(MapStartedEvent mapStartedEvent)
{
	mapStartedEvent.Subscribe(OnMapStarted)

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

static void OnMapStarted()
{
	Match_Start()
}

public MTCH_Event_MatchEndRestart(Handle:event, const String:name[], bool:dontBroadcast)
{
	Match_Start()
}

static void Match_Start()
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
	CreateSjTimer(10, Match_Start, "public mode start in ")
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

stock Team_GetClientCount(team, flags=0)
{
	flags |= CLIENTFILTER_INGAME;

	new numClients = 0;
	for (new client=1; client <= MaxClients; client++) {

		if (!Client_MatchesFilter(client, flags)) {
			continue;
		}

		if (GetClientTeam(client) == team) {
			numClients++;
		}
	}

	return numClients;
}

stock bool:Client_MatchesFilter(client, flags)
{
	new bool:isIngame = false;

	if (flags >= CLIENTFILTER_INGAME) {
		isIngame = IsClientInGame(client);

		if (isIngame) {
			if (flags & CLIENTFILTER_NOTINGAME) {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else if (!IsClientConnected(client)) {
		return false;
	}

	if (!flags) {
		return true;
	}

	if (flags & CLIENTFILTER_INGAMEAUTH) {
		flags |= CLIENTFILTER_INGAME | CLIENTFILTER_AUTHORIZED;
	}

	if (flags & CLIENTFILTER_BOTS && !IsFakeClient(client)) {
		return false;
	}

	if (flags & CLIENTFILTER_NOBOTS && IsFakeClient(client)) {
		return false;
	}

	if (flags & CLIENTFILTER_ADMINS && !Client_IsAdmin(client)) {
		return false;
	}

	if (flags & CLIENTFILTER_NOADMINS && Client_IsAdmin(client)) {
		return false;
	}

	if (flags & CLIENTFILTER_AUTHORIZED && !IsClientAuthorized(client)) {
		return false;
	}

	if (flags & CLIENTFILTER_NOTAUTHORIZED && IsClientAuthorized(client)) {
		return false;
	}

	if (isIngame) {

		if (flags & CLIENTFILTER_ALIVE && !IsPlayerAlive(client)) {
			return false;
		}

		if (flags & CLIENTFILTER_DEAD && IsPlayerAlive(client)) {
			return false;
		}

		if (flags & CLIENTFILTER_SPECTATORS && GetClientTeam(client) != TEAM_SPECTATOR) {
			return false;
		}

		if (flags & CLIENTFILTER_NOSPECTATORS && GetClientTeam(client) == TEAM_SPECTATOR) {
			return false;
		}

		if (flags & CLIENTFILTER_OBSERVERS && !IsClientObserver(client)) {
			return false;
		}

		if (flags & CLIENTFILTER_NOOBSERVERS && IsClientObserver(client)) {
			return false;
		}

		if (flags & CLIENTFILTER_TEAMONE && GetClientTeam(client) != TEAM_ONE) {
			return false;
		}

		if (flags & CLIENTFILTER_TEAMTWO && GetClientTeam(client) != TEAM_TWO) {
			return false;
		}
	}

	return true;
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
	Match_Start()
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

stock bool:Client_IsAdmin(client)
{
	new AdminId:adminId = GetUserAdmin(client);
	
	if (adminId == INVALID_ADMIN_ID) {
		return false;
	}
	
	return GetAdminFlag(adminId, Admin_Generic);
}

stock bool:Game_End()
{
	new game_end = FindEntityByClassname(-1, "game_end");

	if (game_end == -1) {
		game_end = CreateEntityByName("game_end");
		
		if (game_end == -1) {
			ThrowError("Unable to find or create entity \"game_end\"");
		}
	}

	return AcceptEntityInput(game_end, "EndGame");
}