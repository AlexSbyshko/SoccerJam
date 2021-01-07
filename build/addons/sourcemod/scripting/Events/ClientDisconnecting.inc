typedef ClientDisconnectingEventHandler = function void (int client)

methodmap ClientDisconnectingEvent < PrivateForward
{
	public ClientDisconnectingEvent()
	{
		return view_as<ClientDisconnectingEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(ClientDisconnectingEventHandler handler)
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