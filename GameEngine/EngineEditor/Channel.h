#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include<Windows.h>
#include<windowsx.h>
#include<GameEngine\Property.h>

enum class EChannelState
{
	Float,
	Fixed,
};

class CChannel
{
private:
	HWND m_hwnd;
	EChannelState m_state;

public:
	CChannel(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height);
	~CChannel();

	property_r<HWND> WindowHandle = _prop_r(HWND, { return m_hwnd; });
	property_r<EChannelState> State = _prop_r(EChannelState, { return m_state; });
};

#endif