#ifndef _CENGINE_
#define _CENGINE_

#include"Object.h"
#include"GLHead.h"
#include"Camera.h"
#include"Texture.h"
#include"IncludeComponent.h"
#include<map>
#include<vector>
#include<string>
#include<functional>

using namespace std;

typedef function<bool(CGameObject*, int)> ForeachGoCallbackR;
typedef function<void(CGameObject*, int)> ForeachGoCallback;

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

	CGameObject* CreateGameObject(string name);

	void AddGameObject(CGameObject* go);

	void RemoveGameObject(CGameObject* go);

	void DestroyGameObject(CGameObject* go);

	void ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback);

	void ForeachGameObjectR(ForeachGoCallbackR callback);

	void ForeachGameObject(CGameObject* go, ForeachGoCallback callback);

	void ForeachGameObject(ForeachGoCallback callback);

	CEngine* SetDrawGrid(bool drawGrid);

	CEngine* SetDrawDebug(bool drawDebug);

	CEngine* SetClearColor(Color clearColor);

	Color GetClearColor();

private:
	//�Ƿ���ʾ����
	bool drawGrid;

	//�Ƿ���Ƶ���ͼ��
	bool drawDebug;

	//��������ɫ
	Color clearColor;

	//�������
	CCamera* m_camera;

	map<int, Object*> m_objects;

	//���еĸ��ڵ�(û�и������)GameObject
	vector<CGameObject*> m_gameObjects;

	bool ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback, int depth);
	void ForeachGameObject(CGameObject* go, ForeachGoCallback callback, int depth);
};

#endif