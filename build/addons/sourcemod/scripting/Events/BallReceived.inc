typedef BallReceivedEventHandler = function void (int ballHolder, int oldBallHolder)

methodmap BallReceivedEvent < PrivateForward
{
	public BallReceivedEvent()
	{
		return view_as<BallReceivedEvent>(new PrivateForward(ET_Ignore, Param_Cell, Param_Cell))
	}

	public void Subscribe(BallReceivedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise(int ballHolder, int oldBallHolder)
	{
		Call_StartForward(this)
		Call_PushCell(ballHolder)
		Call_PushCell(oldBallHolder)
		Call_Finish()
	}
}