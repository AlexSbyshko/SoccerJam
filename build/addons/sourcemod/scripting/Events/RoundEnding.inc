typedef RoundEndingEventHandler = function void ()

methodmap RoundEndingEvent < PrivateForward
{
	public RoundEndingEvent()
	{
		return view_as<RoundEndingEvent>(new PrivateForward(ET_Ignore))
	}

	public void Subscribe(RoundEndingEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise()
	{
		Call_StartForward(this)
		Call_Finish()
	}
}