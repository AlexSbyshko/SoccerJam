typedef MatchRestartedEventHandler = function void ()

methodmap MatchRestartedEvent < PrivateForward
{
	public MatchRestartedEvent()
	{
		return view_as<MatchRestartedEvent>(new PrivateForward(ET_Ignore))
	}

	public void Subscribe(MatchRestartedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise()
	{
		Call_StartForward(this)
		Call_Finish()
	}
}