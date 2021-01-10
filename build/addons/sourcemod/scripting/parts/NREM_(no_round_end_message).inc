void RoundEndMessageDisabling(RoundEndingEvent roungEndingEvent)
{
	roungEndingEvent.Subscribe(OnRoundEnding)
}

static Action OnRoundEnding()
{
	return Plugin_Handled
}