typedef MapStartedEventHandler = function void ()

methodmap MapStartedEvent < PrivateForward
{
	public MapStartedEvent()
	{
		return view_as<MapStartedEvent>(new PrivateForward(ET_Ignore))
	}

	public void Subscribe(MapStartedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise()
	{
		Call_StartForward(this)
		Call_Finish()
	}
}