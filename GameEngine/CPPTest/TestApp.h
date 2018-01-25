#pragma once

#include"GLAppBase\GLAppBase.h"

class TestApp : public GLAppBase
{
protected:
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;

public:
	TestApp(const wchar_t* className, const wchar_t* title, int width, int height);

private:
	float		m_Angle = 0.0f;
};

