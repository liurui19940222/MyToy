#ifndef _CTEXT_WINDOW_
#define _CTEXT_WINDOW_

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
class CTextTest : public CGameWindow
{
	CGameObject* go;
	CGameObject* textureGo;

public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	virtual void GetApplicationInfo(SApplicationInfo* info);
};

#endif