#pragma once

#include"GLAppBase\GLAppBase.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

class TestApp : public GLAppBase
{
protected:
	virtual void InitGL(HWND hwnd) override;
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnWindowSizeChanged(int width, int height) override;

public:
	TestApp(const wchar_t* className, const wchar_t* title, int width, int height);

private:
	float			m_Angle = 0.0f;
};

