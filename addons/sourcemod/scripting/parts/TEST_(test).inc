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
}

public Action:TEST_Cmd_Test(client, args)
{
	ReplyToCommand(client, "test1")
	//ShowMatchStats(client)
	ShowAssistants(client)
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