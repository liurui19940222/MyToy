#include "GUIImage.h"

using namespace guisystem;

IMPL_CLASS(CGUIImage)

CGUIImage::CGUIImage() : CGUIWidget(), m_texture(NULL)
{
	SetFill(true);
	m_uvs[0].x = 0; m_uvs[0].y = 1;
	m_uvs[1].x = 0; m_uvs[1].y = 0;
	m_uvs[2].x = 1; m_uvs[2].y = 0;
	m_uvs[3].x = 1; m_uvs[3].y = 1;
}

void CGUIImage::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	CGUIWidget::Render(modelMatrix, viewMatrix, projectionMatrix);
}

CGUIImage* CGUIImage::SetTexture(PTexture texture)
{
	m_texture = texture;
	m_texture->SetEnvMode(ETexEnvMode::Modulate);
	m_material->SetMainTexture(m_texture);
	SetWidth(texture->GetWidth());
	SetHeight(texture->GetHeight());
	return this;
}
