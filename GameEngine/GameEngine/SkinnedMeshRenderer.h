#ifndef _SKINNED_MESH_RENDERER_H_
#define _SKINNED_MESH_RENDERER_H_

#include"Texture.h"
#include"FileHead.h"
#include"Component.h"
#include"DynamicFactory.h"
#include"Shader.h"
#include"Renderer.h"
#include"Material.h"
#include"SkeletonAnimation.h"
#include"MeshBuffer.h"
#include"ColladaFile.h"

class CSkinnedMeshRenderer : public CComponent, public IRenderer
{
	REFLECT_CLASS(CSkinnedMeshRenderer)
private:
	Skeleton* m_skeleton;
	CMeshBuffer* m_mesh;
	CMaterial* m_material;

public:

	CSkinnedMeshRenderer* SetSkinningMesh(CMeshBuffer* mesh, Skeleton* skeleton);

	CSkinnedMeshRenderer* SetMaterial(CMaterial* material);

	virtual void OnStart() override;
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) override;
	virtual void RenderDebug(Matrix4x4& modelMatrix) override;
};

#endif