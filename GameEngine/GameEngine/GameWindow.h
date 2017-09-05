#ifndef _CGAME_WINDOW_
#define _CGAME_WINDOW_

#include"Application.h"
#include"ApiDefine.h"

struct ENGINE_API SApplicationInfo;

class ENGINE_API CGameWindow
{
public:
	CGameWindow();
	~CGameWindow();

	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnRender();

	virtual void OnClose();

	//Ӧ�ó������ʵ�ֵĺ���
	virtual void GetApplicationInfo(SApplicationInfo* info) = 0;
};

#endif