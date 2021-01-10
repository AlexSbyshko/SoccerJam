typedef RoundEndingEventHandler = function Action ()

methodmap RoundEndingEvent < PrivateForward
{
	public RoundEndingEvent()
	{
		return view_as<RoundEndingEvent>(new PrivateForward(ET_Event))
	}

	public void Subscribe(RoundEndingEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public Action Raise()
	{
		Call_StartForward(this)
		Action result
		Call_Finish(result)
		return result
	}
}