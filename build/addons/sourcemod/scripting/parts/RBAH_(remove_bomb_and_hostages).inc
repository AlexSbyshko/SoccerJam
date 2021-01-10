void BombAndHostagesRemoving(MapStartedEvent mapStartedEvent, RoundStartedEvent roundStartedEvent)
{
	mapStartedEvent.Subscribe(OnMapStarted)
	roundStartedEvent.Subscribe(OnRoundStarted)
}

static void OnMapStarted()
{
	Entity_KillAllByClassName("func_bomb_target")
}

static void OnRoundStarted()
{	
	Entity_KillAllByClassName("hostage_entity")
}

stock Entity_KillAllByClassName(const String:className[])
{
	new x = 0;

	new entity = INVALID_ENT_REFERENCE;
	while ((entity = FindEntityByClassname(entity, className)) != INVALID_ENT_REFERENCE) {
		RemoveEntity(entity);
		x++;
	}
	
	return x;
}
