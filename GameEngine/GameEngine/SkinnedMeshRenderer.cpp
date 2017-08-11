#include"SkinnedMeshRenderer.h"
#include"GameObject.h"
#include"EngineDefine.h"
#include"Engine.h"
#include"Application.h"
#include"EditorTool.h"

IMPL_CLASS(CSkinnedMeshRenderer)

void CSkinnedMeshRenderer::OnStart()
{
	m_material = CMaterial::GetDefaltMaterial();
}

void CSkinnedMeshRenderer::RenderDebug(Matrix4x4& modelMatrix)
{
	CEditorTool::DrawAxis(modelMatrix);
}

void CSkinnedMeshRenderer::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (!m_mesh) return;
	m_material->Bind();
	m_material->SetParam("M", modelMatrix);
	m_material->SetParam("V", viewMatrix);
	m_material->SetParam("P", projectionMatrix);
	m_mesh->GetBuffer()->BindBuffer();
	glDrawArrays(m_mesh->GetGLMode(), 0, m_mesh->GetVertexNum());
	m_material->Unbind();
}

CSkinnedMeshRenderer* CSkinnedMeshRenderer::SetModel(CMeshProvider* mesh)
{
	this->m_mesh = mesh;
	return this;
}

CSkinnedMeshRenderer* CSkinnedMeshRenderer::SetMaterial(CMaterial* material)
{
	this->m_material = material;
	return this;
}