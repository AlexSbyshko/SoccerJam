#if defined _UpgradeMenuDisplayer_included
 #endinput
#endif
#define _UpgradeMenuDisplayer_included

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