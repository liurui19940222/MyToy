#include "GameObject.h"
#include "Camera.h"
#include "Application.h"
#include "Time.h"

CApplication::CApplication(){ }

CApplication::~CApplication(){ }

CApplication* CApplication::instance = NULL;

CApplication* CApplication::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CApplication;
	}
	return instance;
}

CCamera* CApplication::GetCamera()
{
	return CApplication::GetInstance()->engine->GetCamera();
}

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
	WNDCLASSEX windowClass;
	DWORD      dwExStyle;
	DWORD      dwStyle;
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

	if (appInfo.isFullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = appInfo.windowWidth;
		dmScreenSettings.dmPelsHeight = appInfo.windowHeight;
		dmScreenSettings.dmBitsPerPel = appInfo.windowBits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, L"Display mode failed", NULL, MB_OK);
			appInfo.isFullScreen = FALSE;
		}
	}

	if (appInfo.isFullScreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	int x = (GetSystemMetrics(0) - appInfo.windowWidth) >> 1;
	int y = (GetSystemMetrics(1) - appInfo.windowHeight) >> 1;

	hwnd = CreateWindowEx(NULL,
		appInfo.className,
		appInfo.appName,
		dwStyle | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		x, y,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	hdc = GetDC(hwnd);

	if (!hwnd)
		return 0;

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	engine = new CEngine;
	engine->InitEngine(hInstance, hwnd);
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
		engine->SetupPixelFormat(hDC);
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

		engine->SetupProjection(width, height);
		appInfo.windowWidth = width;
		appInfo.windowHeight = height;

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

int CApplication::GameLoop()
{
	MSG msg;

	while (!isExiting)
	{
		

		engine->Update();
		window->OnUpdate();
		engine->Render();
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

	window->OnClose();
	engine->Quit();

	delete engine;

	if (appInfo.isFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	return (int)msg.wParam;
}

int CApplication::GetWindowWidth()
{
	return appInfo.windowWidth;
}

int CApplication::GetWindowHeight()
{
	return appInfo.windowHeight;
}

const RECT* CApplication::GetWindowRect()
{
	return &clientRect;
}