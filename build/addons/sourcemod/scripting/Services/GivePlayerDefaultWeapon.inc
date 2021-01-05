#if defined _GivePlayerDefaultWeapon_included
 #endinput
#endif
#define _GivePlayerDefaultWeapon_included

void GivePlayerDefaultWeapon(client)
{
	GivePlayerItem(client, "weapon_knife")
}