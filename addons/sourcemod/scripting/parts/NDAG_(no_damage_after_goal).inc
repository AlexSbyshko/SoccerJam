public NDAG_Event_PlayerActivate(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	SDKHook(client, SDKHook_OnTakeDamage, NDAG_OnTakeDamage)
}

public Action:NDAG_OnTakeDamage(victim, &attacker, &inflictor, &Float:damage, &damagetype)
{
	if (g_Goal)
	{
		return Plugin_Handled
	}
	return Plugin_Continue
}