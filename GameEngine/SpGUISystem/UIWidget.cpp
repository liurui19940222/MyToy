#include "UIWidget.h"

USING_NAMESPACE_GUI;

UIWidget::~UIWidget()
{
}

bool UIWidget::Overlay(const Vector2& point) const
{
	SRect2D rect(GetGlobalPosition(), m_Rect.halfSize);
	return rect.Overlay(point);
}

UIWidget* UIWidget::SetWidth(float width)
{
	this->m_Width = width;
	this->m_Rect.halfSize.x = width * 0.5f;
	return this;
}

UIWidget* UIWidget::SetHeight(float height)
{
	this->m_Height = height;
	this->m_Rect.halfSize.y = height * 0.5f;
	return this;
}

UIWidget* UIWidget::SetRect(const SRect2D& rect)
{
	this->m_Rect = rect;
	this->m_Width = rect.halfSize.x * 2;
	this->m_Height = rect.halfSize.y * 2;
	return this;
}

UIWidget* UIWidget::SetLocalPosition(const Vector2& pos)
{
	this->m_Rect.center = pos;
	return this;
}

UIWidget* UIWidget::SetGlobalPosition(const Vector2& pos)
{
	if (m_Parent.get())
		this->m_Rect.center = pos - m_Parent->GetGlobalPosition();
	else
		this->m_Rect.center = pos;
	return this;
}

UIWidget* UIWidget::AddChild(PUIWidget widget)
{
	m_Childreen.push_back(widget);
	widget->m_System = m_System;
	return this;
}

UIWidget* UIWidget::RemoveChild(PUIWidget widget)
{
	if (widget.get())
	{
		for (auto it = m_Childreen.begin(); it != m_Childreen.end(); ++it)
		{
			if (it->get() == widget.get())
			{
				m_Childreen.erase(it);
				break;
			}
		}
	}
	return this;
}
