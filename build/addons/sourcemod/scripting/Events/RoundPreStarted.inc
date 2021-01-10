typedef RoundPreStartedEventHandler = function void ()

methodmap RoundPreStartedEvent < PrivateForward
{
	public RoundPreStartedEvent()
	{
		return view_as<RoundPreStartedEvent>(new PrivateForward(ET_Ignore))
	}

	public void Subscribe(RoundPreStartedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise()
	{
		Call_StartForward(this)
		Call_Finish()
	}
}