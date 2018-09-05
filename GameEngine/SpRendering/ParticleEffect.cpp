#include "ParticleEffect.h"
#include "ParticleBoxEmitter.h"
#include "ParticleHemiSphereEmitter.h"
#include "ParticleSphereEmitter.h"

USING_NAMESPACE_ENGINE;

ParticleEffect::ParticleEffect() : m_Timer(0.0f)
{
}

ParticleEffect::ParticleEffect(const ParticleDesc& desc) : m_Timer(0.0f)
{
	SetDesc(desc);
}

ParticleEffect::~ParticleEffect()
{
}

void ParticleEffect::SetMaterial(MaterialPtr material)
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
	if (m_UnusedParticles.size() > 0)
	{
		Particle particle = m_UnusedParticles.back();
		m_Emitter->Emit(m_Desc, particle);
		m_ActiveParticles.push_back(particle);
		m_UnusedParticles.pop_back();
	}
	else
	{
		Particle particle;
		m_Emitter->Emit(m_Desc, particle);
		m_ActiveParticles.push_back(particle);
	}
}

void ParticleEffect::RestoreParticle(Particle& particle)
{
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
	m_Timer += deltaTime;
	while (m_Timer >= m_Desc.m_Rate && m_ActiveParticles.size() < m_Desc.m_MaxNum)
	{
		m_Timer -= m_Desc.m_Rate;
		CreateParticle();
	}
	Particle* p = NULL;
	float ratio = 0.0f;
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
		ratio = p->m_ElapsedTime / p->m_Life;
		p->m_Velocity += p->m_Acceleration * deltaTime;
		p->m_Position += p->m_Velocity * deltaTime;
		if (!m_Desc.m_ColorSequence.empty())
			p->m_Color = m_Desc.m_ColorSequence.get(ratio);
		if (!m_Desc.m_SizeSequence.empty())
			p->m_Size = p->m_StartSize * m_Desc.m_SizeSequence.get(ratio);
		if (m_Desc.m_UseFrameAnim)
		{
			float t = m_Desc.m_AnimSpeed * (p->m_ElapsedTime + p->m_RandomSeed);
			float y = 1.0f - CMath::Floor(t / m_Desc.m_AnimCellX) / m_Desc.m_AnimCellY;
			float x = floor(fmod(t, m_Desc.m_AnimCellX)) / m_Desc.m_AnimCellX;
			p->m_TexRange.m_Size.x = m_Desc.m_AnimCellX;
			p->m_TexRange.m_Size.y = m_Desc.m_AnimCellY;
			p->m_TexRange.m_StartingPoint.x = x;
			p->m_TexRange.m_StartingPoint.y = y;
		}
		it++;
	}
	UpdatePrimitives();
}

void ParticleEffect::OnRender(IRenderingInterface& ri, MeshBufferParticle& sharedBuffer, Matrix4x4& viewMatrix, Matrix4x4& projMatrix, const Vector3& cameraWorldPos)
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
	m_Material->SetParam("CameraWorldPos", cameraWorldPos);
	ri.RenderInstance(RenderingObject{ &sharedBuffer, m_Material.get() }, size);
}