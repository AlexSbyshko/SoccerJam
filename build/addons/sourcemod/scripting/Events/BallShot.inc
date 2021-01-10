typedef BallShotEventHandler = function void (int client)

methodmap BallShotEvent < PrivateForward
{
	public BallShotEvent()
	{
		return view_as<BallShotEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(BallShotEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise(int client)
	{
		Call_StartForward(this)
		Call_PushCell(client)
		Call_Finish()
	}
}