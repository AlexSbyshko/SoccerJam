typedef DisplayMenuDel = function void (int client)

enum struct UpgradeMenuDisplayer
{
	DisplayMenuDel DisplayMenuFunc

	void DisplayMenu(int client)
	{
		Call_StartFunction(INVALID_HANDLE, this.DisplayMenuFunc)
		Call_PushCell(client)
		Call_Finish()
	}
}

void CreateMenuDisplayer(UpgradeMenuDisplayer displayer, DisplayMenuDel displayMenuFunc)
{
    displayer.DisplayMenuFunc = displayMenuFunc
}