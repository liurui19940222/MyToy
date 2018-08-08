#include "ParticleBoxEmitter.h"

USING_NAMESPACE_ENGINE;

void ParticleBoxEmitter::Emit(const ParticleDesc& desc, Particle& particle)
{
	particle.m_Angle = desc.m_StartAngle.get();
	particle.m_Life = desc.m_Life.get();
	particle.m_Size = desc.m_Size.get();
	particle.m_Velocity = CMath::RandomVec3() * desc.m_Speed.get();
	particle.m_Acceleration = CMath::RandomVec3() * desc.m_AccelSpeed.get();
	particle.m_Position = Vector3::zero;
	particle.m_ElapsedTime = 0.0f;
}