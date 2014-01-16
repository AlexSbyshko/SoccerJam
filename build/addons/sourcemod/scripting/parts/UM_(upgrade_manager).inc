static Handle:InitialCreditsConVar
static Handle:ResetUpgradesEnabledConVar
static Handle:WarmupUpgradesEnabledConVar

const MAX_UPGRADES = 128
const UPGRADE_NAME_LENGTH = 32
const UPGRADE_MEAS_NAME_LENGTH = 8
const INVALID_UPGRADE = -1

new CatchingUpgradeId
static UpgradeMaxLevelSoundId


enum SJUpgradeData
{
	String:SJUpgradeData_Name[UPGRADE_NAME_LENGTH],
	Float:SJUpgradeData_MinValue,
	Float:SJUpgradeData_MaxValue,
	SJUpgradeData_LevelsCount,
	SJUpgradeData_Cost,
	UpgradeFunc:SJUpgradeData_Func,
	String:SJUpgradeData_MeasName[UPGRADE_MEAS_NAME_LENGTH],
}

static UpgradesCount
static PlayerUpgrades[MAXPLAYERS+1][MAX_UPGRADES]
static EnabledUpgrades[MAX_UPGRADES]
static EnabledUpgradesCount
static Float:PlayerUpgradeValue[MAXPLAYERS+1][MAX_UPGRADES]
static UpgradeInfo[MAX_UPGRADES][SJUpgradeData]
static PlayerCredits[MAXPLAYERS+1]

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
	strcopy(UpgradeInfo[UpgradesCount][SJUpgradeData_Name], UPGRADE_NAME_LENGTH, name)
	UpgradeInfo[UpgradesCount][SJUpgradeData_MinValue] = minValue
	UpgradeInfo[UpgradesCount][SJUpgradeData_MaxValue] = maxValue
	UpgradeInfo[UpgradesCount][SJUpgradeData_LevelsCount] = levelsCount
	UpgradeInfo[UpgradesCount][SJUpgradeData_Cost] = cost
	UpgradeInfo[UpgradesCount][SJUpgradeData_Func] = funcion
	strcopy(UpgradeInfo[UpgradesCount][SJUpgradeData_MeasName], UPGRADE_MEAS_NAME_LENGTH, measureName)
	return UpgradesCount++
}

public ClearClientUpgrades(client)
{
	for (new upgrade = 0; upgrade < UpgradesCount; upgrade++)
	{
		SetPlayerUpgradeLevel(client, SJUpgrade:upgrade, 0)
	}
	PlayerCredits[client] = GetConVarInt(InitialCreditsConVar)
}

////////////

public UM_Init()
{
	CreateConfig("upgrades.cfg", "upgrades", UM_ReadConfig)
	CatchingUpgradeId = CreateUpgrade("dexterity", 20.0, 90.0, 5, 1, "%")
	UpgradeMaxLevelSoundId = CreateSound("upgrade_maxlevel")

	InitialCreditsConVar = CreateConVar("sj_initial_credits", "6", "Start credits for upgrades", 0, true, 0.0)
	ResetUpgradesEnabledConVar = CreateConVar("sj_reset_upgrades_enabled", "0", "Enable reset upgrades", 0, true, 0.0, true, 1.0)
	WarmupUpgradesEnabledConVar = CreateConVar("sj_warmup_upgrades_enabled", "0", "Enable upgrades during Warmup", 0, true, 0.0, true, 1.0)
}

public UM_OnPlayerRunCmd(client, &buttons)
{
	static bool:reloading[MAXPLAYERS+1]
	
	if (buttons & IN_RELOAD)
	{
		if (!reloading[client])
		{
			ShowUpgradeMenu(client)
			reloading[client] = true
		}
	}
	else
	{
	   reloading[client] = false;
	}
}

public UM_OnStartPublic()
{
	ForEachClient(ClearClientUpgrades)
}

public UM_OnStartMatch()
{
	ForEachClient(ClearClientUpgrades)
}

GetUpgradeName(upgradeIndex, String:destination[UPGRADE_NAME_LENGTH])
{
	strcopy(destination, UPGRADE_NAME_LENGTH, UpgradeInfo[upgradeIndex][SJUpgradeData_Name])
}

IncreasePlayerUpgradeLevel(client, SJUpgrade:upgrade)
{
	new newLevel = PlayerUpgrades[client][upgrade] + 1
	SetPlayerUpgradeLevel(client, upgrade, newLevel)
}

SetPlayerUpgradeLevel(client, SJUpgrade:upgrade, level)
{
	PlayerUpgrades[client][upgrade] = level
	new Float:maxValue = UpgradeInfo[upgrade][SJUpgradeData_MaxValue]
	new Float:minValue = UpgradeInfo[upgrade][SJUpgradeData_MinValue]
	new levelsCount = UpgradeInfo[upgrade][SJUpgradeData_LevelsCount]
	new Float:step = (maxValue - minValue) / levelsCount
	new Float:value = minValue + step * level
	PlayerUpgradeValue[client][upgrade] = value
	new Function:function = UpgradeInfo[upgrade][SJUpgradeData_Func]
	
	if (function != INVALID_FUNCTION)
	{
		Call_StartFunction(INVALID_HANDLE, function)
		Call_PushCell(client)
		Call_PushFloat(value)
		Call_Finish()
	}
}

GetUpgradeMaxLevel(upgradeIndex)
{
	return UpgradeInfo[upgradeIndex][SJUpgradeData_LevelsCount]
}

public UM_ReadConfig(Handle:kv)
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
				UpgradeInfo[upgrade][SJUpgradeData_MinValue] = KvGetFloat(kv, "min_value", UpgradeInfo[upgrade][SJUpgradeData_MinValue])
				UpgradeInfo[upgrade][SJUpgradeData_MaxValue] = KvGetFloat(kv, "max_value", UpgradeInfo[upgrade][SJUpgradeData_MaxValue])
				UpgradeInfo[upgrade][SJUpgradeData_LevelsCount] = KvGetNum(kv, "levels_count", UpgradeInfo[upgrade][SJUpgradeData_LevelsCount])
				UpgradeInfo[upgrade][SJUpgradeData_Cost] = KvGetNum(kv, "cost", UpgradeInfo[upgrade][SJUpgradeData_Cost])
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
		if (StrEqual(name, UpgradeInfo[upgrade][SJUpgradeData_Name], false))
		{
			return upgrade
		}
	}
	return INVALID_UPGRADE
}

ShowUpgradeMenu(client)
{
	if (GameRules_GetProp("m_bWarmupPeriod") == 1 && !GetConVarBool(WarmupUpgradesEnabledConVar)) 
    {
    	return
    }

	new Handle:menu = CreateMenu(Upgrade_Handler)
	decl String:line[128]
	new currentCredits = PlayerCredits[client]
	Format(line, sizeof(line), "%T\nCredits: %i\n", "MENU_TITLE", client, currentCredits)
	SetMenuTitle(menu, line)
	new style, level, price, maxLevel, value, upgrade
	for (new i = 0; i < EnabledUpgradesCount; i++)
	{
		upgrade = EnabledUpgrades[i]
		decl String:upgradeName[UPGRADE_NAME_LENGTH]
		value = RoundFloat(PlayerUpgradeValue[client][upgrade])
		GetUpgradeName(upgrade, upgradeName)
		style = ITEMDRAW_DEFAULT
		level = PlayerUpgrades[client][upgrade]
		maxLevel = GetUpgradeMaxLevel(upgrade)
		if (level < maxLevel
			&& IsPlayerCanBuyUpgrade(client, upgrade))
		{
			price = UpgradeInfo[upgrade][SJUpgradeData_Cost]
			Format(line, sizeof(line), "%T: %i %s (lvl %i/%i) cost: %i", upgradeName, client, value, UpgradeInfo[upgrade][SJUpgradeData_MeasName], level, maxLevel, price)
		}
		else
		{
			style |= ITEMDRAW_DISABLED
			Format(line, sizeof(line), "%T: %i %s (lvl %i/%i)", upgradeName, client, value, UpgradeInfo[upgrade][SJUpgradeData_MeasName], level, maxLevel)
		}
		style = (currentCredits > 0) ? style : ITEMDRAW_DISABLED
		AddMenuItem(menu, upgradeName, line, style)
	}
	if (GetConVarBool(ResetUpgradesEnabledConVar))
	{
		Format(line, sizeof(line), "%T", "RESET_ALL", client)
		AddMenuItem(menu, "reset", line)
	}
	SetMenuExitButton(menu, true)
	DisplayMenu(menu, client, 20)
}

public Upgrade_Handler(Handle:menu, MenuAction:action, client, choice) 
{
	if (action == MenuAction_End)
	{
		CloseHandle(menu);
	}
	else if (action == MenuAction_Select)
	{
		if (choice < EnabledUpgradesCount)
		{
			new upgrade = EnabledUpgrades[choice]
			decl String:upgradeName[UPGRADE_NAME_LENGTH]
			GetUpgradeName(upgrade, upgradeName)
			decl String:upgradeTitle[64];
			Format(upgradeTitle, sizeof(upgradeTitle), "%T", upgradeName, client);
			new currentLevel = PlayerUpgrades[client][upgrade];		
			new price = UpgradeInfo[upgrade][SJUpgradeData_Cost]			
			PlayerCredits[client] -= price;
			IncreasePlayerUpgradeLevel(client, SJUpgrade:upgrade)
			new maxLevel = GetUpgradeMaxLevel(upgrade)
			if (currentLevel >= maxLevel - 1)
			{
				PlaySoundByIdToClient(client, UpgradeMaxLevelSoundId)
			}				
			if (PlayerCredits[client] > 0)
			{
				ShowUpgradeMenu(client);
			}
		}
		else
		{
			decl String:info[32]
			GetMenuItem(menu, choice, info, sizeof(info))
			if (StrEqual(info, "reset"))
			{
				ClearClientUpgrades(client)				
				ShowUpgradeMenu(client)		
			}
		}
	}
}

bool:IsPlayerCanBuyUpgrade(client, upgrade)
{
	return PlayerCredits[client] >= UpgradeInfo[upgrade][SJUpgradeData_Cost]
}

Float:GetPlayerCatchingPercent(client)
{
	return GetPlayerUpgradeValue(client, CatchingUpgradeId)
}

Float:GetPlayerUpgradeValue(client, upgrade)
{
	return Float:PlayerUpgradeValue[client][upgrade]
}