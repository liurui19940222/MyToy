#include"MeshRenderer.h"
#include"GameObject.h"
#include"EngineDefine.h"
#include"Engine.h"
#include"Application.h"
#include"EditorTool.h"

IMPL_CLASS(CMeshRenderer)

void CMeshRenderer::OnStart()
{
	m_material = CMaterial::GetDefaltMaterial();
}

void CMeshRenderer::RenderDebug(Matrix4x4& modelMatrix)
{
	CEditorTool::DrawAxis(modelMatrix, gameObject->GetLocalScale());
}

void CMeshRenderer::Render(Matrix4x4& modelMatrix)
{
	if (!m_mesh) return;
	m_material->Bind();
	m_material->SetParam("MVP", MainCamera->GetProjectionMatrix() * MainCamera->GetViewMatrix() * modelMatrix);
	m_material->SetParam("Color", Color::white);
	m_material->SetParam("MainTex", 0);
	m_mesh->GetBuffer()->BindBuffer();
	glDrawArrays(m_mesh->GetGLMode(), 0, m_mesh->GetVertexNum());
	m_material->Unbind();
}

CMeshRenderer* CMeshRenderer::SetModel(CMeshProvider* mesh)
{
	this->m_mesh = mesh;
	return this;
}

CMeshRenderer* CMeshRenderer::SetMaterial(CMaterial* material)
{
	this->m_material = material;
	return this;
}