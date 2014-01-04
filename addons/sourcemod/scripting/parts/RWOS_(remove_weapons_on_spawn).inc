public RWOS_Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"))
	Client_RemoveAllWeapons(client, "weapon_knife")
	SetEntProp(client, Prop_Send, "m_ArmorValue", 0, 1)
}

public Action:RWOS_OnWeaponDrop(client, weaponIndex)
{
	PrintToChatAll("drop")
	Entity_Kill(weaponIndex)
	return Plugin_Continue
}