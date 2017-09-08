#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include<Windows.h>
#include<windowsx.h>
#include<GameEngine\Property.h>
#include"Editor.h"

enum EWindowType;

enum class EChannelState
{
	Float = 4,
	Fixed = 5,
};

class CWindow
{
protected:
	HWND m_hwnd;
	EChannelState m_state;

	void Create(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);

public:
	CWindow();
	CWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword);
	virtual ~CWindow();

	virtual EWindowType GetType();
	virtual DWORD GetStyle();
	virtual HMENU GetMenu();
	virtual wchar_t* GetTitle();
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	property_r<HWND> WindowHandle = _prop_r(HWND, { return m_hwnd; });
	property_r<EChannelState> State = _prop_r(EChannelState, { return m_state; });
};

#endif