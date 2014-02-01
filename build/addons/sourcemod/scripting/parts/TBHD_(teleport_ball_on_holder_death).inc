static Handle:OnBallHolderDeathForward

public TBHD_Init()
{
	OnBallHolderDeathForward = CreateForward(ET_Ignore, Param_Cell)
	RegisterCustomForward(OnBallHolderDeathForward, "OnBallHolderDeath")
}

public TBHD_Event_PlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))

	if (client == g_BallOwner)
	{
		RecreateBall()
		TeleportBallToClient(client)
		FireOnBallHolderDeath(client)
	}
}

static FireOnBallHolderDeath(client)
{
	Call_StartForward(OnBallHolderDeathForward)
	Call_PushCell(client)
	Call_Finish()
}