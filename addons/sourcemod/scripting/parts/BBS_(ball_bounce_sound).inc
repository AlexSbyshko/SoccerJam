static HeBounceSoundSpecificId
static BallBounceSoundId
static String:BallBounceSoundPath[PLATFORM_MAX_PATH]

public BBS_Init()
{
	AddNormalSoundHook(BBS_Event_SoundPlayed)
	HeBounceSoundSpecificId = CreateSpecificString("he_bounce_sound")
	BallBounceSoundId = CreateSound("ball_bounce")
}

public BBS_OnSjConfigLoaded()
{
	GetSoundPathById(BallBounceSoundId, BallBounceSoundPath)
}

public Action:BBS_Event_SoundPlayed(clients[64],&numClients,String:sample[PLATFORM_MAX_PATH],&entity,&channel,&Float:volume,&level,&pitch,&flags) 
{
	decl String:heBounceSound[PLATFORM_MAX_PATH]
	GetSpecificString(HeBounceSoundSpecificId, heBounceSound, sizeof(heBounceSound))
	if (StrEqual(sample, heBounceSound))
	{
		EmitSound(clients, numClients, BallBounceSoundPath, entity, channel, level, flags, volume, pitch, entity)
		return Plugin_Handled;
	}
	return Plugin_Continue;
}