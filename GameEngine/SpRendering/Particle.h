#pragma once

#include"..\SpCommon\Math.h"
#include"..\SpCommon\EngineDefine.h"

BEGIN_NAMESPACE_ENGINE;

struct Particle
{
public:
	Particle();
	~Particle();

	Vector3		m_Position;
	Vector3		m_Velocity;
	Color		m_Color;
	float		m_Angle;
	float		m_Size;
	float		m_ElapsedTime;
	float		m_Life;
};

END_NAMESPACE_ENGINE;