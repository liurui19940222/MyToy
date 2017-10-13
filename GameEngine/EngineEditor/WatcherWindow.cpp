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

void CWatcherWindow::OnCreate()
{
	CChannel::OnCreate();
	m_gui.SetGridRowHeight(20);
	m_gui.SetGridRowCount(20);

	CGUITextEdit* edit = m_gui.Create<CGUITextEdit>();
	edit->SetEditValueMode(EEditValueMode::Word);

	CGUITexture* texture = m_gui.Create<CGUITexture>();
	texture->SetTexture(CTexture2D::Create("textures/gui/ico_gameObject.png"));

	CGUIHorizontalLayout* layout = m_gui.Create<CGUIHorizontalLayout>();
	layout->SetWeights(vector<float>{ 0.08f, 0.75f, 0.17f });
	m_gui.PutIntoGrid(0, 0, layout, false);

	layout->AddMember(texture, 0);
	layout->AddMember(edit, 1);
}

void CWatcherWindow::OnDraw()
{
	CChannel::OnDraw();
}