#if defined _module_included
 #endinput
#endif
#define _module_included

typedef InitFuncDel = function void ()
typedef OnPlayerRunCmdDel = function void (int client, int& buttons)

enum struct Module
{
    InitFuncDel InitFunc
    OnPlayerRunCmdDel OnPlayerRunCmdFunc
}