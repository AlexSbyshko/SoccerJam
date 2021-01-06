#include "Services/RemovePlayerWeapons"
#include "Services/GivePlayerDefaultWeapon"

void RemoveBallHolderWeapon(
	const BallReceivedEvent ballReceivedEvent
	)
{
	ballReceivedEvent.Subscribe(OnBallReceived)
}

static OnBallReceived(int ballHolder, int oldBallHolder)
{
	RemovePlayerWeapons(ballHolder)
}

public RBHW_OnBallLost(int client)
{
	GivePlayerDefaultWeapon(client)
}