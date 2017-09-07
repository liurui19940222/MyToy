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

#define _Engine CEngine::GetInstance()
#define _MainCamera _Engine->GetCamera()
#define _MainCameraGo _MainCamera->gameObject
#define _SCW _Engine->GetScreenWidth()
#define _SCH _Engine->GetScreenHeight()

using namespace std;
using namespace container;

int CompareCamera(CCamera* a, CCamera* b);

class ENGINE_API CEngine : public CSingleton<CEngine>
{
public:
	void InitEngine(HINSTANCE instance, HWND hwnd, float screenWidth, float screenHeight);

	void SetupProjection(int width, int height);

	void SetupPixelFormat(HDC hDC);

	void BeginOrtho();

	void EndOrtho();

	void Update();

	void Render();

	void Quit();

	//����ݼ�
	void CheckShortcuts();

	CCamera* GetCamera();

	void AddCamera(CCamera* camera);

	void RemoveCamera(CCamera* camera);

	void UpdateClientRect();

	inline float GetScreenWidth() { return m_screenWidth; }
	inline float GetScreenHeight() { return m_screenHeight; }
	inline void SetScreenWidth(float w) { m_screenWidth = w; }
	inline void SetScreenHeight(float h) { m_screenHeight = h; }
	inline void SetClientRect(RECT rect) { m_clientRect; }
	inline RECT GetClientRect() { return m_clientRect; }
private:
	//��Ļ��ȣ��߶�
	float m_screenWidth = 0, m_screenHeight = 0;

	//�ͻ�����Χ
	RECT m_clientRect;

	//ʵ�����
	HINSTANCE m_hInstance;

	//���ھ��
	HWND m_hwnd;

	//�������
	CCamera* m_mainCamera = NULL;

	//UI�����
	CCamera* m_uiCamera = NULL;

	//���
	CPriorityQueue<CCamera*> m_cameras;
};

#endif