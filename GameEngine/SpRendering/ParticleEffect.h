#pragma once

#include"Particle.h"
#include"ParticleEmitter.h"
#include"Rendering.h"
#include"MeshBufferParticle.h"
#include<list>

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(ParticleEffect) class ParticleEffect
{
public:
	ParticleEffect();
	ParticleEffect(const ParticleDesc& desc);
	~ParticleEffect();

	void SetMaterial(PMaterial material);
	void SetDesc(const ParticleDesc& desc);
	void CreateParticle();
	void RestoreParticle(Particle& particle);
	void UpdatePrimitives();
	void OnUpdate(float deltaTime);
	void OnRender(IRenderingInterface& ri, MeshBufferParticle& sharedBuffer, Matrix4x4& viewMatrix, Matrix4x4& projMatrix);

private:
	PParticleEmitter		m_Emitter;
	list<Particle>			m_ActiveParticles;
	list<Particle>			m_UnusedParticles;
	vector<Matrix4x4>		m_Matrices;
	vector<Color>			m_Colors;
	vector<TexcoordRange>	m_Ranges;

	PMaterial				m_Material;
	ParticleDesc			m_Desc;
	float					m_Timer;
};

END_NAMESPACE_ENGINE