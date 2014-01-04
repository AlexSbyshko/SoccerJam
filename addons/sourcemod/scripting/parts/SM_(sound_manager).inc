const MAX_SOUNDS = 128
const MAX_SOUND_GROUPS = 128
const MAX_SOUNDS_IN_GROUPS = 32

static SoundGroupsCount
static SoundCount[MAX_SOUND_GROUPS]
static String:SoundGroupNames[MAX_SOUNDS][MAX_NAME_LENGTH]
static String:SoundPaths[MAX_SOUND_GROUPS][MAX_SOUNDS][PLATFORM_MAX_PATH]


public SM_Init()
{
	CreateConfig("sounds.cfg", "sounds", SM_ReadConfig)
}

CreateSound(const String:soundGrouopName[])
{
	strcopy(SoundGroupNames[SoundGroupsCount], MAX_NAME_LENGTH, soundGrouopName)
	SoundGroupsCount++
	return SoundGroupsCount - 1
}

public SM_ReadConfig(Handle:kv)
{
	for (new soundGroupId = 0; soundGroupId < SoundGroupsCount; soundGroupId++)
	{
		SoundCount[soundGroupId] = 0
		if (KvJumpToKey(kv, SoundGroupNames[soundGroupId]))
		{
			if (KvGotoFirstSubKey(kv, false))
			{
				do
				{
					new soundCountInThisGroup = SoundCount[soundGroupId]
					KvGetSectionName(kv, SoundPaths[soundGroupId][soundCountInThisGroup], PLATFORM_MAX_PATH)
					SoundCount[soundGroupId]++
				}
				while (KvGotoNextKey(kv, false))
				KvGoBack(kv)
			}
			KvGoBack(kv)
		}
	}
}

public SM_OnSjConfigLoaded()
{
	new soundId
	for (new soundGroupId = 0; soundGroupId < SoundGroupsCount; soundGroupId++)
	{
		for (soundId = 0; soundId < SoundCount[soundGroupId]; soundId++)
		{
			PrecacheSound(SoundPaths[soundGroupId][soundId])

			decl String:fullSoundPath[PLATFORM_MAX_PATH]
			Format(fullSoundPath, sizeof(fullSoundPath), "sound/%s", SoundPaths[soundGroupId][soundId])
			AddFileToDownloadsTable(fullSoundPath)
		}
	}
}

stock String:GetSoundPathById(soundGroupId, String:dest[PLATFORM_MAX_PATH])
{
	if (SoundCount[soundGroupId] == 0)
	{
		return
	}
	new randomSoundId = GetRandomInt(0, SoundCount[soundGroupId] - 1)
	strcopy(dest, PLATFORM_MAX_PATH, SoundPaths[soundGroupId][randomSoundId])
}

stock PlaySoundByIdToAll(soundGroupId)
{
	if (SoundCount[soundGroupId] == 0)
	{
		return
	}
	new randomSoundId = GetRandomInt(0, SoundCount[soundGroupId] - 1)
	if (!StrEqual(SoundPaths[soundGroupId][randomSoundId], "", false))
	{
		for (new i = 1; i <= MaxClients; i++)
		{
			if (IsClientInGame(i))
			{
				EmitSoundToClient(i, SoundPaths[soundGroupId][randomSoundId]);
			}
		}
	}
}

stock PlaySoundByIdToClient(client, soundGroupId)
{
	if (SoundCount[soundGroupId] == 0)
	{
		return
	}
	new randomSoundId = GetRandomInt(0, SoundCount[soundGroupId] - 1)
	if (!StrEqual(SoundPaths[soundGroupId][randomSoundId], "", false))
	{
		EmitSoundToClient(client, SoundPaths[soundGroupId][randomSoundId]);
	}
}

stock PlaySoundByIdFromEntity(soundGroupId, entity)
{
	if (SoundCount[soundGroupId] == 0)
	{
		return
	}
	new randomSoundId = GetRandomInt(0, SoundCount[soundGroupId] - 1)
	if (!StrEqual(SoundPaths[soundGroupId][randomSoundId], "", false))
	{
		EmitSoundToAll(SoundPaths[soundGroupId][randomSoundId], entity);
	}
}