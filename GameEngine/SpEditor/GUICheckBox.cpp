#include"GUICheckBox.h"
#include"GUIManager.h"

CGUICheckBox::CGUICheckBox() : CGUIElement()
{
	SetFill(true)->SetCollide(true);
	AddOnMouseClickListener([this](Vector2 pos) {
		this->SetSelected(!this->m_selected);
	});
}

void CGUICheckBox::OnRender()
{
	CRawRenderer& renderer = *m_manager->GetRenderer();
	renderer.DrawRect(m_rect, Color::grey);
	renderer.SetRenderMode(ERenderMode::Wireframe);
	renderer.DrawRect(m_rect, Color::white);
	renderer.SetRenderMode(ERenderMode::Fill);
	if (m_selected && m_icon)
	{	
		renderer.DrawTexture(*m_icon, SRect2D{ m_rect.center_x, m_rect.center_y, m_icon->GetWidth() * 0.5f, m_icon->GetHeight() * 0.5f });
	}
}

void CGUICheckBox::SetIcon(CTexture* tex)
{
	m_icon = tex;
}

void CGUICheckBox::SetOnValueChangedListener(OnValueChanged callback)
{
	m_callback = callback;
}

void CGUICheckBox::SetSelected(bool isSelected)
{
	m_selected = isSelected;
	if (m_callback)
	{
		m_callback(m_selected);
	}
}

bool CGUICheckBox::IsSelected()
{
	return m_selected;
}