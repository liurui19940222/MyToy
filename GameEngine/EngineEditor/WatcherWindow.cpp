#include "WatcherWindow.h"


CWatcherWindow::CWatcherWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword) : CChannel()
{
	m_caption = L"Watcher";
	Create(class_name, instance, parent, width, height, dword);
}

EWindowType CWatcherWindow::GetType()
{
	return EWindowType::Watcher;
}

DWORD CWatcherWindow::GetStyle()
{
	return WS_CHILD;
}

LRESULT CALLBACK CWatcherWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CChannel::WindowProc(hWnd, uMsg, wParam, lParam);
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

void CWatcherWindow::OnCreate()
{
	CChannel::OnCreate();
	m_gui.SetGridRowHeight(20);
	m_gui.SetGridRowCount(20);
	m_gui.SetGridColumns(2, vector<float>{ 0.3f, 0.7f });
	CGUIElement* ui = m_gui.Create<CGUIElement>();
	ui->SetFill(true)->SetFillColor(Color::red)->SetCollide(true)->AddOnMouseClickListener([this](Vector2 pos) {
		this->GetGUIManager().SetLayoutOffsetY(this->GetGUIManager().GetLayoutOffsetY() + 10);
	});
	m_gui.PutIntoGrid(1, 0, ui);
}

void CWatcherWindow::OnDraw()
{
	CChannel::OnDraw();
}