#include"GUIManager.h"
#include"GUIElement.h"
#include<GameEngine\Application.h>
#include<GameEngine\RawRenderer.h>
#include<GameEngine\Input.h>

CGUIManager::CGUIManager()
{
	m_layout.m_gui = this;
	m_elements.SetComparator([](CGUIElement* a, CGUIElement* b) {
		if (*a > *b) return 1;
		else if (*a < *b) return -1;
		return 0;
	});
}

void CGUIManager::InitGUI(float resolution_x, float resolution_y)
{
	m_renderer = NULL;
	SetResolution(resolution_x, resolution_y);
}

void CGUIManager::UpdateWidgetLayer(CGUIElement* widget)
{
	m_elements.Remove(widget);
	m_elements.Enqueue(widget);
}

void CGUIManager::Destroy(CGUIElement* widget)
{
	if (!widget) return;
	if (m_curOverlay == widget) m_curOverlay = NULL;
	if (m_lastDown == widget) m_lastDown = NULL;
	widget->OnDestroy();
	m_elements.Remove(widget);
	delete(widget);
}

void CGUIManager::DestroyAll()
{
	CGUIElement* e = m_elements.Head();
	m_elements.Foreach([this](CGUIElement* e) {
		e->OnDestroy();
	});
	m_elements.Foreach([this](CGUIElement* e) {
		delete(e);
	});
	m_curOverlay = NULL;
	m_lastDown = NULL;
	m_elements.Clear();
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
	m_layout.MaxWidth = resolution_x;
	m_layout.MaxHeight = resolution_y;
}

void CGUIManager::SetAbsoluteWindowLTPos(const Vector2& pos)
{
	m_absoluteWindowLTPos = pos;
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

Vector2 CGUIManager::GetAbsoluteWindowLTPos() const
{
	return m_absoluteWindowLTPos;
}

void CGUIManager::SetRenderer(CRawRenderer* renderer)
{
	m_renderer = renderer;
}

CRawRenderer* CGUIManager::GetRenderer() const
{
	return m_renderer;
}

void CGUIManager::DrawLayout()
{
	m_layout.DrawLayout(*m_renderer);
}

void CGUIManager::PrintLayout()
{
	m_layout.PrintLayout();
}

float CGUIManager::GetLayoutOffsetX() const
{
	return m_layout.OffsetX;
}

float CGUIManager::GetLayoutOffsetY() const
{
	return m_layout.OffsetY;
}

CGUIManager* CGUIManager::SetRowsVisible(int startIndex, int count, bool visible)
{
	m_layout.SetRowsVisible(startIndex, count, visible);
	return this;
}

CGUIManager* CGUIManager::UpdateLayout()
{
	m_layout.UpdateLayout();
	return this;
}

CGUIManager* CGUIManager::SetLayoutOffsetX(float offset)
{
	m_layout.OffsetX = offset;
	return this;
}

CGUIManager* CGUIManager::SetLayoutOffsetY(float offset)
{
	m_layout.OffsetY = offset;
	return this;
}

CGUIManager* CGUIManager::SetGridRowHeight(float rowHeight)
{
	m_layout.RowHeight = rowHeight;
	return this;
}

CGUIManager* CGUIManager::SetGridRowCount(int rowCount)
{
	m_layout.ResizeRow(rowCount);
	return this;
}

CGUIManager* CGUIManager::SetGridColumns(int colCount, vector<float>& weights)
{
	m_layout.ResizeCol(colCount);
	m_layout.SetWeights(weights);
	return this;
}

CGUIManager* CGUIManager::PutIntoGrid(int rowIndex, int colIndex, CGUIElement* element, bool newline)
{
	if (rowIndex == -1)
		rowIndex = m_layout.GetUnfilledRowIndex();
	if (rowIndex >= m_layout.RowCount)
	{
		m_layout.ResizeRow(rowIndex + 1);
		m_layout.UpdateLayout();
	}
	if (newline)
	{
		m_layout.InsertRow(rowIndex);
		m_layout.UpdateLayout();
	}
	m_layout[rowIndex][colIndex].set(element);
	return this;
}

CGUIManager* CGUIManager::DeleteRow(int rowIndex, bool releaseElement)
{
	m_layout.DeleteRow(rowIndex, releaseElement);
	return this;
}

void CGUIManager::OnUpdate()
{
	Vector2 mousePos = CInput::InputMousePosition() - m_absoluteWindowLTPos;
	mousePos.y = m_resolutionY - mousePos.y;
	bool hasAnyOneDown = false;
	InverseForeachElementR([this, &mousePos, &hasAnyOneDown](CGUIElement* widget) {
		if (!widget->IsState(EElementState::Disabled) && widget->IsCollide())
		{
			if (widget->Overlay(mousePos))
			{
				if (widget != m_curOverlay)
				{
					if (m_curOverlay)
					{
						m_curOverlay->SetState(EElementState::Normal);
						m_curOverlay->OnMouseExit(mousePos);
					}
					widget->OnMouseEnter(mousePos);
				}

				if (CInput::GetMouseDown(EMouseKey::Left))
				{
					hasAnyOneDown = true;
					widget->SetState(EElementState::Pressed);
					widget->OnMouseDown(mousePos);
					if (m_lastDown && m_lastDown != widget)
					{
						m_lastDown->m_haveFocus = false;
						m_lastDown->OnLostFocus();
					}
					m_lastDown = widget;
				}
				else if (CInput::GetMouseUp(EMouseKey::Left))
				{
					if (widget->IsState(EElementState::Pressed))
					{
						widget->SetState(EElementState::Hover);
						widget->OnMouseUp(mousePos);
						widget->OnMouseClick(mousePos);
					}
				}
				else if (!widget->IsState(EElementState::Pressed))
				{
					widget->SetState(EElementState::Hover);
					if (widget->m_lastOverPos != mousePos)
					{
						widget->OnMouseOver(mousePos);
						widget->m_lastOverPos = mousePos;
					}
				}

				m_curOverlay = widget;
				return false;
			}
			else if (widget->IsState(EElementState::Hover) || widget->IsState(EElementState::Pressed))
			{
				widget->SetState(EElementState::Normal);
				widget->OnMouseExit(mousePos);
				if (widget == m_curOverlay)
					m_curOverlay = NULL;
			}
		}
		return true;
	});
	if (CInput::GetMouseDown(EMouseKey::Left) && !hasAnyOneDown)
	{
		if (m_lastDown)
		{
			m_lastDown->m_haveFocus = false;
			m_lastDown->OnLostFocus();
		}
	}
	ForeachElement([](CGUIElement* widget) {
		widget->OnUpdate();
	});
}

void CGUIManager::OnRender()
{
	if (!m_renderer) return;
	ForeachElement([](CGUIElement* element) {
		if (element->IsEnalbe() && (element->m_cell == NULL || (element->m_cell && element->m_cell->m_visible)))
			element->OnRender();
	});
}

void CGUIManager::Quit()
{

}