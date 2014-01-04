public NOFF_Event_PlayerActivate(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	SDKHook(client, SDKHook_OnTakeDamage, NOFF_OnTakeDamage)
}

public Action:NOFF_OnTakeDamage(victim, &attacker, &inflictor, &Float:damage, &damagetype)
{
	if (victim == attacker || attacker == 0)
	{
		return Plugin_Continue
	}
	if (GetClientTeam(victim) == GetClientTeam(attacker))
	{
		return Plugin_Handled;
	}
	return Plugin_Continue
}