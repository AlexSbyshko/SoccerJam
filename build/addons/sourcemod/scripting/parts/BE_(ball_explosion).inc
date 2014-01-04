static ParticleSystemSpecificId
static BallExplosionSoundId

public BE_Init()
{
	ParticleSystemSpecificId = CreateSpecificString("explosion_particle_system")
	BallExplosionSoundId = CreateSound("ball_explosion")
}

public BE_OnGoal()
{
	new BallExplosionEntity = CreateEntityByName("info_particle_system")

	decl String:explosionParticlesSystem[MAX_NAME_LENGTH]
	GetSpecificString(ParticleSystemSpecificId, explosionParticlesSystem, sizeof(explosionParticlesSystem))

	DispatchKeyValue(BallExplosionEntity, "effect_name", explosionParticlesSystem)
	DispatchKeyValue(BallExplosionEntity, "start_active", "0")
	DispatchKeyValue(BallExplosionEntity, "flag_as_weather", "0")
	DispatchKeyValue(BallExplosionEntity, "angles", "0 0 0")
	DispatchSpawn(BallExplosionEntity)
	ActivateEntity(BallExplosionEntity)
	decl Float:ballOrigin[3]
	Entity_GetAbsOrigin(g_Ball, ballOrigin)
	TeleportEntity(BallExplosionEntity, ballOrigin, NULL_VECTOR, NULL_VECTOR)
	AcceptEntityInput(BallExplosionEntity, "Start")
	PlaySoundByIdToAll(BallExplosionSoundId)
}