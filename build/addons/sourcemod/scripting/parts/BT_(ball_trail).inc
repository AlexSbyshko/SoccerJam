I_void_void CreateBallTrailProc
new g_BallSpriteTrail

void BallTrailShowing()
{
	InitGame()
	MapFunction(CreateBallTrailProc, "CreateBallTrail")
}

public BT_OnBallSpawned(ballEntity, team)
{
	SetEntityRenderColor(g_BallSpriteTrail, g_TeamColors[team][0], g_TeamColors[team][1], g_TeamColors[team][2], g_TeamColors[team][3])
}

public BT_OnBallReceived(ballHolder, oldBallOwner)
{
	new team = GetClientTeam(ballHolder)
	SetEntityRenderColor(g_BallSpriteTrail, g_TeamColors[team][0], g_TeamColors[team][1], g_TeamColors[team][2], g_TeamColors[team][3]);
}

InitGame()
{
	GetGameFolderName(GameFolderName, sizeof(GameFolderName))
	I_void_void initGameFunc
	MapFunction(initGameFunc, "Init")
	I_Call_void_void(initGameFunc)
}

public BT_OnBallCreated(ballEntity)
{
	I_Call_void_void(CreateBallTrailProc)
}

MapFunction(&Function:func, const String:functionName[])
{
	MapFunctionPrefix(func, GameFolderName, functionName)
}

public csgo_Init()
{
	CreateTimer(BALL_TRAIL_TIME, Timer_ShowBallTrail, INVALID_HANDLE, TIMER_REPEAT);
}

public csgo_CreateBallTrail()
{
	ShowBallTrail();
}

public Action:Timer_ShowBallTrail(Handle:timer)
{
	if (IsValidEntity(g_Ball))
	{
		ShowBallTrail();
	}
}

ShowBallTrail()
{
	TE_SetupBeamFollow(g_Ball, g_LaserCache, g_LaserCache, BALL_TRAIL_TIME, 13.0, 13.0, 1, g_TeamColors[g_BallTeam]);
	TE_SendToAll();
}

public cstrike_CreateBallTrail()
{
	g_BallSpriteTrail = CreateEntityByName("env_spritetrail");
	DispatchKeyValue(g_BallSpriteTrail, "spritename", LASER_SPRITE);
	DispatchKeyValue(g_BallSpriteTrail, "startwidth", "25.0");
	DispatchKeyValue(g_BallSpriteTrail, "endwidth", "25.0");
	DispatchKeyValueFloat(g_BallSpriteTrail, "lifetime", BALL_TRAIL_TIME);
	DispatchKeyValue(g_BallSpriteTrail, "renderamt", "255");
	DispatchKeyValue(g_BallSpriteTrail, "rendermode", "5");
	DispatchSpawn(g_BallSpriteTrail);
	
	SetVariantString(SJ_BALL_ENTITY_NAME);
	AcceptEntityInput(g_BallSpriteTrail, "SetParent");
}