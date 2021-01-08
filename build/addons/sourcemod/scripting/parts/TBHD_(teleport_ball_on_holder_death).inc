static Handle:OnBallHolderDeathForward

void BallTeleportingOnHolderDeath(ClientDiedEvent clientDiedEvent)
{
	clientDiedEvent.Subscribe(OnClientDied)

	OnBallHolderDeathForward = CreateForward(ET_Ignore, Param_Cell)
	RegisterCustomForward(OnBallHolderDeathForward, "OnBallHolderDeath")
}

static void OnClientDied(int client)
{
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