#ifndef _SCENE_WINDOW_H_
#define _SCENE_WINDOW_H_

#include"Channel.h"

class CSceneWindow : public CChannel
{
protected:
	virtual void OnCreate() override;

public:
	CSceneWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);

	virtual EWindowType GetType() override;
	virtual DWORD GetStyle() override;
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

#endif