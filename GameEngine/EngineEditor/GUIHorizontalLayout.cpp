#include"GUIHorizontalLayout.h"
#include"GUIManager.h"

CGUIHorizontalLayout::CGUIHorizontalLayout() : CGUIElement()
{
	SetCollide(false)->SetFill(false);
}

void CGUIHorizontalLayout::AddMember(CGUIElement* element, int index)
{
	if (m_members.size() <= index) return;
	m_members[index] = element;
	element->SetEnable(m_cell->m_visible);
	if (index < m_rects.size())
	{
		element->SetRect(m_rects[index]);
	}
}

void CGUIHorizontalLayout::SetWeights(vector<float> weights, bool normalize)
{
	if (normalize)
	{
		float magnitude = 0;
		for (float v : weights)
			magnitude += v;
		magnitude = 1.0f / magnitude;
		for (int i = 0; i < weights.size(); ++i)
			weights[i] *= magnitude;
	}
	m_weights = weights;
	m_members.resize(weights.size());
}

void CGUIHorizontalLayout::OnLayoutChanged(const SRect2D& rect)
{
	CGUIElement::OnLayoutChanged(rect);
	m_rects = m_rect.Split(m_weights);
	for (int i = 0; i < m_rects.size(); ++i)
	{
		if (m_members[i])
		{
			m_members[i]->SetRect(m_rects[i]);
		}
	}
}

void CGUIHorizontalLayout::OnVisibleChanged(bool visible)
{
	CGUIElement::OnVisibleChanged(visible);

	for (CGUIElement* element : m_members)
	{
		if (element)
			element->SetEnable(visible);
	}
}