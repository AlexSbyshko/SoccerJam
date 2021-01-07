#include "Services/RemovePlayerWeapons"

void WeaponsOnSpawnRemoving(ClientSpawnedEvent clienSpawnedEvent)
{
	clienSpawnedEvent.Subscribe(OnClientSpawned)
}

static void OnClientSpawned(int client)
{
	RemovePlayerWeapons(client, CS_SLOT_KNIFE)
}