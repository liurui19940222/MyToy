#ifndef _CTEXT_WINDOW_
#define _CTEXT_WINDOW_

#include"SpCommon\EngineDefine.h"
#include"SpCommon\Math.h"
#include"Application.h"
#include"SpCommon\Input.h"
#include "SpCommon\Debug.h"
#include"Time.h"
#include"GameObject.h"
#include"Camera.h"

using namespace std;
class CTextTest : public CGameWindow
{
	CGameObject* go;
	CGameObject* textureGo;
	CTextRenderer* text;

public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	virtual void GetApplicationInfo(SApplicationInfo* info);
};

#endif