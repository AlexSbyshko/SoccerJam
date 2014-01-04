static Handle:LongJumpEnableTimers[MAXPLAYERS+1]
static bool:IsLongJumpEnabled[MAXPLAYERS+1]
static LongJumpCooldownUpgradeId

public LJ_Init()
{
	LongJumpCooldownUpgradeId = CreateUpgrade("longjump_cooldown", 10.0, 5.0, 5, 1, "s")
}

public LJ_Event_PlayerActivate(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	SDKHook(client, SDKHook_PreThink, LJ_OnPreThink)
}

public LJ_OnMapStart()
{
	ForEachClient(ResetClientSideJump)
}

public LJ_Event_MatchEndRestart(Handle:event, const String:name[], bool:dontBroadcast)
{	
	ForEachClient(ResetClientSideJump)
}

public LJ_Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{	
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	new team = GetClientTeam(client);
	if (team != CS_TEAM_NONE)
	{
		ResetClientSideJump(client)
	}
}

public LJ_Event_PlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	ResetClientSideJump(client)
}

public LJ_OnPreThink(client)
{
	if (client != g_BallHolder)
	{
		new buttons = GetClientButtons(client)
		new flags = GetEntityFlags(client)
		if((buttons & IN_MOVERIGHT || buttons & IN_MOVELEFT) 
			&& buttons & IN_JUMP
			&& flags & FL_ONGROUND
			&& !Client_IsInTurboMode(client)
			&& IsLongJumpEnabled[client])
		{
			LongJumpClient(client)
		}
	}
}

LongJumpClient(client)
{
	decl Float:vel[3];
	Entity_GetLocalVelocity(client, vel);
	
	vel[0] *= 2.0;
	vel[1] *= 2.0;
	vel[2] = 300.0;

	TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, vel);
	IsLongJumpEnabled[client] = false;
	LongJumpEnableTimers[client] = CreateTimer(GetPlayerUpgradeValue(client, LongJumpCooldownUpgradeId), Timer_LongJumpEnable, client);
}

public Action:Timer_LongJumpEnable(Handle:timer, any:client)
{
	IsLongJumpEnabled[client] = true
	if (LongJumpEnableTimers[client] != INVALID_HANDLE)
	{
		KillTimer(LongJumpEnableTimers[client])
		LongJumpEnableTimers[client] = INVALID_HANDLE
	}
	return Plugin_Continue
}

public ResetClientSideJump(client)
{
	if (LongJumpEnableTimers[client] != INVALID_HANDLE)
	{
		KillTimer(LongJumpEnableTimers[client]);
		LongJumpEnableTimers[client] = INVALID_HANDLE;
	}
	IsLongJumpEnabled[client] = true;
}
