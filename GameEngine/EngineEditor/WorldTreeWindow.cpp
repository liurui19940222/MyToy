#include"WorldTreeWindow.h"
#include<GameEngine\Converter.h>
#include<GameEngine\Input.h>

CWorldTreeWindow::CWorldTreeWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword) : CChannel()
{
	m_caption = L"WorldTree";
	Create(class_name, instance, parent, width, height, dword);
}

EWindowType CWorldTreeWindow::GetType()
{
	return EWindowType::WorldTree;
}

DWORD CWorldTreeWindow::GetStyle()
{
	return WS_CHILD;
}

LRESULT CALLBACK CWorldTreeWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CChannel::WindowProc(hWnd, uMsg, wParam, lParam);
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

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
CGUIText* ui2;
void CWorldTreeWindow::OnCreate()
{
	CChannel::OnCreate();
	m_gui.SetGridRowHeight(20);
	m_gui.SetGridRowCount(20);
	m_gui.SetGridColumns(2, vector<float>{ 0.3f, 0.7f });
	CGUIElement* ui = m_gui.Create<CGUIElement>();
	ui->SetFill(true)->SetFillColor(Color::red)->SetCollide(true)->AddOnMouseClickListener([this](Vector2 pos) {
		this->GetGUIManager().SetRowsVisible(3, 10, true);
	});
	m_gui.PutIntoGrid(1, 0, ui);

	ui2 = m_gui.Create<CGUIText>();
	ui2->SetText(L"WorldTree")->SetColor(Color::green)->SetFontSize(16)->SetAlignment(EAlignment::CENTER_MIDDLE)->SetCollide(true)->AddOnMouseClickListener([this](Vector2 pos) {
		//this->GetGUIManager().PutIntoGrid(10, 0, NULL, true);
		this->GetGUIManager().DeleteRow(10, true);
	});
	m_gui.PutIntoGrid(10, 1, ui2);

	CTexture2D* texture = CTexture2D::Create("textures/gui/button.png");
	CGUITexture* guit = m_gui.Create<CGUITexture>();
	guit->SetTexture(texture)->SetCollide(true)->AddOnMouseClickListener([this](Vector2 pos) {
		this->GetGUIManager().SetRowsVisible(3, 10, false);
	});
	m_gui.PutIntoGrid(19, 0, guit);
}

void CWorldTreeWindow::OnDraw()
{
	CChannel::OnDraw();
	ui2->SetText(CConverter::FormatWString(L"ÃÌº”“ª––"));
}