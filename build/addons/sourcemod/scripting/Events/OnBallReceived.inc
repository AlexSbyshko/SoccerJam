typedef OnBallReceivedEventHandler = function void ()

methodmap OnBallReceivedEvent < PrivateForward
{
	public OnBallReceivedEvent()
	{
		return view_as<OnBallReceivedEvent>(new PrivateForward(ET_Ignore))
	}

	public void Subscribe(OnBallReceivedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise(int data)
	{
		Call_StartForward(this)
		Call_Finish()
	}
}