#include"Application.h"
#include"Engine.h"
#include"EngineDefine.h"

class CTestApp : public CGameWindow
{
	int window_width = 800;
	int window_height = 600;
	CGameObject* go;
public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	//Ӧ�ó������ʵ�ֵĺ���
	virtual void GetApplicationInfo(SApplicationInfo* info);
};

