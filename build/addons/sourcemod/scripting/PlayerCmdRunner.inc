#if defined _PlayerCmdRunner_included
 #endinput
#endif
#define _PlayerCmdRunner_included

typedef RunPlayerCmdDel = function void (int client, int &buttons)

enum struct PlayerCmdRunner
{
    RunPlayerCmdDel RunPlayerCmdFunc

    void RunPlayerCmd(int client, int &buttons)
    {
        Call_StartFunction(INVALID_HANDLE, this.RunPlayerCmdFunc)
        Call_PushCell(client)
        Call_PushCellRef(buttons)
        Call_Finish()
    }
}

CreateDefaultPlayerCmdRunner(PlayerCmdRunner playerCmdRunner)
{
    playerCmdRunner.RunPlayerCmdFunc = EmptyRunPlayerCmd
}

static void EmptyRunPlayerCmd(int client, int &buttons)
{
}