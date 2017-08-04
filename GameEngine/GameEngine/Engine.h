#ifndef _CENGINE_
#define _CENGINE_

#include"Object.h"
#include"GLHead.h"
#include"Camera.h"
#include"Texture.h"
#include"Texture2D.h"
#include"IncludeComponent.h"
#include"PriorityQueue.h"
#include<map>
#include<vector>
#include<string>
#include<functional>

using namespace std;
using namespace container;

typedef function<bool(CGameObject*, int)> ForeachGoCallbackR;
typedef function<void(CGameObject*, int)> ForeachGoCallback;

int CompareCamera(CCamera* a, CCamera* b);

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

	void AddCamera(CCamera* camera);

	void RemoveCamera(CCamera* camera);

	void DestroyGameObject(CGameObject* go);

	void ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback);

	void ForeachGameObjectR(ForeachGoCallbackR callback);

	void ForeachGameObject(CGameObject* go, ForeachGoCallback callback);

	void ForeachGameObject(ForeachGoCallback callback);

	CEngine* SetDrawGrid(bool drawGrid);

	CEngine* SetDrawDebug(bool drawDebug);

private:
	//�Ƿ���ʾ����
	bool drawGrid;

	//�Ƿ���Ƶ���ͼ��
	bool drawDebug;

	//�������
	CCamera* m_camera;

	//���е�Object
	map<int, Object*> m_objects;

	//���еĸ��ڵ�(û�и������)GameObject
	vector<CGameObject*> m_gameObjects;

	//���
	CPriorityQueue<CCamera*> m_cameras;

	bool ForeachGameObjectR(CGameObject* go, ForeachGoCallbackR callback, int depth);
	void ForeachGameObject(CGameObject* go, ForeachGoCallback callback, int depth);
};

#endif