static TSpawnPoints[32]
static Float:TSpawnPointOrigins[32][3]
static Float:TSpawnPointAngles[32][3]
static TSpawnsCount
static CtSpawnPoints[32]
static Float:CtSpawnPointOrigins[32][3]
static Float:CtSpawnPointAngles[32][3]
static CtSpawnsCount
static SwapsCount


void SpawnPointsSwapping(MapStartedEvent mapStartedEvent)
{
	mapStartedEvent.Subscribe(OnMapStarted)
}

static void OnMapStarted()
{
	SwapsCount = 0
	TSpawnsCount = 0
	CtSpawnsCount = 0
	new entity = INVALID_ENT_REFERENCE
	while ((entity = FindEntityByClassname(entity, "info_player_terrorist")) != INVALID_ENT_REFERENCE) 
	{
    	TSpawnPoints[TSpawnsCount] = entity
    	Entity_GetAbsOrigin(entity, TSpawnPointOrigins[TSpawnsCount])
    	Entity_GetAbsAngles(entity, TSpawnPointAngles[TSpawnsCount])
    	TSpawnsCount++
	}

	while ((entity = FindEntityByClassname(entity, "info_player_counterterrorist")) != INVALID_ENT_REFERENCE) 
	{
    	CtSpawnPoints[CtSpawnsCount] = entity
    	Entity_GetAbsOrigin(entity, CtSpawnPointOrigins[CtSpawnsCount]);
    	Entity_GetAbsAngles(entity, CtSpawnPointAngles[CtSpawnsCount]);
    	CtSpawnsCount++
	}
}

stock Entity_GetAbsAngles(entity, Float:vec[3])
{
	GetEntPropVector(entity, Prop_Data, "m_angAbsRotation", vec);
}


public SSP_OnGoal(team, scorer)
{
	if (IsMatchPublic())
	{
		SpawnPlaces()
	}
}

public SSP_OnEndFirstHalf()
{
	SpawnPlaces()
}

GetSwapsCount()
{
	return SwapsCount
}

SpawnPlaces()
{
	SpawnSpawnPoints()
	SwapGoals()
	SwapsCount++
}

SpawnSpawnPoints()
{
	decl Float:tSpawnPointOrigin[3]
	decl Float:ctSpawnPointOrigin[3]
	decl Float:tSpawnPointAngles[3]
	decl Float:ctSpawnPointAngles[3]
	for (new i = 0; i < TSpawnsCount; i++)
	{
		new tSpawnPoint = TSpawnPoints[i]
		Entity_GetAbsOrigin(tSpawnPoint, tSpawnPointOrigin)
		Entity_GetAbsAngles(tSpawnPoint, tSpawnPointAngles)
		new ctSpawnPoint = CtSpawnPoints[i]
		Entity_GetAbsOrigin(ctSpawnPoint, ctSpawnPointOrigin)
		Entity_GetAbsAngles(ctSpawnPoint, ctSpawnPointAngles)
		TeleportEntity(tSpawnPoint , ctSpawnPointOrigin, ctSpawnPointAngles, NULL_VECTOR)
		TeleportEntity(ctSpawnPoint , tSpawnPointOrigin, tSpawnPointAngles, NULL_VECTOR)
	}
}

SwapGoals()
{
	new tGoal = g_Goals[CS_TEAM_T]
	new ctGoal = g_Goals[CS_TEAM_CT]
	g_Goals[CS_TEAM_T] = ctGoal
	g_Goals[CS_TEAM_CT] = tGoal
}