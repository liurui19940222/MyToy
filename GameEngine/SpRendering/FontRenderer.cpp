#include"FontRenderer.h"
#include"..\SpAssetLoader\ImageLoader.h"
#include"..\SpRendering\MeshFactory.h"
#include"..\SpCommon\FastPainter.h"
#include"Texture2D.h"
#include<iostream>

USING_NAMESPACE_ENGINE;

FontRenderer::FontRenderer(IRenderingInterface* ri) : m_RI(ri) {}

void FontRenderer::OnRender(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (m_Primitives.empty())
		return;
	BuildInstanceData();
	PMaterial material = GetDefaultMaterial();
	PMeshBufferUIInstance mesh = GetDefaultBuffer();
	size_t size = m_TexcoordRanges.size();
	mesh->MakeInstanceBuffer(m_TexcoordRanges, m_Colors, m_RectList, m_ModelMatrices, size);
	material->SetMainTexture(m_Primitives[0]->m_Sprite->m_Texture);
	material->Bind();
	material->SetParam("u_M", modelMatrix);
	material->SetParam("u_V", viewMatrix);
	material->SetParam("u_P", projectionMatrix);
	m_RI->RenderInstance(RenderingObject{ mesh.get(), material.get() }, size);
}

void FontRenderer::OnRenderDebug(Matrix4x4& modelMatrix)
{

}

PMeshBufferUIInstance FontRenderer::GetDefaultBuffer()
{
	static PMeshBufferUIInstance mesh;
	if (!mesh)
		mesh = _MeshFactory->CreateBuffer<MeshBufferUIInstance>(EMeshType::Quad);
	return mesh;
}