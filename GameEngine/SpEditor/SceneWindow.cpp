#include"SceneWindow.h"

CSceneWindow::CSceneWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword) : CChannel()
{
	Create(class_name, instance, parent, width, height, dword);
	_Engine->InitEngine(_Editor->WinInstance, m_hwnd, 800, 600);
}

EWindowType CSceneWindow::GetType()
{
	return EWindowType::Scene;
}

DWORD CSceneWindow::GetStyle()
{
	return WS_CHILD;
}

void CSceneWindow::OnCreate()
{

}

LRESULT CALLBACK CSceneWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindow::WindowProc(hWnd, uMsg, wParam, lParam);
	static HGLRC hRC;
	static HDC hDC;
	static POINT p{ 0, 0 };
	static RECT rect;
	static int height, width;

	switch (uMsg)
	{
	case WM_CREATE:
		_Engine->SetupRenderContext(hWnd);
		break;
	case WM_CLOSE:
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