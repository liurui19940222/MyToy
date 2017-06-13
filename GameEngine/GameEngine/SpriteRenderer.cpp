#include"SpriteRenderer.h"
#include"GameObject.h"

CSpriteRenderer::CSpriteRenderer()
{
}

CSpriteRenderer::~CSpriteRenderer()
{

}

CSpriteRenderer* CSpriteRenderer::SetTexture(CTexture* texture)
{
	this->texture = texture;
	return this;
}

void CSpriteRenderer::OnUpdate()
{

}

void CSpriteRenderer::OnRender()
{
	if (!texture) return;
	static float width = (texture->GetWidth() >> 1) * 0.01f;
	static float height = (texture->GetHeight() >> 1) * 0.01f;
	glPushMatrix();

	glMultMatrixf((float*)&gameObject->GetModelToWorldMat());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	texture->Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(width, height, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-width, height, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-width, -height, 0);
	glTexCoord2f(1, 0);
	glVertex3f(width, -height, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
}

void CSpriteRenderer::OnDestroy()
{

}