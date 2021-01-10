typedef RoundStartedEventHandler = function void ()

methodmap RoundStartedEvent < PrivateForward
{
	public RoundStartedEvent()
	{
		return view_as<RoundStartedEvent>(new PrivateForward(ET_Ignore))
	}

	public void Subscribe(RoundStartedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise()
	{
		Call_StartForward(this)
		Call_Finish()
	}
}