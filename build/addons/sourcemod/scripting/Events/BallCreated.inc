typedef BallCreatedEventHandler = function void (int ballEntity)

methodmap BallCreatedEvent < PrivateForward
{
	public BallCreatedEvent()
	{
		return view_as<BallCreatedEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(BallCreatedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise(int ballEntity)
	{
		Call_StartForward(this)
		Call_PushCell(ballEntity)
		Call_Finish()
	}
}