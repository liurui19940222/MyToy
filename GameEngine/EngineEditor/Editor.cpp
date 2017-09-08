#include"Editor.h"
#include"resource.h"
#include"Channel.h"
#include<GameEngine\Config.h>

int CEditor::InitEditor(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	m_windowWidth = _Config->GetValue<float>("window", "width");
	m_windowHeight = _Config->GetValue<float>("window", "height");

	m_hInstance = hInstance;
	WNDCLASSEX windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = GetStockBrush(WHITE_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = CLASS_NAME;
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&windowClass))
		return NULL;

	int x = (GetSystemMetrics(0) - (int)m_windowWidth) >> 1;
	int y = (GetSystemMetrics(1) - (int)m_windowHeight) >> 1;

	HWND hwnd = CreateWindowEx(NULL,
		CLASS_NAME,
		L"Win32Test",
		WS_OVERLAPPEDWINDOW,
		x, y,
		m_windowWidth,
		m_windowHeight,
		NULL,
		LoadMenu(NULL, MAKEINTRESOURCE(IDM_MENU_ROOT)),
		hInstance,
		NULL);

	if (!hwnd)
		return NULL;
	m_hwnd = hwnd;
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	m_channels.push_back(new CChannel(CLASS_NAME, hInstance, hwnd, 400, 400));

	_Engine->InitEngine(hInstance, m_channels[0]->WindowHandle, m_windowWidth, m_windowHeight);

	return 0;
}

int CEditor::EditorLoop()
{
	MSG msg;

	bool isExiting = false;

	while (!isExiting)
	{
		_Engine->Update();
		_Engine->Render();
		SwapBuffers(_Engine->DCHandle);
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
	}
	return 0;
}

LRESULT CALLBACK CEditor::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;
	static HDC hDC;
	static POINT p{ 0, 0 };
	static RECT rect;
	static int height, width;
	static LPPAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		_Engine->SetupPixelFormat(hDC);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		break;
	case WM_PAINT:

		break;
	case WM_DESTROY:
		break;
	case WM_QUIT:
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		_Engine->SetupProjection(width, height);
		_Engine->UpdateClientRect();
		break;
	case WM_COMMAND:

		break;
	case WM_MOVE:
		_Engine->UpdateClientRect();
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return _Editor->WindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_Editor->InitEditor(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	return _Editor->EditorLoop();
}