static BallKickSoundId
static BallPickUpSoundId
static BallInterceptionSoundId

static BallStealsStatsId

void BallReceiving(ClientDyingEvent clientDyingEvent)
{
	clientDyingEvent.Subscribe(OnClientDying)
	
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

static void OnClientDying(int client, int killer)
{	
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
		SetBallHolder(client)
	}
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
		BallReceived.Raise(client, oldBallOwner)
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