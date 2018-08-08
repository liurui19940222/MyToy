#include "Particle.h"

USING_NAMESPACE_ENGINE;

Particle::Particle() :
	m_Position(Vector3::zero),
	m_Velocity(Vector3::zero),
	m_Acceleration(Vector3::zero),
	m_Color(Color::white),
	m_TexRange(TexcoordRange::full),
	m_Angle(0.0f),
	m_Size(1.0f),
	m_StartSize(1.0f),
	m_ElapsedTime(0.0f),
	m_Life(0.0f),
	m_RandomSeed(0.0f)
{
	static int id = 0;
	m_Id = ++id;
}


Particle::~Particle()
{
}

void Particle::MakeInstanceData(vector<Matrix4x4>& matrices, vector<Color>& colors, vector<TexcoordRange>& ranges, int index)
{
	static Matrix4x4 position, rotate, scale;
	position.MakeTranslate(m_Position);
	rotate.MakeRotate(Vector3(0, 0, m_Angle));
	scale.MakeScale(Vector3::one * m_Size);
	matrices[index] = position * rotate * scale;
	ranges[index] = m_TexRange;
	colors[index] = m_Color;
}

ParticleDesc::ParticleDesc() :
	m_MaxNum(100.0f),
	m_Rate(0.01f),
	m_Shape(EParticleShape::HemiSphere),
	m_Billboard(EParticleBillboard::Billboard),
	m_AnimCellX(1),
	m_AnimCellY(1),
	m_AnimSpeed(1.0f),
	m_UseFrameAnim(false),
	m_UseGravity(false),
	m_Size(1.0f, 1.0f)
{

}

ParticleDesc::ParticleDesc(const string& filename)
{

}