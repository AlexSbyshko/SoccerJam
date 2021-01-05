#if defined _RemovePlayerWeapons_included
 #endinput
#endif
#define _RemovePlayerWeapons_included

void RemovePlayerWeapons(int client)
{
	for (int slot = 0; slot < 4; slot++)
	{
		int weaponEntity
		
		while((weaponEntity = GetPlayerWeaponSlot(client, slot)) != -1)
		{
			RemovePlayerItem(client, weaponEntity)
			RemoveEntity(weaponEntity)
		}
	} 
}