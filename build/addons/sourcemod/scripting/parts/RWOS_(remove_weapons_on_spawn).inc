#include "Services/RemovePlayerWeapons"

public RWOS_Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	
	RemovePlayerWeapons(client, CS_SLOT_KNIFE)
}