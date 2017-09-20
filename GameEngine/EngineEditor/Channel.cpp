#include "Channel.h"
#include<GameEngine\Converter.h>

LRESULT CALLBACK CChannel::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CGLWindow::WindowProc(hWnd, uMsg, wParam, lParam);
	static HGLRC hRC;
	static HDC hDC;
	static POINT p{ 0, 0 };
	static RECT rect;
	static int height, width;

	switch (uMsg)
	{
	case WM_CREATE:
		
		break;
	case WM_CLOSE:
		break;
	case WM_SIZE:

		break;
	case WM_COMMAND:

		break;
	case WM_MOVE:

		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CChannel::OnCreate()
{
	CGUIElement* e = m_gui.Create<CGUIElement>();
	e->SetFill(true)->SetWidth(100)->SetHeight(30)->SetPosition(Vector2(m_width * .5f, m_height * .5f));
}

void CChannel::OnDraw()
{
	m_renderer.DrawRect(SRect2D{ m_width * 0.5f, m_height - 12, m_width * 0.5f, 14 }, Color(0.1, 0.1, 0.1, 1));
	m_renderer.DrawString(CConverter::FormatWString(L" %s", m_caption.c_str()), SRect2D{ 0, 0, m_width * 0.5f, 20 }, Vector3(m_width * 0.5f, m_height - 12, 0), Color::white, 16, EAlignment::LEFT_MIDDLE);
}