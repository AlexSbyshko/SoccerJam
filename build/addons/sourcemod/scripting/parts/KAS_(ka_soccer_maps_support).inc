void KaMapsSupport()
{
}

public Action:KAS_Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	FindPoints();
}

FindPoints()
{
	decl String:mapName[MAX_NAME_LENGTH];
	GetCurrentMap(mapName, MAX_NAME_LENGTH);
	if (StrContains(mapName, "ka_", false) >= 0)
	{
		FindKaBallSpawn();
		FindKaGoals();
	}
}

FindKaBallSpawn()
{
	new kaBallEntity = FindEntityByClassname(-1, "func_physbox");
	if (kaBallEntity > 0)
	{
		decl Float:ballSpawnPoint[3];
		Entity_GetAbsOrigin(kaBallEntity, ballSpawnPoint);
		SetBallSpawnPoint(ballSpawnPoint);
		
		RemoveEntity(kaBallEntity);
	}
}

FindKaGoals()
{
	if (FindKaGoalsByClass("trigger_multiple"))
	{
		return;
	}
	FindKaGoalsByClass("trigger_once");
}

bool:FindKaGoalsByClass(const String:className[])
{
	new entity = -1;
	while ((entity = FindEntityByClassname(entity, className)) != INVALID_ENT_REFERENCE) 
	{
		if (IsGoalEntity(entity))
		{
			new team = GetEntityTeam(entity);
			SetGoalEntity(team, entity)
		}
	}
	return g_Goals[CS_TEAM_CT] > 0
		&& g_Goals[CS_TEAM_T] > 0;
}

bool:IsGoalEntity(entity)
{
	decl Float:entityOrigin[3];
	Entity_GetAbsOrigin(entity, entityOrigin);
	decl Float:ballSpawnOrigin[3];
	GetBallSpawnOrigin(ballSpawnOrigin);
	
	new Float:heightDifference = entityOrigin[2] - ballSpawnOrigin[2];
	return GetEntityRadius(entity) < 300.0
		&& FloatAbs(heightDifference) < 100.0;
}

GetEntityTeam(entity)
{
	new someBlueSpawn = FindEntityByClassname(-1, "info_player_counterterrorist");
	new someRedSpawn = FindEntityByClassname(-1, "info_player_terrorist");
	new Float:blueDistance = Entity_GetDistance(entity, someBlueSpawn);
	new Float:redDistance = Entity_GetDistance(entity, someRedSpawn);
	if (blueDistance < redDistance)
	{
		return CS_TEAM_CT;
	}
	return CS_TEAM_T;
}

stock Float:Entity_GetDistance(entity, target)
{
	new Float:targetVec[3];
	Entity_GetAbsOrigin(target, targetVec);
	
	return Entity_GetDistanceOrigin(entity, targetVec);
}

stock Float:Entity_GetDistanceOrigin(entity, const Float:vec[3])
{
	new Float:entityVec[3];
	Entity_GetAbsOrigin(entity, entityVec);
	
	return GetVectorDistance(entityVec, vec);
}