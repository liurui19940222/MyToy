#ifndef _CENGINE_
#define _CENGINE_

#include<map>
#include<vector>
#include<string>
#include<functional>
#include"Object.h"
#include"GLHead.h"
#include"Camera.h"
#include"Texture.h"
#include"Texture2D.h"
#include"IncludeComponent.h"
#include"PriorityQueue.h"
#include"Resources.h"
#include"Maker.h"

using namespace std;
using namespace container;

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

	void AddCamera(CCamera* camera);

	void RemoveCamera(CCamera* camera);

	CEngine* SetDrawGrid(bool drawGrid);

	CEngine* SetDrawDebug(bool drawDebug);

private:
	//是否显示网格
	bool drawGrid;

	//是否绘制调试图形
	bool drawDebug;

	//主摄像机
	CCamera* m_camera;

	//相机
	CPriorityQueue<CCamera*> m_cameras;
};

#endif