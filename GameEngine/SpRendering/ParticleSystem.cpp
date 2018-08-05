#include "ParticleSystem.h"

USING_NAMESPACE_ENGINE;

ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::StartUp(IRenderingInterface* ri)
{
	m_RI = ri;
}

void ParticleSystem::ShutDown()
{
	
}