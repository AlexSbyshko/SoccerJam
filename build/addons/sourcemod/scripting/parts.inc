
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

	HookPartEvent("cs_match_end_restart", partName, "MatchEndRestart")
	HookPartEvent("cs_win_panel_round", partName, "WinPanelRound")
	HookPartEvent("player_activate", partName, "PlayerActivate")
	HookPartEvent("player_hurt", partName, "PlayerHurt")
	HookPartEvent("player_spawn", partName, "PlayerSpawn")
	HookPartEvent("player_team", partName, "PlayerTeam")
	HookPartEvent("player_death", partName, "PlayerDeath")
	HookPartEvent("player_disconnect", partName, "PlayerDisconnect")
	HookPartEvent("round_end", partName, "RoundEnd")
	HookPartEvent("round_freeze_end", partName, "RoundFreezeEnd")
	HookPartEvent("round_prestart", partName, "RoundPreStart")
	HookPartEvent("round_start", partName, "RoundStart")	
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
		PrintToServer("Hook forward: %s", partFunctionName)
		AddToForward(fwd, INVALID_HANDLE, partFunction);
	}
}

HookPartEvent(const String:hookName[], const String:partName[PART_NAME_LENGTH], const String:postfix[])
{
	decl String:partFunctionName[MAX_NAME_LENGTH]
	Format(partFunctionName, MAX_NAME_LENGTH, "%s_Event_%s", partName, postfix);
	EventHook partFunction = view_as<EventHook>(GetFunctionByName(INVALID_HANDLE, partFunctionName))
	if (partFunction != INVALID_FUNCTION)
	{
		HookEventEx(hookName, partFunction)
	}

	decl String:partPreFunctionName[MAX_NAME_LENGTH]
	Format(partPreFunctionName, MAX_NAME_LENGTH, "%s_Event_Pre%s", partName, postfix);
	EventHook preFunction = view_as<EventHook>(GetFunctionByName(INVALID_HANDLE, partPreFunctionName))
	if (preFunction != INVALID_FUNCTION)
	{
		HookEventEx(hookName, preFunction, EventHookMode_Pre)
	}
}