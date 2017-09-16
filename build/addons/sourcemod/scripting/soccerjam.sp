#include <sourcemod>
#include <sdkhooks>
#include <sdktools>
#include <cstrike>
#include <smlib>

#undef REQUIRE_PLUGIN
#include <adminmenu>
#define REQUIRE_PLUGIN

#define SOCCERJAMSOURCE_VERSION	 "2.1.3"
#define SOCCERJAMSOURCE_URL "http://steamcommunity.com/groups/sj-source"

functag public UpgradeFunc(client, Float:upgradeValue)

#include "soccerjam/constants"
#include "soccerjam/globalvars"
#include "soccerjam/enginetools"
#include "soccerjam/sjtools"
#include "soccerjam/clients"

#include "parts"
#include "parts/BALL_(ball)"
#include "parts/BAR_(ball_autorespawn)"
#include "parts/BBM_(ball_bounce_multiplier)"
#include "parts/BBS_(ball_bounce_sound)"
#include "parts/BE_(ball_explosion)"
#include "parts/BK_(ball_kick)"
#include "parts/BR_(ball_receiving)"
#include "parts/BT_(ball_trail)"
#include "parts/CB_(curve_ball)"
#include "parts/CM_(config_manager)"
#include "parts/DSRM_(disarm)"
#include "parts/DZ_(death_zone)"
#include "parts/FFI_(frags_for_interception)"
#include "parts/GA_(goal_assist)"
#include "parts/GD_(goal_distance)"
#include "parts/GOAL_(goal)"
#include "parts/GSM_(game_specific_manager)"
#include "parts/HLF_(half)"
#include "parts/HLP_(help)"
#include "parts/HLTH_(health)"
#include "parts/KAS_(ka_soccer_maps_support)"
#include "parts/LJ_(long_jump)"
#include "parts/MM_(model_manager)"
#include "parts/MSM_(match_stats_manager)"
#include "parts/MTCH_(match)"
#include "parts/MVP_(mvp_stars)"
#include "parts/NDAG_(no_damage_after_goal)"
#include "parts/NFFK_(no_frags_for_kill)"
#include "parts/NOFF_(no_friendly_fire)"
#include "parts/NRD_(no_round_draw)"
#include "parts/NREM_(no_round_end_message)"
#include "parts/PAC_(player_attack_check)"
#include "parts/PR_(player_respawn)"
#include "parts/RBAH_(remove_bomb_and_hostages)"
#include "parts/RS_(reward_system)"
#include "parts/RTE_(round_time_extend)"
#include "parts/RWOS_(remove_weapons_on_spawn)"
#include "parts/SG_(speed_and_gravity)"
#include "parts/SJB_(sj_builder)"
#include "parts/SJE_(sj_entities)"
#include "parts/SJT_(sj_timer)"
#include "parts/SM_(sound_manager)"
#include "parts/SSP_(swap_spawn_points)"
#include "parts/TBHD_(teleport_ball_on_holder_death)"
#include "parts/TEST_(test)"
#include "parts/TM_(team_models)"
#include "parts/TRB_(turbo)"
#include "parts/TU_(team_upgrade)"
#include "parts/UM_(upgrade_manager)"
#include "parts/WM_(welcome_message)"

public Plugin:myinfo = 
{
	name = "SoccerJam: Source",
	author = "AlexSbysh",
	description = "SoccerJam mod for CS:GO",
	version = SOCCERJAMSOURCE_VERSION,
	url = SOCCERJAMSOURCE_URL
}

public OnPluginStart()
{
	CreateConVar("soccerjamsource_version", SOCCERJAMSOURCE_VERSION, "SoccerJam: Source Version", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_UNLOGGED|FCVAR_DONTRECORD|FCVAR_REPLICATED|FCVAR_NOTIFY)
	InitPartSystem()

	RegisterPart("BALL") // Ball
	RegisterPart("BAR") // Ball AutoRespawn
	RegisterPart("BBM") // Ball Bounce Multiplier
	RegisterPart("BBS") // Ball Bounce Sound
	RegisterPart("BE") // Ball Explosion
	RegisterPart("BK") // Ball Kick
	RegisterPart("BR") // Ball Receiving
	RegisterPart("BT") // Ball Trail
	RegisterPart("CB") // Curve Ball
	RegisterPart("CM") // Config Manager
	RegisterPart("DSRM") // Disarm
	RegisterPart("DZ") // Death Zone
	RegisterPart("FFI") // Frags For Interception
	RegisterPart("GA") // Goal Assist
	RegisterPart("GD") // Goal Distance
	RegisterPart("GOAL") // Goal
	RegisterPart("GSM") // Game Specific Manager
	RegisterPart("HLF") // Half
	RegisterPart("HLP") // Help
	RegisterPart("HLTH") // Health
	RegisterPart("KAS") // KA_Soccer maps support
	RegisterPart("LJ") // Long Jump
	RegisterPart("MM") // Model Manager
	RegisterPart("MSM") // Match Stats Manager
	RegisterPart("MTCH") // Match
	RegisterPart("MVP") // MVP Stars
	RegisterPart("NDAG") // No Damage After Goal
	RegisterPart("NFFK") // No Frags For Kill
	RegisterPart("NOFF") // No Friendly Fire
	RegisterPart("NRD") // No Round Draw
	RegisterPart("NREM") // No Round End Message
	RegisterPart("PAC") // Player Attack Check
	RegisterPart("PR") // Player Respawn
	RegisterPart("RBAH") // Remove Bomb And Hostages
	RegisterPart("RS") // Reward System
	RegisterPart("RTE") // Round Time Extend
	RegisterPart("RWOS") // Remove Weapons On Spawn
	RegisterPart("SG") // Speed and Gravity
	RegisterPart("SJB") // SJ Builder
	RegisterPart("SJE") // SJ Entities
	RegisterPart("SJT") // SJ Timer	
	RegisterPart("SM") // Sound Manager
	RegisterPart("SSP") // Swap Spawn Points
	RegisterPart("TBHD") // Teleport Ball on Holder Death
	RegisterPart("TEST") // Sound Manager
	RegisterPart("TM") // Team Models
	RegisterPart("TRB") // Turbo
	RegisterPart("TU") // Team Upgrade
	RegisterPart("UM") // Upgrades
	RegisterPart("WM") // Welcome Message

	InitParts()
	
	LoadTranslations("soccerjam.phrases")
}

public OnMapStart()
{
	FireOnMapStart()
}

public Action:CS_OnTerminateRound(&Float:delay, &CSRoundEndReason:reason)
{
	if (reason == CSRoundEnd_GameStart)
	{
		SJ_ResetTeamScores()
		return Plugin_Continue
	}
	return FireOnTerminateRound(delay, reason)
}

public OnEntityCreated(entity, const String:classname[])
{
	FireOnEntityCreated(entity, classname)
}

public OnClientDisconnect(client)
{
	FireOnClientDisconnect(client)
	if (IsClientInGame(client))
	{
		ClearClient(client)
		if (client == g_BallHolder)
		{
			TeleportBallToClient(client)
		}
		if (client == g_BallOwner
			&& !g_Goal)
		{
			ClearBall()			
		}
	}
}

public Action:OnPlayerRunCmd(client, &buttons, &impulse, Float:vel[3], Float:angles[3], &weapon)
{	
	return FireOnPlayerRunCmd(client, buttons)
}
