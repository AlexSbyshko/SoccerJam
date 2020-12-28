public FFI_OnBallReceive(ballHolder, oldBallOwner)
{
	if (oldBallOwner > 0
		&& GetClientTeam(ballHolder) != GetClientTeam(oldBallOwner))
	{
		Client_SetScore(ballHolder, Client_GetScore(ballHolder) + 1)
		Client_SetDeaths(oldBallOwner, Client_GetDeaths(oldBallOwner) + 1)
	}
}

stock Client_GetScore(client)
{
	return GetClientFrags(client);
}

stock Client_GetDeaths(client)
{
	return GetEntProp(client, Prop_Data, "m_iDeaths");
}