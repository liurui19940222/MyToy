#include "Window.h"
#include "..\SpCommon\Debug.h"

using namespace dxgame;

void IWindowEventHandler::OnCreated() {}
void IWindowEventHandler::OnSizeChanged(int width, int height) {}
void IWindowEventHandler::OnActivate(bool active) {}
void IWindowEventHandler::OnDestroy() {}

std::shared_ptr<Window>	Window::windowPtr;

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return Window::getWindowPtr()->procMsg(hwnd, msg, wparam, lparam);
}

Window::Window(IWindowEventHandler* handler) : m_Hwnd(NULL), m_Active(false), 
	m_IsMinimized(false), m_IsMaximized(false), m_IsResizing(false)
{
	m_EventHandler = handler;
}

Window::~Window()
{
	m_EventHandler = NULL;
}

void Window::init()
{
	windowPtr = shared_from_this();
	const wchar_t* className = L"TEST_WND";
	const wchar_t* wndName = L"DXWindow";
	HINSTANCE instance = GetModuleHandle(NULL);
	WNDCLASSEX wndCls = {
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		mainWndProc,
		0, 0,
		instance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		GetStockBrush(WHITE_BRUSH),
		NULL,
		className,
		LoadIcon(NULL, IDI_APPLICATION)
	};

	if (!RegisterClassEx(&wndCls))
		throw std::exception("RegisterClass was failed!!");

	DWORD style = WS_OVERLAPPEDWINDOW;
	RECT rect = { 0, 0, 800, 600 };
	AdjustWindowRectEx(&rect, style, false, 0);
	DWORD width = rect.right - rect.left;
	DWORD height = rect.bottom - rect.top;
	DWORD x = (GetSystemMetrics(SM_CXSCREEN) - width) >> 1;
	DWORD y = (GetSystemMetrics(SM_CYSCREEN) - height) >> 1;
	m_Hwnd = CreateWindowEx(0, className, wndName, style, x, y, width, height, NULL, NULL, instance, NULL);

	if (!m_Hwnd)
		throw std::exception("CreateWindow was failed!!");

	ShowWindow(m_Hwnd, SW_SHOW);
	UpdateWindow(m_Hwnd);
}

void Window::close() noexcept
{
	PostQuitMessage(0);
}

int Window::getWidth() const noexcept
{
	RECT rect;
	GetClientRect(m_Hwnd, &rect);
	return rect.right - rect.left;
}

int Window::getHeight() const noexcept
{
	RECT rect;
	GetClientRect(m_Hwnd, &rect);
	return rect.bottom - rect.top;
}

void Window::getRect(RECT& rect) const noexcept
{
	GetClientRect(m_Hwnd, &rect);
}

void Window::onResize() noexcept
{
	RECT rect;
	getRect(rect);
	m_EventHandler->OnSizeChanged(rect.right - rect.left, rect.bottom - rect.top);
}

LRESULT CALLBACK Window::procMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
{
	switch (msg)
	{
	case WM_CREATE:
		if (m_EventHandler) m_EventHandler->OnCreated();
		return 0;
	case WM_ACTIVATE:
		{
			WORD activeFlag = LOWORD(wparam);
			m_Active = activeFlag != WA_INACTIVE;
			if (m_EventHandler) m_EventHandler->OnActivate(m_Active);
		}
		return 0;
	case WM_PAINT:
		return 0;
	case WM_SIZE:
		if (wparam == SIZE_MINIMIZED)
		{
			m_IsMinimized = true;
			m_IsMaximized = false;
			m_Active = false;
		}
		else if (wparam == SIZE_MAXIMIZED)
		{
			m_IsMinimized = false;
			m_IsMaximized = true;
			m_Active = true;
			onResize();
		}
		else if (wparam == SIZE_RESTORED)
		{
			if (m_IsMinimized)
			{
				m_IsMinimized = false;
				m_Active = true;
				onResize();
			}
			else if (m_IsMaximized)
			{
				m_IsMaximized = false;
				m_Active = true;
				onResize();
			}
			else if (m_IsResizing)
			{
			}
			else onResize();
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		m_IsResizing = true;
		m_Active = false;
		return 0;
	case WM_EXITSIZEMOVE:
		m_IsResizing = false;
		m_Active = true;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		if (m_EventHandler) m_EventHandler->OnDestroy();
		return 0;
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lparam)->ptMinTrackSize.x = 320;
		((MINMAXINFO*)lparam)->ptMinTrackSize.y = 240;
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

std::shared_ptr<Window> Window::getWindowPtr() noexcept { return windowPtr; }