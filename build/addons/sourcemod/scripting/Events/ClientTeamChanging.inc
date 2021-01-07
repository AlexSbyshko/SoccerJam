typedef ClientTeamChangingEventHandler = function void (int client)

methodmap ClientTeamChangingEvent < PrivateForward
{
	public ClientTeamChangingEvent()
	{
		return view_as<ClientTeamChangingEvent>(new PrivateForward(ET_Ignore))
	}

	public void Subscribe(ClientTeamChangingEventHandler handler)
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