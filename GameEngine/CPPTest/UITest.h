#pragma once

#include"GLAppBase\GLAppBase.h"
#include"SpRendering\Shader.h"
#include"SpRendering\Material.h"
#include"SpRendering\Texture2D.h"
#include"SpGUISystem\UISystem.h"

USING_NAMESPACE_GUI;

class UITest : public GLAppBase
{
public:
	UITest(const wchar_t* className, const wchar_t* title, int width, int height);

protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnWindowSizeChanged(int width, int height) override;

private:
	UISystem*		m_UISystem;
};

