typedef ShotChargingEndedEventHandler = function void (int client)

methodmap ShotChargingEndedEvent < PrivateForward
{
	public ShotChargingEndedEvent()
	{
		return view_as<ShotChargingEndedEvent>(new PrivateForward(ET_Ignore, Param_Cell))
	}

	public void Subscribe(ShotChargingEndedEventHandler handler)
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