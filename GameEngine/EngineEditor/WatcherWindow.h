#ifndef _WATCHER_WINDOW_H_
#define _WATCHER_WINDOW_H_

#include"Channel.h"
#include"GameObjectWatcher.h"

enum class EBeWatchedType
{
	GameObject = 1,
};

enum class EIconTypeOnWatcher
{
	Transform,
	GameObject,
	Gou,
};

class CWatcherWindow : public CChannel
{
	CWatcher* m_watcher;

	CWatcher* CreateWatcherWithType(EBeWatchedType type);
public:
	static map<EIconTypeOnWatcher, CTexture*> m_icons;
	CWatcherWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);

	virtual EWindowType GetType() override;
	virtual DWORD GetStyle() override;
	virtual void OnCreate() override;
	virtual void OnDraw() override;
	virtual void OnClose() override;
	virtual void OnReceiveMsg(SMessage& message) override;
};

#endif