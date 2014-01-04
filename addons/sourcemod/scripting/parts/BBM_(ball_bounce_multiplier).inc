static Handle:BallBounceMultiplierConVar

public BBM_Init()
{
	BallBounceMultiplierConVar = CreateConVar("sj_ball_bounce_multiplier", "0.85", "Ball bounce multiplier", 0, true, 0.0, true, 1.0)
}

public BBM_OnBallCreated(ballEntity)
{
	SDKHook(ballEntity, SDKHook_Touch, BBM_OnBallTouch)
}

public BBM_OnBallTouch(ball, entity)
{
	if (!g_Goal
		&& g_IsBallFree
		&& IsEntitySolidForClients(entity)
		&& GetBallSpeed() >= 50.0)
	{
		decl Float:oldVelocity[3]
		Entity_GetLocalVelocity(ball, oldVelocity)
		decl Float:newVelocity[3]		
		new Float:bounceMultiplier = GetConVarFloat(BallBounceMultiplierConVar)
		for (new i = 0; i < 3; i++)
		{
			newVelocity[i] = oldVelocity[i] * bounceMultiplier
		}
		TeleportEntity(ball, NULL_VECTOR, NULL_VECTOR, newVelocity)
	}
}