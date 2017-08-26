#include"BoneAnimation.h"

bool AnimationClip::Sample(float t)
{
	t = m_isLooping ? fmod(t, m_length) : t;
	if (t > m_length || t < 0 || m_aSamples.size() == 0)
		return false;

	for (int i = 0; i < m_aSamples.size() - 1; i++)
	{
		AnimationSample& a = m_aSamples[i];
		AnimationSample& b = m_aSamples[i + 1];

		if (a.m_time <= t && b.m_time >= t)
		{
			Matrix4x4 mat_a;
			Matrix4x4 mat_b;
			for (byte i = 0; i < m_pSkeleton->GetSize(); i++)
			{
				auto it_a = a.m_jointPoses.find(i);
				auto it_b = b.m_jointPoses.find(i);
				if (it_a != a.m_jointPoses.end())
					mat_a = it_a->second.m_matrix;
				else
					mat_a = m_pSkeleton->GetJoint(i)->m_localAnimMatrix;
				if (it_b != b.m_jointPoses.end())
					mat_b = it_b->second.m_matrix;
				else
					mat_b = m_pSkeleton->GetJoint(i)->m_localAnimMatrix;

				m_pSkeleton->GetJoint(i)->m_localAnimMatrix = Matrix4x4::Lerp(mat_a, mat_b, (t - a.m_time) / (b.m_time - a.m_time));
			}
		}
	}
	return true;
}

bool AnimationClip::FullMatchSample(float t)
{
	t = m_isLooping ? fmod(t, m_length) : t;
	if (t > m_length || t < 0 || m_aSamples.size() == 0)
		return false;

	for (byte joint = 0, index = 0; joint < m_pSkeleton->GetSize(); joint++)
	{
		AnimationSample* a = NULL;
		AnimationSample* b = NULL;
		for (int i = 0; i < m_aSamples.size() && (!a || !b); i++)
		{
			index = m_aSamples.size() - i - 1;
			if (!a && m_aSamples[index].m_time <= t && m_aSamples[index].m_jointPoses.find(joint) != m_aSamples[index].m_jointPoses.end())
				a = &m_aSamples[index];
			if (!b && m_aSamples[i].m_time >= t && m_aSamples[i].m_jointPoses.find(joint) != m_aSamples[i].m_jointPoses.end())
				b = &m_aSamples[i];
		}
		Matrix4x4 mat_a = a ? a->m_jointPoses.find(joint)->second.m_matrix : m_pSkeleton->GetJoint(joint)->m_localAnimMatrix;
		Matrix4x4 mat_b = b ? b->m_jointPoses.find(joint)->second.m_matrix : m_pSkeleton->GetJoint(joint)->m_localAnimMatrix;
		float a_time = a ? a->m_time : 0;
		float b_time = b ? b->m_time : 1;
		m_pSkeleton->GetJoint(joint)->m_localAnimMatrix = Matrix4x4::Lerp(mat_a, mat_b, (t - a_time) / (b_time - a_time));
	}
	return true;
}
