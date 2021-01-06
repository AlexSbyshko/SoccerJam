static Handle:BallSpawnCenterConVar
static Handle:OnBallSpawnedForward
static Handle:OnBallCreatedForward
static bool:IsSpawnExists
static BallModelId
static FastBallPickUpSoundId
static BallRespawnedSoundId

enum SolidFlags_t
{
	FSOLID_CUSTOMRAYTEST		= 0x0001,	// Ignore solid type + always call into the entity for ray tests
	FSOLID_CUSTOMBOXTEST		= 0x0002,	// Ignore solid type + always call into the entity for swept box tests
	FSOLID_NOT_SOLID			= 0x0004,	// Are we currently not solid?
	FSOLID_TRIGGER				= 0x0008,	// This is something may be collideable but fires touch functions
											// even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
	FSOLID_NOT_STANDABLE		= 0x0010,	// You can't stand on this
	FSOLID_VOLUME_CONTENTS		= 0x0020,	// Contains volumetric contents (like water)
	FSOLID_FORCE_WORLD_ALIGNED	= 0x0040,	// Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
	FSOLID_USE_TRIGGER_BOUNDS	= 0x0080,	// Uses a special trigger bounds separate from the normal OBB
	FSOLID_ROOT_PARENT_ALIGNED	= 0x0100,	// Collisions are defined in root parent's local coordinate space
	FSOLID_TRIGGER_TOUCH_DEBRIS	= 0x0200,	// This trigger will touch debris objects

	FSOLID_MAX_BITS	= 10
};

static BallLostEvent _ballLostEvent

void SpawnBall(BallLostEvent ballLostEvent)
{
	_ballLostEvent = ballLostEvent
}

public BALL_Init()
{
	OnBallSpawnedForward = CreateForward(ET_Ignore, Param_Cell, Param_Cell)
	RegisterCustomForward(OnBallSpawnedForward, "OnBallSpawned")
	OnBallCreatedForward = CreateForward(ET_Ignore, Param_Cell)
	RegisterCustomForward(OnBallCreatedForward, "OnBallCreated")

	BallModelId = CreateModel("ball")
	FastBallPickUpSoundId = CreateSound("fastball_pickup")
	BallRespawnedSoundId = CreateSound("ball_respawned")
	

	BallSpawnCenterConVar = CreateConVar("sj_ball_spawn_always_center", "0", "if 0, ball spawn always near team that conceded goal", 0, true, 0.0, true, 1.0)

	RegAdminCmd("sj_respawn_ball", CMD_RespawnBall, ADMFLAG_CHANGEMAP)
}

public BALL_OnMapStart()
{
	ClearBallSpawnPoint()
}

public BALL_Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	CreateBall()
	if (g_BallSpawnTeam == CS_TEAM_NONE)
	{
		if (IsSpawnExists)
		{
			Ball_RespawnAtCenter()
		}
		else
		{
			RespawnBallNearRandomTeam()
		}
	}
	else
	{
		RespawnBallNearTeam(g_BallSpawnTeam)
	}
}

public BALL_OnGoal(team, scorer)
{
	g_BallSpawnTeam = GetConVarBool(BallSpawnCenterConVar) ? CS_TEAM_NONE :
		(team == CS_TEAM_T) ? CS_TEAM_CT : CS_TEAM_T
	DestroyBall()
}

RecreateBall()
{
	DestroyBall()
	CreateBall()
	SetBallTeam(g_BallTeam)
}

CreateBall()
{
	g_Ball = CreateEntityByName("hegrenade_projectile")
	DispatchKeyValue(g_Ball, "targetname", SJ_BALL_ENTITY_NAME)
	DispatchSpawn(g_Ball)
	SetEntityModelById(g_Ball, BallModelId)
	g_BallRadius = GetEntityRadius(g_Ball)
	SetEntityRenderMode(g_Ball, RENDER_TRANSCOLOR)

	SetEntProp(g_Ball, Prop_Send, "m_usSolidFlags", FSOLID_NOT_SOLID | FSOLID_TRIGGER)

	FireOnBallCreated(g_Ball)
}

DestroyBall()
{
	AcceptEntityInput(g_Ball, "Kill")
}

ClearBallSpawnPoint()
{
	IsSpawnExists = false
}

SetBallSpawnPoint(const Float:spawnPoint[3])
{
	g_BallSpawnOrigin = spawnPoint
	IsSpawnExists = true
}

GetBallSpawnOrigin(Float:buffer[3])
{
	buffer = g_BallSpawnOrigin;
}


GetBallOrigin(Float:dest[3])
{
	Entity_GetAbsOrigin(g_Ball, dest)
}

stock Entity_GetAbsOrigin(entity, Float:vec[3])
{
	GetEntPropVector(entity, Prop_Data, "m_vecOrigin", vec);
}

public bool:BallTraceFilter(entity, mask, any:client)
{
	return (entity < 1 || entity > MaxClients)
		&& entity != g_Ball
}

Float:GetBallSpeed()
{
	decl Float:ballVelocity[3];
	GetEntPropVector(g_Ball, Prop_Data, "m_vecVelocity", ballVelocity);	
	new Float:tempValue = 0.0;
	for (new i = 0; i < 3; i++)
	{
		tempValue += Pow(ballVelocity[i], 2.0);
	}
	return SquareRoot(tempValue);
}

RespawnBallWithNotify()
{
	Ball_RespawnAtCenter();
	PlaySoundByIdToAll(BallRespawnedSoundId)
	PrintSJMessageAll("%t", "BALL_RESPAWNED")
}

Ball_RespawnAtCenter()
{
	ClearBall()
	SetEntityMoveType(g_Ball, MOVETYPE_FLYGRAVITY)
	TeleportEntity(g_Ball, g_BallSpawnOrigin, NULL_VECTOR, g_StartBallVelocity)
	FireOnBallSpawned(g_Ball, CS_TEAM_NONE)
}

RespawnBallNearRandomTeam()
{
	new randomTeam = GetRandomInt(CS_TEAM_T, CS_TEAM_CT)
	RespawnBallNearTeam(randomTeam)
}

RespawnBallNearTeam(team)
{
	decl Float:spawnOrigin[3]
	GetRandomTeamSpawnOrigin(team, spawnOrigin)
	spawnOrigin[2] += 64
	ClearBall()
	SetEntityMoveType(g_Ball, MOVETYPE_FLYGRAVITY)
	TeleportEntity(g_Ball, spawnOrigin, NULL_VECTOR, g_StartBallVelocity)
	FireOnBallSpawned(g_Ball, team)
}

RemoveBallHolder()
{
	if (g_BallHolder > 0)
	{
		g_BallHolder = 0;
		SetBallFree()
		_ballLostEvent.Raise(g_BallHolder)
	}
}

ClearBall()
{
	RecreateBall()
	SetBallTeam(CS_TEAM_NONE)
	RemoveBallHolder()
	SetBallOwner(0)
	SetBallFree()
}

SetBallTeam(team)
{
	g_BallTeam = team;
	SetBallTeamColor(team);
}

SetBallTeamColor(team)
{
	SetEntityRenderColor(g_Ball, g_TeamColors[team][0], g_TeamColors[team][1], g_TeamColors[team][2], g_TeamColors[team][3]);
}

SetBallOwner(client)
{
	g_BallOwner = client;
}

SetBallFree()
{
	g_IsBallFree = true;
}

SetBallNotFree()
{
	g_IsBallFree = false;
}

SetBallCatchEffect()
{
	decl Float:orig[3];
	Entity_GetAbsOrigin(g_Ball, orig);
	new Float:dir[3];
	TE_SetupSparks(orig, dir, 20, 20);
	TE_SendToAll();
	PlaySoundByIdFromEntity(FastBallPickUpSoundId, g_Ball);
}

TeleportBallToClient(client)
{
	decl Float:clientOrigin[3];
	GetClientAbsOrigin(client, clientOrigin);
	TeleportEntity(g_Ball, clientOrigin, NULL_VECTOR, g_StartBallVelocity);
}

static GetRandomTeamSpawnOrigin(team, Float:destOrigin[3])
{
	new spawnPoints[64];
	new spawnPointCount = 0;
	new index = -1;
	while ((index = FindEntityByClassname(index, g_TeamSpawnEntityNames[team])) != -1)
	{
		spawnPoints[spawnPointCount++] = index;
	}
	if (spawnPointCount > 0)
	{
		new randomIndex = GetRandomInt(0, spawnPointCount - 1);
		Entity_GetAbsOrigin(spawnPoints[randomIndex], destOrigin);
	}
}

public Action:CMD_RespawnBall(client, argc)
{
	RespawnBallWithNotify()
	return Plugin_Handled
}

FireOnBallSpawned(ballEntity, ballTeam)
{
	Call_StartForward(OnBallSpawnedForward)
	Call_PushCell(ballEntity)
	Call_PushCell(ballTeam)
	Call_Finish()
}

FireOnBallCreated(ballEntity)
{	
	Call_StartForward(OnBallCreatedForward)
	Call_PushCell(ballEntity)
	Call_Finish()
}