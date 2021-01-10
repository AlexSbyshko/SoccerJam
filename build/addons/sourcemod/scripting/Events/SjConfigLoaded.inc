typedef SjConfigLoadedEventHandler = function void ()

methodmap SjConfigLoadedEvent < PrivateForward
{
	public SjConfigLoadedEvent()
	{
		return view_as<SjConfigLoadedEvent>(new PrivateForward(ET_Ignore))
	}

	public void Subscribe(SjConfigLoadedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise()
	{
		Call_StartForward(this)
		Call_Finish()
	}
}