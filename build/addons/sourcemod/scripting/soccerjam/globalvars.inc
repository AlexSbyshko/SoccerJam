//GAME
new String:GameFolderName[MAX_FILENAME_LENGTH]
new TeamScore[TEAMS_COUNT]

new g_Goals[TEAMS_COUNT]
new g_TeamColors[TEAMS_COUNT][] = 
{
	{128, 0, 128, 255},	// None
	{128, 0, 128, 255},	// None
	{255, 10, 10, 255},	// T
	{10, 10, 255, 255}	// CT	
}
new bool:g_Goal;
new String:g_TeamSpawnEntityNames[TEAMS_COUNT][MAX_NAME_LENGTH] =
{
	"",	// None
	"",	// None
	"info_player_terrorist",	// T
	"info_player_counterterrorist"	// CT	
}

// BALL
new g_Ball
new g_BallSpawnTeam
new bool:g_IsBallFree
new g_BallOwner
new g_BallHolder
new String:g_BallOwnerName[MAX_NAME_LENGTH]
new Float:g_BallRadius
new g_BallTeam

new g_LaserCache
new g_MiniExplosionSprite

new Float:g_StartBallVelocity[3] = {0.0, 0.0, 100.0}
//new Float:g_DeathZoneOrigins[TEAMS_COUNT][3]
new Float:g_BallSpawnOrigin[3]
new Float:g_BallDistOrigin[2][3]

// PLAYERS

new Float:g_PlayerSpeedMultiplier[MAXPLAYERS+1]
new Float:g_PlayerGravityMultiplier[MAXPLAYERS+1]

// ENGINE
new g_CollisionOffset