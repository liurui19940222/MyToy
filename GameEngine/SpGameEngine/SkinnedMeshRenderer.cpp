#include"SkinnedMeshRenderer.h"
#include"GameObject.h"
#include"..\SpCommon\EngineDefine.h"
#include"Engine.h"
#include"Application.h"
#include"EditorTool.h"
#include"ColladaFile.h"

IMPL_CLASS(CSkinnedMeshRenderer)

void CSkinnedMeshRenderer::OnStart()
{
	m_material = CMaterial::GetDefaltMaterial();
}

void CSkinnedMeshRenderer::RenderDebug(Matrix4x4& modelMatrix)
{
	CEditorTool::DrawAxis(modelMatrix);
	CEditorTool::DrawSkeleton(modelMatrix, *m_skeleton);
}

void CSkinnedMeshRenderer::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (!m_mesh) return;
	m_material->Bind();
	m_material->SetParam("GlobalPoseMatrices", &m_skeleton->m_skiningMatrices[0], m_skeleton->GetSize());
	m_material->SetParam("M", modelMatrix);
	m_material->SetParam("V", viewMatrix);
	m_material->SetParam("P", projectionMatrix);
	m_mesh->BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, m_mesh->GetVertexNum());
	m_material->Unbind();
}

void CSkinnedMeshRenderer::Render(CShader* shader)
{
	if (!m_mesh) return;
	m_mesh->BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, m_mesh->GetVertexNum());
}

CSkinnedMeshRenderer* CSkinnedMeshRenderer::SetSkinningMesh(CMeshBuffer* mesh, Skeleton* skeleton)
{
	this->m_mesh = mesh;
	this->m_skeleton = skeleton;
	return this;
}

CSkinnedMeshRenderer* CSkinnedMeshRenderer::SetMaterial(CMaterial* material)
{
	this->m_material = material;
	return this;
}