#include"Window.h"
#include"Editor.h"

CWindow::CWindow() {}

CWindow::CWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword)
{	
	Create(class_name, instance, parent, width, height, dword);
}

CWindow::~CWindow()
{
	
}

void CWindow::Create(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword)
{
	int x = 0, y = 0;
	if (parent == NULL)
	{
		x = (GetSystemMetrics(0) - (int)width) >> 1;
		y = (GetSystemMetrics(1) - (int)height) >> 1;
	}
	else
	{
		RECT rect;
		GetWindowRect(parent, &rect);
		x = (((rect.right - rect.left) - (int)width) >> 1) + rect.left;
		y = (((rect.bottom - rect.top) - (int)height) >> 1) + rect.top;
	}
	m_state = EChannelState::Fixed;
	m_hwnd = CreateWindowEx(NULL, class_name, GetTitle(), dword | GetStyle(),
		x, y, width, height, parent, GetMenu(), instance, (void*)this);
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
}

EWindowType CWindow::GetType()
{
	return EWindowType::Other;
}

DWORD CWindow::GetStyle()
{
	return 	0;
}

HMENU CWindow::GetMenu()
{
	return NULL;
}

wchar_t* CWindow::GetTitle()
{
	return L"";
}

LRESULT CALLBACK CWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}