#include"EngineDefine.h"
#include"Math.h"
#include"Application.h"
#include"Input.h"
#include"Debug.h"
#include"Time.h"
#include"GameObject.h"
#include"Camera.h"
#include"FileHead.h"
#include"GUISystem.h"

using namespace std;
using namespace guisystem;

class GUITest : public CGameWindow
{
	CGameObject* go;
	CTextRenderer* text;
	CGUIWidget* widget;
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	virtual void GetApplicationInfo(SApplicationInfo* info);
};

