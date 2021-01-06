typedef BallLostEventHandler = function void (int ballHolder)

methodmap BallLostEvent < PrivateForward
{
	public BallLostEvent()
	{
		return view_as<BallLostEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(BallLostEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise(int ballHolder)
	{
		Call_StartForward(this)
		Call_PushCell(ballHolder)
		Call_Finish()
	}
}