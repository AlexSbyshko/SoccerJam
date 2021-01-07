void DrawDisabling(RoundTerminatedEvent roundTerminatedEvent)
{
	roundTerminatedEvent.Subscribe(OnRoundTerminated)
}

static Action OnRoundTerminated(float& delay, CSRoundEndReason& reason)
{
	if (reason == CSRoundEnd_Draw)
	{
		return Plugin_Handled
	}
	return Plugin_Continue
}