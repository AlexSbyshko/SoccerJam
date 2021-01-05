#if defined _RemovePlayerWeapons_included
 #endinput
#endif
#define _RemovePlayerWeapons_included

void RemovePlayerWeapons(int client, int excludeSlot = -1)
{
	for (int slot = 0; slot < 4 && slot != excludeSlot; slot++)
	{
		int weaponEntity
		
		while((weaponEntity = GetPlayerWeaponSlot(client, slot)) != -1)
		{
			CS_DropWeapon(client, weaponEntity, false, true)
			RemoveEntity(weaponEntity)
		}
	} 
}