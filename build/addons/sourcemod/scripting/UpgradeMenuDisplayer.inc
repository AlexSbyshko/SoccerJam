typedef DisplayMenuDel = function void (int client, bool isResetEnabled)

enum struct UpgradeMenuDisplayer
{
	DisplayMenuDel DisplayMenuFunc

	void DisplayMenu(int client, bool isResetEnabled)
	{
		Call_StartFunction(INVALID_HANDLE, this.DisplayMenuFunc)
		Call_PushCell(client)
		Call_Finish()
	}
}

void CreateMenuDisplayer(UpgradeMenuDisplayer displayer)
{
    displayer.DisplayMenuFunc = ShowUpgradeMenu
}

void ShowUpgradeMenu(int client, bool isResetEnabled)
{
	if (StrEqual(GameFolderName, "csgo", true)
		&& GameRules_GetProp("m_bWarmupPeriod") == 1
		&& !GetConVarBool(WarmupUpgradesEnabledConVar)) 
    {
    	return
    }

	new Handle:menu = CreateMenu(Upgrade_Handler)
	decl String:line[512]
	new currentCredits = PlayerCredits[client]
	Format(line, sizeof(line), "%T\nCredits: %i", "MENU_TITLE", client, currentCredits)
	SetMenuTitle(menu, line)
	new style, level, price, maxLevel, value, upgrade
	for (new i = 0; i < EnabledUpgradesCount; i++)
	{
		DataPack data = new DataPack()
		data.WriteCell(0)

		upgrade = EnabledUpgrades[i]
		data.WriteCell(upgrade)

		decl String:upgradeName[UPGRADE_NAME_LENGTH]
		value = RoundFloat(PlayerUpgradeValue[client][upgrade])
		GetUpgradeName(upgrade, upgradeName)
		style = ITEMDRAW_DEFAULT
		level = PlayerUpgrades[client][upgrade]

		maxLevel = UpgradeInfo[upgrade].SJUpgradeData_LevelsCount
		if (level < maxLevel
			&& IsPlayerCanBuyUpgrade(client, upgrade))
		{
			Format(line, sizeof(line), "%T: %i %s (lvl %i/%i) cost: %i", upgradeName, client, value, UpgradeInfo[upgrade].SJUpgradeData_MeasName, level, maxLevel, price)
		}
		else
		{
			style |= ITEMDRAW_DISABLED
			Format(line, sizeof(line), "%T: %i %s (lvl %i/%i)", upgradeName, client, value, UpgradeInfo[upgrade].SJUpgradeData_MeasName, level, maxLevel)
		}
		style = (currentCredits > 0) ? style : ITEMDRAW_DISABLED

		char info[64]
		Format(info, sizeof(info), "%d", view_as<int>(data));
		AddMenuItem(menu, info, line, style)
	}
	if (isResetEnabled)
	{
		DataPack data = new DataPack()
		data.WriteCell(1)

		Format(line, sizeof(line), "%T", "RESET_ALL", client)

		char info[64]
		Format(info, sizeof(info), "%d", view_as<int>(data));
		AddMenuItem(menu, info, line)
	}
	SetMenuExitButton(menu, true)

	DisplayMenu(menu, client, 20)
}

static Upgrade_Handler(Handle:menu, MenuAction:action, client, choice) 
{
	if (action == MenuAction_End)
	{
		int itemsCount = GetMenuItemCount(menu)
		for (int i = 0; i < itemsCount; i++)
		{
			decl String:info[16]
			GetMenuItem(menu, i, info, sizeof(info))
			DataPack data = view_as<DataPack>(StringToInt(info))
			delete data
		}

		CloseHandle(menu);
	}
	else if (action == MenuAction_Select)
	{
		decl String:info[16]
		GetMenuItem(menu, choice, info, sizeof(info))
		DataPack data = view_as<DataPack>(StringToInt(info))
		data.Reset()

		int upgradeAction = data.ReadCell()
		if (upgradeAction == 0)
		{
			int upgradeId = data.ReadCell()
			IncreasePlayerUpgradeLevel(client, upgradeId)
		}
		else if (upgradeAction == 1)
		{
			ClearClientUpgrades(client)	
		}
	}
}