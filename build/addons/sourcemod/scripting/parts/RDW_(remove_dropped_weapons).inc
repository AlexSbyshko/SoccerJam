public Action RDW_OnWeaponDrop(int client, int weapon)
{
	if (IsValidEdict(weapon))
	{
		RemoveEntity(weapon)
	}
	return Plugin_Continue
}