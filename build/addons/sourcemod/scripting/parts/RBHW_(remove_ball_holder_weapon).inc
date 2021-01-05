#include "Services/RemovePlayerWeapons"
#include "Services/GivePlayerDefaultWeapon"

public RBHW_OnBallReceived(int ballHolder, int oldBallHolder)
{
	RemovePlayerWeapons(ballHolder)
}

public RBHW_OnBallLost(int client)
{
	GivePlayerDefaultWeapon(client)
}