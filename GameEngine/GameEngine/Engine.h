#ifndef _CENGINE_
#define _CENGINE_

#include"Object.h"
#include"GLHead.h"
#include"Camera.h"
#include"Texture.h"
#include"IncludeComponent.h"
#include<map>
#include<vector>

using namespace std;

class CEngine
{
public:
	CEngine();
	~CEngine();

	void InitEngine(HINSTANCE instance, HWND hwnd);

	void SetupProjection(int width, int height);

	void SetupPixelFormat(HDC hDC);

	void BeginOrtho();

	void EndOrtho();

	void Update();

	void Render();

	void Quit();

	CCamera* GetCamera();

	template<typename T>
	T* CreateObject()
	{
		T* t = DynamicFactory::Instance().Create<T>();
		((Object*)t)->OnInitialize();
		m_objects.insert(make_pair(t->GetInstanceId(), t));
		return t;
	}

	void Destroy(Object* obj);

	CBitImage* CreateImage(char* filename);

	void ReleaseImage(CBitImage* image);

	CGameObject* CreateGameObject();

	void DestroyGameObject(CGameObject* go);

	CEngine* SetDrawGrid(bool drawGrid);

	CEngine* SetClearColor(Color clearColor);

private:
	//是否显示网格
	bool drawGrid;

	//清屏的颜色
	Color clearColor;

	//主摄像机
	CCamera* m_camera;

	map<int, Object*> m_objects;

	map<int, CGameObject*> m_gameObjects;
};

#endif