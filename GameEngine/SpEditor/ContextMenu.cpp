#include "ContextMenu.h"

CContextMenu::CContextMenu(HWND hwnd) : m_hwnd(hwnd)
{
	m_menu = CreatePopupMenu();
}

CContextMenu::~CContextMenu()
{
	DeleteMenu(m_menu, 0, 0);
}

void CContextMenu::Append(int id, const wstring& text, ContextMenuItemClickEvent call)
{
	AppendMenu(m_menu, MF_STRING, id, text.c_str());
	m_events.insert(make_pair(id, call));
}

void CContextMenu::Show(POINT& point)
{
	ClientToScreen(m_hwnd, &point);
	TrackPopupMenu(m_menu, TPM_RIGHTBUTTON, point.x, point.y, 0, m_hwnd, NULL);
}

void CContextMenu::HandleCommand(int id)
{
	auto it = m_events.find(id);
	if (it != m_events.end())
	{
		it->second(id);
	}
}