#ifndef _ASSETS_WINDOW_H_
#define _ASSETS_WINDOW_H_

#include"Channel.h"
#include"GUITree.h"

enum class EIconTypeOnAssets
{
	Other = 0,
	Folder = 1,
	Texture = 2,
};

class CAssetsWindow : public CChannel
{
	static map<EIconTypeOnAssets, CTexture*> m_icons;
	CGUITree<wstring> m_tree;
	void ForeachAssets(const wstring& path, CGUITreeNode<wstring>* node);
	void RefreshAssetsList();
public:
	CAssetsWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);

	virtual EWindowType GetType() override;
	virtual DWORD GetStyle() override;
	virtual void OnCreate() override;
	virtual void OnDraw() override;
	virtual void OnClose() override;
};

#endif