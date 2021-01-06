static Handle:DeathZoneRadiusConVar
static Handle:DeathZoneDamageMinConVar
static Handle:DeathZoneDamageMaxConVar
static DeathZoneKillSoundId

enum DissolveType
{
	DISSOLVE_NORMAL = 0,
	DISSOLVE_ELECTRICAL,
	DISSOLVE_ELECTRICAL_LIGHT,
	DISSOLVE_CORE
};

void DeathZoneProcessing()
{
	DeathZoneRadiusConVar = CreateConVar("sj_death_zone_radius", "650", "Radius of death zone that does damage to enemy", 0, true, 0.0)
	DeathZoneDamageMinConVar = CreateConVar("sj_death_zone_damage_min", "5", "Min damage of death zone", 0, true, 0.0)
	DeathZoneDamageMaxConVar = CreateConVar("sj_death_zone_damage_max", "15", "Max damage of death zone", 0, true, 0.0)
	DeathZoneKillSoundId = CreateSound("death_zone_kill")
	CreateTimer(1.0, Timer_CheckDeathZone, _, TIMER_REPEAT)
}

public Action:Timer_CheckDeathZone(Handle:timer)
{	
	if (!g_Goal)
	{
		CheckDeathZones()
	}
	return Plugin_Continue
}

static CheckDeathZones()
{
	CheckDeathZone(CS_TEAM_CT)
	CheckDeathZone(CS_TEAM_T)
}

static CheckDeathZone(team)
{
	decl playersInZone[MaxClients+1]
	new	playersInZoneCount = 0
	new chosenPlayer = 0
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			if (IsPlayerAlive(i)
				&& GetClientOpponentSJTeam(i) == team
				&& IsClientInDeathZone(i, team))
			{	
				playersInZone[playersInZoneCount++] = i
				if (i == g_BallHolder) 
				{
					chosenPlayer = i
					break
				}
			}
		}
	}
	if (playersInZoneCount > 0)
	{
		if (!chosenPlayer) 
		{
			new randomIndex = GetRandomInt(0, (playersInZoneCount-1))
			chosenPlayer = playersInZone[randomIndex]
		}
		new minDamage = GetConVarInt(DeathZoneDamageMinConVar)
		new maxDamage = GetConVarInt(DeathZoneDamageMaxConVar)
		new damage = (chosenPlayer == g_BallHolder) ? GetClientHealth(chosenPlayer) : GetRandomInt(minDamage, maxDamage)
		TerminatePlayer(chosenPlayer, team, damage)
	}
	new Float:radius = GetConVarFloat(DeathZoneRadiusConVar) * 2
	decl Float:goalOverrideOrigin[3]
	GetGoalOverrideOrigin(goalOverrideOrigin, team)
	TE_SetupBeamRingPoint(goalOverrideOrigin, radius + 0.1, radius, g_LaserCache, 0, 0, 66, 0.5, 10.0, 1.0, g_TeamColors[team], 0, 0)
	TE_SendToAll()
}

TerminatePlayer(client, team, dmg) 
{	
	Entity_Hurt(client, dmg, 0, DMG_DROWN, "sj_death_zone")
	if (!IsPlayerAlive(client))
	{
		Effect_DissolvePlayerRagDoll(client)
		PlaySoundByIdToAll(DeathZoneKillSoundId)
	}
	decl Float:goalOverrideOrigin[3]
	GetGoalOverrideOrigin(goalOverrideOrigin, team)
	decl Float:startOrigin[3]
	startOrigin[0] += goalOverrideOrigin[0]
	startOrigin[1] += goalOverrideOrigin[1]
	startOrigin[2] += goalOverrideOrigin[2] + 256.0
	decl Float:clientOrigin[3]
	GetClientAbsOrigin(client, clientOrigin)
	TE_SetupBeamPoints(startOrigin, clientOrigin, g_LaserCache, 0, 0, 66, 0.5, 10.0, 10.0, 0, 1.0, g_TeamColors[team], 0)
	TE_SendToAll()
}

stock bool:Effect_DissolvePlayerRagDoll(client, DissolveType:dissolveType=DISSOLVE_NORMAL)
{
	new m_hRagdoll = GetEntPropEnt(client, Prop_Send, "m_hRagdoll");

	if (m_hRagdoll == -1) {
		return false;
	}
	
	return Effect_DissolveEntity(m_hRagdoll, dissolveType);
}

public bool:IsClientInOwnDeathZone(client)
{
	new team = GetClientTeam(client);
	return IsClientInDeathZone(client, team);
}

bool:IsClientInDeathZone(client, deathZoneTeam)
{
	decl Float:goalOverrideOrigin[3]
	GetGoalOverrideOrigin(goalOverrideOrigin, deathZoneTeam)
	decl Float:clientOrigin[3]
	GetClientAbsOrigin(client, clientOrigin)
	return GetVectorDistance(goalOverrideOrigin, clientOrigin) < GetConVarFloat(DeathZoneRadiusConVar)
}

stock bool:Effect_DissolveEntity(entity, DissolveType:dissolveType=DISSOLVE_NORMAL)
{
	new env_entity_dissolver = CreateEntityByName("env_entity_dissolver");

	if (env_entity_dissolver == -1) {
		return false;
	}

	Entity_PointAtTarget(env_entity_dissolver, entity);
	SetEntProp(env_entity_dissolver, Prop_Send, "m_nDissolveType", _:dissolveType);
	AcceptEntityInput(env_entity_dissolver,	"Dissolve");
	RemoveEntity(env_entity_dissolver);

	return true;
}

stock Entity_PointAtTarget(entity, target, const String:name[]="")
{
	decl String:targetName[128];
	Entity_GetTargetName(entity, targetName, sizeof(targetName));

	if (name[0] == '\0') {

		if (targetName[0] == '\0') {
			// Let's generate our own name
			Format(
				targetName,
				sizeof(targetName),
				"_smlib_Entity_PointAtTarget:%d",
				target
			);
		}
	}
	else {
		strcopy(targetName, sizeof(targetName), name);
	}
	
	Entity_SetTargetName(entity, targetName);
	Entity_SetName(target, targetName);
}

stock Entity_SetTargetName(entity, const String:name[], any:...)
{
	decl String:format[128];
	VFormat(format, sizeof(format), name, 3);

	DispatchKeyValue(entity, "target", format);
}