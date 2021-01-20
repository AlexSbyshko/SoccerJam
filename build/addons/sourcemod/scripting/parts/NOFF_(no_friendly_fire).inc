void DisablingFriendlyFire()
{
	ClientActivated.Subscribe(OnClientActivated)
}

static void OnClientActivated(int client)
{
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