#pragma once
#include"Math.h"
#include"EngineDefine.h"

USING_NAMESPACE_ENGINE;

class InterpolateHelper
{
public:
	static float Interpolate(float a, float b, float t);
	static Color Interpolate(const Color& a, const Color& b, float t);
	static Vector2 Interpolate(const Vector2& a, const Vector2& b, float t);
	static Vector3 Interpolate(const Vector3& a, const Vector3& b, float t);
	static Vector4 Interpolate(const Vector4& a, const Vector4& b, float t);
	static Matrix4x4 Interpolate(Matrix4x4& a, Matrix4x4& b, float t);
	static Quaternion Interpolate(const Quaternion& a, const Quaternion& b, float t);
};

template<typename T>
struct InterpolatorNode {
	T		value;
	float	time;

	InterpolatorNode() { }
	InterpolatorNode(float time, T value) : time(time), value(value) { }
};

template<typename T>
class Interpolator {
private:
	vector<InterpolatorNode<T>> m_List;

public:
	void insert(float time, const T& t)
	{
		for (auto it = m_List.begin(); it != m_List.end(); ++it)
		{
			if (time < it->time)
			{
				m_List.insert(it, InterpolatorNode<T>(time, t));
				return;
			}
		}
		m_List.push_back(InterpolatorNode<T>(time, t));
	}

	T get(float time) const
	{
		uint size = m_List.size();
		if (size == 0)
			return T{};
		else if (size == 1)
			return m_List[0].value;
		time = CMath::Clamp(time, m_List[0].time, m_List[m_List.size() - 1].time);
		for (uint sampleIndex = 0; sampleIndex < size - 1; sampleIndex++)
		{
			const InterpolatorNode<T>& a = m_List[sampleIndex];
			const InterpolatorNode<T>& b = m_List[sampleIndex + 1];

			if (a.time <= time && b.time >= time)
			{
				float t = (time - a.time) / (b.time - a.time);
				return InterpolateHelper::Interpolate(a.value, b.value, t);
			}
		}
		return T{};
	}

	bool empty() const
	{
		return m_List.size() == 0;
	}
};

typedef Interpolator<float> FloatInterpolator;
typedef Interpolator<Color> ColorInterpolator;
typedef Interpolator<Vector2> Vec2Interpolator;
typedef Interpolator<Vector3> Vec3Interpolator;
typedef Interpolator<Vector4> Vec4Interpolator;
typedef Interpolator<Quaternion> QuatInterpolator;
typedef Interpolator<Matrix4x4> Mat4Interpolator;