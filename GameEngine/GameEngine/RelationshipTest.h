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

	int window_width = 800;
	int window_height = 600;

	float moveSpeed = 50;
	Vector3 cameraPos;
	CGameObject* go;
	CGameObject* go2;
	vector<CGameObject*> childs;

	float direction[3] = { -55.0f, -22.5f, 0.0f };
	float diffuse[3] = { 0, 1, 0 };
	float specular[3] = { 1, 1, 1 };

	int max = 999;
public:

	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	virtual void GetApplicationInfo(SApplicationInfo* info);

	void InitLight();
};

#endif
