#include "GUILable.h"

using namespace guisystem;

IMPL_CLASS(CGUILable)

CGUILable::CGUILable()
{
	SetFill(false);
}

void CGUILable::OnStart()
{
	Init(NULL, 10, 0, 50.0f, Color::white, EAlignment::CENTER_MIDDLE, SRect2D(0.0f, 0.0f, 100.0f, 100.0f));
}

void CGUILable::OnUIRender()
{
	CGUIWidget::OnUIRender();
}

CFontRenderer* CGUILable::SetTextRect(SRect2D rect)
{
	return CFontRenderer::SetTextRect(rect);
}

float CGUILable::GetPixelScale()
{
	return 1;
}

CGUIWidget* CGUILable::SetRect(SRect2D rect)
{
	CGUIWidget::SetRect(rect);
	SetTextRect(rect);
	return this;
}

CGUIWidget* CGUILable::SetWidth(float width)
{
	CGUIWidget::SetWidth(width);
	SetTextRect(m_rect);
	return this;
}

CGUIWidget* CGUILable::SetHeight(float height)
{
	CGUIWidget::SetHeight(height);
	SetTextRect(m_rect);
	return this;
}