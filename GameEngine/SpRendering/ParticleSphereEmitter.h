#pragma once

#include"ParticleEmitter.h"

BEGIN_NAMESPACE_ENGINE

class ParticleSphereEmitter : public ParticleEmitter
{
public:

	virtual void Emit(const ParticleDesc& desc, Particle& particle) override;
};

END_NAMESPACE_ENGINE