#include "GameObject.h"
#include "TextRenderer.h"

IMPL_CLASS(CTextRenderer)

void CTextRenderer::OnStart()
{
	Init(NULL, 10, 0, 0.0f, Color::white, EAlignment::CENTER_MIDDLE, SRect2D(0.0f, 0.0f, 1.0f, 1.0f));
}

void CTextRenderer::OnUpdate()
{

}

void CTextRenderer::OnDestroy()
{
	ClearLineData();
}

float CTextRenderer::GetPixelScale()
{
	return 0.01f;
}