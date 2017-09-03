#include "GUIImage.h"

using namespace guisystem;

IMPL_CLASS(CGUIImage)

CGUIImage::CGUIImage() : CGUIWidget(), m_texture(NULL)
{
	SetFill(false);
	m_uvs[0].x = 0; m_uvs[0].y = 1;
	m_uvs[1].x = 0; m_uvs[1].y = 0;
	m_uvs[2].x = 1; m_uvs[2].y = 0;
	m_uvs[3].x = 1; m_uvs[3].y = 1;
}

void CGUIImage::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (m_texture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_BLEND);
	}

	CGUIWidget::Render(modelMatrix, viewMatrix, projectionMatrix);
}

CGUIImage* CGUIImage::SetTexture(CTexture* texture)
{
	m_texture = texture;
	m_texture->SetEnvMode(ETexEnvMode::Modulate);
	SetWidth(texture->GetWidth());
	SetHeight(texture->GetHeight());
	return this;
}
