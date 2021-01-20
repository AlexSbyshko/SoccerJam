void DisablingDamageAfterGoal()
{
	ClientActivated.Subscribe(OnClientActivated)
}

static void OnClientActivated(int client)
{
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