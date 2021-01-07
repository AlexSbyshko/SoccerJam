typedef RoundTerminatedEventHandler = function void (float& delay, CSRoundEndReason& reason)

methodmap RoundTerminatedEvent < PrivateForward
{
	public RoundTerminatedEvent()
	{
		return view_as<RoundTerminatedEvent>(new PrivateForward(ET_Event, Param_FloatByRef, Param_CellByRef))
	}

	public void Subscribe(RoundTerminatedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public Action Raise(float& delay, CSRoundEndReason& reason)
	{
		Call_StartForward(this)
		Call_PushFloatRef(delay);
		Call_PushCellRef(reason)
		Action result
		Call_Finish(result)
		return result
	}
}