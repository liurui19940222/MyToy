#include"GLWindow.h"
#include<GameEngine\EngineSetting.h>

CRawRenderer& CGLWindow::GetRenderer()
{
	return m_renderer;
}

CGUIManager& CGLWindow::GetGUIManager()
{
	return m_gui;
}

LRESULT CALLBACK CGLWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindow::WindowProc(hWnd, uMsg, wParam, lParam);
	static float pwidth;
	static float pheight;
	int height, width;

	switch (uMsg)
	{
	case WM_SIZE:
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		m_renderer.SetupProjection(width, height);
		m_gui.SetRenderer(&m_renderer);
		m_gui.SetResolution(width, height);
		break;
	case WM_CREATE:
		m_renderer.SetupRenderContext(hWnd);
		break;
	case WM_MOVE:
		OnPositionChanged();
		break;
	case WM_CLOSE:
		m_gui.SetRenderer(NULL);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CGLWindow::OnPositionChanged()
{
	RECT rect = GetRectClient();
	m_gui.SetAbsoluteWindowLTPos(Vector2(rect.left, rect.top));
}

void CGLWindow::OnUpdate()
{
	m_gui.OnUpdate();
}

void CGLWindow::OnRender()
{
	if (!m_renderer.Initialized()) return;
	m_renderer.BeginOrtho();
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	OnDraw();
	//draw a border
	Color borderColor(0.16f, 0.16f, 0.16f, 1.0f);
	m_renderer.DrawLine(Vector3(0.0f, _Editor->HeaderHeight + 8, 0.0f), Vector3(m_width, _Editor->HeaderHeight + 8, 0.0f), borderColor, 4);
	m_renderer.DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, m_height, 0.0f), borderColor, 4);
	m_renderer.DrawLine(Vector3(0.0f, m_height, 0.0f), Vector3(m_width, m_height, 0.0f), borderColor, 4);
	m_renderer.DrawLine(Vector3(m_width, 0.0f, 0.0f), Vector3(m_width, m_height, 0.0f), borderColor, 4);

	//draw gui
	m_gui.OnRender();
	SwapBuffers(m_renderer.GetDcHandle());

	m_renderer.EndOrtho();
}

void CGLWindow::OnDraw()
{
	if(CEngineSetting::DrawGUILayout)
		m_gui.DrawLayout();
}