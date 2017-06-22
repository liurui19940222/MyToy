#include "GUISystem.h"
#include "Engine.h"
#include "Application.h"

using namespace guisystem;
using namespace container;

#pragma region CGUIWidget

bool CGUIWidget::Overlay(Vector2 pos)
{
	//因为rect被定义在模型空间，所以要用pos减去世界空间位置
	return m_rect.Overlay(pos - gameObject->GetPosition());
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

CGUIWidget* CGUIWidget::SetFill(bool isFill)
{
	m_fill = isFill;
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
	this->m_width = rect.half_size_x * 2;
	this->m_height = rect.half_size_y * 2;
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
	this->m_rect.half_size_x = width * 0.5f;
	return this;
}

CGUIWidget* CGUIWidget::SetHeight(float height)
{
	this->m_height = height;
	this->m_rect.half_size_y = height * 0.5f;
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
	GUISystem->AddWidget(this);
}

void CGUIWidget::OnDestroy()
{
	GUISystem->DestroyWidget(this);
}

void CGUIWidget::OnUIUpdate()
{
	
}

void CGUIWidget::OnUIRender()
{
	static Vector3 vertices[4];
	vertices[0].x = -m_rect.half_size_x; vertices[0].y = m_rect.half_size_y; vertices[0].z = 0;
	vertices[1].x = -m_rect.half_size_x; vertices[1].y = -m_rect.half_size_y; vertices[1].z = 0;
	vertices[2].x = m_rect.half_size_x; vertices[2].y = -m_rect.half_size_y; vertices[2].z = 0;
	vertices[3].x = m_rect.half_size_x; vertices[3].y = m_rect.half_size_y; vertices[3].z = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (m_fill) glColor4f(m_fillColor.r, m_fillColor.g, m_fillColor.b, m_fillColor.a);

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; ++i) glVertex3fv((float*)&vertices[i]);
	glEnd();

	glDisable(GL_BLEND);
}

void CGUIWidget::OnUIDrawDebug()
{
	CEditorTool::DrawRect(m_rect, gameObject->GetModelToWorldMat());
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
	m_resolutionX = resolution_x;
	m_resolutionY = resolution_y;
}

void CGUISystem::AddWidget(CGUIWidget* widget)
{
	widgets.Enqueue(widget);
}

void CGUISystem::DestroyWidget(CGUIWidget* dwidget)
{
	widgets.Remove(dwidget);
}

void CGUISystem::OnUpdate()
{
	widgets.Foreach([](CGUIWidget* widget) {
		widget->OnUIUpdate();
	});
}

void CGUISystem::OnRender()
{
	widgets.Foreach([](CGUIWidget* widget) {
		widget->gameObject->BeginRender();
		widget->OnUIRender();
		widget->gameObject->EndRender();
	});
}

void CGUISystem::OnDrawDebug()
{
	widgets.Foreach([](CGUIWidget* widget) {
		widget->OnUIDrawDebug();
	});
}

void CGUISystem::Quit()
{
	widgets.Foreach([](CGUIWidget* widget) {
		Engine->DestroyGameObject(widget->gameObject);
	});
	widgets.Clear();
}

#pragma endregion