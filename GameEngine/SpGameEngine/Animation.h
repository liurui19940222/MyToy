#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include"..\SpCommon\ApiDefine.h"

class ENGINE_API IAnimation
{
public:
	
	virtual void SetSpeed(float speed) = 0;

	virtual float GetSpeed() = 0;

	virtual void Play() = 0;

	virtual void Pause() = 0;

	virtual void Stop() = 0;

	virtual void ResetToBegin(bool play) = 0;
};

#endif