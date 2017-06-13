#ifndef _CAPPLICATION_
#define _CAPPLICATION_

#include<Windows.h>
#include<windowsx.h>

struct SApplicationInfo
{
	int windowWidth;
	int windowHeight;
	int windowBits;
	bool isFullScreen;
	WCHAR* appName;
	WCHAR* className;
};

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include"Camera.h"
#include"Engine.h"
#include"GameWindow.h"

class CGameWindow;

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class CApplication
{
public:
	CApplication();
	~CApplication();

	int CreateApp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd, CGameWindow* window);

	LRESULT CALLBACK MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetWindowSize(int width, int height);

	int GetWindowWidth();

	int GetWindowHeight();

	const RECT* GetWindowRect();

	void ToggleFullOrWindow();

	int GameLoop();

	void QuitApp();

	static CApplication* GetInstance();

	static CCamera* GetCamera();

	static CEngine* GetEngine();

private:
	CEngine* engine;
	CGameWindow* window;
	SApplicationInfo appInfo;
	HDC hdc;
	HWND hwnd;
	bool isExiting;
	RECT clientRect;

	static CApplication* instance;
};

#endif