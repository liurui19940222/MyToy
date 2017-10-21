#include"GUITexture.h"
#include"GUIManager.h"
#include<GameEngine\Maker.h>

CGUITexture::CGUITexture() : CGUIElement(), m_texture(NULL)
{
	SetFill(false);
}

void CGUITexture::OnDestroy()
{
	CGUIElement::OnDestroy();
	if (m_managedTex)
		_Maker->Destroy(m_texture);
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

CGUITexture* CGUITexture::SetTexture(CTexture* texture, bool managed)
{
	m_managedTex = managed;
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

CGUITexture* CGUITexture::ConstrainSize(float maxWidth, float maxHeight)
{
	if (!m_texture) 
		return this;
	float width = m_texture->GetWidth();
	float height = m_texture->GetHeight();
	if (width <= maxWidth && height <= maxHeight)
	{
		SetWidth(width)->SetHeight(height);
		return this;
	}
	float ratio = width / height;
	if (width > maxWidth)
	{
		width = maxWidth;
		height = width / ratio;
	}
	else if (height > maxHeight)
	{
		height = maxHeight;
		width = height * ratio;
	}
	SetWidth(width)->SetHeight(height);
	return this;
}