typedef ClientSpawnedEventHandler = function void (int client)

methodmap ClientSpawnedEvent < PrivateForward
{
	public ClientSpawnedEvent()
	{
		return view_as<ClientSpawnedEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(ClientSpawnedEventHandler handler)
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