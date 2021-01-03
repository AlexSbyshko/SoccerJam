#include "UpgradeMenuDisplayer"

static UpgradeMenuDisplayer _decoratee

void CreateWmCheckMenuDisplayer(UpgradeMenuDisplayer displayer, UpgradeMenuDisplayer decoratee)
{
    _decoratee = decoratee    
    displayer.DisplayMenuFunc = ShowUpgradeMenu
}

static void ShowUpgradeMenu(int client, bool isResetEnabled)
{
    if (GameRules_GetProp("m_bWarmupPeriod") == 1
      && !GetConVarBool(WarmupUpgradesEnabledConVar)) 
    {
      return
    }

    _decoratee.DisplayMenu(client, isResetEnabled)
}