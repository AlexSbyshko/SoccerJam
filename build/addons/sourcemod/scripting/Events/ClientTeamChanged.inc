typedef ClientTeamChangedEventHandler = function void (int client)

methodmap ClientTeamChangedEvent < PrivateForward
{
	public ClientTeamChangedEvent()
	{
		return view_as<ClientTeamChangedEvent>(new PrivateForward(ET_Ignore))
	}

	public void Subscribe(ClientTeamChangedEventHandler handler)
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