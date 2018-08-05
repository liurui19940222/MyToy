#include "ParticleEffect.h"
#include "ParticleBoxEmitter.h"
#include "ParticleHemiSphereEmitter.h"
#include "ParticleSphereEmitter.h"

USING_NAMESPACE_ENGINE;

ParticleEffect::ParticleEffect()
{
}

ParticleEffect::ParticleEffect(const ParticleDesc& desc)
{
	SetDesc(desc);
}

ParticleEffect::~ParticleEffect()
{
}

void ParticleEffect::SetMaterial(PMaterial material)
{
	m_Material = material;
}

void ParticleEffect::SetDesc(const ParticleDesc& desc)
{
	m_Desc = desc;
	switch (m_Desc.m_Shape)
	{
	case EParticleShape::Box:
		m_Emitter = make_shared<ParticleBoxEmitter>();
		break;
	case EParticleShape::Sphere:
		m_Emitter = make_shared<ParticleSphereEmitter>();
		break;
	case EParticleShape::HemiSphere:
		m_Emitter = make_shared<ParticleHemiSphereEmitter>();
		break;
	default:
		m_Emitter = make_shared<ParticleHemiSphereEmitter>();
		break;
	}
}

void ParticleEffect::CreateParticle()
{
	Particle particle;
	m_Emitter->Emit(m_Desc, particle);
	m_ActiveParticles.push_back(particle);
}

void ParticleEffect::RestoreParticle(Particle& particle)
{
	//for (auto it = m_ActiveParticles.begin(); it != m_ActiveParticles.end(); ++it)
	//{
	//	if (it->m_Id == particle.m_Id)
	//	{
	//		m_ActiveParticles.erase(it);
	//		break;
	//	}
	//}
	m_UnusedParticles.push_back(particle);
}

void ParticleEffect::UpdatePrimitives()
{
	int particleNum = m_ActiveParticles.size();
	int index = 0;
	m_Matrices.resize(particleNum);
	m_Colors.resize(particleNum);
	m_Ranges.resize(particleNum);
	Particle* p = NULL;
	for (auto it = m_ActiveParticles.begin(); it != m_ActiveParticles.end(); ++it, ++index)
	{
		p = &*it;
		p->MakeInstanceData(m_Matrices, m_Colors, m_Ranges, index);
	}
}

void ParticleEffect::OnUpdate(float deltaTime)
{
	Particle* p = NULL;
	for (auto it = m_ActiveParticles.begin(); it != m_ActiveParticles.end(); )
	{
		p = &*it;
		if (p->m_ElapsedTime > p->m_Life)
		{
			RestoreParticle(*it);
			it = m_ActiveParticles.erase(it);
			continue;
		}
		p->m_ElapsedTime += deltaTime;
		p->m_Velocity += p->m_Acceleration * deltaTime;
		p->m_Position += p->m_Velocity * deltaTime;
		it++;
	}
	UpdatePrimitives();
}

void ParticleEffect::OnRender(IRenderingInterface& ri, MeshBufferParticle& sharedBuffer, Matrix4x4& viewMatrix, Matrix4x4& projMatrix)
{
	if (m_Material.get() == NULL)
		return;
	size_t size = m_ActiveParticles.size();
	if (size == 0)
		return;
	sharedBuffer.MakeInstanceBuffer(m_Matrices, m_Colors, m_Ranges, size);
	m_Material->Bind();
	m_Material->SetParam("M", Matrix4x4::identity);
	m_Material->SetParam("V", viewMatrix);
	m_Material->SetParam("P", projMatrix);
	ri.RenderInstance(RenderingObject{ &sharedBuffer, m_Material.get() }, size);
}