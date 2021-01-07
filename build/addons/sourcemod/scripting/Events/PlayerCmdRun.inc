typedef PlayerCmdRunEventHandler = function Action (int client, int& buttons)

methodmap PlayerCmdRunEvent < PrivateForward
{
	public PlayerCmdRunEvent()
	{
		return view_as<PlayerCmdRunEvent>(new PrivateForward(ET_Event, Param_Cell, Param_CellByRef))
	}

	public void Subscribe(PlayerCmdRunEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public Action Raise(int client, int& buttons)
	{
		Call_StartForward(this)
		Call_PushCell(client)
		Call_PushCellRef(buttons)
		Action result
		Call_Finish(result)
		return result
	}
}