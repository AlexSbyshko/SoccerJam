typedef ShotChargingStartedEventHandler = function void (int client)

methodmap ShotChargingStartedEvent < PrivateForward
{
	public ShotChargingStartedEvent()
	{
		return view_as<ShotChargingStartedEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(ShotChargingStartedEventHandler handler)
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