#include "SkeletonAnimator.h"
#include "SpCommon\Math.h"

USING_NAMESPACE_ENGINE;

SkeletonAnimator::SkeletonAnimator() : m_CurPlayingIndex(-1), m_IsPlaying(false), 
	m_FadingOutLength(INVALID_FADINGOUT_PARAM)
{
}

SkeletonAnimator::~SkeletonAnimator()
{
}

void SkeletonAnimator::Play(const string& name)
{
	m_CurPlayingIndex = GetStateIndex(name);
	if (m_CurPlayingIndex != -1)
	{
		m_IsPlaying = true;
	}
	m_CurState = State::SinglePlaying;
}

void SkeletonAnimator::FadeIn(const string& name, float fadingLength)
{
	if (m_CurPlayingIndex == -1)
	{
		Play(name);
		return;
	}
	m_FadingTargetIndex = GetStateIndex(name);
	if (m_FadingTargetIndex == -1)
		return;
	m_IsPlaying = true;
	m_FadingLength = fadingLength;
	m_FadingElapsedTime = 0.0f;
	m_States[m_FadingTargetIndex].elapsedTime = 0.0;
	m_CurState = State::Fading;
}

void SkeletonAnimator::FadeInOut(const string& name, float fadingInLength, float fadingOutLength)
{
	FadeIn(name, fadingInLength);
	m_FadingOutName = m_States[m_CurPlayingIndex].clip->m_Name;
	m_FadingOutLength = fadingOutLength;
}

void SkeletonAnimator::Pause()
{
	m_IsPlaying = false;
}

void SkeletonAnimator::Continue()
{
	m_IsPlaying = true;
}

void SkeletonAnimator::Stop()
{
	if (m_CurPlayingIndex == -1 || m_CurPlayingIndex >= m_States.size())
		return;
	AnimationState& state = m_States[m_CurPlayingIndex];
	state.elapsedTime = 0.0f;
	m_IsPlaying = false;
}

void SkeletonAnimator::OnUpdate(float deltaTime)
{
	if (!m_IsPlaying || !m_Skeleton.get() || m_CurPlayingIndex == -1 || m_CurPlayingIndex >= m_States.size())
		return;

	if (m_CurState == State::SinglePlaying)
		UpdateSinglePlaying(deltaTime);
	else if (m_CurState == State::Fading)
		UpdateFading(deltaTime);
	else if (m_CurState == State::Blending)
		UpdateBlending(deltaTime);
}

void SkeletonAnimator::UpdateSinglePlaying(float deltaTime)
{
	AnimationState& state = m_States[m_CurPlayingIndex];
	state.elapsedTime += deltaTime;
	vector<JointPose> jointPoses = SkeletonAnimation::Sample(state.clip, m_Skeleton, state.elapsedTime * state.clip->m_FrameCountPerSecond, 1.0f);
	SkeletonAnimation::CalculateGlobalMatrix(m_Skeleton, jointPoses);
	SkeletonAnimation::CalculateSkinningMatrix(m_Skeleton);
}

void SkeletonAnimator::UpdateFading(float deltaTime)
{
	m_FadingElapsedTime += deltaTime;
	AnimationState& curState = m_States[m_CurPlayingIndex];
	AnimationState& targetState = m_States[m_FadingTargetIndex];
	float fps = curState.clip->m_FrameCountPerSecond;
	bool isOut = false;
	if (m_FadingOutLength == INVALID_FADINGOUT_PARAM)
		isOut = m_FadingElapsedTime * fps >= m_FadingLength;
	else
		isOut = m_FadingElapsedTime * fps + m_FadingOutLength >= m_FadingLength;
	if (isOut)
	{
		if (m_FadingOutLength == INVALID_FADINGOUT_PARAM)
		{
			m_States[m_CurPlayingIndex].elapsedTime = 0.0f;
			m_CurPlayingIndex = m_FadingTargetIndex;
			m_States[m_CurPlayingIndex].elapsedTime = m_FadingElapsedTime;
			m_CurState = State::SinglePlaying;
			UpdateSinglePlaying(0.0f);
		}
		else
		{
			FadeIn(m_FadingOutName, m_FadingOutLength);
			m_FadingOutName.clear();
			m_FadingOutLength = INVALID_FADINGOUT_PARAM;
		}
	}
	else
	{
		PAnimationClip clips[2];
		clips[0] = curState.clip;
		clips[1] = targetState.clip;
		float weights[2];
		float timePoss[2];
		curState.elapsedTime += deltaTime;
		targetState.elapsedTime += deltaTime;
		float result = CMath::Lerp(1.0f, 0.0f, m_FadingElapsedTime * fps / m_FadingLength);
		weights[0] = result;
		weights[1] = 1.0f - result;
		timePoss[0] = curState.elapsedTime * fps;
		timePoss[1] = targetState.elapsedTime * fps;
		vector<JointPose> jointPoses = SkeletonAnimation::Blend(clips, timePoss, weights, 2, m_Skeleton);
		SkeletonAnimation::CalculateGlobalMatrix(m_Skeleton, jointPoses);
		SkeletonAnimation::CalculateSkinningMatrix(m_Skeleton);
	}
}

void SkeletonAnimator::UpdateBlending(float deltaTime)
{
	
}

PAnimationClip SkeletonAnimator::GetClip() const
{
	PAnimationClip clip;
	std::find_if(m_States.begin(), m_States.end(), [&](AnimationState c) {
		if (clip->m_Name == c.clip->m_Name)
		{
			clip = c.clip;
			return true;
		}
		return false;
	});
	return clip;
}

int SkeletonAnimator::GetStateIndex(const string& name) const
{
	int index = -1;
	for (int i = 0; i < m_States.size(); ++i)
	{
		if (m_States[i].clip->m_Name == name)
		{
			index = i;
			break;
		}
	}
	return index;
}

void SkeletonAnimator::AddClip(PAnimationClip clip) 
{
	AddClip(clip, 1.0f);
}

void SkeletonAnimator::AddClip(PAnimationClip clip, float speed)
{
	m_States.push_back(AnimationState{ clip, 0.0f, speed });
}

void SkeletonAnimator::AddClips(const vector<PAnimationClip>& clips) 
{ 
	for_each(clips.begin(), clips.end(), [&](PAnimationClip clip) {
		AddClip(clip);
	});
}

void SkeletonAnimator::SetClipSpeed(const string& name, float speed)
{
	int index = GetStateIndex(name);
	if (index == -1)
		return;
	m_States[index].speed = speed;
}