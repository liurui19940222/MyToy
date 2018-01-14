#ifndef _CAPPLICATION_
#define _CAPPLICATION_

#include<Windows.h>
#include<windowsx.h>
#include"SpCommon\ApiDefine.h"
#include"Camera.h"
#include"Engine.h"
#include"GameWindow.h"
#include"SpCommon\Singleton.h"

#define _Application CApplication::GetInstance()

typedef function<LRESULT CALLBACK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)> WndProcFunc;

struct ENGINE_API SApplicationInfo
{
	int windowWidth;
	int windowHeight;
	int windowBits;
	int menuId;
	bool isFullScreen;
	WCHAR* appName;
	WCHAR* className;
	WndProcFunc wndProc;
};

enum class ENGINE_API EDisplayMode
{
	None = -1,
	Window = 0,
	FullScreen = 1,
};

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

class ENGINE_API CGameWindow;

class ENGINE_API CApplication : public CSingleton<CApplication>
{
public:

	CApplication* CreateApp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd, CGameWindow* window);

	LRESULT CALLBACK MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetWindowSize(int width, int height);

	void SetCursor(LPWSTR cursorId);

	int GetWindowWidth();

	int GetWindowHeight();

	HINSTANCE GetInstanceHandle();

	HWND GetWindowHandle();

	CGameWindow* GetGameWindow();

	void ToggleFullOrWindow();

	void ChangeDisplayMode(EDisplayMode mode);

	int GameLoop();

	void QuitApp();

private:
	int oldWindowRawWidth = 0;
	int oldWindowRawHeight = 0;
	int oldWindowPosX = 0;
	int oldWindowPosY = 0;
	bool mChangedDisplayMode = false;
	EDisplayMode mDisplayMode = EDisplayMode::None;
	CGameWindow* window;
	SApplicationInfo appInfo;
	HINSTANCE mHIntance;
	HWND hwnd;
	bool isExiting;
};

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ENGINE_API CApplication* CreateApp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd, CGameWindow* window);

#endif