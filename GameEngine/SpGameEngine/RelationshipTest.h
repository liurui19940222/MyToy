#ifndef _CMAIN_WINDOW_
#define _CMAIN_WINDOW_

#include"SpCommon\EngineDefine.h"
#include"SpCommon\Math.h"
#include"Application.h"
#include"SpCommon\Input.h"
#include "SpCommon\Debug.h"
#include"Time.h"
#include"GameObject.h"
#include"Camera.h"
#include"SpRendering\Shader.h"
#include"SpRendering\MeshBuffer.h"
#include"SpRendering\RenderCamera.h"

using namespace std;
using namespace spgameengine;

class CRelationshipTest : public CGameWindow
{
private:
	Matrix4x4 model = { 1 };

	CGameObject* go;
	CGameObject* axis;
public:

	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	virtual void GetApplicationInfo(SApplicationInfo* info);

	void InitLight();
};

#endif
