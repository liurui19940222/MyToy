#include "SkeletonAnimator.h"
#include "SpCommon\Math.h"
#include "SpCommon\Debug.h"

USING_NAMESPACE_ENGINE;

SkeletonAnimator::AnimationState::AnimationState() :
	state(State::SinglePlaying),
	elapsedTime(0.0f),
	speed(1.0f),
	nextIndex(-1),
	fadeOutLength(0.0f),
	fadeOutElapsedTime(0.0f)
{ }

SkeletonAnimator::AnimationState::AnimationState(AnimationClipPtr clip, float speed) :
	clip(clip),
	state(State::SinglePlaying),
	elapsedTime(0.0f),
	speed(speed),
	nextIndex(-1),
	fadeOutLength(0.0f),
	fadeOutElapsedTime(0.0f)
{ }

SkeletonAnimator::SkeletonAnimator() : m_CurPlayingIndex(-1), m_IsPlaying(false)
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
	AnimationState& curState = m_States[m_CurPlayingIndex];
	if (!curState.clip->m_IsLooping)
		curState.elapsedTime = 0.0f;
	curState.state = State::SinglePlaying;
}

void SkeletonAnimator::FadeIn(const string& name, float fadingLength)
{
	if (m_CurPlayingIndex == -1)
	{
		Play(name);
		return;
	}
	int fadingTargetIndex = GetStateIndex(name);
	if (fadingTargetIndex == -1)
		return;
	AnimationState& animState = m_States[m_CurPlayingIndex];
	animState.nextIndex = fadingTargetIndex;
	animState.fadeOutLength = fadingLength;
	animState.fadeOutElapsedTime = 0.0f;
	animState.state = State::Fading;
	m_IsPlaying = true;
}

void SkeletonAnimator::FadeInOut(const string& name, float fadingInLength, float fadingOutLength)
{
	FadeIn(name, fadingInLength);
	AnimationState& curState = m_States[m_CurPlayingIndex];
	if (curState.nextIndex != -1 && curState.nextIndex != m_CurPlayingIndex)
	{
		AnimationState& nextState = m_States[curState.nextIndex];
		nextState.Reset();
		nextState.nextIndex = m_CurPlayingIndex;
		nextState.fadeOutLength = fadingOutLength;
	}
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

	AnimationState& curState = m_States[m_CurPlayingIndex];
	if (curState.state == State::SinglePlaying)
		UpdateSinglePlaying(deltaTime);
	else if (curState.state == State::Fading)
		UpdateFading(deltaTime);
	else if (curState.state == State::Blending)
		UpdateBlending(deltaTime);
}

void SkeletonAnimator::UpdateSinglePlaying(float deltaTime)
{
	AnimationState& state = m_States[m_CurPlayingIndex];
	state.elapsedTime += deltaTime * state.speed;
	if (state.nextIndex != -1 && state.elapsedTime * state.clip->m_FrameCountPerSecond + state.fadeOutLength >= state.clip->m_Length)
	{
		AnimationState& nextState = m_States[state.nextIndex];
		nextState.Reset();
		FadeIn(nextState.clip->m_Name, state.fadeOutLength);
		UpdateFading(0.0f);
	}
	else
	{
		bool success = SkeletonAnimation::Sample(m_TempJointPoses, state.clip, m_Skeleton, state.elapsedTime * state.clip->m_FrameCountPerSecond, 1.0f);
		if (success)
		{
			SkeletonAnimation::CalculateGlobalMatrix(m_Skeleton, m_TempJointPoses);
			SkeletonAnimation::CalculateSkinningMatrix(m_Skeleton);
		}
	}
}

void SkeletonAnimator::UpdateFading(float deltaTime)
{
	AnimationState& curState = m_States[m_CurPlayingIndex];
	curState.fadeOutElapsedTime += deltaTime * curState.speed;
	AnimationState& targetState = m_States[curState.nextIndex];
	float fps = curState.clip->m_FrameCountPerSecond;

	if (curState.fadeOutElapsedTime * fps >= curState.fadeOutLength)
	{
		m_CurPlayingIndex = curState.nextIndex;
		curState.Reset();
		targetState.state = State::SinglePlaying;
		UpdateSinglePlaying(0.0f);
	}
	else
	{
		AnimationClipPtr clips[2];
		clips[0] = curState.clip;
		clips[1] = targetState.clip;
		float weights[2];
		float timePoss[2];
		curState.elapsedTime += deltaTime * curState.speed;
		targetState.elapsedTime += deltaTime * curState.speed;
		float result = CMath::Lerp(1.0f, 0.0f, curState.fadeOutElapsedTime * fps / curState.fadeOutLength);
		weights[0] = result;
		weights[1] = 1.0f - result;
		timePoss[0] = curState.elapsedTime * fps;
		timePoss[1] = targetState.elapsedTime * fps;
		vector<JointPose> v1 = m_TempJointPoses, v2;
		bool success = SkeletonAnimation::Blend(m_TempJointPoses, clips, timePoss, weights, 2, m_Skeleton);

		if (success)
		{
			SkeletonAnimation::CalculateGlobalMatrix(m_Skeleton, m_TempJointPoses);
			SkeletonAnimation::CalculateSkinningMatrix(m_Skeleton);
		}
	}
}

void SkeletonAnimator::UpdateBlending(float deltaTime)
{

}

AnimationClipPtr SkeletonAnimator::GetClip() const
{
	AnimationClipPtr clip;
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

void SkeletonAnimator::AddClip(AnimationClipPtr clip)
{
	AddClip(clip, 1.0f);
}

void SkeletonAnimator::AddClip(AnimationClipPtr clip, float speed)
{
	m_States.push_back(AnimationState(clip, speed));
}

void SkeletonAnimator::AddClips(const vector<AnimationClipPtr>& clips)
{
	for_each(clips.begin(), clips.end(), [&](AnimationClipPtr clip) {
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