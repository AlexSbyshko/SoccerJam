typedef GoalScoredEventHandler = function void (int team, int scorer)

methodmap GoalScoredEvent < PrivateForward
{
	public GoalScoredEvent()
	{
		return view_as<GoalScoredEvent>(new PrivateForward(ET_Ignore, Param_Cell, Param_Cell))
	}

	public void Subscribe(GoalScoredEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise(int team, int scorer)
	{
		Call_StartForward(this)
		Call_PushCell(team)
		Call_PushCell(scorer)
		Call_Finish()
	}
}