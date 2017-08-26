#include"BoneAnimation.h"

void AnimationClip::Sample(float t)
{
	t = m_isLooping ? fmod(t, m_length) : t;
	if (t > m_length || t < 0)
		return;

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
}

AnimationSample& AnimationClip::FindPrev(byte joint, float time)
{
	for (int i = 0; i < m_aSamples.size() - 1; i++)
	{

	}
}

AnimationSample& AnimationClip::FindNext(byte joint, float time)
{
	
}