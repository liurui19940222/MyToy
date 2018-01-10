#include"..\SpCommon\EngineDefine.h"
#include"..\SpCommon\Math.h"
#include"Application.h"
#include"Input.h"
#include "..\SpCommon\Debug.h"
#include"Time.h"
#include"GameObject.h"
#include"Camera.h"

class CTestTest : public CGameWindow
{
	CGameObject* go;
	CGameObject* go2;
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	virtual void GetApplicationInfo(SApplicationInfo* info);
};

