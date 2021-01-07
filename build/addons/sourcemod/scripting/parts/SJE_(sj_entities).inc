void SjEntitiesFinding(EntityCreatedEvent entityCreatedEvent)
{
	entityCreatedEvent.Subscribe(OnEntityCreated)
}

static void OnEntityCreated(entity, const String:classname[])
{
	if (IsValidEdict(entity))
	{
		SDKHook(entity, SDKHook_Spawn, SJE_OnEntitySpawned)
	}
}

static void SJE_OnEntitySpawned(entity)
{	
	decl String:name[MAX_NAME_LENGTH];
	GetEntPropString(entity, Prop_Data, "m_iName", name, sizeof(name));
	
	if (StrEqual(name, "sj_ballspawn", false))
	{
		decl Float:ballSpawnPoint[3];
		Entity_GetAbsOrigin(entity, ballSpawnPoint);
		SetBallSpawnPoint(ballSpawnPoint);
	} 
	/*else if (StrEqual(name, "sj_teamball_t", false) || StrEqual(name, "sj_death_zone_red", false))
	{
		Entity_GetAbsOrigin(entity, g_DeathZoneOrigins[CS_TEAM_T]);
	}
	else if (StrEqual(name, "sj_teamball_ct", false) || StrEqual(name, "sj_death_zone_blue", false))
	{
		Entity_GetAbsOrigin(entity, g_DeathZoneOrigins[CS_TEAM_CT]);
	}*/
	else if (StrEqual(name, "sj_goal_t", false) || StrEqual(name, "sj_goal_red", false))
	{
		if (GetSwapsCount() % 2 == 0)
		{
			SetGoalEntity(CS_TEAM_T, entity)
		}
		else
		{
			SetGoalEntity(CS_TEAM_CT, entity)
		}
		
	}
	else if (StrEqual(name, "sj_goal_ct", false) || StrEqual(name, "sj_goal_blue", false))
	{
		if (GetSwapsCount() % 2 == 0)
		{
			SetGoalEntity(CS_TEAM_CT, entity)
		}
		else
		{
			SetGoalEntity(CS_TEAM_T, entity)
		}
	}
}