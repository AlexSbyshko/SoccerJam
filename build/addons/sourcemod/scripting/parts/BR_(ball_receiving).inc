static Handle:OnBallReceivedForward

static BallKickSoundId
static BallPickUpSoundId
static BallInterceptionSoundId

static BallStealsStatsId
static CatchingUpgradeId

public BR_Init()
{
	OnBallReceivedForward = CreateForward(ET_Ignore, Param_Cell, Param_Cell)
	RegisterCustomForward(OnBallReceivedForward, "OnBallReceived")

	CatchingUpgradeId = CreateUpgrade("dexterity", 20.0, 90.0, 5, 1, "%")
	
	BallKickSoundId = CreateSound("ball_kill")
	BallPickUpSoundId = CreateSound("ball_pickup")
	BallInterceptionSoundId = CreateSound("ball_steal")

	BallStealsStatsId = CreateMatchStats("Most steals")

	AddCommandListener(CMD_JoinTeam, "jointeam")
}

public BR_OnBallCreated(ballEntity)
{
	SDKHook(ballEntity, SDKHook_Touch, BR_OnBallTouch)
}

public BR_OnBallTouch(ball, entity)
{
	if (g_IsBallFree 
		&& entity >= 1 
		&& entity <= MaxClients)
	{
		CheckPlayerTouchBall(entity)
	}
}

public BR_Event_PrePlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))	
	if (client == g_BallHolder)
	{
		RemoveBallHolder()
	}
}

public BR_OnBallKicked(client)
{
	RemoveBallHolder()
}

static CheckPlayerTouchBall(client)
{
	if (IsPlayerAlive(client))
	{
		if (g_BallOwner == 0 || GetClientTeam(client) != GetClientTeam(g_BallOwner))
		{		
			CheckPlayerInterception(client);
		}
		else
		{
			SetBallHolder(client)
		}
	}
}

static CheckPlayerInterception(client)
{
	new Float:ballSpeed = GetBallSpeed();
	if (IsClientSmacked(client, ballSpeed))
	{
		SmackPlayerOnBallSpeed(client, ballSpeed);
		SetBallSmackEffect();
	}
	else
	{
		SetBallHolder(client)
	}
}

static SmackPlayerOnBallSpeed(client, Float:ballSpeed)
{
	new Float:playerDexterity = GetPlayerCatchingPercent(client);
	new Float:damage = ballSpeed / 16.0 - playerDexterity;
	if (damage < 10.0)
	{
		damage = 10.0;
	}
	decl Float:ballVelovity[3];
	Entity_GetLocalVelocity(g_Ball, ballVelovity);
	Entity_Hurt(client, RoundFloat(damage), 0, DMG_SHOCK, "sj_ball")
	if(!IsPlayerAlive(client))
	{
		PlaySoundByIdToAll(BallKickSoundId)
	}
	else
	{
		decl Float:pushVel[3]
		pushVel[0] = ballVelovity[0]
		pushVel[1] = ballVelovity[1]
		pushVel[2] = ballVelovity[2] + ((ballVelovity[2] < 0) ? GetRandomFloat(-200.0,-50.0) : GetRandomFloat(50.0,200.0))
		TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, pushVel)
	}
	for (new i = 0; i < 3; i++)
	{
		ballVelovity[i] = ballVelovity[i] * GetRandomFloat(0.1,0.9)
	}
	TeleportEntity(g_Ball, NULL_VECTOR, NULL_VECTOR, ballVelovity)
}

stock bool:Entity_Hurt(entity, damage, attacker=0, damageType=DMG_GENERIC, const String:fakeClassName[]="")
{
	static point_hurt = INVALID_ENT_REFERENCE;
	
	if (point_hurt == INVALID_ENT_REFERENCE || !IsValidEntity(point_hurt)) {
		point_hurt = EntIndexToEntRef(Entity_Create("point_hurt"));
		
		if (point_hurt == INVALID_ENT_REFERENCE) {
			return false;
		}
		
		DispatchSpawn(point_hurt);
	}
	
	AcceptEntityInput(point_hurt, "TurnOn");
	SetEntProp(point_hurt, Prop_Data, "m_nDamage", damage);
	SetEntProp(point_hurt, Prop_Data, "m_bitsDamageType", damageType);
	Entity_PointHurtAtTarget(point_hurt, entity);
	
	if (fakeClassName[0] != '\0') {
		Entity_SetClassName(point_hurt, fakeClassName);
	}
	
	AcceptEntityInput(point_hurt, "Hurt", attacker);
	AcceptEntityInput(point_hurt, "TurnOff");
	
	if (fakeClassName[0] != '\0') {
		Entity_SetClassName(point_hurt, "point_hurt");
	}
	
	return true;
}

stock Entity_Create(const String:className[], ForceEdictIndex=-1)
{
	if (ForceEdictIndex != -1 && Entity_IsValid(ForceEdictIndex)) {
		return INVALID_ENT_REFERENCE;
	}

	return CreateEntityByName(className, ForceEdictIndex);
}

stock Entity_GetLocalVelocity(entity, Float:vec[3])
{
	GetEntPropVector(entity, Prop_Data, "m_vecVelocity", vec);
}

stock Entity_SetClassName(entity, const String:className[])
{
	DispatchKeyValue(entity, "classname", className);
}

stock Entity_IsValid(entity)
{
	return IsValidEntity(entity);
}

stock Entity_PointHurtAtTarget(entity, target, const String:name[]="")
{
	decl String:targetName[128];
	Entity_GetTargetName(entity, targetName, sizeof(targetName));

	if (name[0] == '\0') {

		if (targetName[0] == '\0') {
			// Let's generate our own name
			Format(
				targetName,
				sizeof(targetName),
				"_smlib_Entity_PointHurtAtTarget:%d",
				target
			);
		}
	}
	else {
		strcopy(targetName, sizeof(targetName), name);
	}

	DispatchKeyValue(entity, "DamageTarget", targetName);
	Entity_SetName(target, targetName);
}

stock Entity_GetTargetName(entity, String:buffer[], size)
{
	GetEntPropString(entity, Prop_Data, "m_target", buffer, size);
}

stock Entity_SetName(entity, const String:name[], any:...)
{
	decl String:format[128];
	VFormat(format, sizeof(format), name, 3);

	DispatchKeyValue(entity, "targetname", format);
}

static bool:IsClientSmacked(client, Float:ballSpeed)
{
	if (ballSpeed < BALL_SPEED_FAST)
	{
		return false;
	}
	new buttons = GetClientButtons(client);
	new Float:catchChancePercent = ((buttons & IN_USE) ? 10.0 : 0.0) + GetPlayerCatchingPercent(client);
	if (Client_IsInTurboMode(client))
	{
		catchChancePercent += 5.0;
	}	
	if (GetChance(catchChancePercent))
	{
		return false;
	}
	return true;
}

SetBallHolder(client)
{
	if (client != g_BallHolder)
	{
		new oldBallOwner = g_BallOwner
		OnClientReceivedBall(client)
		
		new team = GetClientTeam(client)
		if (team != g_BallTeam)
		{
			if (g_BallTeam != CS_TEAM_NONE)
			{
				OnClientInterceptedBall()
				AddMatchStatsValue(BallStealsStatsId, client, 1)
			}
			SetBallTeam(team)
		}
		g_BallHolder = client
		
		SetBallOwner(client)
		GetClientName(client, g_BallOwnerName, sizeof(g_BallOwnerName))
		SetBallNotFree()
		FireOnBallReceived(client, oldBallOwner)
	}
}

OnClientInterceptedBall()
{
	if (GetBallSpeed() > BALL_SPEED_FAST) 
	{
		SetBallCatchEffect();
	}
	PlaySoundByIdToAll(BallInterceptionSoundId)
}

OnClientReceivedBall(client)
{
	new team = GetClientTeam(client)
	FadeClient(client, 50, g_TeamColors[team])
	PlaySoundByIdFromEntity(BallPickUpSoundId, client)
}

bool:IsBallHolder(client)
{
	return (g_BallOwner == client) && !g_IsBallFree
}

static FireOnBallReceived(ballHolder, oldBallOwner)
{
	Call_StartForward(OnBallReceivedForward)
	Call_PushCell(ballHolder)
	Call_PushCell(oldBallOwner)
	Call_Finish()
}

public Action:CMD_JoinTeam(client, const String:command[], argc) 
{	
	if (argc)
	{
		decl String:team[3]
		GetCmdArgString(team, sizeof(team))
		if (GetClientTeam(client) == StringToInt(team))
		{
			return Plugin_Handled
		}
	}
	if (client == g_BallOwner)
	{
		RespawnBallWithNotify()
	}
	return Plugin_Continue;
}

Float:GetPlayerCatchingPercent(client)
{
	return GetPlayerUpgradeValue(client, CatchingUpgradeId)
}