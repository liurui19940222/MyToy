#include"GUITexture.h"
#include"GUIManager.h"

CGUITexture::CGUITexture() : CGUIElement(), m_texture(NULL)
{
	SetFill(false);
}

void CGUITexture::OnRender()
{
	CGUIElement::OnRender();
	if (!m_texture) return;
	glColor3f(m_addColor.r, m_addColor.g, m_addColor.b);
	m_manager->GetRenderer()->DrawTexture(*m_texture, m_rect);
}

void CGUITexture::OnStateChanged()
{
	CGUIElement::OnStateChanged();
}

CGUITexture* CGUITexture::SetTexture(CTexture* texture)
{
	m_texture = texture;
	m_texture->SetEnvMode(ETexEnvMode::Add);
	return this;
}

CGUITexture* CGUITexture::SetRawSize()
{
	if (m_texture)
		SetWidth(m_texture->GetWidth())->SetHeight(m_texture->GetHeight());
	return this;
}