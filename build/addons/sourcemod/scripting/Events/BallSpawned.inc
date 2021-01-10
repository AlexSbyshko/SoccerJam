typedef BallSpawnedEventHandler = function void (int ballEntity, int ballTeam)

methodmap BallSpawnedEvent < PrivateForward
{
	public BallSpawnedEvent()
	{
		return view_as<BallSpawnedEvent>(new PrivateForward(ET_Ignore, Param_Cell, Param_Cell))
	}

	public void Subscribe(BallSpawnedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise(int ballEntity, int ballTeam)
	{
		Call_StartForward(this)
		Call_PushCell(ballEntity)
		Call_PushCell(ballTeam)
		Call_Finish()
	}
}