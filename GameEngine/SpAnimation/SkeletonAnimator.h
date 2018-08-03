#include"..\SpRendering\SkeletonAnimation.h"
#include<algorithm>

#pragma once

BEGIN_NAMESPACE_ENGINE

#define INVALID_FADINGOUT_PARAM -1.0f

class SkeletonAnimator
{
	enum class State {
		SinglePlaying,
		Fading,
		Blending,
	};

	class AnimationState {
	public:
		PAnimationClip	clip;
		State			state;
		float			elapsedTime;
		float			speed;
		int				nextIndex;
		float			fadeOutLength;
		float			fadeOutElapsedTime;

		AnimationState();
		AnimationState(PAnimationClip clip, float speed);

		inline void Reset() 
		{
			state = State::SinglePlaying;
			elapsedTime = 0.0f;
			nextIndex = -1;
			fadeOutLength = 0.0f;
			fadeOutElapsedTime = 0.0f;
		}
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

	int						m_CurPlayingIndex;
	bool					m_IsPlaying;
	vector<AnimationState>  m_States;
	PSkeleton				m_Skeleton;
	vector<JointPose>		m_TempJointPoses;
};

END_NAMESPACE_ENGINE