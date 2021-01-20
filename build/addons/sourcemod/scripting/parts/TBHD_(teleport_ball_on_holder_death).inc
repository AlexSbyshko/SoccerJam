void BallTeleportingOnHolderDeath()
{
	ClientDied.Subscribe(OnClientDied)
}

static void OnClientDied(int client)
{
	if (client == g_BallOwner)
	{
		RecreateBall()
		TeleportBallToClient(client)
		BallHolderDied.Raise(client)
	}
}