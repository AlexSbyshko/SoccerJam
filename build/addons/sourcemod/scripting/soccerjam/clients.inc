void Client_SetScore(client, value)
{
	SetEntProp(client, Prop_Data, "m_iFrags", value);
}

void Client_SetDeaths(client, value)
{
	SetEntProp(client, Prop_Data, "m_iDeaths", value);
}

void ClearClient(client)
{
	if (IsClientInGame(client))
	{
		Client_SetScore(client, 0)
		Client_SetDeaths(client, 0)
	}
}