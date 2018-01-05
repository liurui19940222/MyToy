#include "WatcherWindow.h"

map<EIconTypeOnWatcher, CTexture*> CWatcherWindow::m_icons;

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

	m_icons.insert(make_pair(EIconTypeOnWatcher::GameObject, CTexture2D::Create("textures/gui/ico_gameObject.png")->SetFilterMode(ETexFilterMode::Point)));
	m_icons.insert(make_pair(EIconTypeOnWatcher::Transform, CTexture2D::Create("textures/gui/ico_transform.png")->SetFilterMode(ETexFilterMode::Point)));
	m_icons.insert(make_pair(EIconTypeOnWatcher::Gou, CTexture2D::Create("textures/gui/ico_ok.png")->SetFilterMode(ETexFilterMode::Point)));

	CMessageCenter::Register(MSG_ON_SELECTED_GO_ON_WORLDTREE, this);
}

CWatcher* CWatcherWindow::CreateWatcherWithType(EBeWatchedType type)
{
	if (type == EBeWatchedType::GameObject)
		return new CGameObjectWatcher(this);
	return NULL;
}

void CWatcherWindow::OnDraw()
{
	CChannel::OnDraw();
}

void CWatcherWindow::OnClose()
{
	CMessageCenter::Unregister(MSG_ON_SELECTED_GO_ON_WORLDTREE, this);
}

void CWatcherWindow::OnReceiveMsg(SMessage& message)
{
	CChannel::OnReceiveMsg(message);
	if (message.m_msgType == MSG_ON_SELECTED_GO_ON_WORLDTREE)
	{
		if (m_watcher == NULL)
		{
			m_watcher = new CGameObjectWatcher(this);
		}
		else if (!IS_TYPE(CGameObjectWatcher, m_watcher))
		{
			delete(m_watcher);
			m_watcher = new CGameObjectWatcher(this);
		}
		m_watcher->Show(message.m_body);
	}
}