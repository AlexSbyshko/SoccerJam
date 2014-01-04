const Float:UNIT_TO_METERS_NIULTIPLIER = 0.01905

static Float:LastShotOrigin[3]

static LongestGoalStatsId

public GD_Init()
{
	LongestGoalStatsId = CreateMatchStats("Longest goal (m)")
}

public GD_OnBallKicked(client)
{
	GetClientAbsOrigin(client, LastShotOrigin)
}

public GD_OnGoal(team, scorer)
{
	decl Float:ballOrigin[3]
	GetBallOrigin(ballOrigin)
	new Float:distance = GetVectorDistance(LastShotOrigin, ballOrigin)
	new Float:distanceInMeters = distance * UNIT_TO_METERS_NIULTIPLIER
	SetMatchStatsValue(LongestGoalStatsId, scorer, RoundFloat(distanceInMeters))
}