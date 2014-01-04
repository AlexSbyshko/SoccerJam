public Action:NRD_OnTerminateRound(&Float:delay, &CSRoundEndReason:reason)
{
	if (reason == CSRoundEnd_Draw)
	{
		return Plugin_Handled
	}
	return Plugin_Continue
}