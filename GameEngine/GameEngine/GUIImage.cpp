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

void CGUIImage::OnUIRender()
{
	if (m_texture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		m_texture->Bind();
		glBegin(GL_QUADS);
		if (m_state == EWidgetState::Normal)
		{
			glColor4f(1, 1, 1, 1);
		}
		else if (m_state == EWidgetState::Hover)
		{
			glColor4f(0.9f, 0.9f, 0.9f, 1);
		}
		else if (m_state == EWidgetState::Pressed)
		{
			glColor4f(0.76f, 0.76f, 0.76f, 1);
		}
		else if (m_state == EWidgetState::Disabled)
		{
			glColor4f(0.35f, 0.35f, 0.35f, 1);
		}
		for (int i = 0; i < 4; ++i)
		{
			glTexCoord2fv((float*)&m_uvs[i]);
			glVertex3fv((float*)&m_vertices[i]);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	CGUIWidget::OnUIRender();
}

CGUIImage* CGUIImage::SetTexture(CTexture* texture)
{
	m_texture = texture;
	m_texture->SetEnvMode(ETexEnvMode::Modulate);
	SetWidth(texture->GetWidth());
	SetHeight(texture->GetHeight());
	return this;
}
