const MAX_MESSAGE_LENGTH = 64
static TimeLeft
static Function:CallBack
static String:Message[MAX_MESSAGE_LENGTH]
static IsSoundNotifyEnabled
static TimerTickSoundId

public SJT_Init()
{
	TimerTickSoundId = CreateSound("timer_tick")
	CreateTimer(1.0, Timer_CheckSjTimer, _, TIMER_REPEAT)
}

public Action:Timer_CheckSjTimer(Handle:timer)
{	
	DecreaseCounter()
	return Plugin_Continue
}

DecreaseCounter()
{
	TimeLeft--
	if (TimeLeft > 0 )
	{
		if (!StrEqual(Message, ""))
		{
			decl String:message[MAX_MESSAGE_LENGTH]
			Format(message, MAX_MESSAGE_LENGTH, "%s%i%s", Message, TimeLeft, "...");
			PrintCenterTextAll(message)
		}
		if (IsSoundNotifyEnabled)
		{
			PlaySoundByIdToAll(TimerTickSoundId)
		}
	}
	CheckForCallBack()
}

CreateSjTimer(timeInSeconds, Function:callBackFunc, const String:message[] = "", bool:isSoundNotifyEnabled = false)
{
	TimeLeft = timeInSeconds
	CallBack = callBackFunc
	IsSoundNotifyEnabled = isSoundNotifyEnabled
	strcopy(Message, MAX_MESSAGE_LENGTH, message)
	CheckForCallBack()
}

static CheckForCallBack()
{
	if (TimeLeft == 0)
	{
		Call_StartFunction(INVALID_HANDLE, CallBack)
		Call_Finish()
	}
}