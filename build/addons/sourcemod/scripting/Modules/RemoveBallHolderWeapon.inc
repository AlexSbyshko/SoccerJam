#include "Services/RemovePlayerWeapons"
#include "Services/GivePlayerDefaultWeapon"

void RemoveBallHolderWeapon(
	const BallReceivedEvent ballReceivedEvent,
	const BallLostEvent ballLostEvent
	)
{
	ballReceivedEvent.Subscribe(OnBallReceived)
	ballLostEvent.Subscribe(OnBallLost)
}

static OnBallReceived(int ballHolder, int oldBallHolder)
{
	RemovePlayerWeapons(ballHolder)
}

static OnBallLost(int client)
{
	GivePlayerDefaultWeapon(client)
}