#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include"Object.h"
#include"DynamicFactory.h"

class CGameObject;

class CComponent : public Object
{
public:
	CGameObject* gameObject;

	CComponent();
	~CComponent();

	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();

	CGameObject* GetGameObject();
};

#endif