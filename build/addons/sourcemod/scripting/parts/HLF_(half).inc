static Handle:TimeUpTimer
static Handle:RoundTimeConVar
static Handle:HalfDurationConVar
static Handle:OnHalfEndForward
static Handle:OnHalfStartForward

void StartHalf(MapStartedEvent mapStartedEvent)
{
	mapStartedEvent.Subscribe(OnMapStarted)

	RoundTimeConVar = FindConVar("mp_roundtime")
	HookConVarChange(RoundTimeConVar, OnCVRoundTimeChanged)
	HalfDurationConVar = CreateConVar("sj_half_duration", "15", "Half duration in minutes", 0, true, 1.0, true, 60.0)	
	HookConVarChange(HalfDurationConVar, OnCVHalfDurationChanged)

	OnHalfStartForward = CreateForward(ET_Ignore)
	RegisterCustomForward(OnHalfStartForward, "OnHalfStart")

	OnHalfEndForward = CreateForward(ET_Ignore)
	RegisterCustomForward(OnHalfEndForward, "OnHalfEnd")
}

static void OnMapStarted()
{
	TimeUpTimer = INVALID_HANDLE
}

public Half_Start()
{
	Half_StartAfterDelay(0.0)
}

public Half_StartAfterDelay(Float:delayInSeconds)
{
	CS_TerminateRound(delayInSeconds, CSRoundEnd_BombDefused, true)
}

public HLF_Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	ClearTimer(TimeUpTimer)
	FireOnHalfStart()
}

public HLF_Event_RoundFreezeEnd(Handle:event, const String:name[], bool:dontBroadcast)
{
	TimeUpTimer = CreateTimer(GetConVarFloat(HalfDurationConVar) * 60, Timer_TimeUp, _, TIMER_FLAG_NO_MAPCHANGE)
}

public Action:Timer_TimeUp(Handle:timer)
{
	ClearTimer(TimeUpTimer)
	FireOnHalfEnd()
	return Plugin_Continue
}

public OnCVRoundTimeChanged(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateHalfDuration()
}

public OnCVHalfDurationChanged(Handle:cvar, const String:oldValue[], const String:newValue[])
{
	UpdateHalfDuration()
}

UpdateHalfDuration()
{
	new Float:halfDuration = GetConVarFloat(HalfDurationConVar)
	new Float:roundTime = GetConVarFloat(RoundTimeConVar)
	if (roundTime != halfDuration)
	{
		SetConVarFloat(RoundTimeConVar, halfDuration)
	}
}

FireOnHalfStart()
{
	Call_StartForward(OnHalfStartForward)
	Call_Finish()
}

FireOnHalfEnd()
{
	Call_StartForward(OnHalfEndForward)
	Call_Finish()
}