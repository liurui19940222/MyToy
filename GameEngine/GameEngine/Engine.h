#ifndef _CENGINE_
#define _CENGINE_

#include<map>
#include<vector>
#include<string>
#include<functional>
#include<gl\glew.h>
#include"Object.h"
#include"Camera.h"
#include"Texture.h"
#include"Texture2D.h"
#include"IncludeComponent.h"
#include"PriorityQueue.h"
#include"Resources.h"
#include"Maker.h"

#define _Engine CApplication::GetEngine()
#define _MainCamera _Engine->GetCamera()
#define _MainCameraGo _MainCamera->gameObject

using namespace std;
using namespace container;

int CompareCamera(CCamera* a, CCamera* b);

class CEngine
{
public:
	void InitEngine(HINSTANCE instance, HWND hwnd);

	void SetupProjection(int width, int height);

	void SetupPixelFormat(HDC hDC);

	void BeginOrtho();

	void EndOrtho();

	void Update();

	void Render();

	void Quit();

	//检查快捷键
	void CheckShortcuts();

	CCamera* GetCamera();

	void AddCamera(CCamera* camera);

	void RemoveCamera(CCamera* camera);
private:
	//主摄像机
	CCamera* m_camera = NULL;

	//相机
	CPriorityQueue<CCamera*> m_cameras;
};

#endif