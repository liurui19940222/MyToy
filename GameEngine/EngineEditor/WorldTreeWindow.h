#ifndef _WORLD_TREE_WINDOW_H_
#define _WORLD_TREE_WINDOW_H_

#include"Channel.h"
#include"GUITree.h"

#define CMENU_ID_CREATE_EMPTY	1
#define CMENU_ID_CREATE_CUBE	2
#define CMENU_ID_CREATE_QUAD	3
#define CMENU_ID_GO_DESTORYED	4

class CWorldTreeWindow : public CChannel
{
	CGUIGroup* group;
	CGUIElement* CreateElement(const Color& color);
	CGUIGroup* CreateGroup(const wstring& name);
	CGUITree<int> m_tree;

	void OnForeachGameObjects(CGameObject* go, int depth);
	void OnGameObjectCreated(CGameObject* go);
	void OnGameObjectDestoryed(int instanceId);

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
