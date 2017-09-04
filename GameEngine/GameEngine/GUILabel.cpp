#include "GUILabel.h"

using namespace guisystem;

IMPL_CLASS(CGUILabel)

CGUILabel::CGUILabel() : CGUIWidget()
{
	SetFill(false);
}

void CGUILabel::OnStart()
{
	CGUIWidget::OnStart();
	Init(NULL, 18, 0, Color::white, EAlignment::CENTER_MIDDLE, SRect2D(0.0f, 0.0f, 100.0f, 100.0f));
}

CFontRenderer* CGUILabel::SetTextRect(SRect2D rect)
{
	return CFontRenderer::SetTextRect(rect);
}

float CGUILabel::GetPixelScale()
{
	return 1;
}

CGUIWidget* CGUILabel::SetRect(SRect2D rect)
{
	CGUIWidget::SetRect(rect);
	SetTextRect(rect);
	return this;
}

CGUIWidget* CGUILabel::SetWidth(float width)
{
	CGUIWidget::SetWidth(width);
	SetTextRect(m_rect);
	return this;
}

CGUIWidget* CGUILabel::SetHeight(float height)
{
	CGUIWidget::SetHeight(height);
	SetTextRect(m_rect);
	return this;
}

void CGUILabel::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	CGUIWidget::Render(modelMatrix, viewMatrix, projectionMatrix);
	OnRender(modelMatrix, viewMatrix, projectionMatrix);
}

void CGUILabel::RenderDebug(Matrix4x4& modelMatrix)
{
	OnRenderDebug(modelMatrix);
}