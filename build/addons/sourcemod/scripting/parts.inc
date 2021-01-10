
static Handle:CustomForwardsArray
static Handle:CustomForwardNamesArray

#define PART_NAME_LENGTH 16

static Handle:PartsNamesArray = INVALID_HANDLE

InitPartSystem()
{
	PartsNamesArray = CreateArray(PART_NAME_LENGTH)
	CustomForwardsArray = CreateArray()
	CustomForwardNamesArray = CreateArray(MAX_NAME_LENGTH)
}

RegisterCustomForward(Handle:fwd, const String:postfix[])
{
	PushArrayCell(CustomForwardsArray, fwd)
	PushArrayString(CustomForwardNamesArray, postfix)
	new Function:func = GetFunctionByName(INVALID_HANDLE, postfix)
	if (func != INVALID_FUNCTION)
	{
		AddToForward(fwd, INVALID_HANDLE, func)
	}
}

RegisterPart(const String:partName[PART_NAME_LENGTH])
{
	PushArrayString(PartsNamesArray, partName)
}

InitParts()
{
	new partsCount = GetArraySize(PartsNamesArray)
	for (new i = 0; i < partsCount; i++)
	{
		decl String:partName[PART_NAME_LENGTH]
		GetArrayString(PartsNamesArray, i, partName, sizeof(partName))
		HookCustomForwards(partName)
	}
}

HookCustomForwards(const String:partName[PART_NAME_LENGTH])
{
	new forwardsCount = GetArraySize(CustomForwardsArray)
	for (new i = 0; i < forwardsCount; i++)
	{
		new Handle:fwd = GetArrayCell(CustomForwardsArray, i)
		decl String:postfix[MAX_NAME_LENGTH]
		GetArrayString(CustomForwardNamesArray, i, postfix, sizeof(postfix))
		HookPartForward(fwd, partName, postfix)
	}
}

HookPartForward(Handle:fwd, const String:partName[PART_NAME_LENGTH], const String:postfix[])
{
	decl String:partFunctionName[MAX_NAME_LENGTH]
	Format(partFunctionName, MAX_NAME_LENGTH, "%s_%s", partName, postfix);
	new Function:partFunction = GetFunctionByName(INVALID_HANDLE, partFunctionName)
	if (partFunction != INVALID_FUNCTION)
	{
		AddToForward(fwd, INVALID_HANDLE, partFunction);
	}
}