#include"Window.h"
#include"Editor.h"
#include<GameEngine\Input.h>
#include<GameEngine\Debug.h>

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
		x = (((rect.right - rect.left) - (int)width) >> 1);
		y = (((rect.bottom - rect.top) - (int)height) >> 1);
	}
	m_state = EChannelState::Fixed;
	m_hwnd = CreateWindowEx(NULL, class_name, GetTitle(), dword | GetStyle(),
		x, y, width, height, parent, GetMenu(), instance, (void*)this);
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
	OnCreate();
}

void CWindow::OnCreate()
{
	
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
	static FRect frect;
	static float pwidth;
	static float pheight;
	static HDC hDC;
	static HGLRC hRC;

	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
		pwidth = _Editor->WindowWidth;
		pheight = _Editor->WindowHeight;
		frect = _LocalClientToFRect(GetLocalRect(), pwidth, pheight);
		_Editor->UpdateColumn(frect, m_col);
		_Editor->UpdateRow(frect, m_col, m_row);

		m_height = HIWORD(lParam);
		m_width = LOWORD(lParam);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CWindow::SetRect(RECT& rect)
{
	HWND parent = GetParent(m_hwnd);
	if (parent == NULL)
	{
		SetLocalRect(rect, rect.right - rect.left, rect.bottom - rect.top);
		return;
	}
	RECT parentRect = GetGlobalClientRect(parent);
	SetLocalRect(RECT{ rect.left - parentRect.left, rect.top - parentRect.top,
		rect.right - parentRect.left, rect.bottom - parentRect.top },
		parentRect.right - parentRect.left, parentRect.bottom - parentRect.top);
}

RECT CWindow::GetRect()
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	return rect;
}

RECT CWindow::GetRectClient()
{
	return GetGlobalClientRect(m_hwnd);
}

void CWindow::SetLocalRect(RECT& rect, float parent_width, float parent_height)
{
	m_normalizedRect.left = (float)rect.left / parent_width;
	m_normalizedRect.top = (float)rect.top / parent_height;
	m_normalizedRect.right = (float)rect.right / parent_width;
	m_normalizedRect.bottom = (float)rect.bottom / parent_height;
	m_parentWidth = parent_width;
	m_parentHeight = parent_height;
	MoveWindow(m_hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
}

RECT CWindow::GetLocalRect()
{
	HWND parent = GetParent(m_hwnd);
	if (parent == NULL)
		return GetRect();
	RECT parentRect = GetGlobalClientRect(parent);
	RECT childRect = GetRect();
	if (childRect.bottom - childRect.top < 100)
	{
		childRect.top = m_normalizedRect.top * m_parentHeight;
	}
	return RECT{ childRect.left - parentRect.left, childRect.top - parentRect.top,
		childRect.right - parentRect.left, childRect.bottom - parentRect.top };
}

RECT CWindow::LocalToGlobalRect(const RECT& _rect)
{
	RECT rect;
	static POINT p{ 0, 0 };
	p.x = 0; p.y = 0;
	ClientToScreen(m_hwnd, &p);
	rect.left = p.x;
	rect.right = p.x + _rect.right - _rect.left;
	rect.top = p.y;
	rect.bottom = p.y + _rect.bottom - _rect.top;
	return rect;
}

RECT CWindow::GlobalToLocalRect(const RECT& rect)
{
	RECT childRect = GetRect();
	return RECT{ childRect.left - rect.left, childRect.top - rect.top,
		childRect.right - rect.left, childRect.bottom - rect.top };
}

RECT CWindow::GetGlobalClientRect(HWND hwnd)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	RECT rect;
	static POINT p{ 0, 0 };
	p.x = 0; p.y = 0;
	ClientToScreen(hwnd, &p);
	rect.left = p.x;
	rect.right = p.x + clientRect.right - clientRect.left;
	rect.top = p.y;
	rect.bottom = p.y + clientRect.bottom - clientRect.top;
	return rect;
}

float CWindow::GetHeaderHeight()
{
	RECT rect;
	POINT p{ 0, 0 };
	p.x = 0; p.y = 0;
	ClientToScreen(m_hwnd, &p);
	GetWindowRect(m_hwnd, &rect);
	return abs(rect.top - p.y);
}

void CWindow::UpdateRect(float parent_width, float parent_height)
{
	SetLocalRect(_FRectToLocalClient(m_normalizedRect, parent_width, parent_height), parent_width, parent_height);
}

void CWindow::OnUpdate()
{
	
}

void CWindow::OnRender()
{

}

void CWindow::OnPositionChanged()
{
	
}

void CWindow::OnFieldChanged(const FRect& rect)
{
	SetLocalRect(_FRectToLocalClient(rect, _Editor->WindowWidth, _Editor->WindowHeight)
		, _Editor->WindowWidth, _Editor->WindowHeight);
}
