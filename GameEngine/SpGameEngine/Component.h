#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include"SpCommon\Object.hpp"
#include"DynamicFactory.h"
#include"EditorTool.h"

using namespace spgameengine;

class ENGINE_API CGameObject;

class ENGINE_API CComponent : public Object
{
	REFLECT_CLASS(CComponent)
public:
	CGameObject* gameObject;

	CComponent();
	virtual ~CComponent();

	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnDestroy();

	CGameObject* GetGameObject();
};

#endif