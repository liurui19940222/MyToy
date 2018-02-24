#pragma once

#include"GLAppBase\GLAppBase.h"
#include"SpRendering\Shader.h"
#include"SpRendering\Material.h"
#include"SpRendering\Texture2D.h"
#include"SpRendering\GLRendering.h"
#include"SpRendering\FontMeshGenerator.h"
#include"SpGUISystem\UISystem.h"
#include"SpGUISystem\UIImage.h"
#include"SpGUISystem\UILabel.h"

USING_NAMESPACE_GUI;

//#define SHOW_TEXT L"h新款MINI家族正式发布 handle to face object2018北美车展：新款MINI家族正式发布"
#define SHOW_TEXT L"Button"
#define FONT_SIZE 14
#define FONT_PATH "../Assets/Arial.ttf"

class UITest : public GLAppBase
{
public:
	UITest(const wchar_t* className, const wchar_t* title, int width, int height);

protected:
	virtual void InitGL(HWND hwnd) override;
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnWindowSizeChanged(int width, int height) override;

private:
	UISystem*				m_UISystem;
	IRenderingInterface*	m_RI;
	FontMeshGenerator*		m_FMG;

	PUIImage				m_MovedWidget;
	PUIImage				m_Button;
	PUIImage				m_SubWidget;
	PUILabel				m_Label;
	GLuint					m_TexId;
	PTexture				m_Texture;
};

