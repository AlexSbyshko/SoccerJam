
static Handle:OnGoalForward
static GoalSoundId
static GoalSirenSoundId

void GoalScoring(RoundStartedEvent roundStartedEvent)
{
	roundStartedEvent.Subscribe(OnRoundStarted)

	OnGoalForward = CreateForward(ET_Ignore, Param_Cell, Param_Cell)
	RegisterCustomForward(OnGoalForward, "OnGoal")

	GoalSoundId = CreateSound("goal")
	GoalSirenSoundId = CreateSound("goal_siren")
}

static void OnRoundStarted()
{
	g_Goal = false
}

public GOAL_OnBallCreated(ballEntity)
{
	SDKHook(g_Ball, SDKHook_Touch, GOAL_OnBallTouch)
}

public GOAL_OnBallTouch(ball, entity)
{
	if (g_IsBallFree)
	{
		if (entity == g_Goals[CS_TEAM_CT])
		{
			CheckForGoal(CS_TEAM_T);		
		}
		else if (entity == g_Goals[CS_TEAM_T])
		{
			CheckForGoal(CS_TEAM_CT);
		}
	}
}

CheckForGoal(team)
{
	if (!g_Goal)
	{
		if (g_BallOwner 
			&& GetClientTeam(g_BallOwner) == team)
		{			
			ScoreGoal(team, g_BallOwner)
		}
	}
}

ScoreGoal(team, scorer)
{
	g_Goal = true
	SJ_IncreaseTeamScore(team)
	ShowGoalInfoForSpecs()
	PlaySoundByIdToAll(GoalSirenSoundId)
	PlaySoundByIdToAll(GoalSoundId)	

	new Handle:event = CreateEvent("cs_win_panel_round") 
	SetEventBool(event, "show_timer_defend", true)
	SetEventBool(event, "show_timer_attack", true)
	SetEventInt(event, "timer_time", 10)
	if (team == CS_TEAM_CT)
	{
		SetEventInt(event, "final_event", _:CSRoundEnd_CTWin)
	}
	else if (team == CS_TEAM_T)
	{
		SetEventInt(event, "final_event", _:CSRoundEnd_TerroristWin)
	}
	
	FireOnGoal(team, scorer)
}

FireOnGoal(team, scorer)
{
	Call_StartForward(OnGoalForward)
	Call_PushCell(team)
	Call_PushCell(scorer)
	Call_Finish()
}

ShowGoalInfoForSpecs()
{
	for (new client = 1; client <= MaxClients; client++)
	{
		if (IsClientInGame(client)
			&& GetClientTeam(client) == CS_TEAM_SPECTATOR)
		{
			decl String:status[MAX_HINTTEXT_LENGTH];
			Format(status, sizeof(status), "%T!\n%T", "GOAL", client, "SCORER", client, g_BallOwnerName);
			PrintHintText(client, status);
		}
	}
}