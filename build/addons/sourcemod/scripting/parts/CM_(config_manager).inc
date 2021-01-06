static Handle:OnSjConfigLoadedForward

#define SJ_CONFIG_DIRECTORY "cfg/sourcemod/soccerjam"

//functag public ConfigProcessingFunc(Handle:kv)
typedef ConfigProcessingFunc = function void (Handle kv);

const MAX_CONFIGS = 32
static ConfigsCount
static String:FileNames[MAX_CONFIGS][MAX_NAME_LENGTH]
static String:RootNames[MAX_CONFIGS][MAX_NAME_LENGTH]
static ConfigProcessingFunc:processingFunctions[MAX_CONFIGS]

void ManageConfigs(MapStartedEvent mapStartedEvent)
{
	mapStartedEvent.Subscribe(OnMapStarted)

	OnSjConfigLoadedForward = CreateForward(ET_Ignore)
	RegisterCustomForward(OnSjConfigLoadedForward, "OnSjConfigLoaded")
}

CreateConfig(const String:fileName[], const String:rootSectionName[], ConfigProcessingFunc:processingFunc)
{
	strcopy(FileNames[ConfigsCount], MAX_NAME_LENGTH, fileName)
	strcopy(RootNames[ConfigsCount], MAX_NAME_LENGTH, rootSectionName)
	processingFunctions[ConfigsCount] = processingFunc
	ConfigsCount++
	return ConfigsCount - 1
}

static void OnMapStarted()
{
	LoadMainConfig()
	LoadDownloadList("downloads.txt")
	for (new configId = 0; configId < ConfigsCount; configId++)
	{
		LoadConfig(configId)
	}
	FireOnSjConfigLoaded()
}

LoadConfig(configId)
{
	decl String:filePath[PLATFORM_MAX_PATH]
	SjBuildPath(filePath, PLATFORM_MAX_PATH, FileNames[configId])
	if (FileExists(filePath))
	{
		new Handle:kv = CreateKeyValues(RootNames[configId])
		if (FileToKeyValues(kv, filePath))
		{
			Call_StartFunction(INVALID_HANDLE, processingFunctions[configId])
			Call_PushCell(kv)
			Call_Finish()
		}
		CloseHandle(kv)
	}
}

LoadMainConfig()
{
	AutoExecConfig(true, "soccerjam")
}

LoadDownloadList(const String:fileName[])
{
	decl String:filePath[PLATFORM_MAX_PATH]
	SjBuildPath(filePath, PLATFORM_MAX_PATH, fileName)
	if (FileExists(filePath))
	{
		new Handle:file = OpenFile(filePath, "r");
		decl String:line[128];
		while (!IsEndOfFile(file))
		{
			ReadFileLine(file, line, sizeof(line));
			TrimString(line);
			if((line[0] != '/') && (line[1] != '/') && (line[0] != '\0'))
			{
				if (FileExists(line))
				{
					AddFileToDownloadsTable(line);
				}
			}
		}
		CloseHandle(file)
	}	
}

SjBuildPath(String:buffer[], maxlength, const String:additionPath[], any:...)
{
	decl String:path[PLATFORM_MAX_PATH];
	VFormat(path, sizeof(path), additionPath, 4);
	decl String:configDirectory[PLATFORM_MAX_PATH];
	strcopy(configDirectory, sizeof(configDirectory), SJ_CONFIG_DIRECTORY);
	Format(configDirectory, sizeof(configDirectory), "%s/%s", configDirectory, GameFolderName);
	Format(buffer, maxlength, "%s/%s", configDirectory, path);
}

FireOnSjConfigLoaded()
{
	Call_StartForward(OnSjConfigLoadedForward)
	Call_Finish()
}