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

using namespace std;

class CRelationshipTest : public CGameWindow
{
private:

	int window_width = 800;
	int window_height = 600;

	float moveSpeed = 50;
	Vector3 cameraPos;
	CGameObject* go;
	vector<CGameObject*> childs;

	float direction[3] = { -55.0f, -22.5f, 0.0f };
	float diffuse[3] = { 0, 1, 0 };
	float specular[3] = { 1, 1, 1 };

public:

	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	virtual void GetApplicationInfo(SApplicationInfo* info);

	void InitLight();
};

#endif