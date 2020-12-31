typedef DisplayMenuDel = function void (int client)
const MAX_DISPLAYERS = 4;

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


static UpgradeMenuDisplayer _displayers[MAX_DISPLAYERS]
static int Count

void CreateMenuDisplayer(DisplayMenuDel displayMenuFunc, UpgradeMenuDisplayer displayer = 0)
{
    int id = Count++
    _displayers[id].DisplayMenuFunc = displayMenuFunc
    displayer = _displayers[id]
}