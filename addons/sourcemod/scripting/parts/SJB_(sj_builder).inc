static GoalOverrideModelId

public SJB_Init()
{
	GoalOverrideModelId = CreateModel("goal_override")
}

public SJB_OnMapStart()
{
	SetGoalEntity(CS_TEAM_T, INVALID_ENT_REFERENCE)
	SetGoalEntity(CS_TEAM_CT, INVALID_ENT_REFERENCE)
}

public SJB_Event_RoundPreStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	SetGoalEntity(CS_TEAM_T, INVALID_ENT_REFERENCE)
	SetGoalEntity(CS_TEAM_CT, INVALID_ENT_REFERENCE)
}

public SJB_Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (g_Goals[CS_TEAM_CT] == INVALID_ENT_REFERENCE)
	{
		BuildGoalEntity(CS_TEAM_CT)
	}
	if (g_Goals[CS_TEAM_T] == INVALID_ENT_REFERENCE)
	{
		BuildGoalEntity(CS_TEAM_T)
	}
}

BuildGoalEntity(team)
{
	decl Float:goalOrigin[3]
	GetRandomTeamSpawnOrigin(team, goalOrigin)
	new goalEntity = SpawnGoalEntity()
	TeleportEntity(goalEntity , goalOrigin, NULL_VECTOR, NULL_VECTOR)
	SetGoalEntity(team, goalEntity)
}

static GetRandomTeamSpawnOrigin(team, Float:destOrigin[3])
{
	new spawnPoints[MAXPLAYERS]
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

SetGoalEntity(team, entity)
{
	g_Goals[team] = entity
	if (entity != INVALID_ENT_REFERENCE)
	{
		SetEntityRenderColor(entity, 255, 255, 255, 255)
	}
}

GetGoalOverrideOrigin(Float:dest[3], team)
{
	if (g_Goals[team] != INVALID_ENT_REFERENCE)
	{
		Entity_GetAbsOrigin(g_Goals[team], dest)
	}
}

SpawnGoalEntity()
{
	decl String:modelPath[PLATFORM_MAX_PATH]
	GetModelPath(GoalOverrideModelId, modelPath)
	new goalEntity = CreateEntityByName("prop_dynamic")
	DispatchKeyValue(goalEntity, "solid", "6")
	DispatchKeyValue(goalEntity, "StartDisabled", "0")
	DispatchKeyValue(goalEntity, "model", modelPath)
	DispatchSpawn(goalEntity)
	SetEntityRenderMode(goalEntity, RENDER_TRANSCOLOR)
	return goalEntity
}