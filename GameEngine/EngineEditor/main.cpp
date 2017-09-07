#include<Windows.h>
#include<windowsx.h>
#include<GameEngine\Engine.h>
#include<GameEngine\Math.h>

#define CLASS_NAME L"MainClass"
#define WIDTH 1280
#define HEIGHT 720

HWND m_hwnd;
HINSTANCE m_hInstance;

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
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

	int x = (GetSystemMetrics(0) - WIDTH) >> 1;
	int y = (GetSystemMetrics(1) - HEIGHT) >> 1;

	HWND hwnd = CreateWindowEx(NULL,
		CLASS_NAME,
		L"Win32Test",
		WS_OVERLAPPEDWINDOW,
		x, y,
		WIDTH,
		HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hwnd)
		return NULL;
	m_hwnd = hwnd;
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	m_hwnd = hwnd = CreateWindow(CLASS_NAME, L"", WS_VISIBLE | WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SIZEBOX,
		550, 10, 800, 600, hwnd, NULL, m_hInstance, NULL);

	_Engine->InitEngine(hInstance, hwnd, WIDTH, HEIGHT);

	MSG msg;

	bool isExiting = false;
	HDC hdc = GetDC(m_hwnd);
	while (!isExiting) 
	{
		_Engine->Update();
		_Engine->Render();
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
	}

	return 0;
}