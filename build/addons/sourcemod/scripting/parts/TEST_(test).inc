static MatchStats1Id
static MatchStats2Id
static MatchStats3Id
static MatchStats4Id
static MatchStats5Id


public TEST_Init()
{
	MatchStats1Id = CreateMatchStats("stats1")
	MatchStats2Id = CreateMatchStats("stats2")
	MatchStats3Id = CreateMatchStats("stats3")
	MatchStats4Id = CreateMatchStats("stats4")
	MatchStats5Id = CreateMatchStats("stats5")
	RegAdminCmd("sm_t", TEST_Cmd_Test, ADMFLAG_ROOT)
	RegAdminCmd("sm_t2", TEST_Cmd_Test2, ADMFLAG_ROOT)

	HookEvent("grenade_bounce", Bounced)
}

public Action:Bounced(Handle:event, const String:name[], bool:dontBroadcast)
{
	PrintToChatAll("bounce")
	return Plugin_Continue
}

public Action:TEST_Cmd_Test(client, args)
{
	ReplyToCommand(client, "test1")
	//ShowMatchStats(client)
	//ShowAssistants(client)

	SetRemainingRoundTime( 100.0 );
	TeleportEntity(g_Ball, NULL_VECTOR, NULL_VECTOR, g_StartBallVelocity)
	return Plugin_Handled
}

public Action:TEST_Cmd_Test2(client, args)
{
	ReplyToCommand(client, "test2")
	AddMatchStatsValue(MatchStats1Id, client, 100)
	AddMatchStatsValue(MatchStats2Id, client, 200)
	AddMatchStatsValue(MatchStats3Id, client, 300)
	AddMatchStatsValue(MatchStats4Id, client, 400)
	AddMatchStatsValue(MatchStats5Id, client, 500)
	return Plugin_Handled
}

Float:GetRemainingRoundTime() {
    new Float:roundtime = float(GameRules_GetProp( "m_iRoundTime" ));
    new Float:elapsed = GetGameTime() - GameRules_GetPropFloat( "m_fRoundStartTime" );
    return roundtime - elapsed;
}

SetRemainingRoundTime( Float:time ) 
{
	PrintToChatAll("roundtime: %f, starttime: %f, time: %f", float(GameRules_GetProp( "m_iRoundTime")), GetGameTime(), time );
    GameRules_SetPropFloat( "m_fRoundStartTime", GetGameTime() - float(GameRules_GetProp( "m_iRoundTime"))  + time );
}