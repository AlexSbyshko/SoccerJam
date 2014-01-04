public RBAH_OnMapStart()
{
	Entity_KillAllByClassName("func_bomb_target")
}

public RBAH_Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{	
	Entity_KillAllByClassName("hostage_entity")
}