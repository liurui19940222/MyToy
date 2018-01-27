#include "UITest.h"

UITest::UITest(const wchar_t* className, const wchar_t* title, int width, int height)
	: GLAppBase(className, title, width, height) {}

void UITest::OnInitialize()
{
	GLAppBase::OnInitialize();
	m_UISystem = new UISystem();
	m_UISystem->StartUp(m_WindowWidth, m_WindowHeight);
	PMaterial mat = make_shared<Material>();
	PUIView view1 = make_shared<UIView>();
	PUIView view2 = make_shared<UIView>();
	PUIView view3 = make_shared<UIView>();
	PUIView view4 = make_shared<UIView>();
	PUIView view5 = make_shared<UIView>();
	view4->SetMaterial(mat);
	m_UISystem->AddChild(view1);
	m_UISystem->AddChild(view2);
	m_UISystem->AddChild(view4);
	m_UISystem->AddChild(view5);
	view1->AddChild(view3);
}

void UITest::OnUpdate(float deltaTime)
{
	GLAppBase::OnUpdate(deltaTime);
}

void UITest::OnRender()
{
	GLAppBase::OnRender();

	m_UISystem->RenderAll();
}

void UITest::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	m_UISystem->SetSize(width, height);
}
