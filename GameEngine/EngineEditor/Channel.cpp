#include"Channel.h"
#include"Editor.h"

CChannel::CChannel(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height)
{	
	m_hwnd = CreateWindow(class_name, L"", WS_VISIBLE | WS_POPUP | WS_THICKFRAME,
		550, 10, width, height, parent, NULL, instance, NULL);
}

CChannel::~CChannel()
{
	
}