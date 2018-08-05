#pragma once

#include"Particle.h"
#include"ParticleEmitter.h"
#include"Rendering.h"

BEGIN_NAMESPACE_ENGINE

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void StartUp(IRenderingInterface* ri);
	void ShutDown();

private:
	IRenderingInterface*	m_RI;
};

END_NAMESPACE_ENGINE