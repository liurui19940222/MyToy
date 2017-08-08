#include"Application.h"
#include"Engine.h"
#include"EngineDefine.h"

class CTestApp : public CGameWindow
{
	int window_width = 800;
	int window_height = 600;
	CGameObject* go1;
	CGameObject* go2;
	CGameObject* go3;
	CGameObject* camera;
	CGameObject* model;
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	//Ӧ�ó������ʵ�ֵĺ���
	virtual void GetApplicationInfo(SApplicationInfo* info);
};

