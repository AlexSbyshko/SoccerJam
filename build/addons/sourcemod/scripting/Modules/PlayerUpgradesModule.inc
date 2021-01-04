#include "Module"
#include "PlayerCmdRunner"
#include "Data/DisplayMenuData"
#include "Data/Upgrade"

static UpgradeMenuDisplayer _menuDisplayer

void CreatePlayerUpgradesModule(
	Module result,
	const UpgradeMenuDisplayer menuDisplayer
	)
{
	_menuDisplayer = menuDisplayer

	result.InitFunc = Init
	result.OnPlayerRunCmdFunc = OnRunCmd
	result.OnStartPublicFunc = OnStartPublicMatch
	result.OnStartMatchFunc = OnStartMatch
}

static Handle:InitialCreditsConVar
static Handle:ResetUpgradesEnabledConVar

static Handle:OnClientResetUpgradesForward
static Handle:OnGetClientUpgradeInfoForward

static UpgradeMaxLevelSoundId

/// API ////

stock AddPlayerCredits(client, creditsCount)
{
	new oldCredits = GetPlayerCredits(client, creditsCount)
	SetPlayerCredits(client, oldCredits + creditsCount)
}

stock GetPlayerCredits(client, creditsCount)
{
	return PlayerCredits[client]
}

stock SetPlayerCredits(client, creditsCount)
{
	PlayerCredits[client] = creditsCount
}

stock CreateUpgrade(const String:name[], Float:minValue, Float:maxValue, levelsCount, cost, 
		const String:measureName[] = "", UpgradeFunc:funcion = INVALID_FUNCTION)
{
	strcopy(UpgradeInfo[UpgradesCount].SJUpgradeData_Name, UPGRADE_NAME_LENGTH, name)
	UpgradeInfo[UpgradesCount].SJUpgradeData_MinValue = minValue
	UpgradeInfo[UpgradesCount].SJUpgradeData_MaxValue = maxValue
	UpgradeInfo[UpgradesCount].SJUpgradeData_LevelsCount = levelsCount
	UpgradeInfo[UpgradesCount].SJUpgradeData_Cost = cost
	UpgradeInfo[UpgradesCount].SJUpgradeData_Func = funcion
	strcopy(UpgradeInfo[UpgradesCount].SJUpgradeData_MeasName, UPGRADE_MEAS_NAME_LENGTH, measureName)
	return UpgradesCount++
}

public ClearClientUpgrades(client)
{
	for (new upgrade = 0; upgrade < UpgradesCount; upgrade++)
	{
		SetPlayerUpgradeLevel(client, upgrade, 0)
	}
	PlayerCredits[client] = GetConVarInt(InitialCreditsConVar)
	FireOnClientResetUpgrades(client)
}

////////////

static void Init()
{
	CreateConfig("upgrades.cfg", "upgrades", OnReadConfig)
	
	UpgradeMaxLevelSoundId = CreateSound("upgrade_maxlevel")

	InitialCreditsConVar = CreateConVar("sj_initial_credits", "12", "Start credits for upgrades", 0, true, 0.0)
	ResetUpgradesEnabledConVar = CreateConVar("sj_reset_upgrades_enabled", "0", "Enable reset upgrades", 0, true, 0.0, true, 1.0)

	OnClientResetUpgradesForward = CreateForward(ET_Ignore, Param_Cell)
	RegisterCustomForward(OnClientResetUpgradesForward, "OnClientResetUpgrades")

	OnGetClientUpgradeInfoForward = CreateForward(ET_Ignore, Param_Cell, Param_String)
	RegisterCustomForward(OnGetClientUpgradeInfoForward, "OnGetUpgradeInfo")
}

static OnRunCmd(int client, int &buttons)
{
	static bool:reloading[MAXPLAYERS+1]
	
	if (buttons & IN_RELOAD)
	{
		if (!reloading[client])
		{
			_menuDisplayer.DisplayMenu(client, GetConVarBool(ResetUpgradesEnabledConVar))
			reloading[client] = true
		}
	}
	else
	{
	   reloading[client] = false;
	}
}

static void OnStartPublicMatch()
{
	ForEachClient(ClearClientUpgrades)
}

static OnStartMatch()
{
	ForEachClient(ClearClientUpgrades)
}

GetUpgradeName(upgradeIndex, String:destination[UPGRADE_NAME_LENGTH])
{
	strcopy(destination, UPGRADE_NAME_LENGTH, UpgradeInfo[upgradeIndex].SJUpgradeData_Name)
}

IncreasePlayerUpgradeLevel(int client, int upgradeId)
{
	new newLevel = PlayerUpgrades[client][upgradeId] + 1
	SetPlayerUpgradeLevel(client, upgradeId, newLevel)
	int price = UpgradeInfo[upgradeId].SJUpgradeData_Cost
	PlayerCredits[client] -= price
	new maxLevel = UpgradeInfo[upgradeId].SJUpgradeData_LevelsCount
	if (newLevel >= maxLevel)
	{
		PlaySoundByIdToClient(client, UpgradeMaxLevelSoundId)
	}
}

SetPlayerUpgradeLevel(client, int upgrade, level)
{
	PlayerUpgrades[client][upgrade] = level
	new Float:maxValue = UpgradeInfo[upgrade].SJUpgradeData_MaxValue
	new Float:minValue = UpgradeInfo[upgrade].SJUpgradeData_MinValue
	new levelsCount = UpgradeInfo[upgrade].SJUpgradeData_LevelsCount
	new Float:step = (maxValue - minValue) / levelsCount
	new Float:value = minValue + step * level
	PlayerUpgradeValue[client][upgrade] = value
	new Function:func = UpgradeInfo[upgrade].SJUpgradeData_Func
	
	if (func != INVALID_FUNCTION)
	{
		Call_StartFunction(INVALID_HANDLE, func)
		Call_PushCell(client)
		Call_PushFloat(value)
		Call_Finish()
	}
}

static void OnReadConfig(Handle:kv)
{
	EnabledUpgradesCount = 0
	if (KvGotoFirstSubKey(kv))
	{
		decl String:upgradeName[UPGRADE_NAME_LENGTH]
		do
		{
			KvGetSectionName(kv, upgradeName, sizeof(upgradeName))
			new upgrade = FindUpgradeByName(upgradeName)
			if (upgrade != INVALID_UPGRADE)
			{					
				UpgradeInfo[upgrade].SJUpgradeData_MinValue = KvGetFloat(kv, "min_value", UpgradeInfo[upgrade].SJUpgradeData_MinValue)
				UpgradeInfo[upgrade].SJUpgradeData_MaxValue = KvGetFloat(kv, "max_value", UpgradeInfo[upgrade].SJUpgradeData_MaxValue)
				UpgradeInfo[upgrade].SJUpgradeData_LevelsCount = KvGetNum(kv, "levels_count", UpgradeInfo[upgrade].SJUpgradeData_LevelsCount)
				UpgradeInfo[upgrade].SJUpgradeData_Cost = KvGetNum(kv, "cost", UpgradeInfo[upgrade].SJUpgradeData_Cost)
				new isEnabled = KvGetNum(kv, "enabled", 1)
				if (isEnabled)
				{
					EnabledUpgrades[EnabledUpgradesCount] = upgrade
					EnabledUpgradesCount++
				}
			}
		}
		while (KvGotoNextKey(kv))
	}
}

FindUpgradeByName(const String:name[])
{
	for (new upgrade = 0; upgrade < UpgradesCount; upgrade++)
	{
		if (StrEqual(name, UpgradeInfo[upgrade].SJUpgradeData_Name, false))
		{
			return upgrade
		}
	}
	return INVALID_UPGRADE
}

bool:IsPlayerCanBuyUpgrade(client, upgrade)
{
	return PlayerCredits[client] >= UpgradeInfo[upgrade].SJUpgradeData_Cost
}

Float:GetPlayerUpgradeValue(client, upgrade)
{
	return Float:PlayerUpgradeValue[client][upgrade]
}

static FireOnClientResetUpgrades(client)
{
	Call_StartForward(OnClientResetUpgradesForward)
	Call_PushCell(client)
	Call_Finish()
}

stock FireOnGetClientUpgradeInfo(client, String:info[512])
{
	Call_StartForward(OnGetClientUpgradeInfoForward)
	Call_PushCell(client)
	Call_PushStringEx(info, sizeof(info), SM_PARAM_STRING_COPY, SM_PARAM_COPYBACK)
	Call_Finish()
}