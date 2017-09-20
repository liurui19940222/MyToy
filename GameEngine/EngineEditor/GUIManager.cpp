#include"GUIManager.h"
#include"GUIElement.h"
#include<GameEngine\Application.h>
#include<GameEngine\RawRenderer.h>

void CGUIManager::InitGUI(float resolution_x, float resolution_y)
{
	m_renderer = NULL;
	SetResolution(resolution_x, resolution_y);
	m_elements.SetComparator([](CGUIElement* a, CGUIElement* b) {
		if (*a > *b) return 1;
		else if (*a < *b) return -1;
		return 0;
	});
}

void CGUIManager::UpdateWidgetLayer(CGUIElement* widget)
{
	m_elements.Remove(widget);
	m_elements.Enqueue(widget);
}

void CGUIManager::Destroy(CGUIElement* widget)
{
	m_elements.Remove(widget);
	delete(widget);
}

void CGUIManager::ForeachElement(ElementForeachCallback callback)
{
	m_elements.Foreach(callback);
}

void CGUIManager::ForeachElementR(ElementForeachCallbackR callback)
{
	m_elements.ForeachR(callback);
}

void CGUIManager::InverseForeachElement(ElementForeachCallback callback)
{
	m_elements.ForeachInverse(callback);
}

void CGUIManager::InverseForeachElementR(ElementForeachCallbackR callback)
{
	m_elements.ForeachInverseR(callback);
}

void CGUIManager::SetResolution(float resolution_x, float resolution_y)
{
	m_resolutionX = resolution_x;
	m_resolutionY = resolution_y;
	m_centerPos.x = resolution_x * 0.5f;
	m_centerPos.y = resolution_y * 0.5f;
}

float CGUIManager::GetResolutionX()
{
	return m_resolutionX;
}

float CGUIManager::GetResolutionY()
{
	return m_resolutionY;
}

Vector3 CGUIManager::GetCenterPosition()
{
	return m_centerPos;
}

void CGUIManager::SetRenderer(CRawRenderer* renderer)
{
	m_renderer = renderer;
}

CRawRenderer* CGUIManager::GetRenderer() const
{
	return m_renderer;
}

void CGUIManager::OnUpdate()
{

}

void CGUIManager::OnRender()
{
	if (!m_renderer) return;
	ForeachElement([](CGUIElement* element) {
		element->OnRender();
	});
}

void CGUIManager::Quit()
{
	
}