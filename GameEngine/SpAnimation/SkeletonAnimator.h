#include"SpRendering\SkeletonAnimation.h"
#include<algorithm>

#pragma once

BEGIN_NAMESPACE_ENGINE

#define INVALID_FADINGOUT_PARAM -1.0f

struct AnimationState {
	PAnimationClip clip;
	float elapsedTime;
	float speed;
};

class SkeletonAnimator
{
	enum class State {
		SinglePlaying,
		Fading,
		Blending,
	};

public:
	SkeletonAnimator();
	~SkeletonAnimator();

	void Play(const string& name);
	void FadeIn(const string& name, float fadingLength);
	void FadeInOut(const string& name, float fadingInLength, float fadingOutLength);
	void Pause();
	void Continue();
	void Stop();
	void OnUpdate(float deltaTime);

	PAnimationClip GetClip() const;
	int GetStateIndex(const string& name) const;
	void AddClip(PAnimationClip clip);
	void AddClip(PAnimationClip clip, float speed);
	void AddClips(const vector<PAnimationClip>& clips);
	void SetClipSpeed(const string& name, float speed);
	inline bool IsPlaying() const { return m_IsPlaying; }
	inline void SetSkeleton(PSkeleton skeleton) { m_Skeleton = skeleton; }
	inline PSkeleton GetSkeleton() const { return m_Skeleton; }
private:
	void UpdateSinglePlaying(float deltaTime);
	void UpdateFading(float deltaTime);
	void UpdateBlending(float deltaTime);

	State					m_CurState;
	int						m_CurPlayingIndex;
	int						m_FadingTargetIndex;
	float					m_FadingElapsedTime;
	float					m_FadingLength;
	float					m_FadingOutLength;
	string					m_FadingOutName;
	bool					m_IsPlaying;
	vector<AnimationState>  m_States;
	PSkeleton				m_Skeleton;
};

END_NAMESPACE_ENGINE