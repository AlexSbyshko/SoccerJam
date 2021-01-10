const Float:UNIT_TO_METERS_NIULTIPLIER = 0.01905

static Float:LastShotOrigin[3]

static LongestGoalStatsId

void GoalDistanceCounting()
{
	BallShot.Subscribe(OnBallShot)
	GoalScored.Subscribe(OnGoalScored)

	LongestGoalStatsId = CreateMatchStats("Longest goal (m)")
}

static void OnBallShot(client)
{
	GetClientAbsOrigin(client, LastShotOrigin)
}

static void OnGoalScored(team, scorer)
{
	decl Float:ballOrigin[3]
	GetBallOrigin(ballOrigin)
	new Float:distance = GetVectorDistance(LastShotOrigin, ballOrigin)
	new Float:distanceInMeters = distance * UNIT_TO_METERS_NIULTIPLIER
	SetMatchStatsValue(LongestGoalStatsId, scorer, RoundFloat(distanceInMeters))
}