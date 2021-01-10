typedef BallHolderDiedEventHandler = function void (int client)

methodmap BallHolderDiedEvent < PrivateForward
{
	public BallHolderDiedEvent()
	{
		return view_as<BallHolderDiedEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(BallHolderDiedEventHandler handler)
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