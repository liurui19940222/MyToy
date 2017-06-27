#include "GUIImage.h"

CGUIImage::CGUIImage() : CGUIWidget(), m_texture(NULL) 
{
	m_uvs[0].x = 0; m_uvs[0].y = 1;
	m_uvs[1].x = 0; m_uvs[1].y = 0;
	m_uvs[2].x = 1; m_uvs[2].y = 0;
	m_uvs[3].x = 1; m_uvs[3].y = 1;
}

void CGUIImage::OnUIRender()
{
	CGUIWidget::OnUIRender();

	if (m_texture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		m_texture->Bind();
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; ++i)
		{
			glVertex3fv((float*)&m_vertices[i]);
			glTexCoord2fv((float*)&m_uvs[i]);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
}

CGUIImage* CGUIImage::SetTexture(CTexture* texture)
{
	m_texture = texture;
	SetWidth(texture->GetWidth());
	SetHeight(texture->GetHeight());
	return this;
}