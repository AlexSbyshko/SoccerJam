#include <sourcemod>
#include <sdkhooks>
#include <sdktools>
#include <cstrike>

#undef REQUIRE_PLUGIN
#include <adminmenu>
#define REQUIRE_PLUGIN

#define SOCCERJAMSOURCE_VERSION	 "2.1.4"
#define SOCCERJAMSOURCE_URL "https://github.com/AlexSbyshko/SoccerJam"

typedef UpgradeFunc = function void (int client, float upgradeValue)
const MAX_UPGRADES = 128
const INVALID_UPGRADE = -1

#include "soccerjam/constants"
#include "soccerjam/globalvars"
#include "soccerjam/enginetools"
#include "soccerjam/sjtools"
#include "soccerjam/clients"

#include "Enums/SjEngine"

SjEngine CurrentEngine

#include "Events/BallLost"
#include "Events/BallReceived"
#include "Events/ClientActivated"
#include "Events/ClientDied"
#include "Events/ClientDying"
#include "Events/ClientDisconnecting"
#include "Events/ClientSpawned"
#include "Events/ClientTeamChanging"
#include "Events/ClientTeamChanged"
#include "Events/EntityCreated"
#include "Events/MapStarted"
#include "Events/MatchRestarted"
#include "Events/PlayerCmdRun"
#include "Events/RoundEnding"
#include "Events/RoundPreStarted"
#include "Events/RoundStarted"
#include "Events/RoundTerminated"

BallReceivedEvent BallReceived

#include "Modules/RemoveBallHolderWeapon"

#include "PlayerGreeter"

#include "parts"
#include "parts/BALL_(ball)"
#include "parts/BAR_(ball_autorespawn)"
#include "parts/BBM_(ball_bounce_multiplier)"
#include "parts/BBS_(ball_bounce_sound)"
#include "parts/BE_(ball_explosion)"
#include "parts/BR_(ball_receiving)"
#include "parts/BT_(ball_trail)"
#include "parts/CM_(config_manager)"
#include "parts/DZ_(death_zone)"
#include "parts/FFI_(frags_for_interception)"
#include "parts/GA_(goal_assist)"
#include "parts/GD_(goal_distance)"
#include "parts/GOAL_(goal)"
#include "parts/GSM_(game_specific_manager)"
#include "parts/HLP_(help)"
#include "parts/HLTH_(health)"
#include "parts/KAS_(ka_soccer_maps_support)"
#include "parts/MM_(model_manager)"
#include "parts/MSM_(match_stats_manager)"
#include "parts/MVP_(mvp_stars)"
#include "parts/NDAG_(no_damage_after_goal)"
#include "parts/NFFK_(no_frags_for_kill)"
#include "parts/NOFF_(no_friendly_fire)"
#include "parts/NRD_(no_round_draw)"
#include "parts/NREM_(no_round_end_message)"
#include "parts/PAC_(player_attack_check)"
#include "parts/PR_(player_respawn)"
#include "parts/RBAH_(remove_bomb_and_hostages)"
#include "parts/RTE_(round_time_extend)"
#include "parts/RWOS_(remove_weapons_on_spawn)"
#include "parts/SCPB_(shot_charge_progress_bar)"
#include "parts/SG_(speed_and_gravity)"
#include "parts/SJB_(sj_builder)"
#include "parts/SJE_(sj_entities)"
#include "parts/SHOT_(shot).inc"
#include "parts/SM_(sound_manager)"
#include "parts/SSP_(swap_spawn_points)"
#include "parts/TBHD_(teleport_ball_on_holder_death)"
#include "parts/TEST_(test)"
#include "parts/TM_(team_models)"

public Plugin:myinfo = 
{
	name = "SoccerJam: Source",
	author = "Alex Sbyshko",
	description = "SoccerJam mod for CS:GO",
	version = SOCCERJAMSOURCE_VERSION,
	url = SOCCERJAMSOURCE_URL
}

static PlayerGreeter playerGreeter


static ClientActivatedEvent _clientActivatedEvent
static ClientDiedEvent _clientDiedEvent
static ClientDyingEvent _clientDyingEvent
static ClientDisconnectingEvent _clientDisconnectingEvent
static ClientSpawnedEvent _clientSpawnedEvent
static ClientTeamChangingEvent _clientTeamChangingEvent
static ClientTeamChangedEvent _clientTeamChangedEvent
static EntityCreatedEvent _entityCreatedEvent
static MapStartedEvent _mapStartedEvent
static MatchRestartedEvent _matchRestartedEvent
static PlayerCmdRunEvent _playerCmdRunEvent
static RoundEndingEvent _roundEndingEvent
static RoundPreStartedEvent _roundPreStartedEvent
static RoundStartedEvent _roundStartedEvent
static RoundTerminatedEvent _roundTerminatedEvent

public void OnPluginStart()
{
	EngineVersion engineVersion = GetEngineVersion()
	if (engineVersion == Engine_CSS)
	{
		CurrentEngine = SjEngine_Css
	}
	else if (engineVersion == Engine_CSGO)
	{
		CurrentEngine = SjEngine_Csgo
	}
	else
	{
		ThrowError("[SoccerJam] Unsupported game")
	}

	CreateConVar("soccerjamsource_version", SOCCERJAMSOURCE_VERSION, "SoccerJam: Source Version", FCVAR_SPONLY|FCVAR_UNLOGGED|FCVAR_DONTRECORD|FCVAR_REPLICATED|FCVAR_NOTIFY)
	
	InitPartSystem()

	RegisterPart("BALL") // Ball
	RegisterPart("BAR") // Ball AutoRespawn
	RegisterPart("BBM") // Ball Bounce Multiplier
	RegisterPart("BBS") // Ball Bounce Sound
	RegisterPart("BE") // Ball Explosion
	RegisterPart("BR") // Ball Receiving
	RegisterPart("BT") // Ball Trail
	RegisterPart("CM") // Config Manager
	RegisterPart("DZ") // Death Zone
	RegisterPart("FFI") // Frags For Interception
	RegisterPart("GA") // Goal Assist
	RegisterPart("GD") // Goal Distance
	RegisterPart("GOAL") // Goal
	RegisterPart("GSM") // Game Specific Manager
	RegisterPart("HLP") // Help
	RegisterPart("HLTH") // Health
	RegisterPart("KAS") // KA_Soccer maps support
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
	RegisterPart("RTE") // Round Time Extend
	RegisterPart("RWOS") // Remove Weapons On Spawn
	RegisterPart("SCPB") // Shot Charge Progress Bar
	RegisterPart("SG") // Speed and Gravity
	RegisterPart("SJB") // SJ Builder
	RegisterPart("SJE") // SJ Entities
	RegisterPart("SHOT") // Shot
	RegisterPart("SM") // Sound Manager
	RegisterPart("SSP") // Swap Spawn Points
	RegisterPart("TBHD") // Teleport Ball on Holder Death
	RegisterPart("TEST") // Sound Manager
	RegisterPart("TM") // Team Models

	BallReceived = new BallReceivedEvent()
	_clientActivatedEvent = new ClientActivatedEvent()
	_clientDiedEvent = new ClientDiedEvent()
	_clientDyingEvent = new ClientDyingEvent()
	_clientDisconnectingEvent = new ClientDisconnectingEvent()
	_clientSpawnedEvent = new ClientSpawnedEvent()
	_clientTeamChangingEvent = new ClientTeamChangingEvent()
	_clientTeamChangedEvent = new ClientTeamChangedEvent()
	_mapStartedEvent = new MapStartedEvent()
	_entityCreatedEvent = new EntityCreatedEvent()
	_roundEndingEvent = new RoundEndingEvent()
	_roundPreStartedEvent = new RoundPreStartedEvent()
	_roundStartedEvent = new RoundStartedEvent()
	_roundTerminatedEvent = new RoundTerminatedEvent()
	_playerCmdRunEvent = new PlayerCmdRunEvent()
	_matchRestartedEvent = new MatchRestartedEvent()

	BallBouncing()

	BallExplosion()

	BallTrailShowing()

	BallBounceSoundPlaying()

	DeathZoneProcessing()

	GoalAssistProcessing(_clientTeamChangingEvent)

	GoalDistanceCounting()

	GameSpecificConfigManaging()

	GoalScoring(_roundStartedEvent)

	HelpShowing(_clientActivatedEvent, _clientSpawnedEvent)

	SpawnHealthSetting(_clientSpawnedEvent)

	BallShooting(_playerCmdRunEvent)

	ManageConfigs(_mapStartedEvent)

	RoundTimeExtending()

	ModelManaging()

	BombAndHostagesRemoving(_mapStartedEvent, _roundStartedEvent)

	GoalSpawning(_mapStartedEvent, _roundPreStartedEvent, _roundStartedEvent)

	SpawnPointsSwapping(_mapStartedEvent)

	SoundManaging()

	BallTeleportingOnHolderDeath(_clientDiedEvent)

	KaMapsSupport(_roundStartedEvent)

	PlayerAttackChecking(_clientActivatedEvent)

	SpeedAndGravityManaging(_clientActivatedEvent)

	ShotChargeProgressBarShowing()

	TeamModelSetting(_clientSpawnedEvent)

	Testing()

	DisablingDamageAfterGoal(_clientActivatedEvent)

	SjEntitiesFinding(_entityCreatedEvent)

	DrawDisabling(_roundTerminatedEvent)

	BallLostEvent ballLostEvent = new BallLostEvent()
	BallSpawning(ballLostEvent, _mapStartedEvent, _roundStartedEvent)

	BallReceiving(_clientDyingEvent)

	BallAutoReturning(_mapStartedEvent, _clientDisconnectingEvent, _roundStartedEvent, BallReceived)

	RemoveBallHolderWeapon(BallReceived, ballLostEvent)

	MatchStatsManaging(_clientDisconnectingEvent)

	DisablingFriendlyFire(_clientActivatedEvent)

	WeaponsOnSpawnRemoving(_clientSpawnedEvent)

	PlayerRespawning(_clientDiedEvent)

	FragsForKillsDisabling(_clientDyingEvent)

	RoundEndMessageDisabling(_roundEndingEvent)

	InitParts()
	
	LoadTranslations("soccerjam.phrases")

	HookEventEx("cs_match_end_restart", OnMatchEndRestart)
	HookEvent("player_activate", OnPlayerActivate)
	HookEvent("player_death", OnPlayerDying, EventHookMode_Pre)
	HookEvent("player_death", OnPlayerDeath)
	HookEvent("player_disconnect", OnPrePlayerDisconnected, EventHookMode_Pre)
	HookEvent("player_spawn", OnPlayerSpawn)
	HookEvent("player_team", OnPrePlayerTeam, EventHookMode_Pre)
	HookEvent("player_team", OnPlayerTeam)
	HookEvent("round_end", OnPreRoundEnd, EventHookMode_Pre)
	HookEventEx("round_prestart", OnRoundPreStarted)
	HookEvent("round_start", OnRoundStarted)
}

public void OnMapStart()
{
	_mapStartedEvent.Raise()
}

public Action CS_OnTerminateRound(float& delay, CSRoundEndReason& reason)
{
	if (reason == CSRoundEnd_GameStart)
	{
		SJ_ResetTeamScores()
		return Plugin_Continue
	}
	return _roundTerminatedEvent.Raise(delay, reason)
}

public void OnEntityCreated(int entity, const char[] classname)
{
	_entityCreatedEvent.Raise(entity, classname)
}

public void OnClientDisconnect(int client)
{
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

static void OnPlayerActivate(Handle event, const char[] name, bool dontBroadcast)
{
	new userId = GetEventInt(event, "userid")
	int client = GetClientOfUserId(userId)

	playerGreeter.GreetPlayer(userId)
	_clientActivatedEvent.Raise(client)
}

public Action OnPlayerRunCmd(int client, int& buttons, int& impulse, float vel[3], float angles[3], int &weapon)
{
	return _playerCmdRunEvent.Raise(client, buttons)
}

static void OnMatchEndRestart(Handle event, const char[] name, bool dontBroadcast)
{
	_matchRestartedEvent.Raise()
}

static void OnPlayerDying(Handle event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"))
	int attacker = GetClientOfUserId(GetEventInt(event, "attacker"))

	_clientDyingEvent.Raise(client, attacker)
}

static void OnPlayerDeath(Handle event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"))

	_clientDiedEvent.Raise(client)
}

static void OnPlayerSpawn(Handle event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"))

	_clientSpawnedEvent.Raise(client)
}

static void OnPrePlayerTeam(Handle event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"))

	_clientTeamChangingEvent.Raise(client)
}

static void OnPlayerTeam(Handle event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"))

	_clientTeamChangedEvent.Raise(client)
}

static void OnPrePlayerDisconnected(Handle event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"))

	_clientDisconnectingEvent.Raise(client)
}

static Action OnPreRoundEnd(Handle event, const char[] name, bool dontBroadcast)
{
	return _roundEndingEvent.Raise()
}

static void OnRoundPreStarted(Handle event, const char[] name, bool dontBroadcast)
{
	_roundPreStartedEvent.Raise()
}

static void OnRoundStarted(Handle event, const char[] name, bool dontBroadcast)
{
	_roundStartedEvent.Raise()
}