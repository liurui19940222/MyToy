#ifndef _GL_WINDOW_H_
#define _GL_WINDOW_H_

#include"Window.h"
#include<GameEngine\RawRenderer.h>

class CGLWindow : public CWindow
{
protected:
	CRawRenderer m_renderer;

public:
	virtual LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void OnRender() override;
	virtual void OnDraw();
};

#endif