const MAX_SPECIFICS = 128
static SpecificCount
static String:SpecificNames[MAX_SPECIFICS][MAX_NAME_LENGTH]
static String:SpecificStrings[MAX_SPECIFICS][PLATFORM_MAX_PATH]

public GSM_Init()
{
	CreateConfig("game_specific.cfg", "specific", GSM_ReadConfig)
}

public GSM_ReadConfig(Handle:kv)
{
	for (new specificId = 0; specificId < SpecificCount; specificId++)
	{
		KvGetString(kv, SpecificNames[specificId], SpecificStrings[specificId], PLATFORM_MAX_PATH)
	}
}

CreateSpecificString(const String:specificName[])
{
	strcopy(SpecificNames[SpecificCount], MAX_NAME_LENGTH, specificName)
	SpecificCount++
	return SpecificCount - 1
}

stock GetSpecificString(specificId, String:dest[], deatLen)
{
	strcopy(dest, deatLen, SpecificStrings[specificId])
}