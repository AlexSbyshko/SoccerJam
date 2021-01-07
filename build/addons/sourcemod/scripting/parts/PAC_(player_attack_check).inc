static Handle:FightRadiusConVar = INVALID_HANDLE

void PlayerAttackChecking(ClientActivatedEvent clientActivatedEvent)
{
	clientActivatedEvent.Subscribe(OnClientActivated)

	FightRadiusConVar = CreateConVar("sj_fight_radius", "0", "The possibility of attack players.\n-1 = Everyone can attack all\n0 = Possible only attack ball holder, ball holder can attack all\n> 0 = Radius around the ball, where everyone can attack each", 0, true, -1.0)
}

static void OnClientActivated(int client)
{
	SDKHook(client, SDKHook_OnTakeDamage, PAC_OnTakeDamage)
}

public Action:PAC_OnTakeDamage(victim, &attacker, &inflictor, &Float:damage, &damagetype)
{
	if (victim == attacker || attacker == 0)
	{
		return Plugin_Continue;
	}
	new fightRadius = GetConVarInt(FightRadiusConVar)
	if (fightRadius < 0)
	{
		return Plugin_Continue;
	}
	if (fightRadius == 0
		&& (victim == g_BallHolder || attacker == g_BallHolder))
	{
		return Plugin_Continue;
	}
	decl Float:playerOrig[3];
	decl Float:ballOrigin[3];
	if (IsValidEntity(g_Ball))
	{
		Entity_GetAbsOrigin(g_Ball, ballOrigin);
		Entity_GetAbsOrigin(victim, playerOrig);
		if (GetVectorDistance(ballOrigin, playerOrig) <= fightRadius)
		{
			return Plugin_Continue;
		}
	}
	return Plugin_Handled
}