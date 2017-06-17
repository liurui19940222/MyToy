#include "GUISystem.h"

using namespace GUISystem;

#pragma region CGUIWidget

bool CGUIWidget::Overlay(Vector2 pos)
{
	return false;
}

bool CGUIWidget::isCollide()
{
	return m_collide;
}

SRect2D CGUIWidget::GetRect()
{
	return m_rect;
}

int CGUIWidget::GetLayer()
{
	return m_layer;
}

float CGUIWidget::GetWidth()
{
	return m_width;
}

float CGUIWidget::GetHeight()
{
	return m_height;
}

CGUIWidget* CGUIWidget::SetCollide(bool isCollide)
{
	m_collide = isCollide;
	return this;
}

CGUIWidget* CGUIWidget::SetFillColor(Color fillColor)
{
	this->m_fillColor = fillColor;
	return this;
}

CGUIWidget* CGUIWidget::SetRect(SRect2D rect)
{
	this->m_rect = rect;
	return this;
}

CGUIWidget* CGUIWidget::SetLayer(int layer)
{
	this->m_layer = layer;
	return this;
}

CGUIWidget* CGUIWidget::SetWidth(float width)
{
	this->m_width = width;
	return this;
}

CGUIWidget* CGUIWidget::SetHeight(float height)
{
	this->m_height = height;
	return this;
}

bool CGUIWidget::operator>(CGUIWidget* widget)
{
	return m_layer > widget->m_layer;
}

bool CGUIWidget::operator<(CGUIWidget* widget)
{
	return m_layer < widget->m_layer;
}

void CGUIWidget::OnStart()
{
	
}

void CGUIWidget::OnDestroy()
{
	
}

void CGUIWidget::OnUIUpdate()
{
	
}

void CGUIWidget::OnUIRender()
{

}

void CGUIWidget::OnUIDrawDebug()
{

}

#pragma endregion

#pragma region CGUISystem

CGUISystem* CGUISystem::m_instance = NULL;

CGUISystem* CGUISystem::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new CGUISystem();
	return m_instance;
}

void CGUISystem::InitGUI(float resolution_x, float resolution_y)
{
	
}

void CGUISystem::OnUpdate()
{
	
}

void CGUISystem::OnRender()
{
	
}

void CGUISystem::Quit()
{
	
}

#pragma endregion