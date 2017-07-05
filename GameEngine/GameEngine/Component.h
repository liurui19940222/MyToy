#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include"Object.h"
#include"DynamicFactory.h"
#include"EditorTool.h"

class CGameObject;

class CComponent : public Object
{
	REFLECT_CLASS(CComponent)
public:
	CGameObject* gameObject;

	CComponent();
	~CComponent();

	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDrawDebug();
	virtual void OnDestroy();

	CGameObject* GetGameObject();
};

#endif