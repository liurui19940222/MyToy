#ifndef _CCAMERA_
#define _CCAMERA_

#include"GameObject.h"

class CCamera : public CGameObject
{
public:
	CCamera();
	~CCamera();

	virtual void OnUpdate();

	virtual void OnRender();
};

#endif