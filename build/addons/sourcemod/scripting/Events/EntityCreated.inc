typedef EntityCreatedEventHandler = function void (int entity, const char[] classname)

methodmap EntityCreatedEvent < PrivateForward
{
	public EntityCreatedEvent()
	{
		return view_as<EntityCreatedEvent>(new PrivateForward(ET_Ignore, Param_Cell, Param_String))
	}

	public void Subscribe(EntityCreatedEventHandler handler)
	{
		this.AddFunction(INVALID_HANDLE, handler)
	}

	public void Raise(int entity, const char[] classname)
	{
		Call_StartForward(this)
		Call_PushCell(entity)
		Call_PushString(classname)
		Call_Finish()
	}
}