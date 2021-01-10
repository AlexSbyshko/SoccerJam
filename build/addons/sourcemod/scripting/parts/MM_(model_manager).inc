const MAX_MODELS = 128
static ModelsCount
static String:ModelNames[MAX_MODELS][MAX_NAME_LENGTH]
static String:ModePaths[MAX_MODELS][PLATFORM_MAX_PATH]
static ModelCaches[MAX_MODELS]

void ModelManaging()
{
	SjConfigLoaded.Subscribe(OnSjConfigLoaded)

	CreateConfig("models.cfg", "models", MM_ReadConfig)
}

static void OnSjConfigLoaded()
{
	for (new modelId = 0; modelId < ModelsCount; modelId++)
	{
		ModelCaches[modelId] = PrecacheModel(ModePaths[modelId])
	}
	g_LaserCache = PrecacheModel(LASER_SPRITE)
	PrecacheModel("materials/sprites/animglow02.vmt")
	g_MiniExplosionSprite = PrecacheModel("materials/sprites/blueglow2.vmt")
}

CreateModel(const String:modelName[])
{
	strcopy(ModelNames[ModelsCount], MAX_NAME_LENGTH, modelName)
	ModelsCount++
	return ModelsCount - 1
}

public MM_ReadConfig(Handle:kv)
{
	for (new modelId = 0; modelId < ModelsCount; modelId++)
	{
		KvGetString(kv, ModelNames[modelId], ModePaths[modelId], PLATFORM_MAX_PATH);
	}
}

stock GetModelPath(modelId, String:dest[PLATFORM_MAX_PATH])
{
	strcopy(dest, PLATFORM_MAX_PATH, ModePaths[modelId])
}

stock GetModelCache(modelId)
{
	return ModelCaches[modelId]
}

stock SetEntityModelById(entity, modelId)
{
	SetEntityModel(entity, ModePaths[modelId])
}