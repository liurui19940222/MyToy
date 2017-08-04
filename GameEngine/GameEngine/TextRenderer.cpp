#include "GameObject.h"
#include "TextRenderer.h"
//#include "BitImage.h"

IMPL_CLASS(CTextRenderer)

CTextRenderer::CTextRenderer()
{
}


CTextRenderer::~CTextRenderer()
{
}

void CTextRenderer::OnStart()
{
	Init(NULL, NULL, 10, 0, 0.5f, Color::white, EAlignment::CENTER_MIDDLE, SRect2D(0.0f, 0.0f, 1.0f, 1.0f));
}

void CTextRenderer::OnUpdate()
{

}

void CTextRenderer::OnRender()
{
	Render();
}

void CTextRenderer::OnDrawDebug()
{
	CEditorTool::DrawRect(GetTextRect(), gameObject->GetModelToWorldMat());
}

void CTextRenderer::OnDestroy()
{
	ClearPrimitive();
	ClearLineData();
}

float CTextRenderer::GetPixelScale()
{
	return 0.01f;
}