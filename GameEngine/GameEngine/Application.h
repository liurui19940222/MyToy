#ifndef _CAPPLICATION_
#define _CAPPLICATION_

#include<Windows.h>
#include<windowsx.h>

#define _Application CApplication::GetInstance()

struct SApplicationInfo
{
	int windowWidth;
	int windowHeight;
	int windowBits;
	bool isFullScreen;
	WCHAR* appName;
	WCHAR* className;
};

enum class EDisplayMode
{
	None = -1,
	Window = 0,
	FullScreen = 1,
};

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

#include"Camera.h"
#include"Engine.h"
#include"GameWindow.h"
#include"Singleton.h"

class CGameWindow;

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class CApplication : public CSingleton<CApplication>
{
public:

	int CreateApp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd, CGameWindow* window);

	LRESULT CALLBACK MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetWindowSize(int width, int height);

	int GetWindowWidth();

	int GetWindowHeight();

	const RECT* GetRect();

	void ToggleFullOrWindow();

	void ChangeDisplayMode(EDisplayMode mode);

	int GameLoop();

	void QuitApp();

	static CEngine* GetEngine();

private:
	int oldWindowRawWidth = 0;
	int oldWindowRawHeight = 0;
	int oldWindowPosX = 0;
	int oldWindowPosY = 0;
	bool mChangedDisplayMode = false;
	EDisplayMode mDisplayMode = EDisplayMode::None;
	CEngine* engine;
	CGameWindow* window;
	SApplicationInfo appInfo;
	HDC hdc;
	HWND hwnd;
	HINSTANCE mHIntance;
	bool isExiting;
	RECT clientRect;
};

#endif