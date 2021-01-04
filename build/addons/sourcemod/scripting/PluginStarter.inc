#if defined _PluginStarter_included
 #endinput
#endif
#define _PluginStarter_included

typedef StartPluginDel = function void ()

enum struct PluginStarter
{
    StartPluginDel StartPluginFunc

    void StartPlugin()
    {
        Call_StartFunction(INVALID_HANDLE, this.StartPluginFunc)
        Call_Finish()
    }
}

void CreateDefaultPluginStarter(PluginStarter pluginStarter)
{
    pluginStarter.StartPluginFunc = EmptyStatPlugin
}

static void EmptyStatPlugin()
{
}