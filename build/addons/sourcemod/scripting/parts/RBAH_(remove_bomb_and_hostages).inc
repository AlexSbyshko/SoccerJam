public RBAH_OnMapStart()
{
	Entity_KillAllByClassName("func_bomb_target")
}

public RBAH_Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{	
	Entity_KillAllByClassName("hostage_entity")
}

stock Entity_KillAllByClassName(const String:className[])
{
	new x = 0;

	new entity = INVALID_ENT_REFERENCE;
	while ((entity = FindEntityByClassname(entity, className)) != INVALID_ENT_REFERENCE) {
		RemoveEntity(entity);
		x++;
	}
	
	return x;
}
