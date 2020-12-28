#define CURVE_ANGLE 15
#define CURVE_COUNT 6
#define CURVE_TIME 0.2
#define DIRECTIONS 2
#define	ANGLEDIVIDE 6
static g_BallDirection
static Float:g_BallSpinDirection[3]
static g_CurveCount
//functag public CurveBallFunc()
typedef CurveBallFunc = function void ()

public CB_Init()
{
	RegConsoleCmd("sj_curveleft", CMD_CurveLeft)
	RegConsoleCmd("sj_curveright", CMD_CurveRight)
}

public CB_OnPlayerRunCmd(client, &buttons)
{
	static bool:curveleft[MAXPLAYERS+1]
	static bool:curveright[MAXPLAYERS+1]

	if (client == g_BallHolder)
	{
		if ((buttons & IN_SPEED) && (buttons & IN_MOVERIGHT))
		{
			if (!curveright[client])
			{
				CurveBallRight(client)
				curveright[client] = true
			}
		}
		else
		{
		   curveright[client] = false;
		}
		if ((buttons & IN_SPEED) && (buttons & IN_MOVELEFT))
		{
			if (!curveleft[client])
			{			
				CurveBallLeft(client)
				curveleft[client] = true
			}
		}
		else
		{
		   curveleft[client] = false;
		}
	}

	if (buttons & IN_SPEED)
	{
		buttons &= ~IN_SPEED
	}
}

public CB_OnBallReceived(ballHolder, oldBallOwner)
{
	g_BallDirection = 0
}

public CB_OnBallKicked(client)
{
	if (g_BallDirection)
	{
		decl Float:clientEyeAngles[3]
		GetAngleVectors(clientEyeAngles, g_BallSpinDirection, NULL_VECTOR, NULL_VECTOR);
		g_CurveCount = CURVE_COUNT;
	}
	CreateTimer(CURVE_TIME * 2, Timer_CurveBall)
}

public Action:Timer_CurveBall(Handle:timer) 
{
	if(g_BallDirection
		&& g_CurveCount > 0) 
	{
		new Float:dAmt = float((g_BallDirection * CURVE_ANGLE) / ANGLEDIVIDE);
		decl Float:v[3];
		decl Float:v_forward[3];
		Entity_GetLocalVelocity(g_Ball, v);
		GetVectorAngles(v, g_BallSpinDirection);

		g_BallSpinDirection[1] = g_BallSpinDirection[1] + dAmt;
		g_BallSpinDirection[2] = 0.0;
		
		GetAngleVectors(g_BallSpinDirection, v_forward, NULL_VECTOR, NULL_VECTOR);
		
		new Float:speed = GetVectorLength(v);
		v[0] = v_forward[0] * speed;
		v[1] = v_forward[1] * speed;
		
		TeleportEntity(g_Ball, NULL_VECTOR, NULL_VECTOR, v);

		g_CurveCount--;
		CreateTimer(CURVE_TIME, Timer_CurveBall);		
	}
	return Plugin_Continue;
}

public Action:CMD_CurveLeft(client, argc)
{
	CurveBallLeft(client)
	return Plugin_Handled
}

public Action:CMD_CurveRight(client, argc)
{
	CurveBallRight(client)
	return Plugin_Handled
}

CurveBallLeft(client)
{
	CurveBall(client, CurveBallLeftFunc);
}

CurveBallRight(client)
{
	CurveBall(client, CurveBallRigthFunc);
}

CurveBall(client, CurveBallFunc:curveFunc)
{
	CallCurveFunc(curveFunc)
	SendCurveString(client)
}

CallCurveFunc(CurveBallFunc:curveFunc)
{
	Call_StartFunction(INVALID_HANDLE, curveFunc);
	Call_Finish();
}

public CurveBallLeftFunc()
{
	g_BallDirection++;
	if (g_BallDirection > DIRECTIONS)
	{
		g_BallDirection = DIRECTIONS;
	}
}

public CurveBallRigthFunc()
{
	g_BallDirection--;
	if (g_BallDirection < -(DIRECTIONS))
	{
		g_BallDirection = -(DIRECTIONS);
	}
}

SendCurveString(client)
{
	decl String:curveString[8];
	GetBallCurveString(curveString, sizeof(curveString));
	PrintCenterText(client, curveString);
}

GetBallCurveString(String:string[], length)
{
	switch (g_BallDirection)
	{
		case -2:
		{
			return strcopy(string, length, "----O\n");
		}
		case -1:
		{
			return strcopy(string, length, "---O-\n");
		}
		case 0:
		{
			return strcopy(string, length, "--O--\n");
		}
		case 1:
		{
			return strcopy(string, length, "-O---\n");
		}
		case 2:
		{
			return strcopy(string, length, "O----\n");
		}
	}
	return 0;
}