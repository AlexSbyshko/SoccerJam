typedef ClientDyingEventHandler = function void (int client, int killer)

methodmap ClientDyingEvent < PrivateForward
{
	public ClientDyingEvent()
	{
		return view_as<ClientDyingEvent>(new PrivateForward(ET_Ignore, Param_Cell, Param_Cell))
	}

	public void Subscribe(ClientDyingEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise(int client, int killer)
	{
		Call_StartForward(this)
		Call_PushCell(client)
		Call_PushCell(killer)
		Call_Finish()
	}
}