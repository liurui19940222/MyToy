#pragma once

#include"..\SpCommon\Math.h"
#include"..\SpCommon\EngineDefine.h"
#include"Texture.h"

BEGIN_NAMESPACE_ENGINE;

SMART_CLASS(Particle) class Particle
{
public:
	Particle();
	~Particle();

	void MakeInstanceData(vector<Matrix4x4>& matrices, vector<Color>& colors, vector<TexcoordRange>& ranges, int index);

public:
	Vector3		m_Position;
	Vector3		m_Velocity;
	Vector3		m_Acceleration;
	Color		m_Color;
	float		m_Angle;
	float		m_Size;
	float		m_ElapsedTime;
	float		m_Life;
	int			m_Id;
};

enum class EParticleShape
{
	Box,
	Sphere,
	HemiSphere,
};

enum class EParticleBillboard
{
	None,
	Billboard,
	VerticalBillboard,
};

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

template<typename T> struct LimitedValue
{
	T	min;
	T	max;

	LimitedValue() : min(0), max(0) {}
	LimitedValue(T min, T max) : min(min), max(max) {}

	void set(T value) { min = value; max = value; }
	void set(T min, T max) { this->min = min; this->max = max; }
	T get() const { return  CMath::Lerp(min, max, CMath::Random()); }
};

struct ParticleDesc
{
public:
	ParticleDesc();
	ParticleDesc(const string& filename);

	int					m_MaxNum;
	float				m_Rate;
	LimitedValue<float>	m_StartAngle;
	LimitedValue<float>	m_EndAngle;
	LimitedValue<float>	m_Speed;
	LimitedValue<float>	m_AccelSpeed;
	LimitedValue<float>	m_Life;
	LimitedValue<float> m_Size;
	Color				m_StartColor;
	Color				m_EndColor;
	EParticleShape		m_Shape;
	EParticleBillboard	m_Billboard;

	int			m_AnimCellX;
	int			m_AnimCellY;
	bool		m_UseFrameAnim;
	bool		m_UseGravity;
};

END_NAMESPACE_ENGINE;