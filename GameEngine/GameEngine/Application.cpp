#include "GameObject.h"
#include "Camera.h"
#include "Application.h"
#include "Time.h"
#include "Debug.h"
#include "Config.h"

CEngine* CApplication::GetEngine()
{
	return CApplication::GetInstance()->engine;
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return CApplication::GetInstance()->MessageHandle(hWnd, uMsg, wParam, lParam);
}

int CApplication::CreateApp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd, CGameWindow* window)
{
	mHIntance = hInstance;
	WNDCLASSEX windowClass;
	RECT       windowRect;

	this->window = window;
	window->GetApplicationInfo(&this->appInfo);

	windowRect.left = (long)0;
	windowRect.right = (long)appInfo.windowWidth;
	windowRect.top = (long)0;
	windowRect.bottom = (long)appInfo.windowHeight;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = appInfo.className;
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&windowClass))
		return 0;

	int x = (GetSystemMetrics(0) - appInfo.windowWidth) >> 1;
	int y = (GetSystemMetrics(1) - appInfo.windowHeight) >> 1;
	
	hwnd = CreateWindowEx(NULL,
		appInfo.className,
		appInfo.appName,
		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		x, y,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hwnd)
		return 0;
	oldWindowPosX = (GetSystemMetrics(0) - appInfo.windowWidth) >> 1;
	oldWindowPosY = (GetSystemMetrics(1) - appInfo.windowHeight) >> 1;
	oldWindowRawWidth = appInfo.windowWidth;
	oldWindowRawHeight = appInfo.windowHeight;

	ChangeDisplayMode((EDisplayMode)appInfo.isFullScreen);
	hdc = GetDC(hwnd);

	engine = new CEngine;
	_Engine->InitEngine(hInstance, hwnd);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	window->OnStart();

	return 0;
}

LRESULT CALLBACK CApplication::MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hDC;
	static HGLRC hRC;
	static POINT p{ 0, 0 };
	int height, width;

	switch (uMsg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		_Engine->SetupPixelFormat(hDC);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		break;

	case WM_DESTROY:
	case WM_QUIT:
	case WM_CLOSE:
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		SetWindowSize(width, height);
		break;
	case WM_MOVE:
		p.x = LOWORD(lParam);
		p.y = HIWORD(lParam);
		clientRect.left = p.x;
		clientRect.right = p.x + appInfo.windowWidth;
		clientRect.top = p.y;
		clientRect.bottom = p.y + appInfo.windowHeight;
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CApplication::SetWindowSize(int width, int height)
{
	//CDebug::Log("change w:%d\th:%d", width, height);
	_Engine->SetupProjection(width, height);
	appInfo.windowWidth = width;
	appInfo.windowHeight = height;
}

void CApplication::ToggleFullOrWindow()
{
	appInfo.isFullScreen = appInfo.isFullScreen == TRUE ? FALSE : TRUE;
	ChangeDisplayMode((EDisplayMode)appInfo.isFullScreen);
}

void CApplication::ChangeDisplayMode(EDisplayMode mode)
{
	if (mDisplayMode == mode)
		return;
	mDisplayMode = mode;
	appInfo.isFullScreen = (int)mode;
	if (mode == EDisplayMode::FullScreen)
	{
		RECT  rectProgram;
		GetWindowRect(hwnd, &rectProgram);
		oldWindowRawWidth = rectProgram.right - rectProgram.left;
		oldWindowRawHeight = rectProgram.bottom - rectProgram.top;
		oldWindowPosX = rectProgram.left;
		oldWindowPosY = rectProgram.top;
	}
	else
	{
		appInfo.windowWidth = oldWindowRawWidth;
		appInfo.windowHeight = oldWindowRawHeight;
	}
	RECT windowRect;
	DWORD dwExStyle;
	DWORD dwStyle;
	windowRect.left = (long)0;
	windowRect.right = (long)appInfo.windowWidth;
	windowRect.top = (long)0;
	windowRect.bottom = (long)appInfo.windowHeight;
	if (mode == EDisplayMode::FullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = oldWindowRawWidth;
		dmScreenSettings.dmPelsHeight = oldWindowRawHeight;
		dmScreenSettings.dmBitsPerPel = appInfo.windowBits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, L"Display mode failed", NULL, MB_OK);
			appInfo.isFullScreen = FALSE;
		}
		else
		{
			mChangedDisplayMode = true;
		}
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		ShowCursor(FALSE);

		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);
		ShowWindow(hwnd, SW_MAXIMIZE);
		UpdateWindow(hwnd);

		SetWindowLong(hwnd, GWL_STYLE, dwStyle);
		MoveWindow(hwnd, 0, 0, oldWindowRawWidth, oldWindowRawHeight, false);
	}
	else
	{
		if(mChangedDisplayMode)
			ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
		SetWindowLong(hwnd, GWL_STYLE, dwStyle);

		MoveWindow(hwnd, oldWindowPosX, oldWindowPosY, oldWindowRawWidth, oldWindowRawHeight, false);
	}
}

int CApplication::GameLoop()
{
	MSG msg;

	while (!isExiting)
	{
		_Engine->Update();
		window->OnUpdate();
		_Engine->Render();
		window->OnRender();
		SwapBuffers(hdc);

		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
			{
				isExiting = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CTime::UpdateTime();
		CTime::Lock();
	}

	QuitApp();

	return (int)msg.wParam;
}

void CApplication::QuitApp()
{
	window->OnClose();
	_Engine->Quit();

	delete engine;

	if (appInfo.isFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}
}

int CApplication::GetWindowWidth()
{
	return appInfo.windowWidth;
}

int CApplication::GetWindowHeight()
{
	return appInfo.windowHeight;
}

const RECT* CApplication::GetRect()
{
	return &clientRect;
}