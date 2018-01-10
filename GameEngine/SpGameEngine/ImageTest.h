#include"..\SpCommon\EngineDefine.h"
#include"..\SpCommon\Math.h"
#include"Application.h"
#include"Input.h"
#include "..\SpCommon\Debug.h"
#include"Time.h"
#include"GameObject.h"
#include"Camera.h"

using namespace std;

class CImageTest : public CGameWindow
{
	CGameObject* go;
	CGameObject* go2;
	float direction[3] = { -55.0f, -22.5f, 0.0f };
	float diffuse[3] = { 0.5f, 0.5f, 0.5f };
	float specular[3] = { 1, 1, 1 };
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	virtual void GetApplicationInfo(SApplicationInfo* info);

	void InitLight();
};

