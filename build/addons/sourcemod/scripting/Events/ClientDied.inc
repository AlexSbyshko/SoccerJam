typedef ClientDiedEventHandler = function void (int client)

methodmap ClientDiedEvent < PrivateForward
{
	public ClientDiedEvent()
	{
		return view_as<ClientDiedEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(ClientDiedEventHandler handler)
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