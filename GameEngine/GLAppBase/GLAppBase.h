#pragma once

#include<Windows.h>
#include<gl\glew.h>

using namespace std;

#define IS_KEY_DOWN(code) GetAsyncKeyState(code) & 0x8000

class GLAppBase
{
	const int MAX_CHAR = 256;
public:
	GLAppBase(const wchar_t* className, const wchar_t* title, int width, int height);

	void CreateMainWindow();
	void Run();
	void SetBackgroundColor(float r, float g, float b, float a);
	void DrawString(const char* str);
	void BeginOrtho();
	void EndOrtho();

protected:
	virtual void InitGL(HWND hwnd);
	virtual void OnInitialize();
	virtual void OnUpdate(float deltaTime);
	virtual void OnRender();
	virtual void OnWindowSizeChanged(int width, int height);
	virtual void OnRelease();

private:
	void MakeRenderContext();
	void SetupPixelFormat(HDC hDC);
	void ComputeFPS();
	void DrawInfo();

	static LRESULT CALLBACK MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HWND			m_Hwnd;
	HDC				m_HDC;
	HGLRC			m_HRC;
	int				m_WindowWidth;
	int				m_WindowHeight;
	float			m_TimeSinceStarUp;
	float			m_BackgroundColor[4];
	float			m_FPS;
	const wchar_t*  m_ClassName;
	const wchar_t*	m_WindowName;

public:
	static GLAppBase* AppInstance;
};

extern GLAppBase* CreateApp();
