#ifndef _WORLD_TREE_WINDOW_H_
#define _WORLD_TREE_WINDOW_H_

#include"Channel.h"
#include"GUITree.h"

class CWorldTreeWindow : public CChannel
{
	CGUIGroup* group;
	CGUIElement* CreateElement(const Color& color);
	CGUIGroup* CreateGroup(const wstring& name);
	CGUITree<int> m_tree;
public:
	CWorldTreeWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);

	virtual EWindowType GetType() override;
	virtual DWORD GetStyle() override;
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void OnCreate() override;
	virtual void OnClose() override;
	virtual void OnDraw() override;
	virtual void OnReceiveMsg(SMessage& message) override;
};

#endif
