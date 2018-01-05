#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include"Window.h"

class CMainWindow : public CWindow
{
public:
	CMainWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);

	virtual EWindowType GetType() override;
	virtual DWORD GetStyle() override;
	virtual HMENU GetMenu() override;
	virtual wchar_t* GetTitle() override;
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

#endif