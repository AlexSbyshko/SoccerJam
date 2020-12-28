#define FFADE_OUT			0x0002
#define	FFADE_PURGE		0x0010  


stock Entity_GetMaxSize(entity, Float:vec[3])
{
	GetEntPropVector(entity, Prop_Send, "m_vecMaxs", vec);
}

stock Array_FindHighestValue(any:array[], size, start=0)
{
	if (start < 0) {
		start = 0;
	}

	new any:value = array[start];
	new any:tempValue;
	new x = start;
	
	for (new i=start; i < size; i++) {
		
		tempValue = array[i];
		
		if (tempValue > value) {
			value = tempValue;
			x = i;
		}
		
	}
	
	return x;
}

FadeClient(client, duration, const color[4]) 
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

ClearTimer(&Handle:timer)
{
	if (timer != INVALID_HANDLE)
	{
		KillTimer(timer);
	}
	timer = INVALID_HANDLE;
}

Float:GetEntityRadius(entity)
{
	decl Float:vecMax[3];
	Entity_GetMaxSize(entity, vecMax);
	new highestSizeIndex = Array_FindHighestValue(vecMax, sizeof(vecMax));
	return vecMax[highestSizeIndex];
}



