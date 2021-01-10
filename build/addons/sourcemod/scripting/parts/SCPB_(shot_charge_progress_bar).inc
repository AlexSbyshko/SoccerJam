static int ProgressBarStartTimeOffset
static int ProgressBarDurationOffset

void ShotChargeProgressBarShowing()
{
	ShotChargingStarted.Subscribe(OnShotChargingStarted)

	ProgressBarStartTimeOffset = FindSendPropInfo("CCSPlayer", "m_flProgressBarStartTime")
	ProgressBarDurationOffset = FindSendPropInfo("CCSPlayer", "m_iProgressBarDuration")
}

static void OnShotChargingStarted(int client)
{
	StartProgressBar(client)
}

public void SCPB_OnShotChargindEnded(int client)
{
	StopProgressBar(client)
}

static void StartProgressBar(int client)
{
	SetEntData(client, ProgressBarDurationOffset, 1, 4, true)
	SetEntDataFloat(client, ProgressBarStartTimeOffset, GetGameTime(), true)
}

static void StopProgressBar(int client)
{
	SetEntDataFloat(client, ProgressBarStartTimeOffset, 0.0, true)
	SetEntData(client, ProgressBarDurationOffset, 0, 1, true)
}