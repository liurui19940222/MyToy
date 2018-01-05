#ifndef _CMAIN_WINDOW_
#define _CMAIN_WINDOW_

#include"EngineDefine.h"
#include"Math.h"
#include"Application.h"
#include"Input.h"
#include"Debug.h"
#include"Time.h"
#include"GameObject.h"
#include"Camera.h"
#include"FileHead.h"
#include"Shader.h"
#include"MeshBuffer.h"
#include"RenderCamera.h"

using namespace std;

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
