#ifndef _CONTEXT_MENU_H_
#define _CONTEXT_MENU_H_

#include<Windows.h>
#include<string>
#include<functional>
#include<map>

using namespace std;

class CContextMenu
{
	typedef function<void(int)> ContextMenuItemClickEvent;
	map<int, ContextMenuItemClickEvent> m_events;
	HWND m_hwnd;
	HMENU m_menu;
public:
	CContextMenu(HWND hwnd);
	virtual ~CContextMenu();
	void Append(int id, const wstring& text, ContextMenuItemClickEvent call);
	void Show(POINT& point);
	void HandleCommand(int id);
};

#endif