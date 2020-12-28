#include "soccerjam/tools/cycles"
#include "soccerjam/tools/func_caller"

#define MAX_WEAPONS				48	// Max number of weapons availabl

stock Client_RemoveAllWeapons(client, const String:exclude[]="", bool:clearAmmo=false)
{
	new offset = Client_GetWeaponsOffset(client) - 4;
	
	new numWeaponsRemoved = 0;
	for (new i=0; i < MAX_WEAPONS; i++) {
		offset += 4;

		new weapon = GetEntDataEnt2(client, offset);
		
		if (!Weapon_IsValid(weapon)) {
			continue;
		}
		
		if (exclude[0] != '\0' && Entity_ClassNameMatches(weapon, exclude)) {
			Client_SetActiveWeapon(client, weapon);
			continue;
		}
		
		if (clearAmmo) {
			Client_SetWeaponPlayerAmmoEx(client, weapon, 0, 0);
		}

		if (RemovePlayerItem(client, weapon)) {
			Entity_Kill(weapon);
		}

		numWeaponsRemoved++;
	}
	
	return numWeaponsRemoved;
}

stock Client_GetWeaponsOffset(client)
{
	static offset = -1;

	if (offset == -1) {
		offset = FindDataMapInfo(client, "m_hMyWeapons");
	}
	
	return offset;
}

stock Weapon_IsValid(weapon)
{
	if (!IsValidEdict(weapon)) {
		return false;
	}

	return Entity_ClassNameMatches(weapon, "weapon_", true);
}

stock bool:Entity_ClassNameMatches(entity, const String:className[], partialMatch=false)
{
	decl String:entity_className[64];
	Entity_GetClassName(entity, entity_className, sizeof(entity_className));

	if (partialMatch) {
		return (StrContains(entity_className, className) != -1);
	}
	
	return StrEqual(entity_className, className);
}

stock Client_SetActiveWeapon(client, weapon)
{
	SetEntPropEnt(client, Prop_Data, "m_hActiveWeapon", weapon);
	ChangeEdictState(client, FindDataMapInfo(client, "m_hActiveWeapon"));
}

stock Weapon_GetPrimaryAmmoType(weapon)
{
	return GetEntProp(weapon, Prop_Data, "m_iPrimaryAmmoType");
}

stock Weapon_GetSecondaryAmmoType(weapon)
{
	return GetEntProp(weapon, Prop_Data, "m_iSecondaryAmmoType");
}

stock bool:Entity_IsPlayer(entity)
{
	if (entity < 1 || entity > MaxClients) {
		return false;
	}
	
	return true;
}

stock Client_SetWeaponPlayerAmmoEx(client, weapon, primaryAmmo=-1, secondaryAmmo=-1)
{
	new offset_ammo = FindDataMapInfo(client, "m_iAmmo");

	if (primaryAmmo != -1) {
		new offset = offset_ammo + (Weapon_GetPrimaryAmmoType(weapon) * 4);
		SetEntData(client, offset, primaryAmmo, 4, true);
	}

	if (secondaryAmmo != -1) {
		new offset = offset_ammo + (Weapon_GetSecondaryAmmoType(weapon) * 4);
		SetEntData(client, offset, secondaryAmmo, 4, true);
	}
}

stock bool:Entity_Kill(kenny)
{
	if (Entity_IsPlayer(kenny)) {
		// Oh My God! They Killed Kenny!!
		ForcePlayerSuicide(kenny);
		return true;
	}
	
	return AcceptEntityInput(kenny, "kill");
}

stock Entity_GetClassName(entity, String:buffer[], size)
{
	GetEntPropString(entity, Prop_Data, "m_iClassname", buffer, size);
	
	if (buffer[0] == '\0') {
		return false;
	}
	
	return true;
}


GivePlayerWeapon(client)
{
	Client_RemoveAllWeapons(client);
	SetEntProp(client, Prop_Send, "m_ArmorValue", 0, 1);
	GivePlayerItem(client, "weapon_knife");
}

PrintSJMessageAll(String:text[], any:...)
{
	decl String:message[100];
	VFormat(message, sizeof(message), text, 2);
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			PrintSJMessage(i, message);
		}
	}
}

PrintSJMessage(client, String:text[], any:...)
{
	decl String:message[100];
	VFormat(message, sizeof(message), text, 3);
	PrintToChat(client, "\x01\x0B\x04[\x03SJ\x04] \x03%s", message);
}

GetClientOpponentSJTeam(client)
{
	new team = GetClientTeam(client);
	switch (team)
	{
		case CS_TEAM_CT:
		{
			return CS_TEAM_T;
		}
		case CS_TEAM_T:
		{
			return CS_TEAM_CT;
		}
	}
	return CS_TEAM_NONE;
}

SJ_IncreaseTeamScore(team)
{
	TeamScore[team]++;
	SJ_UpdateTeamScores();
}

SJ_UpdateTeamScores()
{
	CS_SetTeamScore(CS_TEAM_T, TeamScore[CS_TEAM_T]);
	SetTeamScore(CS_TEAM_T, TeamScore[CS_TEAM_T]);
	CS_SetTeamScore(CS_TEAM_CT, TeamScore[CS_TEAM_CT]);
	SetTeamScore(CS_TEAM_CT, TeamScore[CS_TEAM_CT]);
}

SJ_ResetTeamScores()
{
	TeamScore[CS_TEAM_T] = 0;
	TeamScore[CS_TEAM_CT] = 0;
	SJ_UpdateTeamScores();
}

MapFunctionPrefix(&Function:func, const String:prefix[], const String:functionName[])
{
	decl String:realFunctionName[MAX_FUNCTION_NAME_LENGTH]
	Format(realFunctionName, sizeof(realFunctionName), "%s_%s", prefix, functionName)
	func = GetFunctionByName(INVALID_HANDLE, realFunctionName)
}

bool:GetChance(Float:percent)
{
	new Float:random = GetRandomFloat(1.0, 100.0);
	return percent >= random;
}