#include"SpriteRenderer.h"
#include"GameObject.h"

IMPL_CLASS(CSpriteRenderer)

CSpriteRenderer* CSpriteRenderer::SetTexture(PTexture texture)
{
	this->texture = texture;
	return this;
}

void CSpriteRenderer::OnStart()
{
	
}

void CSpriteRenderer::OnUpdate()
{

}

void CSpriteRenderer::OnDestroy()
{

}