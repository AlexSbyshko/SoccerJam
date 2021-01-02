#if defined _Upgrade_included
 #endinput
#endif
#define _Upgrade_included

const UPGRADE_MEAS_NAME_LENGTH = 8
const UPGRADE_NAME_LENGTH = 32

enum struct SJUpgradeData
{
	char SJUpgradeData_Name[UPGRADE_NAME_LENGTH]
	float SJUpgradeData_MinValue
	float SJUpgradeData_MaxValue
	int SJUpgradeData_LevelsCount
	int SJUpgradeData_Cost
	UpgradeFunc SJUpgradeData_Func
	char SJUpgradeData_MeasName[UPGRADE_MEAS_NAME_LENGTH]
}