#pragma once

#include"Particle.h"

BEGIN_NAMESPACE_ENGINE;

SMART_CLASS(ParticleEmitter) class ParticleEmitter
{
public:

	virtual void Emit(const ParticleDesc& desc, Particle& particle) = 0;
};

END_NAMESPACE_ENGINE;