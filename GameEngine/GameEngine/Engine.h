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
#include"Singleton.h"
#include"Property.h"
#include"TaskManager.h"

inline CCamera* GetMainCamera()
{
	CGameObject* go = _Maker->FindGameObjectWithTag("MainCamera");
	return go ? go->GetComponent<CCamera>() : NULL;
}

#define _Engine CEngine::GetInstance()
#define _MainCamera GetMainCamera()
#define _MainCameraGo _Maker->FindGameObjectWithTag("MainCamera")
#define _SCW _Engine->ClientWidth
#define _SCH _Engine->ClientHeight

using namespace std;
using namespace container;

int CompareCamera(CCamera* a, CCamera* b);

class ENGINE_API CEngine : public CSingleton<CEngine>
{
public:
	void InitEngine(HINSTANCE instance, HWND hwnd, float screenWidth, float screenHeight);
	void SetupProjection(int width, int height);
	void SetupPixelFormat(HDC hDC);
	void SetupRenderContext(HWND hwnd);
	void MakeRenderContext();
	void BeginOrtho();
	void EndOrtho();
	void Update();
	void Render();
	void Quit();

	//检查快捷键
	void CheckShortcuts();
	void AddCamera(CCamera* camera);
	void RemoveCamera(CCamera* camera);
	void UpdateClientRect();

	property<float> ClientWidth = _prop(float, { m_clientWidth = value; },  { return m_clientWidth; });
	property<float> ClientHeight = _prop(float, { m_clientHeight = value; }, { return m_clientHeight; });
	property<RECT> ClientRect = _prop(RECT, { m_clientRect = value; }, { return m_clientRect; });
	property_r<HDC> DCHandle = _prop_r(HDC, { return m_hdc; });
	property<HWND> WindowHandle = _prop(HWND, { 
		m_hwnd = value; 
		m_hdc = GetDC(m_hwnd);
	}, { return m_hwnd; });

private:
	//屏幕宽度，高度
	float m_clientWidth = 0, m_clientHeight = 0;

	//客户区范围
	RECT m_clientRect;

	//实例句柄
	HINSTANCE m_hInstance;

	//窗口句柄
	HWND m_hwnd;
	HDC m_hdc;
	HGLRC m_hrc;

	//相机
	CPriorityQueue<CCamera*> m_cameras;
};

#endif