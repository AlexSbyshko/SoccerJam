#define FFADE_OUT			0x0002
#define	FFADE_PURGE		0x0010  

stock FadeClient(client, duration, const color[4]) 
{
	new Handle:hFadeClient = StartMessageOne("Fade",client)
	if (GetUserMessageType() == UM_Protobuf)
	{
		PbSetInt(hFadeClient, "duration", duration)
		PbSetInt(hFadeClient, "hold_time", 0)
		PbSetInt(hFadeClient, "flags", (FFADE_PURGE|FFADE_OUT))
		PbSetColor(hFadeClient, "clr", color)
	}
	else
	{		
		BfWriteShort(hFadeClient, duration)
		BfWriteShort(hFadeClient, 0)
		BfWriteShort(hFadeClient, (FFADE_PURGE|FFADE_OUT))
		BfWriteByte(hFadeClient, color[0])
		BfWriteByte(hFadeClient, color[1])
		BfWriteByte(hFadeClient, color[2])
		BfWriteByte(hFadeClient, 128)		
	}
	EndMessage()
}

bool:IsEntitySolidForClients(entity)
{
	decl String:classname[32];
	GetEdictClassname(entity, classname, sizeof(classname));	
	return StrContains(classname, "trigger_", false) < 0 
		&& StrContains(classname, "func_", false) < 0;
}

ClearTimer(&Handle:timer)
{
	if (timer != INVALID_HANDLE)
	{
		KillTimer(timer);
	}
	timer = INVALID_HANDLE;
}

stock SetEntitySolidForClients(entity)
{
	SetEntData(entity, g_CollisionOffset, 5, 1, true);
}

stock SetEntityNotSolidForClients(entity)
{
	SetEntData(entity, g_CollisionOffset, 2, 1, true);
}

stock Float:GetEntityRadius(entity)
{
	decl Float:vecMax[3];
	Entity_GetMaxSize(entity, vecMax);
	new highestSizeIndex = Array_FindHighestValue(vecMax, sizeof(vecMax));
	return vecMax[highestSizeIndex];
}

stock SendTopMessageToAll(String:text[], any:...)
{
	decl String:message[100];
	VFormat(message, sizeof(message), text, 2);
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			SendTopMessage(i, message);			
		}
	}	
}