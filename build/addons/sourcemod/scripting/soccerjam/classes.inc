static Handle:ClassesArray

enum SJClassData
{
	String:SJClassData_Name[MAX_NAME_LENGTH],
	SJClassData_Limit,
	SJClassData_FixedUpgradesCount,
}

static const CLASS_DATA_SIZE = _:SJClassData

InitClasses()
{
	ClassesArray = CreateArray(_:SJClassData)
	CreateClass("Test class", 1)
}

CreateClass(const String:name[], limit)
{
	new class[CLASS_DATA_SIZE]
	strcopy(class[SJClassData_Name], MAX_NAME_LENGTH, name)
	class[SJClassData_Limit] = limit
	PushArrayArray(ClassesArray, class, CLASS_DATA_SIZE)
}

LoadClasses()
{
	
}

/*ShowClassMenu(client)
{
	new Handle:menu = CreateMenu(ClassMenu_Handler)
	SetMenuTitle(menu, "Choose Class")
	new classesCount = GetArraySize(ClassesArray)
	for (new i = 0; i < classesCount; i++)
	{
		new class[CLASS_DATA_SIZE]
		GetArrayArray(ClassesArray, i, class, CLASS_DATA_SIZE)
		AddMenuItem(menu, class[SJClassData_Name], class[SJClassData_Name])
	}
	SetMenuExitButton(menu, true)
	DisplayMenu(menu, client, 20)
}*/

public ClassMenu_Handler(Handle:menu, MenuAction:action, client, choice) 
{
	if (action == MenuAction_End)
	{
		CloseHandle(menu);
	}
	else if (action == MenuAction_Select)
	{
		/*decl String:info[32]
		GetMenuItem(menu, choice, info, sizeof(info))
		if (StrEqual(info, "ball_spawn"))
		{
			BuildBallSpawnByClient(client)
		}
		else if (StrEqual(info, "goal_override_red"))
		{
			BuildGoalOverrideByClient(CS_TEAM_T, client)
		}
		else if (StrEqual(info, "goal_override_blue"))
		{
			BuildGoalOverrideByClient(CS_TEAM_CT, client)
		}
		ShowSjBuildMenu(client)*/
	}
}