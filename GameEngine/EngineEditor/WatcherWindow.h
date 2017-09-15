#ifndef _WATCHER_WINDOW_H_
#define _WATCHER_WINDOW_H_

#include"Channel.h"

class CWatcherWindow : public CChannel
{
public:
	CWatcherWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);

	virtual EWindowType GetType() override;
	virtual DWORD GetStyle() override;
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void OnDraw() override;
};

#endif