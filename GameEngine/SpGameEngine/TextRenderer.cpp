#include "GameObject.h"
#include "TextRenderer.h"

IMPL_CLASS(CTextRenderer)

CTextRenderer::CTextRenderer() : FontRenderer(NULL) {}

void CTextRenderer::OnStart()
{
	SetPixelScale(0.01f);
}

void CTextRenderer::OnUpdate()
{

}

void CTextRenderer::OnDestroy()
{
	ClearLineData();
}

void CTextRenderer::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	OnRender(modelMatrix, viewMatrix, projectionMatrix);
}

void CTextRenderer::RenderDebug(Matrix4x4& modelMatrix)
{
	OnRenderDebug(modelMatrix);
}