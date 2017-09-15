#ifndef _CANNEL_H_
#define _CANNEL_H_

#include"GLWindow.h"

class CChannel : public CGLWindow
{
protected:
	wstring m_caption;

public:
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void OnDraw() override;
};

#endif