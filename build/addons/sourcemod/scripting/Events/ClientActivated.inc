typedef ClientActivatedEventHandler = function void (int client)

methodmap ClientActivatedEvent < PrivateForward
{
	public ClientActivatedEvent()
	{
		return view_as<ClientActivatedEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(ClientActivatedEventHandler handler)
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