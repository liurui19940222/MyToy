#include "WorldTreeWindow.h"

CWorldTreeWindow::CWorldTreeWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword) : CChannel()
{
	Create(class_name, instance, parent, width, height, dword);
}

EWindowType CWorldTreeWindow::GetType()
{
	return EWindowType::WorldTree;
}

DWORD CWorldTreeWindow::GetStyle()
{
	return WS_CHILDWINDOW | WS_THICKFRAME;
}

LRESULT CALLBACK CWorldTreeWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static int height, width;
	static PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(m_hwnd, &ps);
		EndPaint(m_hwnd, &ps);
		break;
	default:
		break;
	}

	return CWindow::WindowProc(hWnd, uMsg, wParam, lParam);
}